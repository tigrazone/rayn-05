////////////////////////////////////////////////////////////////////////////
//
// nrScene.cpp
//
// A class for Scenes.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrScene.h"

#include "nrHit.h"
#include "nrInterval.h"
#include "nrLight.h"
#include "nrList.h"
#include "nrLog.h"
#include "nrParser.h"
#include "nrRay.h"
#include "nrSurface.h"
#include "nrSurfaceBox.h"
#include "nrSurfaceBVH.h"
#include "nrSurfaceRGS.h"
#include "nrSurfaceSphere.h"
#include "nrSurfaceTriangle.h"
#include "nrVector3.h"
#include "nrView.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrScene::nrScene( void )
{
    m_BVH = 0;
    m_RGS = 0;
    m_Cull = true;
    m_Ambient = nrColor( 0, 0, 0 );
    m_Background = nrColor( 0, 0, 0 );
    m_View = new nrView( nrVector3( 0, 0, 5 ), nrVector3( 0, 0, 0 ), nrVector3( 0, 1, 0 ) );
    assert( m_View );
}

////////////////////////////////////////////////////////////////////////////

nrScene::~nrScene( void )
{
    if ( m_BVH )
    {
        delete m_BVH;
    }
    else if ( m_RGS )
    {
        delete m_RGS;

        for ( int i = 0; i < m_Surfaces.Length(); i++ )
        {
            nrSurface* surface = m_Surfaces[ i ];
            
            delete surface;
        }
    }
    else
    {
        for ( int i = 0; i < m_Surfaces.Length(); i++ )
        {
            nrSurface* surface = m_Surfaces[ i ];
            
            delete surface;
        }
    }
    
    for ( int i = 0; i < m_Lights.Length(); i++ )
    {
        delete m_Lights[ i ];
    }
    
    delete m_View;
}

////////////////////////////////////////////////////////////////////////////

bool nrScene::Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    bool hit_something = false;
    
    if ( m_BVH != 0 )
    {
        if ( m_BVH->Hit( ray, interval, hit ) )
        {
            interval.m_Maximum = hit.t;
            hit_something = true;
        }
    }
    else if ( m_RGS != 0 )
    {
        if ( m_RGS->Hit( ray, interval, hit ) )
        {
            interval.m_Maximum = hit.t;
            hit_something = true;
        }
    }
    else
    {
        for ( int i = 0; i < m_Surfaces.Length(); i++ )
        {
            nrSurface* surface = m_Surfaces[ i ];
            
            if ( surface->Hit( ray, interval, hit ) )
            {
                interval.m_Maximum = hit.t;
                hit_something = true;
            }
        }
    }
    
    return hit_something;
}

////////////////////////////////////////////////////////////////////////////

bool nrScene::Parse( const char* scene_file )
{
    int num_spheres = 0;
    int num_triangles = 0;
    int num_boxes = 0;
    int num_culled = 0;
    int num_lights = 0;
    
    nrParser parser;
    if ( ! parser.Open( scene_file ) )
    {
        g_Log.Write( "Unable to open scene file \"%s\".\n", scene_file );
        return false;
    }
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "sphere" ) )
        {
            nrSurfaceSphere* s = nrSurfaceSphere::Parse( parser );
            if ( s == 0 )
            {
                parser.ParseError( "malformed sphere key" );
            }
            else
            {
                m_Surfaces.Add( s );
            }
            
            num_spheres++;
        }
        else if ( parser.KeyMatches( key, "box" ) )
        {
            nrSurfaceBox* s = nrSurfaceBox::Parse( parser );
            if ( s == 0 )
            {
                parser.ParseError( "malformed box key" );
            }
            else
            {
                m_Surfaces.Add( s );
            }
            
            num_boxes++;
        }
        else if ( parser.KeyMatches( key, "triangle" ) )
        {
            nrSurfaceTriangle* s = nrSurfaceTriangle::Parse( parser );
            if ( s == 0 )
            {
                parser.ParseError( "malformed triangle key" );
            }
            else
            {
                if ( m_Cull )
                {
                    nrVector3 v = m_View->m_Gaze;
                    if ( v.Dot( s->Normal( s->A() ) ) < 0 )
                    {
                        m_Surfaces.Add( s );
                    }
                    else
                    {
                        delete s;
                        num_culled++;
                    }
                }
                else
                {
                    m_Surfaces.Add( s );
                }
            }
            
            num_triangles++;
        }
        else if ( parser.KeyMatches( key, "light" ) )
        {
            nrLight* l = nrLight::Parse( parser );
            if ( l == 0 )
            {
                parser.ParseError( "malformed light key" );
            }
            else
            {
                m_Lights.Add( l );
            }
            
            num_lights++;
        }
        else if ( parser.KeyMatches( key, "view" ) )
        {
            nrView* v = nrView::Parse( parser );
            if ( v == 0 )
            {
                parser.ParseError( "malformed view key" );
            }
            else
            {
                delete m_View;
                m_View = v;
            }
        }
        else if ( parser.KeyMatches( key, "material" ) )
        {
            nrMaterial::Parse( parser );
        }
        else if ( parser.KeyMatches( key, "ambient" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            m_Ambient.r = parser.ReadFloat();
            m_Ambient.g = parser.ReadFloat();
            m_Ambient.b = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else if ( parser.KeyMatches( key, "background" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            m_Background.r = parser.ReadFloat();
            m_Background.g = parser.ReadFloat();
            m_Background.b = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else
        {
            parser.ParseError( "unknown key." );
        }
    }
    
    parser.ReadToken( nrParser::TOKEN_EOF );
    
    parser.Close();
    
    if ( parser.Error() )
    {
        return false;
    }
    
    m_Surfaces.Compress();
    m_Lights.Compress();
    
    if ( num_spheres > 0 )
    {
        g_Log.Write( "%4d sphere%s.\n", num_spheres, num_spheres == 1 ? "" : "s" );
    }
    if ( num_triangles > 0 )
    {
        g_Log.Write( "%4d triangle%s", num_triangles, num_triangles == 1 ? "" : "s" );
        if ( m_Cull )
        {
            g_Log.Write( " (%d culled)", num_culled );
        }
        g_Log.Write( ".\n" );
    }
    if ( num_boxes > 0 )
    {
        g_Log.Write( "%4d box%s.\n", num_boxes, num_boxes == 1 ? "" : "es" );
    }
    g_Log.Write( "%4d light%s.\n", num_lights, num_lights == 1 ? "" : "s" );

    return true;
}

////////////////////////////////////////////////////////////////////////////

void nrScene::CreateBVH( bool sort )
{
    assert( m_RGS == 0 );

    if ( m_Surfaces.Length() > 0 )
    {
        m_BVH = nrSurfaceBVH::CreateTree( m_Surfaces, sort );
    }
}

////////////////////////////////////////////////////////////////////////////

void nrScene::CreateRGS( void )
{
    assert( m_BVH == 0 );
    
    if ( m_Surfaces.Length() > 0 )
    {
        m_RGS = nrSurfaceRGS::CreateGrid( m_Surfaces );
    }
}

////////////////////////////////////////////////////////////////////////////

nrColor& nrScene::Ambient( void )
{
    return m_Ambient;
}

////////////////////////////////////////////////////////////////////////////

nrColor& nrScene::Background( void )
{
    return m_Background;
}

////////////////////////////////////////////////////////////////////////////

void nrScene::CullBackfaces( bool cull )
{
    m_Cull = cull;
}

////////////////////////////////////////////////////////////////////////////

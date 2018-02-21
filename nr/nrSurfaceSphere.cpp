////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceSphere.cpp
//
// A class for a sphere.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrSurfaceSphere.h"

#include "nrHit.h"
#include "nrInterval.h"
#include "nrLog.h"
#include "nrMath.h"
#include "nrParser.h"
#include "nrRay.h"

#include <assert.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrSurfaceSphere::nrSurfaceSphere( const nrVector3& center, float radius, const nrMaterial* material )
{
    m_Center   = center;
    m_Radius   = radius;
    m_Material = material;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceSphere::~nrSurfaceSphere( void )
{
}

////////////////////////////////////////////////////////////////////////////

static float epsilon = 0.0001f;

bool nrSurfaceSphere::Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    const nrVector3& d = ray.d;
    const nrVector3& o = ray.o;
    const nrVector3 co = o - m_Center;
    
    float A = d.Dot( d );       assert( ! nrMath::Equal( A, 0.0f ) );
    float B = 2 * d.Dot( co );
    float C = co.Dot( co ) - m_Radius * m_Radius;
    float discriminant = B * B - 4 * A * C;
    
    if ( discriminant >= epsilon )
    {
        float s = nrMath::Sqrt( discriminant );
        
        float t = ( -B - s ) / ( 2 * A );
        if ( interval.Includes( t ) ) 
        {
            hit = nrHit( this, t );
            return true;
        }
        
        t = ( -B + s ) / ( 2 * A );
        if ( interval.Includes( t ) )
        {
            hit = nrHit( this, t );
            return true;
        }
    }
    
    return false;
}

////////////////////////////////////////////////////////////////////////////

nrBound nrSurfaceSphere::Bound( void ) const
{
    nrVector3 r = nrVector3( m_Radius, m_Radius, m_Radius );
    nrBound b = nrBound( m_Center - r, m_Center + r );
    
    b.Validate();

    return b;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrSurfaceSphere::Normal( const nrVector3& point ) const
{
    return ( point - m_Center ).Unit();
}

////////////////////////////////////////////////////////////////////////////

const nrMaterial* nrSurfaceSphere::Material( void ) const
{
    assert( m_Material );
    
    return m_Material;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceSphere* nrSurfaceSphere::Parse( nrParser& parser )
{
    nrVector3   center = nrVector3( 0, 0, 0 );
    float       radius = 0;
    nrMaterial* material = 0;
    
    bool parsed_center = false;
    bool parsed_radius = false;
    bool parsed_material = false;
    
    if ( parser.NextToken() == nrParser::TOKEN_STRING )
    {
        //const char* name = parser.ReadString();
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "center" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            center.x = parser.ReadFloat();
            center.y = parser.ReadFloat();
            center.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );

            parsed_center = true;
        }
        else if ( parser.KeyMatches( key, "radius" ) )
        {
            radius = parser.ReadFloat();

            parsed_radius = true;
        }
        else if ( parser.KeyMatches( key, "color" ) )
        {
            material = nrMaterial::ParseColor( parser );

            parsed_material = true;
        }
        else if ( parser.KeyMatches( key, "material" ) )
        {
            material = nrMaterial::Parse( parser );

            parsed_material = true;
        }
        else
        {
            parser.ParseError( "unknown key." );
        }
    }
    
    if ( ! parsed_center )
    {
        parser.ParseError( "Required key \"center\" missing.\n" );
    }
    if ( ! parsed_radius )
    {
        parser.ParseError( "Required key \"radius\" missing.\n" );
    }
    if ( ! parsed_material )
    {
        parser.ParseError( "Required key \"material\" missing.\n" );
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_END );
    
    if ( parser.Error() )
    {
        return 0;
    }
    
    nrSurfaceSphere* sphere = new nrSurfaceSphere( center, radius, material );
    
    return sphere;
}

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceBox.cpp
//
// A class for a box.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrSurfaceBox.h"

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

nrSurfaceBox::nrSurfaceBox( const nrVector3& p0, const nrVector3& p1, const nrMaterial* material )
{
    nrVector3 mins;
    nrVector3 maxs;
    
    mins.x = nrMath::Min( p0.x, p1.x );
    maxs.x = nrMath::Max( p0.x, p1.x );

    mins.y = nrMath::Min( p0.y, p1.y );
    maxs.y = nrMath::Max( p0.y, p1.y );

    mins.z = nrMath::Min( p0.z, p1.z );
    maxs.z = nrMath::Max( p0.z, p1.z );
    
    m_P0 = mins;
    m_P1 = maxs;
    
    m_Material = material;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceBox::~nrSurfaceBox( void )
{
}

////////////////////////////////////////////////////////////////////////////

bool nrSurfaceBox::Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    const nrVector3& d = ray.d;
    const nrVector3& o = ray.o;
    const nrVector3& p0 = m_P0;
    const nrVector3& p1 = m_P1;
    
    nrInterval x;
    nrInterval y;
    nrInterval z;
    
    float dx = 1.0f / d.x;
    if ( d.x > 0 ) 
    {
        float a = ( p0.x - o.x ) * dx;
        float b = ( p1.x - o.x ) * dx;
        x = nrInterval( a, b );
    }
    else
    {
        float a = ( p0.x - o.x ) * dx;
        float b = ( p1.x - o.x ) * dx;
        x = nrInterval( b, a );
    }
    
    float dy = 1.0f / d.y;
    if ( d.y > 0 ) 
    {
        float a = ( p0.y - o.y ) * dy;
        float b = ( p1.y - o.y ) * dy;
        y = nrInterval( a, b );
    }
    else
    {
        float a = ( p0.y - o.y ) * dy;
        float b = ( p1.y - o.y ) * dy;
        y = nrInterval( b, a );
    }
    
    float dz = 1.0f / d.z;
    if ( d.z > 0 ) 
    {
        float a = ( p0.z - o.z ) * dz;
        float b = ( p1.z - o.z ) * dz;
        z = nrInterval( a, b );
    }
    else
    {
        float a = ( p0.z - o.z ) * dz;
        float b = ( p1.z - o.z ) * dz;
        z = nrInterval( b, a );
    }
    
    nrInterval xy = ( x.Overlap( y ) );
    
    if ( xy.Empty() )
    {
        return false;
    }
    else
    {
        nrInterval xyz = xy.Overlap( z );
        
        if ( xyz.Empty() )
        {
            return false;
        }
        
        float t = xyz.m_Minimum;
        if ( interval.Includes( t ) )
        {
            hit = nrHit( this, t );
            return true;
        }
        
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

nrBound nrSurfaceBox::Bound( void ) const
{
    nrBound b = nrBound( m_P0, m_P1 );
    
    b.Validate();
    
    return b;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrSurfaceBox::Normal( const nrVector3& point ) const
{
    nrVector3 n;
    
    if ( nrMath::Equal( point.x, m_P0.x ) )
    {
        n = nrVector3( -1, 0, 0 );
    }
    else if ( nrMath::Equal( point.y, m_P0.y ) )
    {
        n = nrVector3( 0, -1, 0 );
    }
    else if ( nrMath::Equal( point.z, m_P0.z ) )
    {
        n = nrVector3( 0, 0, -1 );
    }
    else if ( nrMath::Equal( point.x, m_P1.x ) )
    {
        n = nrVector3( 1, 0, 0 );
    }
    else if ( nrMath::Equal( point.y, m_P1.y ) )
    {
        n = nrVector3( 0, 1, 0 );
    }
    else //if ( nrMath::Equal( point.z, m_P1.z ) )
    {
        n = nrVector3( 0, 0, 1 );
    }
    
    return n.Unit();
}

////////////////////////////////////////////////////////////////////////////

const nrMaterial* nrSurfaceBox::Material( void  ) const
{
    assert( m_Material );
    
    return m_Material;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceBox* nrSurfaceBox::Parse( nrParser& parser )
{
    nrVector3   p0, p1;
    nrMaterial* material = 0;
    
    bool parsed_p0 = false;
    bool parsed_p1 = false;
    bool parsed_material = false;
    
    if ( parser.NextToken() == nrParser::TOKEN_STRING )
    {
        //const char* name = parser.ReadString();
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "p0" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            p0.x = parser.ReadFloat();
            p0.y = parser.ReadFloat();
            p0.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );

            parsed_p0 = true;
        }
        else if ( parser.KeyMatches( key, "p1" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            p1.x = parser.ReadFloat();
            p1.y = parser.ReadFloat();
            p1.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );

            parsed_p1 = true;
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
    
    if ( ! parsed_p0 )
    {
        parser.ParseError( "Required key \"p0\" missing.\n" );
    }
    if ( ! parsed_p1 )
    {
        parser.ParseError( "Required key \"p1\" missing.\n" );
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
    
    nrSurfaceBox* box = new nrSurfaceBox( p0, p1, material );
    
    return box;
}

////////////////////////////////////////////////////////////////////////////

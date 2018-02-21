////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceTriangle.cpp
//
// A class for a triangle.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrSurfaceTriangle.h"

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

nrSurfaceTriangle::nrSurfaceTriangle( const nrVector3& a, const nrVector3& b, const nrVector3& c, const nrMaterial* material )
{
    m_A = a;
    m_B = b;
    m_C = c;
    
    m_Material = material;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceTriangle::~nrSurfaceTriangle( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrBound nrSurfaceTriangle::Bound( void ) const
{
    nrVector3 mins = nrVector3( nrMath::Min3( m_A.x, m_B.x, m_C.x ), nrMath::Min3( m_A.y, m_B.y, m_C.y ), nrMath::Min3( m_A.z, m_B.z, m_C.z ) );
    nrVector3 maxs = nrVector3( nrMath::Max3( m_A.x, m_B.x, m_C.x ), nrMath::Max3( m_A.y, m_B.y, m_C.y ), nrMath::Max3( m_A.z, m_B.z, m_C.z ) );
    
    nrBound b = nrBound( mins, maxs );
    b.Validate();
    
    return b;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrSurfaceTriangle::Normal( const nrVector3& point ) const
{
    return ( ( m_B - m_A ).Cross( m_C - m_A ) ).Unit();
}

////////////////////////////////////////////////////////////////////////////

const nrMaterial* nrSurfaceTriangle::Material( void  ) const
{
    assert( m_Material );
    return m_Material;
}

////////////////////////////////////////////////////////////////////////////

bool nrSurfaceTriangle::Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    float a = m_A.x - m_B.x;
    float b = m_A.y - m_B.y;
    float c = m_A.z - m_B.z;
    float d = m_A.x - m_C.x;
    float e = m_A.y - m_C.y;
    float f = m_A.z - m_C.z;
    float g = ray.d.x;
    float h = ray.d.y;
    float i = ray.d.z;
    float j = m_A.x - ray.o.x;
    float k = m_A.y - ray.o.y;
    float l = m_A.z - ray.o.z;
    
    float ei_minus_hf = e * i - h * f;
    float gf_minus_di = g * f - d * i;
    float dh_minus_eg = d * h - e * g;
    float ak_minus_jb = a * k - j * b;
    float jc_minus_al = j * c - a * l;
    float bl_minus_kc = b * l - k * c;
    
    float m = 1.0f / ( a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg );
    
    float beta = ( j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg ) * m;
    
    if ( beta >= 0 )
    {
        float gamma = ( i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc ) * m;
        
        if ( gamma >= 0 )
        {
            if ( ( beta + gamma ) <= 1 )
            {
                float t = -( f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc ) * m;
                
                if ( interval.Includes( t ) )
                {
                    hit = nrHit( this, t );
                    return true;
                }
            }
        }
    }
    
    return false;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceTriangle* nrSurfaceTriangle::Parse( nrParser& parser )
{
    nrVector3   a, b, c;
    nrMaterial* material = 0;
    
    bool parsed_a = false;
    bool parsed_b = false;
    bool parsed_c = false;
    bool parsed_material = false;
    
    if ( parser.NextToken() == nrParser::TOKEN_STRING )
    {
        //const char* name = parser.ReadString();
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "a" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            a.x = parser.ReadFloat();
            a.y = parser.ReadFloat();
            a.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
            
            parsed_a = true;
        }
        else if ( parser.KeyMatches( key, "b" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            b.x = parser.ReadFloat();
            b.y = parser.ReadFloat();
            b.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
            
            parsed_b = true;
        }
        else if ( parser.KeyMatches( key, "c" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            c.x = parser.ReadFloat();
            c.y = parser.ReadFloat();
            c.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
            
            parsed_c = true;
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
    
    if ( ! parsed_a )
    {
        parser.ParseError( "Required key \"a\" missing.\n" );
    }
    if ( ! parsed_b )
    {
        parser.ParseError( "Required key \"b\" missing.\n" );
    }
    if ( ! parsed_c )
    {
        parser.ParseError( "Required key \"c\" missing.\n" );
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
    
    nrSurfaceTriangle* triangle = new nrSurfaceTriangle( a, b, c, material );
    
    return triangle;
}

////////////////////////////////////////////////////////////////////////////

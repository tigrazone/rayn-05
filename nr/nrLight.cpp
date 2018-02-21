////////////////////////////////////////////////////////////////////////////
//
// nrLight.cpp
//
// A class for a light.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrLight.h"

#include "nrHit.h"
#include "nrInterval.h"
#include "nrLog.h"
#include "nrMaterial.h"
#include "nrMath.h"
#include "nrParser.h"
#include "nrRay.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrLight::nrLight( const nrVector3& position, float radius, const nrColor& color )
{
    m_Position = position;
    m_Radius = radius;
    m_Color  = color;
}

////////////////////////////////////////////////////////////////////////////

nrLight::~nrLight( void )
{
}

////////////////////////////////////////////////////////////////////////////

const nrColor& nrLight::Color( void ) const
{
    return m_Color;
}

////////////////////////////////////////////////////////////////////////////

nrLight* nrLight::NewFromString( const char* string )
{
    nrVector3  center;
    float      radius;
    nrColor    color;
    
    if ( sscanf( string, "%f %f %f %f %f %f %f", &center.x, &center.y, &center.z, &radius, &color.r, &color.g, &color.b ) != 7 )
    {
        return 0;
    }
    
    nrLight* light = new nrLight( center, radius, color );
    if ( light == 0 )
    {
        g_Log.Write( "nrLight::NewFromString(): Out of memory.\n" );
    }
    
    return light;
}

////////////////////////////////////////////////////////////////////////////

nrLight* nrLight::Parse( nrParser& parser )
{
    nrVector3  position;
    float      radius = 1.0f;
    nrColor    color = nrColor( 1, 1, 1 );
    
    if ( parser.NextToken() == nrParser::TOKEN_STRING )
    {
        parser.ReadString();
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( strcmp( key, "position" ) == 0 )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            position.x = parser.ReadFloat();
            position.y = parser.ReadFloat();
            position.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else if ( strcmp( key, "radius" ) == 0 )
        {
            radius = parser.ReadFloat();
        }
        else if ( strcmp( key, "color" ) == 0 )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            color.r = parser.ReadFloat();
            color.g = parser.ReadFloat();
            color.b = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else
        {
            parser.ParseError( "unknown key." );
        }
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_END );
    
    if ( parser.Error() )
    {
        return 0;
    }
    
    nrLight* light = new nrLight( position, radius, color );
    if ( light )
    {
        return light;
    }
    else
    {
        assert( 0 );    // out of memory.
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

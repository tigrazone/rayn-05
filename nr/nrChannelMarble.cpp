////////////////////////////////////////////////////////////////////////////
//
// nrChannelMarble.cpp
//
// A class for materials.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrChannelMarble.h"

#include "nrColorRamp.h"
#include "nrNoise.h"
#include "nrParser.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrChannelMarble::nrChannelMarble( float scale, float period, float distortion, int octaves )
{
	m_Scale = scale;
	m_Period = period;
	m_Distortion = distortion;
	m_Octaves = octaves;
	m_ColorRamp = 0;
}

////////////////////////////////////////////////////////////////////////////

nrChannelMarble::~nrChannelMarble( void )
{
	delete m_ColorRamp;
}

////////////////////////////////////////////////////////////////////////////

nrColor nrChannelMarble::Color( const nrVector3& point ) const
{
	float m = nrMath::Abs( nrNoise::Marble3( point * m_Scale, m_Period, m_Distortion, m_Octaves ) );
	
	if ( m_ColorRamp )
	{
		return m_ColorRamp->Color( m );
	}
	else
	{
		return nrColor( m, m, m );
	}
}

////////////////////////////////////////////////////////////////////////////

nrChannelMarble* nrChannelMarble::Parse( nrParser& parser )
{
    nrChannelMarble* c = new nrChannelMarble;
	
	parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );

    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "scale" ) )
        {
            c->m_Scale = parser.ReadFloat();
        }
        else if ( parser.KeyMatches( key, "period" ) )
        {
            c->m_Period = parser.ReadFloat();
        }
        else if ( parser.KeyMatches( key, "distortion" ) )
        {
            c->m_Distortion = parser.ReadFloat();
        }
        else if ( parser.KeyMatches( key, "octaves" ) )
        {
            c->m_Octaves = parser.ReadInt();
        }
        else if ( parser.KeyMatches( key, "ramp" ) )
        {
			nrArray< nrColor > colors;
			
			parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
			
			while ( parser.NextToken() != nrParser::TOKEN_AGGREGATE_END )
			{
				parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
				float r = parser.ReadFloat();
				float g = parser.ReadFloat();
				float b = parser.ReadFloat();
				parser.ReadToken( nrParser::TOKEN_LIST_END );
				
				colors.Add( nrColor( r, g, b ) );
			}
			
			parser.ReadToken( nrParser::TOKEN_AGGREGATE_END );
			
			c->m_ColorRamp = new nrColorRamp();
			c->m_ColorRamp->CreateFromColors( colors );
        }
        else
        {
            parser.ParseError( "unknown key." );
        }
    }
	
	parser.ReadToken( nrParser::TOKEN_AGGREGATE_END );
	
	return c;
}

////////////////////////////////////////////////////////////////////////////

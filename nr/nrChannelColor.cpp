////////////////////////////////////////////////////////////////////////////
//
// nrChannelColor.cpp
//
// A class for materials.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrChannelColor.h"

#include "nrParser.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrChannelColor::nrChannelColor( const nrColor& color )
{
	m_Color = color;
}

////////////////////////////////////////////////////////////////////////////

nrChannelColor::~nrChannelColor( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrColor nrChannelColor::Color( const nrVector3& point ) const
{
	return m_Color;
}

////////////////////////////////////////////////////////////////////////////

nrChannelColor* nrChannelColor::Parse( nrParser& parser )
{
	parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
	float r = parser.ReadFloat();
	float g = parser.ReadFloat();
	float b = parser.ReadFloat();
	parser.ReadToken( nrParser::TOKEN_LIST_END );
	
    nrChannelColor* c = new nrChannelColor( nrColor( r, g, b ) );
	
	return c;
}

////////////////////////////////////////////////////////////////////////////

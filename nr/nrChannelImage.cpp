////////////////////////////////////////////////////////////////////////////
//
// nrChannel.cpp
//
// A class for materials.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrChannel.h"

#include "nrParser.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrChannel::nrChannel( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrChannel::~nrChannel( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrChannel* nrChannel::Parse( nrParser& parser )
{
    nrChannel* channel = 0;
	
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "marble" ) )
        {
			channel = new nrChannelMarble();
			
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
			channel->m_Scale = parser.ReadFloat();
			channel->m_Period = parser.ReadFloat();
			channel->m_Distortion = parser.ReadFloat();
			if ( parser.NextToken() == nrParser::TOKEN_INT )
			{
				channel->m_Octaves = parser.ReadInt();
			}
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else if ( parser.KeyMatches( key, "image" ) )
        {
			channel = new nrChannelImage();
        }
        else if ( parser.KeyMatches( key, "color" ) )
        {
			channel - new nrChannelColor();
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
			channel->m_Color = nrColor( parser.ReadFloat(), parser.ReadFloat(), parser.ReadFloat() );
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
		delete channel;
        return 0;
    }
    
    return channel;
}

////////////////////////////////////////////////////////////////////////////

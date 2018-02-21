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
#include "nrChannelColor.h"
#include "nrChannelMarble.h"

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
            channel = nrChannelMarble::Parse( parser );
        }
        else if ( parser.KeyMatches( key, "image" ) )
        {
// 			channel = new nrChannelImage();
        }
        else if ( parser.KeyMatches( key, "color" ) )
        {
            channel = nrChannelColor::Parse( parser );
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

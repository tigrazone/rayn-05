////////////////////////////////////////////////////////////////////////////
//
// nrChannelColor.h
//
// A class for channels.
//
// Nate Robins, March 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRCHANNELCOLOR_H
#define NRCHANNELCOLOR_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrChannel.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrChannelColor : public nrChannel
{
	friend class nrChannel;

public:
    
    nrChannelColor( const nrColor& color = nrColor( 1, 1, 1 ) );
    virtual ~nrChannelColor( void );
    
    // Return the color of this channel.
    virtual nrColor Color( const nrVector3& point ) const;

    // Return a new channel parsed from a file.  The channel directive
    // has the following form:
    // 
    // color < r g b >
    // 
    // elements in ()'s are optional.
	static nrChannelColor* Parse( nrParser& parser );
	
private:

	nrColor m_Color;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRCHANNELCOLOR_H

////////////////////////////////////////////////////////////////////////////
//
// nrLight.h
//
// A class for a light.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRLIGHT_H
#define NRLIGHT_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColor.h"
#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrParser;

////////////////////////////////////////////////////////////////////////////

class nrLight
{
public:
    
    nrLight( const nrVector3& position, float radius, const nrColor& color = nrColor( 1, 1, 1 ) );
    virtual ~nrLight( void );
    
    // Return the color of the light.
    const nrColor& Color( void ) const;

    // Return a new light from the description in string.
    // 
    // string has the form:
    // centerx centery centerz radius red green blue
    static nrLight* NewFromString( const char* string );
    
    // Return a new light parsed from a file.  The light directive has the
    // following form:
    // 
    // light ("name") 
    // {
    //   position [ x y z ]
    //   (radius r)
    //   color [ r g b ]
    // }
    // 
    // elements in ()'s are optional.
    static nrLight* Parse( nrParser& parser );
    
public:
    
    nrVector3 m_Position;
    float     m_Radius;
    
private:

    nrColor m_Color;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRLIGHT_H

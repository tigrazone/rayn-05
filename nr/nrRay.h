////////////////////////////////////////////////////////////////////////////
//
// nrRay.h
//
// A class for rays.
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRRAY_H
#define NRRAY_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrRay
{
public:
    
    inline nrRay( void );
    inline nrRay( const nrVector3& origin, const nrVector3& direction );
    
    // Return o + d * t;
    inline nrVector3 Point( float t ) const;
    
public:
    
    nrVector3 o;
    nrVector3 d;
};

////////////////////////////////////////////////////////////////////////////

#include "nrRay.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRRAY_H

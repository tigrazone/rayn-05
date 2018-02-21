////////////////////////////////////////////////////////////////////////////
//
// nrBasis.h
//
// A class for orthonormal bases.
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRBASIS_H
#define NRBASIS_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrBasis
{
public:
    
    // Construct an orthonormal basis from two vectors such that u is 
    // parallel to a, v is in the plane of a and b and w is parallel to a 
    // cross b (in the UV case; v is parallel to a in the VU case, etc).
    void ConstructFromUV( const nrVector3& a, const nrVector3& b );
    void ConstructFromUW( const nrVector3& a, const nrVector3& b );
    void ConstructFromVU( const nrVector3& a, const nrVector3& b );
    void ConstructFromVW( const nrVector3& a, const nrVector3& b );
    void ConstructFromWU( const nrVector3& a, const nrVector3& b );
    void ConstructFromWV( const nrVector3& a, const nrVector3& b );
    
    // Construct an orthonormal basis from a single vector such that w is
    // parallel to a, u and v are arbitrary.
    void ConstructFromW( const nrVector3& a );
    
public:
    
    nrVector3 u;
    nrVector3 v;
    nrVector3 w;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRBASIS_H

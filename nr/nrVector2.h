////////////////////////////////////////////////////////////////////////////
//
// nrVector2.h
//
// A class for vector functions.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRVECTOR2_H
#define NRVECTOR2_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrVector2
{
public:
    
    nrVector2( void );
    nrVector2( float x, float y );
    
    // In the following descriptions, a == this vector.
    
    // Return -a.
    nrVector2 operator-( void ) const;
    
    // Return a - b.
    nrVector2 operator-( const nrVector2& b ) const;
    
    // Return a + b.
    nrVector2 operator+( const nrVector2& b ) const;
    
    // Return a * s.
    nrVector2 operator*( float s ) const;
    
    // Return a / s.
    nrVector2 operator/( float s ) const;
    
    // Return a dot b.
    float Dot( const nrVector2& b ) const;
    
    // Return || a ||.
    float Length( void ) const;
    
    // Return a / || a ||.
    nrVector2 Unit( void ) const;
    
    // Return || a || ^ 2.
    float Norm( void ) const;
    
    // Return ( a + b ) / 2.
    nrVector2 MidPoint( const nrVector2& b ) const;
    
public:
    
    float x, y;
};

////////////////////////////////////////////////////////////////////////////

#include "nrVector2.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRVECTOR2_H

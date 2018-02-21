////////////////////////////////////////////////////////////////////////////
//
// nrVector3.h
//
// A class for vector functions.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRVECTOR3_H
#define NRVECTOR3_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrVector3
{
public:
    
    inline nrVector3( void );
    inline nrVector3( float x, float y, float z = 1.0f );
    
    // In the following descriptions, a == this vector.
    
    // Return -a.
    inline nrVector3 operator-( void ) const;
    
    // Return a - b.
    inline nrVector3 operator-( const nrVector3& b ) const;
    
    // Return a + b.
    inline nrVector3 operator+( const nrVector3& b ) const;
    
    // Return a * s.
    inline nrVector3 operator*( float s ) const;
    
    // Return a / s.
    inline nrVector3 operator/( float s ) const;
    
    // Return a dot b.
    inline float Dot( const nrVector3& b ) const;
    
    // Return a cross b.
    inline nrVector3 Cross( const nrVector3& b ) const;
    
    // Return || a ||.
    inline float Length( void ) const;
    
    // Return a / || a ||.
    inline nrVector3 Unit( void ) const;
    
    // Return || a || ^ 2.
    inline float Norm( void ) const;
    
    // Return ( a + b ) / 2.
    inline nrVector3 MidPoint( const nrVector3& b ) const;
    
    // Return ( ( a dot b ) / ( b dot b ) ) * b.
    inline nrVector3 Project( const nrVector3& b ) const;
    
    // Return the vector that bisects the arc between unit vectors u and v.
    inline nrVector3 Bisect( const nrVector3& b ) const;
    
public:
    
    // Return a normal computed from three vertices of a triangle.
    inline static nrVector3 Normal( const nrVector3& a, const nrVector3& b, const nrVector3& c );
    
public:
    
    float x, y, z;
};

////////////////////////////////////////////////////////////////////////////

#include "nrVector3.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRVECTOR3_H

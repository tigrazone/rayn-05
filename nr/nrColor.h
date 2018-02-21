////////////////////////////////////////////////////////////////////////////
//
// nrColor.h
//
// A class for colors.
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRCOLOR_H
#define NRCOLOR_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrColor
{
public:
    
    inline nrColor( void );
    inline nrColor( float r, float g, float b );
    
    // In the following descriptions, a == this color.
    
    // Return -a (unary minus).
    inline nrColor operator-( void ) const;
    
    // Return a - b.
    inline nrColor operator-( const nrColor& b ) const;
    
    // Return a + b ( a.r + b.r, a.g + b.g, a.b + b.b ).
    inline nrColor operator+( const nrColor& b ) const;
    
    // Return a * b ( a.r * b.r, a.g * b.g, a.b * b.b ).
    inline nrColor operator*( const nrColor& b ) const;
    
    // Return a / b.
    inline nrColor operator/( const nrColor& b ) const;
    
    // Return a * s.
    inline nrColor operator*( float s ) const;
    
    // Return a / s.
    inline nrColor operator/( float s ) const;
    
    // Return a color that is linearly interpolated between a and b by 
    // amount t.
    inline static nrColor Lerp( const nrColor& a, const nrColor& b, float t );
    
    // Return a color clamped to the range specified.
    inline nrColor Clamp( float minimum = 0.0f, float maximum = 1.0f ) const;

public:
    
    float r;
    float g;
    float b;
};

////////////////////////////////////////////////////////////////////////////

#include "nrColor.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRCOLOR_H

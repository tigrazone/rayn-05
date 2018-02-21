////////////////////////////////////////////////////////////////////////////
//
// nrMath.h
//
// A class for math.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRMATH_H
#define NRMATH_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrMath
{
public:
    
    // Return angle (in degrees) converted to radians.
    inline static float DegToRad( float angle_in_degrees );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return angle (in radians) converted to degrees.
    inline static float RadToDeg( float angle_in_radians );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the sin of angle.
    inline static float Sin( float angle_in_degrees );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the cosine of angle.
    inline static float Cos( float angle_in_degrees );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the tangent of angle.
    inline static float Tan( float angle_in_degrees );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the arccos (in degrees) of x.
    inline static float ArcCos( float x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the (in degrees) arcsin of x.
    inline static float ArcSin( float x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the arctangent (in degrees) of x over y.
    inline static float ArcTan2( float x, float y );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the square root of x.
    inline static float Sqrt( float x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return x to power y.
    inline static float Pow( float x, float y );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the next highest power of two (i.e., NextPowerOfTwo( 5 ) 
    // would return 8, NextPowerOfTwo( 16 ) would return 16). 
    inline static int NextPowerOfTwo( int x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns true if the argument is a power of two or false if not. 
    inline static bool IsPowerOfTwo( int x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the argument x clamped to the range [min, max].
    inline static float Clamp( float x, float min = 0.0f, float max = 1.0f );
    inline static int Clamp( int x, int min = 0, int max = 255 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the argument x wrapped in the range [0, period].
    inline static float Wrap( float x, float period = 1.0f );
    inline static int Wrap( int x, int period = 100 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns true if the argument is an even number or false if not.
    inline static bool Even( int x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns true if the argument is an odd number or false if not.
    inline static bool Odd( int x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return |x| (the absolute value of x).
    inline static float Abs( float x );
    inline static int Abs( int x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return the floating point remainder of x / y.
    inline static float Mod( float x, float y );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns a float representing the smallest integer that is greater 
    // than or equal to x.
    inline static float Ceil( float x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns a float representing the largest integer that is less than or
    // equal to x.
    inline static float Floor( float x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns x rounded to the nearest integer.
    inline static float Round( float x );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the minimum of x and y.
    inline static float Min( float x, float y );
    inline static int Min( int x, int y );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the maximum of x and y.
    inline static float Max( float x, float y );
    inline static int Max( int x, int y );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the minimum of x, y and z.
    inline static float Min3( float x, float y, float z );
    inline static int Min3( int x, int y, int z );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the maximum of x, y and z.
    inline static float Max3( float x, float y, float z );
    inline static int Max3( int x, int y, int z );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns true if the two values are within epsilon of each other.
    inline static bool Equal( float x, float y, float epsilon = 0.0001f );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return a random floating point number in the range [min, max]. 
    inline static float Random_f( float min = 0.0f, float max = 1.0f );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return a random integer number in the range [min, max].
    inline static int Random_i( int min = 0, int max = 100 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Return a number linearly interpolated between a and b by amount t.
    inline static float Lerp( float a, float b, float t );
    
    ////////////////////////////////////////////////////////////////////////
    
private:
    
    nrMath( void );
    ~nrMath( void );
    
};

////////////////////////////////////////////////////////////////////////////

#include "nrMath.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRMATH_H

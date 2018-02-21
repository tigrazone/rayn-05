////////////////////////////////////////////////////////////////////////////
//
// nrMath.inl
//
// A class for math.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrMath.h"

#include <math.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

inline float nrMath::DegToRad( float angle_in_degrees )
{
    return ( angle_in_degrees * 3.14159265f ) / 180.0f;
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::RadToDeg( float angle_in_radians )
{
    return ( angle_in_radians * 180.0f ) / 3.14159265f;
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Sin( float angle_in_degrees )
{
    return sinf( DegToRad( angle_in_degrees ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Cos( float angle_in_degrees )
{
    return cosf( DegToRad( angle_in_degrees ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Tan( float angle_in_degrees )
{
    return tanf( DegToRad( angle_in_degrees ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::ArcCos( float x )
{
    return RadToDeg( acosf( x ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::ArcSin( float x )
{
    return RadToDeg( asinf( x ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::ArcTan2( float x, float y )
{
    return RadToDeg( atan2f( x, y ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Sqrt( float x )
{
    return sqrtf( x );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Pow( float x, float y )
{
    return powf( x, y );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Ceil( float x )
{
    return ceilf( x );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Floor( float x )
{
    return floorf( x );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Mod( float x, float y )
{
    return fmodf( x, y );
}

////////////////////////////////////////////////////////////////////////////

inline bool nrMath::Even( int x )
{
    if ( ( x % 2 ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

inline bool nrMath::Odd( int x )
{
    if ( Even( x ) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::NextPowerOfTwo( int x )
{
    int i = 1;
    
    while ( i < x )
    {
        i = i << 1;
    }
    
    return i;
}

////////////////////////////////////////////////////////////////////////////

inline bool nrMath::IsPowerOfTwo( int x )
{
    if ( x <= 0 )
    {
        return false;
    }
    else
    {
        unsigned int i = x;
        
        return ( ( i - 1 ) & i ) == 0;
    }
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Abs( float x )
{
    if ( x < 0 )
    {
        return -x;
    }
    else
    {
        return x;
    }
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Abs( int x )
{
    if ( x < 0 )
    {
        return -x;
    }
    else
    {
        return x;
    }
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Clamp( float x, float min, float max )
{
    if ( x < min )
    {
        return min;
    }
    else if ( x > max )
    {
        return max;
    }
    else
    {
        return x;
    }
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Clamp( int x, int min, int max )
{
    if ( x < min )
    {
        return min;
    }
    else if ( x > max )
    {
        return max;
    }
    else
    {
        return x;
    }
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Wrap( float x, float period )
{
    float y = Mod( x, period );
    
    if ( y < 0 )
    {
        y += period;
    }
    
    return y;
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Wrap( int x, int period )
{
    int y = x % period;
    
    if ( y < 0 )
    {
        y += period;
    }
    
    return y;
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Round( float x )
{
    return ( float )( ( int )( x + 0.5f ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Min( float x, float y )
{
    if ( x < y )
    {
        return x;
    }
    else
    {
        return y;
    }
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Min( int x, int y )
{
    if ( x < y )
    {
        return x;
    }
    else
    {
        return y;
    }
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Max( float x, float y )
{
    if ( x > y )
    {
        return x;
    }
    else
    {
        return y;
    }
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Max( int x, int y )
{
    if ( x > y )
    {
        return x;
    }
    else
    {
        return y;
    }
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Min3( float x, float y, float z )
{
    return Min( Min( x, y ), z );
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Min3( int x, int y, int z )
{
    return Min( Min( x, y ), z );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Max3( float x, float y, float z )
{
    return Max( Max( x, y ), z );
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Max3( int x, int y, int z )
{
    return Max( Max( x, y ), z );
}

////////////////////////////////////////////////////////////////////////////

inline bool nrMath::Equal( float x, float y, float epsilon )
{
    float e = Abs( x - y );
    
    if ( e < epsilon )
    {
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Random_f( float min, float max )
{
    return min + ( ( ( float )rand() / RAND_MAX ) * ( max - min ) );
}

////////////////////////////////////////////////////////////////////////////

inline int nrMath::Random_i( int min, int max )
{
    return ( int )( Random_f( ( float )min, ( float )max ) + 0.5f );
}

////////////////////////////////////////////////////////////////////////////

inline float nrMath::Lerp( float a, float b, float t )
{
    return ( a * ( 1.0f - t ) ) + ( b * t );
}

////////////////////////////////////////////////////////////////////////////

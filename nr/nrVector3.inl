////////////////////////////////////////////////////////////////////////////
//
// nrVector3.inl
//
// A class for vector functions.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrVector3.h"

#include "nrMath.h"

#include <assert.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

inline nrVector3::nrVector3( void )
{
}

////////////////////////////////////////////////////////////////////////////

inline nrVector3::nrVector3( float _x, float _y, float _z )
{
    x = _x;
    y = _y;
    z = _z;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::operator-( void ) const
{
    nrVector3 w;
    
    w.x = -x;
    w.y = -y;
    w.z = -z;
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::operator-( const nrVector3& v ) const
{
    nrVector3 w;
    
    w.x = x - v.x;
    w.y = y - v.y;
    w.z = z - v.z;
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::operator+( const nrVector3& v ) const
{
    nrVector3 w;
    
    w.x = x + v.x;
    w.y = y + v.y;
    w.z = z + v.z;
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::operator*( float s ) const
{
    nrVector3 w;
    
    w.x = x * s;
    w.y = y * s;
    w.z = z * s;
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::operator/( float s ) const
{
    nrVector3 w;
    float t = 1.0f / s;
    
    w.x = x * t;
    w.y = y * t;
    w.z = z * t;
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline float nrVector3::Dot( const nrVector3& v ) const
{
    float dot;
    
    dot = ( x * v.x ) + ( y * v.y ) + ( z * v.z );
    
    return dot;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::Cross( const nrVector3& v ) const
{
    nrVector3 w;
    
    w.x = ( y * v.z ) - ( z * v.y );
    w.y = ( z * v.x ) - ( x * v.z );
    w.z = ( x * v.y ) - ( y * v.x );
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline float nrVector3::Length( void ) const
{
    float l;
    
    l = nrMath::Sqrt( ( x * x ) + ( y * y ) + ( z * z ) );
    
    return l;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::Unit( void ) const
{
    float l;
    nrVector3 w;
    const nrVector3& u = *this;
    
    l = Length();
    
    if ( l == 0.0f )
    {
        w = u * 0.0f;
    }
    else 
    {
        w = u * ( 1.0f / l );
    }
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline float nrVector3::Norm( void ) const
{
    float n;
    
    n = ( x * x ) + ( y * y ) + ( z * z );
    
    return n;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::MidPoint( const nrVector3& v ) const
{
    nrVector3 w;
    
    w.x = ( x + v.x ) / 2.0f;
    w.y = ( y + v.y ) / 2.0f;
    w.z = ( z + v.z ) / 2.0f;
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::Project( const nrVector3& v ) const
{
    nrVector3 w;
    const nrVector3& u = *this;
    float u_dot_v = Dot( v );
    float v_dot_v = v.Dot( v );
    
    if ( v_dot_v == 0.0f )
    {
        w = u * 0.0f;
    }
    else
    {
        w = u * ( u_dot_v / v_dot_v );
    }
    
    return w;
}

///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::Bisect( const nrVector3& v ) const
{
    const nrVector3& u = *this;
    nrVector3 w = u + v;
    float n = Norm();
    
    if ( n == 0.0f )
    {
        w.x = 0.0f;
        w.y = 0.0f;
        w.z = 1.0f;
    }
    else
    {
        w = u * ( 1.0f / nrMath::Sqrt( n ) );
    }
    
    return w;
}

///////////////////////////////////////////////////////////////////////////
// Static
///////////////////////////////////////////////////////////////////////////

inline nrVector3 nrVector3::Normal( const nrVector3& a, const nrVector3& b, const nrVector3& c )
{
    nrVector3 ab = b - a;
    nrVector3 ac = c - a;
    
    nrVector3 n = ( ab.Cross( ac ) ).Unit();
    
    return n;
}

///////////////////////////////////////////////////////////////////////////

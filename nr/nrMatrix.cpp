////////////////////////////////////////////////////////////////////////////
//
// nrMatrix.cpp
//
// A matrix class.
//
// Nate Robins, November 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrMatrix.h"

#include "nrMath.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrMatrix::nrMatrix( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrMatrix::~nrMatrix( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Identity( void )
{
    nrMatrix m;
    
    m.e00 = 1.0f;
    m.e01 = 0.0f;
    m.e02 = 0.0f;
    m.e03 = 0.0f;
    
    m.e10 = 0.0f;
    m.e11 = 1.0f;
    m.e12 = 0.0f;
    m.e13 = 0.0f;
    
    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = 1.0f;
    m.e23 = 0.0f;
    
    m.e30 = 0.0f;
    m.e31 = 0.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Translation( float x, float y, float z )
{
    nrMatrix m;
    
    m.e00 = 1.0f;
    m.e01 = 0.0f;
    m.e02 = 0.0f;
    m.e03 = x;
    
    m.e10 = 0.0f;
    m.e11 = 1.0f;
    m.e12 = 0.0f;
    m.e13 = y;
    
    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = 1.0f;
    m.e23 = z;
    
    m.e30 = 0.0f;
    m.e31 = 0.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Rotation( float angle_in_degrees, float x, float y, float z )
{
    nrVector3 v = nrVector3( x, y, z ).Unit();
    x = v.x;
    y = v.y;
    z = v.z;
    
    float s = nrMath::Sin( angle_in_degrees );
    float c = 1.0f - nrMath::Cos( angle_in_degrees );
    
    float xc = x * c;
    float yc = y * c;
    float zc = z * c;
    float xxc = x * xc;
    float yyc = y * yc;
    float zzc = z * zc;
    float xyc = x * yc;
    float yzc = y * zc;
    float zxc = z * xc;
    float xs = x * s;
    float ys = y * s;
    float zs = z * s;
    
    nrMatrix m;
    
    m.e00 = 1.0f - zzc - yyc;
    m.e01 = xyc - zs;
    m.e02 = zxc + ys;
    m.e03 = 0.0f;
    
    m.e10 = xyc + zs;
    m.e11 = 1.0f - zzc - xxc;
    m.e12 = yzc - xs;
    m.e13 = 0.0f;
    
    m.e20 = zxc - ys;
    m.e21 = yzc + xs;
    m.e22 = 1.0f - yyc - xxc;
    m.e23 = 0.0f;
    
    m.e30 = 0.0f;
    m.e31 = 0.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Scale( float x, float y, float z )
{
    nrMatrix m;
    
    m.e00 = x;
    m.e01 = 0.0f;
    m.e02 = 0.0f;
    m.e03 = 0.0f;
    
    m.e10 = 0.0f;
    m.e11 = y;
    m.e12 = 0.0f;
    m.e13 = 0.0f;
    
    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = z;
    m.e23 = 0.0f;
    
    m.e30 = 0.0f;
    m.e31 = 0.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Perspective( float field_of_view_in_degrees, float aspect, float nearz, float farz )
{
    nrMatrix m;
    
    float f = 1.0f / nrMath::Tan( field_of_view_in_degrees / 2.0f );
    float nmf = nearz - farz;
    
    m.e00 = f / aspect;
    m.e01 = 0.0f;  
    m.e02 = 0.0f;
    m.e03 = 0.0f;
    
    m.e10 = 0.0f;
    m.e11 = f;
    m.e12 = 0.0f;
    m.e13 = 0.0f;
    
    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = ( farz + nearz ) / nmf;
    m.e23 = ( 2.0f * farz * nearz ) / nmf;
    
    m.e30 =  0.0f;
    m.e31 =  0.0f;
    m.e32 = -1.0f;
    m.e33 =  0.0f;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Orthographic( float left, float right, float bottom, float top, float nearz, float farz )
{
    nrMatrix m;
    
    float tx = - ( right + left ) / ( right - left );
    float ty = - ( top + bottom ) / ( top - bottom );
    float tz = - ( farz + nearz ) / ( farz - nearz );
    
    m.e00 = 2.0f / ( right - left );
    m.e01 = 0.0f;              
    m.e02 = 0.0f;                
    m.e03 = tx;
    
    m.e10 = 0.0f;
    m.e11 = 2.0f / ( top - bottom );
    m.e12 = 0.0f;
    m.e13 = ty;
    
    m.e20 = 0.0f;
    m.e21 = 0.0f;
    m.e22 = -2.0f / ( farz - nearz );
    m.e23 = tz;
    
    m.e30 = 0.0f;
    m.e31 = 0.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::LookAt( const nrVector3& eye, const nrVector3& lookat, const nrVector3& up )
{
    nrMatrix m;
    
    nrVector3 upn = up.Unit();
    nrVector3 f = ( lookat - eye ).Unit();
    nrVector3 s = f.Cross( upn );
    nrVector3 u = s.Cross( f );
    
    m.e00 = s.x;
    m.e01 = s.y;
    m.e02 = s.z;
    m.e03 = 0.0f;
    
    m.e10 = u.x;
    m.e11 = u.y;
    m.e12 = u.z;
    m.e13 = 0.0f;
    
    m.e20 = -f.x;
    m.e21 = -f.y;
    m.e22 = -f.z;
    m.e23 = 0.0f;
    
    m.e30 = 0.0f;
    m.e31 = 0.0f;
    m.e32 = 0.0f;
    m.e33 = 1.0f;
    
    m = m * Translation( -eye.x, -eye.y, -eye.z );
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::Transpose( void ) const
{
    nrMatrix m;
    
    m.e00 = e00;
    m.e01 = e10;
    m.e02 = e20;
    m.e03 = e30;
    
    m.e10 = e01;
    m.e11 = e11;
    m.e12 = e21;
    m.e13 = e31;
    
    m.e20 = e02;
    m.e21 = e12;
    m.e22 = e22;
    m.e23 = e32;
    
    m.e30 = e03;
    m.e31 = e13;
    m.e32 = e23;
    m.e33 = e33;
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

nrMatrix nrMatrix::operator*( const nrMatrix& m ) const
{
    nrMatrix n;
    
    n.e00 = e00 * m.e00 + e01 * m.e10 + e02 * m.e20 + e03 * m.e30;
    n.e01 = e00 * m.e01 + e01 * m.e11 + e02 * m.e21 + e03 * m.e31;
    n.e02 = e00 * m.e02 + e01 * m.e12 + e02 * m.e22 + e03 * m.e32;
    n.e03 = e00 * m.e03 + e01 * m.e13 + e02 * m.e23 + e03 * m.e33;
    
    n.e10 = e10 * m.e00 + e11 * m.e10 + e12 * m.e20 + e13 * m.e30;
    n.e11 = e10 * m.e01 + e11 * m.e11 + e12 * m.e21 + e13 * m.e31;
    n.e12 = e10 * m.e02 + e11 * m.e12 + e12 * m.e22 + e13 * m.e32;
    n.e13 = e10 * m.e03 + e11 * m.e13 + e12 * m.e23 + e13 * m.e33;
    
    n.e20 = e20 * m.e00 + e21 * m.e10 + e22 * m.e20 + e23 * m.e30;
    n.e21 = e20 * m.e01 + e21 * m.e11 + e22 * m.e21 + e23 * m.e31;
    n.e22 = e20 * m.e02 + e21 * m.e12 + e22 * m.e22 + e23 * m.e32;
    n.e23 = e20 * m.e03 + e21 * m.e13 + e22 * m.e23 + e23 * m.e33;
    
    n.e30 = e30 * m.e00 + e31 * m.e10 + e32 * m.e20 + e33 * m.e30;
    n.e31 = e30 * m.e01 + e31 * m.e11 + e32 * m.e21 + e33 * m.e31;
    n.e32 = e30 * m.e02 + e31 * m.e12 + e32 * m.e22 + e33 * m.e32;
    n.e33 = e30 * m.e03 + e31 * m.e13 + e32 * m.e23 + e33 * m.e33;
    
    return n;
}

////////////////////////////////////////////////////////////////////////////
// Poke Ops
////////////////////////////////////////////////////////////////////////////

void nrMatrix::PokeTranslation( float x, float y, float z )
{
    e03 = x;
    e13 = y;
    e23 = z;
}

////////////////////////////////////////////////////////////////////////////
// Vector Ops
////////////////////////////////////////////////////////////////////////////

nrVector3 nrMatrix::Transform( const nrVector3& v ) const
{
    nrVector3 t;
    
    t.x = e00 * v.x + e01 * v.y + e02 * v.z + e03 /* * 1.0f */;
    t.y = e10 * v.x + e11 * v.y + e12 * v.z + e13 /* * 1.0f */;
    t.z = e20 * v.x + e21 * v.y + e22 * v.z + e23 /* * 1.0f */;
    
    return t;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrMatrix::operator*( const nrVector3& v ) const
{
    return Transform( v );
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrMatrix::Right( void ) const
{
    nrVector3 v;
    
    v.x = e00;
    v.y = e10;
    v.z = e20;
    
    return v;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrMatrix::Up( void ) const
{
    nrVector3 v;
    
    v.x = e01;
    v.y = e11;
    v.z = e21;
    
    return v;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrMatrix::Direction( void ) const
{
    nrVector3 v;
    
    v.x = e02;
    v.y = e12;
    v.z = e22;
    
    return v;
}

////////////////////////////////////////////////////////////////////////////

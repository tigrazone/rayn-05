////////////////////////////////////////////////////////////////////////////
//
// nrBasis.cpp
//
// A class for orthonormal bases.
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrBasis.h"
#include "nrMath.h"

#include <assert.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromUV( const nrVector3& a, const nrVector3& b )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( b.Norm(), 0.0f ) );
    assert( 1 /* a is not parallel to b */ );
    
    u = a.Unit();
    w = a.Cross( b ).Unit();
    v = w.Cross( u );
}

////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromUW( const nrVector3& a, const nrVector3& b )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( b.Norm(), 0.0f ) );
    assert( 1 /* a is not parallel to b */ );
    
    u = a.Unit();
    v = a.Cross( b ).Unit();
    w = v.Cross( u );
}

////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromVU( const nrVector3& a, const nrVector3& b )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( b.Norm(), 0.0f ) );
    assert( 1 /* a is not parallel to b */ );
    
    v = a.Unit();
    w = a.Cross( b ).Unit();
    u = w.Cross( v );
}

////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromVW( const nrVector3& a, const nrVector3& b )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( b.Norm(), 0.0f ) );
    assert( 1 /* a is not parallel to b */ );
    
    v = a.Unit();
    u = a.Cross( b ).Unit();
    w = u.Cross( v );
}

////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromWU( const nrVector3& a, const nrVector3& b )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( b.Norm(), 0.0f ) );
    assert( 1 /* a is not parallel to b */ );
    
    w = a.Unit();
    v = a.Cross( b ).Unit();
    u = v.Cross( w );
}

////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromWV( const nrVector3& a, const nrVector3& b )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( b.Norm(), 0.0f ) );
    assert( ! nrMath::Equal( a.Dot( b ), 1.0f ) ); // a is not parallel to b
    
    w = a.Unit();
    u = b.Cross( a ).Unit();
    v = w.Cross( u );
}

////////////////////////////////////////////////////////////////////////////

void nrBasis::ConstructFromW( const nrVector3& a )
{
    assert( ! nrMath::Equal( a.Norm(), 0.0f ) );
    
    w = a.Unit();
    
    if ( ( nrMath::Abs( w.x ) < nrMath::Abs( w.y ) ) && ( nrMath::Abs( w.x ) < nrMath::Abs( w.z ) ) )
    {
        v = nrVector3( 0.0f, w.z, -w.y ).Unit();
    } 
    else if ( nrMath::Abs( w.y ) < nrMath::Abs( w.z ) )
    {
        v = nrVector3( w.z, 0.0f, -w.x ).Unit();
    }
    else
    {
        v = nrVector3( w.y, -w.x, 0.0f ).Unit();
    }
    
    u = v.Cross( w );
}

////////////////////////////////////////////////////////////////////////////

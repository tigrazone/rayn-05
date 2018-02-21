////////////////////////////////////////////////////////////////////////////
//
// nrBound.cpp
//
// A class for bounds.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrBound.h"

#include "nrInterval.h"
#include "nrRay.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrBound::nrBound( const nrVector3& minimums, const nrVector3& maximums )
{
    m_Minimums = minimums;
    m_Maximums = maximums;

    Validate();
}

////////////////////////////////////////////////////////////////////////////

nrBound::~nrBound( void )
{
}

////////////////////////////////////////////////////////////////////////////

void nrBound::Validate( void )
{
    float epsilon = 0.0001f;
    
    if ( nrMath::Equal( m_Maximums.x, m_Minimums.x ) )
    {
        m_Minimums.x -= epsilon / 2.0f;
        m_Maximums.x += epsilon / 2.0f;
    }
    if ( nrMath::Equal( m_Maximums.y, m_Minimums.y ) )
    {
        m_Minimums.y -= epsilon / 2.0f;
        m_Maximums.y += epsilon / 2.0f;
    }
    if ( nrMath::Equal( m_Maximums.z, m_Minimums.z ) )
    {
        m_Minimums.z -= epsilon / 2.0f;
        m_Maximums.z += epsilon / 2.0f;
    }
}

////////////////////////////////////////////////////////////////////////////

bool nrBound::Hit( const nrRay& ray, const nrInterval& interval ) const
{
    const nrVector3& d = ray.d;
    const nrVector3& o = ray.o;
    const nrVector3& p0 = m_Minimums;
    const nrVector3& p1 = m_Maximums;
    
    nrInterval x;
    nrInterval y;
    nrInterval z;
    
    float dx = 1.0f / d.x;
    if ( d.x > 0 ) 
    {
        float a = ( p0.x - o.x ) * dx;
        float b = ( p1.x - o.x ) * dx;
        x = nrInterval( a, b );
    }
    else
    {
        float a = ( p0.x - o.x ) * dx;
        float b = ( p1.x - o.x ) * dx;
        x = nrInterval( b, a );
    }
    
    float dy = 1.0f / d.y;
    if ( d.y > 0 ) 
    {
        float a = ( p0.y - o.y ) * dy;
        float b = ( p1.y - o.y ) * dy;
        y = nrInterval( a, b );
    }
    else
    {
        float a = ( p0.y - o.y ) * dy;
        float b = ( p1.y - o.y ) * dy;
        y = nrInterval( b, a );
    }
    
    float dz = 1.0f / d.z;
    if ( d.z > 0 ) 
    {
        float a = ( p0.z - o.z ) * dz;
        float b = ( p1.z - o.z ) * dz;
        z = nrInterval( a, b );
    }
    else
    {
        float a = ( p0.z - o.z ) * dz;
        float b = ( p1.z - o.z ) * dz;
        z = nrInterval( b, a );
    }
    
    nrInterval xy = ( x.Overlap( y ) );
    
    if ( xy.Empty() )
    {
        return false;
    }
    else
    {
        nrInterval xyz = xy.Overlap( z );
        
        if ( xyz.Empty() )
        {
            return false;
        }
        
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////

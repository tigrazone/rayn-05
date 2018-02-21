////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceRGS.cpp
//
// A class for a regular grid subdivision of surfaces.
//
// Nate Robins, February 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrSurfaceRGS.h"

#include "nrHit.h"
#include "nrInterval.h"
#include "nrProgress.h"
#include "nrRay.h"


////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////

static nrProgress g_Progress;


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrSurfaceRGS::nrSurfaceRGS( const nrBound& bound, nrArray< nrSurface* >* grids, int nx, int ny, int nz )
{
    m_Nx = nx;
    m_Ny = ny;
    m_Nz = nz;
    m_Grids = grids;
    m_Bound = bound;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceRGS::~nrSurfaceRGS( void )
{
    delete [] m_Grids;
}

////////////////////////////////////////////////////////////////////////////

#define index( x, y, z ) ( ( ( z * ny + y ) * nx ) + x )

bool nrSurfaceRGS::Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    const nrVector3& p0 = m_Bound.m_Minimums;
    const nrVector3& p1 = m_Bound.m_Maximums;
    
    const int& nx = m_Nx;
    const int& ny = m_Ny;
    const int& nz = m_Nz;
    
    float& t = interval.m_Minimum;
    
    const nrVector3& o = ray.o;
    const nrVector3& d = ray.d;
    
    nrVector3 tmin;
    nrVector3 tmax;
    
    if ( d.x >= 0 )
    {
        tmin.x = ( p0.x - o.x ) / d.x;
        tmax.x = ( p1.x - o.x ) / d.x;
    }
    else
    {
        tmin.x = ( p1.x - o.x ) / d.x;
        tmax.x = ( p0.x - o.x ) / d.x;
    }
    
    if ( d.y >= 0 )
    {
        tmin.y = ( p0.y - o.y ) / d.y;
        tmax.y = ( p1.y - o.y ) / d.y;
    }
    else
    {
        tmin.y = ( p1.y - o.y ) / d.y;
        tmax.y = ( p0.y - o.y ) / d.y;
    }
    
    if ( d.z >= 0 )
    {
        tmin.z = ( p0.z - o.z ) / d.z;
        tmax.z = ( p1.z - o.z ) / d.z;
    }
    else
    {
        tmin.z = ( p1.z - o.z ) / d.z;
        tmax.z = ( p0.z - o.z ) / d.z;
    }
    
    float t0 = nrMath::Max3( tmin.x, tmin.y, tmin.z );
    float t1 = nrMath::Min3( tmax.x, tmax.y, tmax.z );
    if ( t0 > t1 )
    {
        return false;
    }
    
    nrVector3 tdelta;
    tdelta.x = ( tmax.x - tmin.x ) / ( float )nx;
    tdelta.y = ( tmax.y - tmin.y ) / ( float )ny;
    tdelta.z = ( tmax.z - tmin.z ) / ( float )nz;
    
    nrVector3 p = o + d * t;
    
    int ix = ( int )nrMath::Clamp( ( ( float )nx * ( p.x - p0.x ) / ( p1.x - p0.x ) ), 0.0f, ( float )( nx - 1 ) );
    int iy = ( int )nrMath::Clamp( ( ( float )ny * ( p.y - p0.y ) / ( p1.y - p0.y ) ), 0.0f, ( float )( ny - 1 ) );
    int iz = ( int )nrMath::Clamp( ( ( float )nz * ( p.z - p0.z ) / ( p1.z - p0.z ) ), 0.0f, ( float )( nz - 1 ) );
    
    int istepx;
    int istepy;
    int istepz;
    int istopx;
    int istopy;
    int istopz;
    
    nrVector3 tnext;
    
    if ( d.x > 0 )
    {
        tnext.x = tmin.x + tdelta.x * ( float )( ix + 1 );
        istepx = 1;
        istopx = nx;
    }
    else
    {
        tnext.x = tmin.x + tdelta.x * ( float )( nx - ix );
        istepx = -1;
        istopx = -1;
    }
    
    if ( d.y > 0 )
    {
        tnext.y = tmin.y + tdelta.y * ( float )( iy + 1 );
        istepy = 1;
        istopy = ny;
    }
    else
    {
        tnext.y = tmin.y + tdelta.y * ( float )( ny - iy );
        istepy = -1;
        istopy = -1;
    }
    
    if ( d.z > 0 )
    {
        tnext.z = tmin.z + tdelta.z * ( float )( iz + 1 );
        istepz = 1;
        istopz = nz;
    }
    else
    {
        tnext.z = tmin.z + tdelta.z * ( float )( nz - iz );
        istepz = -1;
        istopz = -1;
    }
    
    for ( ;; )
    {
        if ( tnext.x < tnext.y && tnext.x < tnext.z )
        {
            const nrArray< nrSurface* >& g = m_Grids[ index( ix, iy, iz ) ];
            
            interval.m_Maximum = tnext.x;
            if ( Hit( g, ray, interval, hit ) )
            {
                return true;
            }
            
            t = tnext.x;
            
            tnext.x += tdelta.x;
            
            ix += istepx;
            
            if ( ix == istopx )
            {
                return false;
            }
        }
        else if ( tnext.y < tnext.z )
        {
            const nrArray< nrSurface* >& g = m_Grids[ index( ix, iy, iz ) ];
            
            interval.m_Maximum = tnext.y;
            if ( Hit( g, ray, interval, hit ) )
            {
                return true;
            }
            
            t = tnext.y;
            
            tnext.y += tdelta.y;
            
            iy += istepy;
            
            if ( iy == istopy )
            {
                return false;
            }
        }
        else
        {
            const nrArray< nrSurface* >& g = m_Grids[ index( ix, iy, iz ) ];
            
            interval.m_Maximum = tnext.z;
            if ( Hit( g, ray, interval, hit ) )
            {
                return true;
            }
            
            t = tnext.z;
            
            tnext.z += tdelta.z;
            
            iz += istepz;
            
            if ( iz == istopz )
            {
                return false;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////

nrBound nrSurfaceRGS::Bound( void ) const
{
    return m_Bound;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrSurfaceRGS::Normal( const nrVector3& point ) const
{
    // This function should never be called.
    assert( 0 );
    
    return nrVector3( 0, 0, 0 );
}

////////////////////////////////////////////////////////////////////////////

const nrMaterial* nrSurfaceRGS::Material( void  ) const
{
    // This function should never be called.
    assert( 0 );
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

inline bool nrSurfaceRGS::Hit( const nrArray< nrSurface* >& surfaces, const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    bool hit_something = false;
    
    for ( int i = 0; i < surfaces.Length(); i++ )
    {
        nrSurface* surface = surfaces[ i ];
        
        if ( surface->Hit( ray, interval, hit ) )
        {
            interval.m_Maximum = hit.t;
            hit_something = true;
        }
    }
    
    return hit_something;
}

////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////

nrSurface* nrSurfaceRGS::CreateGrid( const nrArray< nrSurface* >& surfaces )
{
    assert( surfaces.Length() > 0 );
    
    // Compute the bounding volume of the whole list.
    nrBound bound = surfaces[ 0 ]->Bound();
    for ( int i = 1; i < surfaces.Length(); i++ )
    {
        const nrBound& b = surfaces[ i ]->Bound();
        
        if ( b.m_Minimums.x < bound.m_Minimums.x )
        {
            bound.m_Minimums.x = b.m_Minimums.x;
        }
        if ( b.m_Minimums.y < bound.m_Minimums.y )
        {
            bound.m_Minimums.y = b.m_Minimums.y;
        }
        if ( b.m_Minimums.z < bound.m_Minimums.z )
        {
            bound.m_Minimums.z = b.m_Minimums.z;
        }
        
        if ( b.m_Maximums.x > bound.m_Maximums.x )
        {
            bound.m_Maximums.x = b.m_Maximums.x;
        }
        if ( b.m_Maximums.y > bound.m_Maximums.y )
        {
            bound.m_Maximums.y = b.m_Maximums.y;
        }
        if ( b.m_Maximums.z > bound.m_Maximums.z )
        {
            bound.m_Maximums.z = b.m_Maximums.z;
        }
    }
    
    // Compute the number of grid subdivisions in x,y,z.
    nrVector3 length;
    length.x = ( bound.m_Maximums.x - bound.m_Minimums.x );
    length.y = ( bound.m_Maximums.y - bound.m_Minimums.y );
    length.z = ( bound.m_Maximums.z - bound.m_Minimums.z );
    
    float s = nrMath::Pow( ( ( length.x * length.y * length.z ) / surfaces.Length() ), 1.0f / 3.0f );
    
    int nx = ( int )nrMath::Ceil( length.x / s );
    int ny = ( int )nrMath::Ceil( length.y / s );
    int nz = ( int )nrMath::Ceil( length.z / s );
    
    // Allocate the grids.
    nrArray< nrSurface* >* grids = new nrArray< nrSurface* >[ nx * ny * nz ];
    
    g_Progress.Reset( surfaces.Length() );
    
    // Add surfaces to the grids.
    for ( int j = 0; j < surfaces.Length(); j++ )
    {
        nrBound b = surfaces[ j ]->Bound();
        
        b.m_Minimums = b.m_Minimums - bound.m_Minimums;
        b.m_Maximums = b.m_Maximums - bound.m_Minimums;
        
        int minx = ( int )nrMath::Floor( ( float )nx * b.m_Minimums.x / length.x );
        int maxx = ( int )nrMath::Ceil( ( float )nx * b.m_Maximums.x / length.x );
        int miny = ( int )nrMath::Floor( ( float )ny * b.m_Minimums.y / length.y );
        int maxy = ( int )nrMath::Ceil( ( float )ny * b.m_Maximums.y / length.y );
        int minz = ( int )nrMath::Floor( ( float )nz * b.m_Minimums.z / length.z );
        int maxz = ( int )nrMath::Ceil( ( float )nz * b.m_Maximums.z / length.z );
        
        for ( int z = minz; z < maxz; z++ )
        {
            for ( int y = miny; y < maxy; y++ )
            {
                for ( int x = minx; x < maxx; x++ )
                {
                    grids[ index( x, y, z ) ].Add( surfaces[ j ] );
                }
            }
        }
        
        g_Progress.Update();
    }
    
    // Return a new surface.
    nrSurfaceRGS* rgs = new nrSurfaceRGS( bound, grids, nx, ny, nz );
    
    return rgs;
}

////////////////////////////////////////////////////////////////////////////


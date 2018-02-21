////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceBVH.cpp
//
// A class for a bounding volume hierarchy of surfaces.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrSurfaceBVH.h"

#include "nrHit.h"
#include "nrInterval.h"
#include "nrRay.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrSurfaceBVH::nrSurfaceBVH( const nrBound& bound )
{
    m_Left = 0;
    m_Right = 0;
    
    m_Bound = bound;
}

////////////////////////////////////////////////////////////////////////////

nrSurfaceBVH::~nrSurfaceBVH( void )
{
    delete m_Left;
    delete m_Right;
}

////////////////////////////////////////////////////////////////////////////

bool nrSurfaceBVH::Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const
{
    if ( m_Bound.Hit( ray, interval ) )
    {
        assert( m_Left != 0 && m_Right != 0 );
        
        nrHit hit_left;
        nrHit hit_right;
        
        bool hit_something_left = m_Left->Hit( ray, interval, hit_left );
        bool hit_something_right = m_Right->Hit( ray, interval, hit_right );
        
        if ( hit_something_left && hit_something_right )
        {
            if ( hit_left.t < hit_right.t )
            {
                hit = hit_left;
                return true;
            }
            else
            {
                hit = hit_right;
                return true;
            }
        }
        else if ( hit_something_left )
        {
            hit = hit_left;
            return true;
        }
        else if ( hit_something_right )
        {
            hit = hit_right;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

nrBound nrSurfaceBVH::Bound( void ) const
{
    return m_Bound;
}

////////////////////////////////////////////////////////////////////////////

nrVector3 nrSurfaceBVH::Normal( const nrVector3& point ) const
{
    // This function should never be called.
    assert( 0 );
    
    return nrVector3( 0, 0, 0 );
}

////////////////////////////////////////////////////////////////////////////

const nrMaterial* nrSurfaceBVH::Material( void  ) const
{
    // This function should never be called.
    assert( 0 );
    return 0;
}

////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::CompareInX( const void* _a, const void* _b )
{
    const nrSurface*& sa = *( const nrSurface** )_a;
    const nrSurface*& sb = *( const nrSurface** )_b;
    
    const nrBound& a = sa->Bound();
    const nrBound& b = sb->Bound();
    
    if ( a.m_Minimums.x > b.m_Minimums.x )
    {
        return 1;
    }
    else if ( b.m_Minimums.x > a.m_Minimums.x )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::CompareInY( const void* _a, const void* _b )
{
    const nrSurface*& sa = *( const nrSurface** )_a;
    const nrSurface*& sb = *( const nrSurface** )_b;
    
    const nrBound& a = sa->Bound();
    const nrBound& b = sb->Bound();
    
    if ( a.m_Minimums.y > b.m_Minimums.y )
    {
        return 1;
    }
    else if ( b.m_Minimums.y > a.m_Minimums.y )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::CompareInZ( const void* _a, const void* _b )
{
    const nrSurface*& sa = *( const nrSurface** )_a;
    const nrSurface*& sb = *( const nrSurface** )_b;
    
    const nrBound& a = sa->Bound();
    const nrBound& b = sb->Bound();
    
    if ( a.m_Minimums.z > b.m_Minimums.z )
    {
        return 1;
    }
    else if ( b.m_Minimums.z > a.m_Minimums.z )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::Sort( nrArray< nrSurface* >& surfaces )
{
    static int axis = -1;
    axis = ( axis + 1 ) % 3;
    
    if ( axis == 0 )
    {
        surfaces.Sort( CompareInX );
    }
    else if ( axis == 1 )
    {
        surfaces.Sort( CompareInY );
    }
    else
    {
        surfaces.Sort( CompareInZ );
    }
    
    return surfaces.Length() / 2;
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::SplitInX( const nrArray< nrSurface* >& surfaces, float pivot )
{
    int middle = 0;
    
    for ( int i = 0; i < surfaces.Length(); i++ )
    {
        const nrBound& b = surfaces[ i ]->Bound();
        
        if ( ( ( b.m_Maximums.x + b.m_Minimums.x ) / 2.0 ) < pivot )
        {
            nrSurface* t = surfaces[ i ];
            surfaces[ i ] = surfaces[ middle ];
            surfaces[ middle ] = t;

            middle++;
        }
    }
    
    return middle;
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::SplitInY( const nrArray< nrSurface* >& surfaces, float pivot )
{
    int middle = 0;
    
    for ( int i = 0; i < surfaces.Length(); i++ )
    {
        const nrBound& b = surfaces[ i ]->Bound();
        
        if ( ( ( b.m_Maximums.y + b.m_Minimums.y ) / 2.0 ) < pivot )
        {
            nrSurface* t = surfaces[ i ];
            surfaces[ i ] = surfaces[ middle ];
            surfaces[ middle ] = t;

            middle++;
        }
    }
    
    return middle;
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::SplitInZ( const nrArray< nrSurface* >& surfaces, float pivot )
{
    int middle = 0;
    
    for ( int i = 0; i < surfaces.Length(); i++ )
    {
        const nrBound& b = surfaces[ i ]->Bound();
        
        if ( ( ( b.m_Maximums.z + b.m_Minimums.z ) / 2.0 ) < pivot )
        {
            nrSurface* t = surfaces[ i ];
            surfaces[ i ] = surfaces[ middle ];
            surfaces[ middle ] = t;

            middle++;
        }
    }
    
    return middle;
}

////////////////////////////////////////////////////////////////////////////

int nrSurfaceBVH::Split( const nrArray< nrSurface* >& surfaces, const nrBound& bound )
{
    assert( surfaces.Length() >= 2 );
    
    static int axis = -1;
    axis = ( axis + 1 ) % 3;
    
    int split;
    
    if ( axis == 0 )
    {
        split = SplitInX( surfaces, ( bound.m_Maximums.x + bound.m_Minimums.x ) / 2.0f );
    }
    else if ( axis == 1 )
    {
        split = SplitInY( surfaces, ( bound.m_Maximums.y + bound.m_Minimums.y ) / 2.0f );
    }
    else
    {
        split = SplitInZ( surfaces, ( bound.m_Maximums.z + bound.m_Minimums.z ) / 2.0f );
    }
    
    if ( split == 0 || split == surfaces.Length() )
    {
        split = surfaces.Length() / 2;
    }
    
    return split;
}

////////////////////////////////////////////////////////////////////////////

nrSurface* nrSurfaceBVH::CreateTree( nrArray< nrSurface* >& surfaces, bool sort )
{
    assert( surfaces.Length() > 0 );
    
    // If there is only a single surface in the list, return it as a leaf.
    if ( surfaces.Length() == 1 )
    {
        return surfaces[ 0 ];
    }
    
    // Compute the bounding volume of the parent (must enclose all children).
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
    
    nrSurfaceBVH* parent = new nrSurfaceBVH( bound );
    assert( parent );
    
    // Sort/Split the array of surfaces.
    int split;
    
    if ( sort )
    {
        split = Sort( surfaces );
    }
    else
    {
        split = Split( surfaces, bound );
    }
    
    // Generate an array for each child.
    int num_left = split;
    int num_right = surfaces.Length() - num_left;
    
    nrArray< nrSurface* > left( num_left );
    nrArray< nrSurface* > right( num_right );
    
    for ( int l = 0; l < num_left; l++ )
    {
        left.Add( surfaces[ l ] );
    }
    for ( int r = 0; r < num_right; r++ )
    {
        right.Add( surfaces[ num_left + r ] );
    }
    
    // Create the children.
    parent->m_Left = CreateTree( left, sort );
    parent->m_Right = CreateTree( right, sort );
    
    return parent;
}

////////////////////////////////////////////////////////////////////////////

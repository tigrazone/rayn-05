////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceBVH.h
//
// A class for a bounding volume hierarchy of surfaces.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSURFACEBVH_H
#define NRSURFACEBVH_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"
#include "nrSurface.h"
#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrBound;
class nrHit;
class nrInterval;
class nrRay;

////////////////////////////////////////////////////////////////////////////

class nrSurfaceBVH : public nrSurface
{
public:

    virtual ~nrSurfaceBVH( void );

    // Return true if the ray hit the surface, false otherwise.
    virtual bool Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
    // Return the bound of the surface.
    virtual nrBound Bound() const;
    
    // Return the normal to the surface at the point (on the surface).
    virtual nrVector3 Normal( const nrVector3& point ) const;
    
    // Return the material of the surface.
    virtual const nrMaterial* Material( void ) const;
    
    // Create a hierarchy of bounding volumes from a list of surfaces.
    //
    // If the sort parameter is true, a sort will be performed to arrange 
    // the surfaces array instead of a simple split.  The split is 
    // generally much faster to compute and to render.
    static nrSurface* CreateTree( nrArray< nrSurface* >& surfaces, bool sort = false );
    
private:
    
    nrSurfaceBVH( const nrBound& bound );
    
private:
    
    // Compare functions which will sort along given axes.
    static int CompareInX( const void* _a, const void* _b );
    static int CompareInY( const void* _a, const void* _b );
    static int CompareInZ( const void* _a, const void* _b );
    static int Sort( nrArray< nrSurface* >& surfaces );
    
    // Split functions which will split along given axis.
    //
    // Returns the number of elements on the "left" side of the split.
    static int SplitInX( const nrArray< nrSurface* >& surfaces, float pivot );
    static int SplitInY( const nrArray< nrSurface* >& surfaces, float pivot );
    static int SplitInZ( const nrArray< nrSurface* >& surfaces, float pivot );
    static int Split( const nrArray< nrSurface* >& surfaces, const nrBound& bound );
    
private:
    
    nrSurface* m_Left;
    nrSurface* m_Right;
    nrBound m_Bound;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSURFACEBVH_H

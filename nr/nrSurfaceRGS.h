////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceRGS.h
//
// A class for a regular grid subdivision of surfaces.
//
// Nate Robins, February 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSURFACERGS_H
#define NRSURFACERGS_H


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

class nrSurfaceRGS : public nrSurface
{
public:
    
    virtual ~nrSurfaceRGS( void );

    // Return true if the ray hit the surface, false otherwise.
    virtual bool Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
    // Return the bound of the surface.
    virtual nrBound Bound() const;
    
    // Return the normal to the surface at the point (on the surface).
    virtual nrVector3 Normal( const nrVector3& point ) const;
    
    // Return the material of the surface.
    virtual const nrMaterial* Material( void ) const;
    
    // Create a regular grid subdivision from a list of surfaces.
    static nrSurface* CreateGrid( const nrArray< nrSurface* >& surfaces );
    
private:
    
    nrSurfaceRGS( const nrBound& bound, nrArray< nrSurface* >* grids, int nx, int ny, int nz );
    
    // Return true if the ray hit the surfaces, false otherwise.
    bool Hit( const nrArray< nrSurface* >& surfaces, const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
private:
    
    int m_Nx;
    int m_Ny;
    int m_Nz;
    nrArray< nrSurface* >* m_Grids;
    nrBound m_Bound;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSURFACERGS_H

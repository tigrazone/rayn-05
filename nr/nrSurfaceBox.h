////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceBox.h
//
// A class for a box.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSURFACEBOX_H
#define NRSURFACEBOX_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColor.h"
#include "nrSurface.h"
#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrHit;
class nrInterval;
class nrParser;
class nrRay;

////////////////////////////////////////////////////////////////////////////

class nrSurfaceBox : public nrSurface
{
public:
    
    nrSurfaceBox( const nrVector3& p0, const nrVector3& p1, const nrMaterial* material );
    virtual ~nrSurfaceBox( void );
    
    // Return true if the ray hit the box, false otherwise.
    virtual bool Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
    // Return the bound of the surface.
    virtual nrBound Bound() const;
    
    // Return the normal to the surface at the point (on the surface).
    virtual nrVector3 Normal( const nrVector3& point ) const;
    
    // Return the material of the surface.
    virtual const nrMaterial* Material( void ) const;
    
    // Return a new box parsed from a file.  The box directive has the
    // following form:
    // 
    // box ("name") 
    // {
    //   p0 < x y z >
    //   p1 < x y z >
    //   color < r g b >
    // }
    // 
    // elements in ()'s are optional.
    static nrSurfaceBox* Parse( nrParser& parser );
    
public:
    
    nrVector3 m_P0;
    nrVector3 m_P1;

    const nrMaterial* m_Material;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSURFACEBOX_H

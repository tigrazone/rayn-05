////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceSphere.h
//
// A class for a sphere.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSURFACESPHERE_H
#define NRSURFACESPHERE_H


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

class nrSurfaceSphere : public nrSurface
{
public:
    
    nrSurfaceSphere( const nrVector3& center, float radius, const nrMaterial* material );
    virtual ~nrSurfaceSphere( void );
    
    // Return true if the ray hit the sphere, false otherwise.
    virtual bool Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
    // Return the bound of the surface.
    virtual nrBound Bound() const;
    
    // Return the normal to the surface at the point (on the surface).
    virtual nrVector3 Normal( const nrVector3& point ) const;
    
    // Return the material of the surface.
    virtual const nrMaterial* Material( void ) const;
    
    // Return a new sphere parsed from a file.  The sphere directive has the
    // following form:
    // 
    // sphere ("name") 
    // {
    //   center < x y z >
    //   radius r
    //   color < r g b >
    // }
    // 
    // elements in ()'s are optional.
    static nrSurfaceSphere* Parse( nrParser& parser );
    
private:
    
    nrVector3   m_Center;
    float       m_Radius;
    
    const nrMaterial* m_Material;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSURFACESPHERE_H

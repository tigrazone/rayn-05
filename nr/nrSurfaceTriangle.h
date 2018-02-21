////////////////////////////////////////////////////////////////////////////
//
// nrSurfaceTriangle.h
//
// A class for a triangle.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSURFACETRIANGLE_H
#define NRSURFACETRIANGLE_H


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

class nrSurfaceTriangle : public nrSurface
{
public:
    
    nrSurfaceTriangle( const nrVector3& a, const nrVector3& b, const nrVector3& c, const nrMaterial* material );
    virtual ~nrSurfaceTriangle( void );
    
    // Return true if the ray hit the surface, false otherwise.
    virtual bool Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
    // Return the bound of the surface.
    virtual nrBound Bound() const;
    
    // Return the normal to the surface at the point (on the surface).
    virtual nrVector3 Normal( const nrVector3& point ) const;
    
    // Return the material of the surface.
    virtual const nrMaterial* Material( void ) const;
    
    // Return a new triangle parsed from a file.  The triangle directive 
    // has the following form:
    // 
    // triangle ("name") 
    // {
    //   a < x y z >
    //   b < x y z >
    //   c < x y z >
    //   color < r g b >
    // }
    // 
    // elements in ()'s are optional.
    static nrSurfaceTriangle* Parse( nrParser& parser );
    
public:
    
    inline const nrVector3& A() const { return m_A; };
    inline const nrVector3& B() const { return m_B; };
    inline const nrVector3& C() const { return m_C; };
    
private:
    
    nrVector3 m_A;
    nrVector3 m_B;
    nrVector3 m_C;

    const nrMaterial* m_Material;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSURFACETRIANGLE_H

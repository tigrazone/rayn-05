////////////////////////////////////////////////////////////////////////////
//
// nrMaterial.h
//
// A class for materials.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRMATERIAL_H
#define NRMATERIAL_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColor.h"
#include "nrLibrary.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrChannel;
class nrParser;
class nrVector3;

////////////////////////////////////////////////////////////////////////////

class nrMaterial
{
public:
    
    nrMaterial( void );
    ~nrMaterial( void );
    
    // Return the ambient color (at a point on the surface, if applicable).
    nrColor Ambient( const nrVector3& p ) const;
    
    // Return the diffuse color (at a point on the surface, if applicable).
    nrColor Diffuse( const nrVector3& p ) const;
    
    // Return the specular color (at a point on the surface, if applicable).
    nrColor Specular( const nrVector3& p ) const;
    
    // Return a new material parsed from a file.  The material directive 
    // has the following form:
    // 
    // material ("name") 
    // {
    //   ambient < r g b >
    //   diffuse < r g b >
    //   specular < r g b >
    // }
    // 
    // elements in ()'s are optional.
    static nrMaterial* Parse( nrParser& parser );
    
    // Return a new material parsed from a color directive.  The color 
    // directive has the following form:
    // 
    // color < r g b >
    static nrMaterial* nrMaterial::ParseColor( nrParser& parser );
    
    // Set/Get the material name.
    void SetName( const char* name );
    const char* GetName( void ) const;
    
public:
    
    char* m_Name;
	
    nrChannel* m_Ambient;
	nrChannel* m_Diffuse;
	nrChannel* m_Specular;
	
    static nrLibrary<nrMaterial> m_Library;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRMATERIAL_H

////////////////////////////////////////////////////////////////////////////
//
// nrScene.h
//
// A class for Scenes.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSCENE_H
#define NRSCENE_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"
#include "nrColor.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrHit;
class nrInterval;
class nrRay;
class nrLight;
class nrSurface;
class nrVector3;
class nrView;

////////////////////////////////////////////////////////////////////////////

class nrScene
{
public:
    
    nrScene( void );
    ~nrScene( void );
    
    // Return true if the ray hit something in the scene in the interval.
    bool Hit( const nrRay& ray, nrInterval& interval, nrHit& hit ) const;
    
    // Adds a surface to the scene.
    //void Add( nrSurface* surface );
    
    // Parse a scene file.
    bool Parse( const char* scene_file );
    
    // Create a bounding volume hierarchy with the surfaces in the scene.
    // See nrSurfaceBVH::CreateTree() for information on the sort parameter.
    void CreateBVH( bool sort = false );
    
    // Create a regular grid subdivision of the surfaces in the scene.
    void CreateRGS( void );
    
    // Cull backfacing triangles?
    void CullBackfaces( bool cull = true );
    
    // Return the scene ambient illumination.  The ambient is specified in
    // the scene file as follows:
    // 
    // ambient < r g b >
    // 
    // The ambient defaults to < 0.0 0.0 0.0 >
    nrColor& Ambient( void );
    
    // Return the scene background color.  The background color is 
    // specified in the scene file as follows:
    // 
    // background < r g b >
    // 
    // The background defaults to < 0.0 0.0 0.0 >
    nrColor& Background( void );
    
public:
    
    nrArray<nrLight*>   m_Lights;
    nrArray<nrSurface*> m_Surfaces;
    nrView*             m_View;
    
    nrSurface*          m_BVH;
    nrSurface*          m_RGS;
    
    nrColor				m_Ambient;
    nrColor				m_Background;
    
    bool m_Cull;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSCENE_H

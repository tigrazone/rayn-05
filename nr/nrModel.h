////////////////////////////////////////////////////////////////////////////
//
// nrModel.h
//
// Nate Robins, January 2002.
//
// Model class.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRMODEL_H
#define NRMODEL_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"
#include "nrColor.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrVector2;
class nrVector3;
class nrModel;

////////////////////////////////////////////////////////////////////////////

class nrmVertex
{
public:
    int m_PositionIndex;    // Index into nrModel positions list.
    int m_NormalIndex;      // Index into nrModel normals list.
    int m_ColorIndex;       // Index into nrModel colors list.
    int m_TexCoordIndex;    // Index into nrModel texcoords list.
};

////////////////////////////////////////////////////////////////////////////

class nrmTriangle
{
public:
    nrmTriangle( int p0 = -1, int c0 = -1, int t0 = -1, int n0 = -1, int p1 = -1, int c1 = -1, int t1 = -1, int n1 = -1, int p2 = -1, int c2 = -1, int t2 = -1, int n2 = -1, nrModel* model = 0 );
    ~nrmTriangle();
    
    // Return the n'th (0,1,2) position/etc of the triangle.
    const nrVector3&   Position( int n ) const;
    const nrVector3&   Normal( int n ) const;
    const nrColor&     Color( int n ) const;
    const nrVector3&   TexCoord( int n ) const;
    const nrVector3&   FaceNormal( void ) const;
    
public:
    nrModel*  m_Model;               // Model this triangle is in.
    nrmVertex m_Vertices[ 3 ];       // Array of vertices for the triangle.
    int       m_FaceNormalIndex;     // Index into nrModel facenormals list.
};

////////////////////////////////////////////////////////////////////////////

class nrmMaterial
{
public:
    nrmMaterial( const char* name = 0, nrColor diffuse = nrColor( 0.8f, 0.8f, 0.8f ), nrColor ambient = nrColor( 0.2f, 0.2f, 0.2f ), nrColor specular = nrColor( 0.0f, 0.0f, 0.0f ), float shininess = 0.0f );
    ~nrmMaterial();
    
public:
    // Set/Get the name of the group.
    void SetName( const char* name );
    const char* GetName( void ) const;
    
public:
    char*   m_Name;         // Name of the material
    nrColor m_Diffuse;      // Diffuse color
    nrColor m_Ambient;      // Ambient color
    nrColor m_Specular;     // Specular color
    float   m_Shininess;    // Specular exponent.
};

////////////////////////////////////////////////////////////////////////////

class nrmGroup
{
public:
    nrmGroup( const char* name = 0, nrModel* model = 0 );
    ~nrmGroup();
    
public:
    // Set/Get the name of the group.
    void SetName( const char* name );
    const char* GetName( void ) const;
    
    // Return the number of triangles in the group.
    int NumTriangles( void ) const;
    
    // Return the n'th triangle in the group.
    const nrmTriangle& Triangle( int n ) const;
    
    // Return the group's material.
    const nrmMaterial& Material( void ) const;
    
public:
    nrModel*       m_Model;                // Model this group is in.
    char*          m_Name;                 // Name of the group.
    nrArray< int > m_TriangleIndices;      // Indices into nrModel triangles list.
    int            m_MaterialIndex;        // Index into nrModel materials list.
};

////////////////////////////////////////////////////////////////////////////

class nrModel
{
public:

    nrModel();
    ~nrModel();
    
public:
    // Read a model file.
    //
    // The type of model file is determined by the file_name extension.
    // Supported types:
    //
    // OBJ - Wavefront OBJ file format.
    // 3DS - 3D Studio 3DS file format.
    // 
    // If an error occurs, a message is posted to stderr and 0 is returned.
    static nrModel* NewFromFile( const char* file_name );
    
    // Returns the dimensions of the model.
    void Dimensions( nrVector3& mins, nrVector3& maxs ) const;
    
    // Creates a face normal for each triangle in the model.
    void CreateFaceNormals( void );

    // Creates smooth normals across the model.
    void CreateNormals( void );
    
    // Creates colors (from normals) across the model.
    void CreateColors( void );
    
    // Set/Get the models name.
    void SetName( const char* name );
    const char* GetName( void ) const;
    
    // Returns the number of vectors/etc in the model.
    int NumPositions( void ) const;
    int NumNormals( void ) const;
    int NumColors( void ) const;
    int NumTexCoords( void ) const;
    int NumFaceNormals( void ) const;
    int NumTriangles( void ) const;
    int NumMaterials( void ) const;
    int NumGroups( void ) const;
    
    // Returns the n'th position/etc in the model.
    const nrVector3&   Position( int n ) const;
    const nrVector3&   Normal( int n ) const;
    const nrColor&     Color( int n ) const;
    const nrVector3&   TexCoord( int n ) const;
    const nrVector3&   FaceNormal( int n ) const;
    const nrmTriangle& Triangle( int n ) const;
    const nrmMaterial& Material( int n ) const;
    const nrmGroup&    Group( int n ) const;
    
public:
    nrArray< nrVector3 > m_Positions;       // Array of positions.
    nrArray< nrColor >   m_Colors;          // Array of colors.
    nrArray< nrVector3 > m_Normals;         // Array of normals.
    nrArray< nrVector3 > m_TexCoords;       // Array of texture coordinates.
    
    nrArray< nrmTriangle* > m_Triangles;    // Array of triangles.
    nrArray< nrVector3 >  m_FaceNormals;    // Array of face normals.
    
    nrArray< nrmGroup* > m_Groups;          // Array of groups.
    
    nrArray< nrmMaterial* > m_Materials;    // Array of materials.
    
public:
    char* m_Name;                           // Name of the model.
    char* m_Path;                           // Path of the model (directory).
};

////////////////////////////////////////////////////////////////////////////

#endif        // NRMODEL_H

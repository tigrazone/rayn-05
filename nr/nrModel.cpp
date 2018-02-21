////////////////////////////////////////////////////////////////////////////
//
// nrModel.cpp
//
// Nate Robins, November 2001.
//
// Model class.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrModel.h"

#include "nrVector2.h"
#include "nrVector3.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////

// nrmTriangle //////////////////////////////////////////////////////////////

nrmTriangle::nrmTriangle( int v0, int c0, int t0, int n0, int v1, int c1, int t1, int n1, int v2, int c2, int t2, int n2, nrModel* model )
{
    m_Model = model;
    
    m_Vertices[ 0 ].m_PositionIndex = v0;
    m_Vertices[ 0 ].m_ColorIndex    = c0;
    m_Vertices[ 0 ].m_TexCoordIndex = t0;
    m_Vertices[ 0 ].m_NormalIndex   = n0;
    
    m_Vertices[ 1 ].m_PositionIndex = v1;
    m_Vertices[ 1 ].m_ColorIndex    = c1;
    m_Vertices[ 1 ].m_TexCoordIndex = t1;
    m_Vertices[ 1 ].m_NormalIndex   = n1;
    
    m_Vertices[ 2 ].m_PositionIndex = v2;
    m_Vertices[ 2 ].m_ColorIndex    = c2;
    m_Vertices[ 2 ].m_TexCoordIndex = t2;
    m_Vertices[ 2 ].m_NormalIndex   = n2;
    
    m_FaceNormalIndex = -1;
}

////////////////////////////////////////////////////////////////////////////

nrmTriangle::~nrmTriangle()
{
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrmTriangle::Position( int n ) const
{
    assert( n >= 0 );
    assert( n < 3 );
    assert( m_Model != NULL );
    
    int i = m_Vertices[ n ].m_PositionIndex;
    
    assert( i >= 0 );
    assert( i < m_Model->m_Positions.Length() );
    
    return m_Model->m_Positions[ i ];
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrmTriangle::Normal( int n ) const
{
    assert( n >= 0 );
    assert( n < 3 );
    assert( m_Model != NULL );
    
    int i = m_Vertices[ n ].m_NormalIndex;
    
    assert( i >= 0 );
    assert( i < m_Model->m_Normals.Length() );
    
    return m_Model->m_Normals[ i ];
}

////////////////////////////////////////////////////////////////////////////

const nrColor& nrmTriangle::Color( int n ) const
{
    assert( n >= 0 );
    assert( n < 3 );
    assert( m_Model != NULL );
    
    int i = m_Vertices[ n ].m_ColorIndex;
    
    assert( i >= 0 );
    assert( i < m_Model->m_Colors.Length() );
    
    return m_Model->m_Colors[ i ];
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrmTriangle::TexCoord( int n ) const
{
    assert( n >= 0 );
    assert( n < 3 );
    assert( m_Model != NULL );
    
    int i = m_Vertices[ n ].m_TexCoordIndex;
    
    assert( i >= 0 );
    assert( i < m_Model->m_TexCoords.Length() );
    
    return m_Model->m_TexCoords[ i ];
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrmTriangle::FaceNormal( void ) const
{
    assert( m_Model != NULL );
    
    int i = m_FaceNormalIndex;
    
    assert( i >= 0 );
    assert( i < m_Model->m_FaceNormals.Length() );
    
    return m_Model->m_FaceNormals[ i ];
}

////////////////////////////////////////////////////////////////////////////
// nrMaterial
////////////////////////////////////////////////////////////////////////////

nrmMaterial::nrmMaterial( const char* name, nrColor diffuse, nrColor ambient, nrColor specular, float shininess )
{
    m_Name = 0;
    m_Diffuse = diffuse;
    m_Ambient = ambient;
    m_Specular = specular;
    m_Shininess = shininess;
    
    if ( name != 0 )
    {
        SetName( name );
    }
}

////////////////////////////////////////////////////////////////////////////

nrmMaterial::~nrmMaterial()
{
    delete [] m_Name;
}

////////////////////////////////////////////////////////////////////////////

void nrmMaterial::SetName( const char* name )
{
    assert( name );
    
    delete [] m_Name;
    m_Name = new char[ strlen( name ) + 1 ];
    if ( m_Name == NULL )
    {
        printf( "nrmMaterial::SetName(): Out of memory.\n" );
    }
    else
    {
        strcpy( m_Name, name );
    }
}

////////////////////////////////////////////////////////////////////////////

const char* nrmMaterial::GetName( void ) const
{
    return m_Name;
}

////////////////////////////////////////////////////////////////////////////
// nrGroup
////////////////////////////////////////////////////////////////////////////

nrmGroup::nrmGroup( const char* name, nrModel* model )
{
    m_Name = 0;
    m_Model = model;
    m_MaterialIndex = 0;
    
    if ( name != 0 )
    {
        SetName( name );
    }
}

////////////////////////////////////////////////////////////////////////////

nrmGroup::~nrmGroup()
{
    delete [] m_Name;
}

////////////////////////////////////////////////////////////////////////////

void nrmGroup::SetName( const char* name )
{
    assert( name );
    
    delete [] m_Name;
    m_Name = new char[ strlen( name ) + 1 ];
    if ( m_Name == NULL )
    {
        printf( "nrGroup::SetName(): Out of memory.\n" );
    }
    else
    {
        strcpy( m_Name, name );
    }
}

////////////////////////////////////////////////////////////////////////////

const char* nrmGroup::GetName( void ) const
{
    return m_Name;
}

////////////////////////////////////////////////////////////////////////////

int nrmGroup::NumTriangles( void ) const
{
    return m_TriangleIndices.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrmTriangle& nrmGroup::Triangle( int n ) const
{
    assert( n >= 0 );
    assert( n < NumTriangles() );
    assert( m_Model != NULL );
    
    int i = m_TriangleIndices[ n ];
    
    assert( i >= 0 );
    assert( i < m_Model->m_Triangles.Length() );
    
    return *( m_Model->m_Triangles[ i ] );
}

////////////////////////////////////////////////////////////////////////////

const nrmMaterial& nrmGroup::Material( void ) const
{
    assert( m_MaterialIndex >= 0 );
    assert( m_MaterialIndex < m_Model->NumMaterials() );
    
    return *( m_Model->m_Materials[ m_MaterialIndex ] );
}

////////////////////////////////////////////////////////////////////////////
// nrModel
////////////////////////////////////////////////////////////////////////////

nrModel::nrModel()
{
    m_Name = 0;
    m_Path = 0;
}

////////////////////////////////////////////////////////////////////////////

nrModel::~nrModel()
{
    delete [] m_Name;
    delete [] m_Path;
    
    int i;
    for ( i = 0; i < NumTriangles(); i++ )
    {
        delete m_Triangles[ i ];
    }
    
    for ( i = 0; i < NumMaterials(); i++ )
    {
        delete m_Materials[ i ];
    }
    
    for ( i = 0; i < NumGroups(); i++ )
    {
        delete m_Groups[ i ];
    }
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumPositions( void ) const
{
    return m_Positions.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrModel::Position( int n ) const
{
    assert( n >= 0 && n < NumPositions() );
    
    return m_Positions[ n ];
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumNormals( void ) const
{
    return m_Normals.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrModel::Normal( int n ) const
{
    assert( n >= 0 && n < NumNormals() );
    
    return m_Normals[ n ];
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumColors( void ) const
{
    return m_Colors.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrColor& nrModel::Color( int n ) const
{
    assert( n >= 0 && n < NumColors() );
    
    return m_Colors[ n ];
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumTexCoords( void ) const
{
    return m_TexCoords.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrModel::TexCoord( int n ) const
{
    assert( n >= 0 && n < NumTexCoords() );
    
    return m_TexCoords[ n ];
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumFaceNormals( void ) const
{
    return m_FaceNormals.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrVector3& nrModel::FaceNormal( int n ) const
{
    assert( n >= 0 && n < NumFaceNormals() );
    
    return m_FaceNormals[ n ];
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumTriangles( void ) const
{
    return m_Triangles.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrmTriangle& nrModel::Triangle( int n ) const
{
    assert( n >= 0 && n < NumTriangles() );
    
    return *( m_Triangles[ n ] );
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumMaterials( void ) const
{
    return m_Materials.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrmMaterial& nrModel::Material( int n ) const
{
    assert( n >= 0 && n < NumMaterials() );
    
    return *( m_Materials[ n ] );
}

////////////////////////////////////////////////////////////////////////////

int nrModel::NumGroups( void ) const
{
    return m_Groups.Length();
}

////////////////////////////////////////////////////////////////////////////

const nrmGroup& nrModel::Group( int n ) const
{
    assert( n >= 0 && n < NumGroups() );
    
    return *( m_Groups[ n ] );
}

////////////////////////////////////////////////////////////////////////////

void nrModel::Dimensions( nrVector3& mins, nrVector3& maxs ) const
{
    mins.x = 2e31f;
    mins.y = 2e31f;
    mins.z = 2e31f;
    
    maxs.x = -2e31f;
    maxs.y = -2e31f;
    maxs.z = -2e31f;
    
    for ( int i = 0; i < NumPositions(); i++ )
    {
        const nrVector3& v = Position( i );
        
        if ( v.x < mins.x )
        {
            mins.x = v.x;
        }
        if ( v.x > maxs.x )
        {
            maxs.x = v.x;
        }
        
        if ( v.y < mins.y )
        {
            mins.y = v.y;
        }
        if ( v.y > maxs.y )
        {
            maxs.y = v.y;
        }
        
        if ( v.z < mins.z )
        {
            mins.z = v.z;
        }
        if ( v.z > maxs.z )
        {
            maxs.z = v.z;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel::CreateFaceNormals( void )
{
    // Clobber any old face normals.
    m_FaceNormals.Clear();
    
    // Compute face normals.
    for ( int i = 0; i < NumTriangles(); i++ )
    {
        nrmTriangle& t = *( m_Triangles[ i ] );
        
        t.m_FaceNormalIndex = i;
        
        const nrVector3& v0 = t.Position( 0 );
        const nrVector3& v1 = t.Position( 1 );
        const nrVector3& v2 = t.Position( 2 );
        
        nrVector3 u = v1 - v0;
        nrVector3 v = v2 - v0;
        nrVector3 n = u.Cross( v ).Unit();
        
        m_FaceNormals.Add( n );
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel::CreateNormals( void )
{
    // Clobber any old normals.
    m_Normals.Clear();
    
    // Must have facet normals to create vertex normals.
    if ( NumFaceNormals() == 0 )
    {
        CreateFaceNormals();
    }
    
    // Allocate memory for a structure to speed up the search for the 
    // triangles that a vector belongs to.  (record all of the triangles 
    // that each vector is in).
    typedef struct 
    {
        int num_triangles;
        int total_triangles;
        int* triangles;
    } optimize;
    
    optimize* speed = new optimize[ NumPositions() ];
    memset( speed, 0, sizeof( optimize ) * NumPositions() );
    
    // Populate the optimized structure.
    int i;
    for ( i = 0; i < NumTriangles(); i++ )
    {
        nrmTriangle& t = *( m_Triangles[ i ] );
        
        for ( int j = 0; j < 3; j++ )
        {
            optimize& s = speed[ t.m_Vertices[ j ].m_PositionIndex ];
            
            if ( s.num_triangles + 1 > s.total_triangles )
            {
                int* triangles = s.triangles;
                
                s.total_triangles += 5;
                s.triangles = new int[ s.total_triangles ];
                if ( s.triangles == NULL )
                {
                    printf( "nrModel::CreateNormals(): Out of memory.\n" );
                    return;
                }
                memcpy( s.triangles, triangles, s.num_triangles * sizeof ( int ) );
                delete [] triangles;
                
                s.triangles[ s.num_triangles ] = i;
                s.num_triangles++;
            }
            else
            {
                s.triangles[ s.num_triangles ] = i;
                s.num_triangles++;
            }
        }
    }
    
    // Average normals.
    for ( i = 0; i < NumPositions(); i++ )
    {
        nrVector3 n = nrVector3( 0.0f, 0.0f, 0.0f );
        optimize& s = speed[ i ];
        
        for ( int j = 0; j < s.num_triangles; j++ )
        {
            nrmTriangle& t = *( m_Triangles[ s.triangles[ j ] ] );
            nrVector3&   fn = m_FaceNormals[ t.m_FaceNormalIndex ];
            
            n = n + fn;
        }
        
        n = n.Unit();
        
        m_Normals[ i ] = n;
    }
    
    // Delete the optimized structure
    for ( i = 0; i < NumPositions(); i++ )
    {
        optimize& s = speed[ i ];
        
        delete [] s.triangles;
    }
    delete [] speed;
    
    // This is the old, elegant, but slow (no, really slow) way, which 
    // the above supercedes.
    /*
    for ( int i = 0; i < m_NumVectors; i++ )
    {
        nrVector3 n = nrVector3( 0.0f, 0.0f, 0.0f );
        
        for ( int j = 0; j < NumTriangles(); j++ )
        {
            nrTriangle& t   = Triangle( j );
            nrVector3&   fn = FaceNormal( t.m_FaceNormalIndex );
            
            if ( t.m_Vertices[ 0 ].m_VectorIndex == i ||
                t.m_Vertices[ 1 ].m_VectorIndex == i ||
                t.m_Vertices[ 2 ].m_VectorIndex == i )
            {
                n = n + fn;
            }
        }
        
        n.Normalize();
        
        m_Normals[ i ] = n;
        
        if ( current % 100 == 0 )
        {
            Working( ( float )current / ( float )total );
        }
        
        current++;
    }
    */
    
    // Re-index the normals.
    for( i = 0; i < NumTriangles(); i++ )
    {
        nrmTriangle& t = *( m_Triangles[ i ] );
        
        t.m_Vertices[ 0 ].m_NormalIndex = t.m_Vertices[ 0 ].m_PositionIndex;
        t.m_Vertices[ 1 ].m_NormalIndex = t.m_Vertices[ 1 ].m_PositionIndex;
        t.m_Vertices[ 2 ].m_NormalIndex = t.m_Vertices[ 2 ].m_PositionIndex;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel::CreateColors( void )
{
    // Clobber any old colors.
    m_Colors.Clear();
    
    // Must have normals to create colors.
    if ( NumNormals() == 0 )
    {
        CreateNormals();
    }
    
    // Compute the colors.
    int i;
    for ( i = 0; i < m_Normals.Length(); i++ )
    {
        nrVector3& n = m_Normals[ i ];
        
        n = ( n + nrVector3( 1.0f, 1.0f, 1.0f ) ) / 2.0f;
        
        m_Colors.Add( nrColor( n.x, n.y, n.z ) );
    }
    
    // Re-index the colors.
    for( i = 0; i < NumTriangles(); i++ )
    {
        nrmTriangle& t = *( m_Triangles[ i ] );
        
        t.m_Vertices[ 0 ].m_ColorIndex = t.m_Vertices[ 0 ].m_PositionIndex;
        t.m_Vertices[ 1 ].m_ColorIndex = t.m_Vertices[ 1 ].m_PositionIndex;
        t.m_Vertices[ 2 ].m_ColorIndex = t.m_Vertices[ 2 ].m_PositionIndex;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel::SetName( const char* name )
{
    assert( name );
    
    delete [] m_Name;
    m_Name = new char[ strlen( name ) + 1 ];
    if ( m_Name == NULL )
    {
        printf( "nrModel::SetName(): Out of memory.\n" );
    }
    else
    {
        strcpy( m_Name, name );
    }
}

////////////////////////////////////////////////////////////////////////////

const char* nrModel::GetName( void ) const
{
    return m_Name;
}

////////////////////////////////////////////////////////////////////////////

#include "nrModel3DS.h"
#include "nrModelOBJ.h"

nrModel* nrModel::NewFromFile( const char* file_name )
{
    nrModel* model = 0;
    
    fprintf( stderr, "Loading file \"%s\".\n", file_name );
    
    char* ext = strrchr( file_name, '.' );

    if ( stricmp( ext, ".obj" ) == 0 )
    {
        model = nrModelOBJ::NewFromFile( file_name );
    }
    else if ( stricmp( ext, ".3ds" ) == 0 )
    {
        model = nrModel3DS::NewFromFile( file_name );
    }
    else
    {
        fprintf( stderr, "nrModel::NewFromFile(): Unrecognized model type (%s).\n", ext );
    }
    
    if ( model )
    {
        return model;
    }
    else
    {
        fprintf( stderr, "Load failed.\n" );
        return NULL;
    }
}

////////////////////////////////////////////////////////////////////////////

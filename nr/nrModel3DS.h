////////////////////////////////////////////////////////////////////////////
//
// nrModel3DS.h
//
// Nate Robins, February 2002.
//
// See http://astronomy.swin.edu.au/pbourke/3dformats/3ds/
//
// 3D Studio 3DS model file format reader.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRMODEL3DS_H
#define NRMODEL3DS_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColor.h"

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrVector2;
class nrVector3;
class nrmTriangle;
class nrmGroup;
class nrmMaterial;
class nrModel;

////////////////////////////////////////////////////////////////////////////

class nrChunker;

class nrModel3DS
{
public:
    // Read a Wavefront 3DS model file.
    static nrModel* NewFromFile( const char* file_name );
    
private:
    nrModel3DS();
    ~nrModel3DS();
    
private:
    // Chunk processing functions.
    void Chunk_4D4D( nrChunker& chunker );
    void Chunk_3D3D( nrChunker& chunker );
    void Chunk_4000( nrChunker& chunker );
    void Chunk_4100( nrChunker& chunker );
    void Chunk_4110( nrChunker& chunker );
    void Chunk_4120( nrChunker& chunker );
    void Chunk_4130( nrChunker& chunker );
    void Chunk_AFFF( nrChunker& chunker );
    void Chunk_A000( nrChunker& chunker );
    
    // Chunk class processing functions.
    nrColor Chunk_Color( nrChunker& chunker );
    float Chunk_Percent( nrChunker& chunker );
    void Chunk_Texture( nrChunker& chunker );
    
    // Find utilities.
    int FindMaterial( const char* name ) const;

private:
    nrModel* m_Model;           // Model that we're populating.
    FILE* m_File;               // File from which model is being populated.

    char* m_Name;               // Name of the current object being parsed.

    nrmGroup* m_Group;          // Group currently being parsed.
    int m_BaseVertex;           // Index into vertex list of base vertex in group.
    int m_BaseTriangle;         // Index into triangle list of base triangle in group.

    nrmMaterial* m_Material;    // Material currently being parsed.

    int m_TotalBytes;           // Total number of bytes to be read.
    int m_NumBytes;             // Current number of bytes read.
};

////////////////////////////////////////////////////////////////////////////

#endif    // NRMODEL3DS_H

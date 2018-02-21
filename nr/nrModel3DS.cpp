////////////////////////////////////////////////////////////////////////////
//
// nrModel3DS.cpp
//
// Nate Robins, February 2002.
//
// See http://astronomy.swin.edu.au/pbourke/3dformats/3ds/
//
// 3D Studio 3DS model file format reader.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrModel3DS.h"
#include "nrModelMTL.h"

#include "nrArray.h"
#include "nrModel.h"
#include "nrVector2.h"
#include "nrVector3.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <errno.h>


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrChunker
{
public:

    inline nrChunker( void );
    inline ~nrChunker( void );
    
    // Open a stream (file) for chunking.
    bool Open( const char* file_name );
    
    // Close the stream (file) being chunked.
    void Close( void );
    
    // Prepare the chunker for the next chunk.
    // 
    // If the eof is reached, false is returned.
    // If an error occurred false is returned and an error message is 
    // printed to stderr.
    // Otherwise, true is returned.
    inline bool Next( void );
    
    // Skip the current chunk.
    inline void Skip( void );
    
    // Reads a string from the stream (read up to '\0').  The returned 
    // string should be delete[]'d.
    // 
    // If an error occurs, 0 is returned.
    inline char* ReadString( void );
    
    // Reads the next 2 bytes of the file as a short.
    inline short ReadShort( void );
    
    // Reads the next 4 bytes of the file as an integer.
    inline int ReadInt( void );
    
    // Reads the next 4 bytes of the file as a float.
    inline float ReadFloat( void );
    
    // Reads the next n bytes of the file as a bytes.  The returned data 
    // should be delete[]'d.
    inline unsigned char* ReadBytes( int n );
    
    // Returns the number of bytes read so far.
    inline int Bytes( void ) const;
    
    // Return the ID of the current chunk.
    inline unsigned short ID( void ) const;
    
    // Return the length (in bytes) of the data in the current chunk.
    inline unsigned short Length( void ) const;
    
    // Return the data of the current chunk (should be delete[]'d).
    inline unsigned char* Data( void );
    
private:
    
    FILE* m_File;
    
    unsigned short m_ID;
    unsigned int   m_Length;
    unsigned char* m_Data;
    
    unsigned int m_Byte;
    unsigned int m_Bytes;
};

////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////

nrChunker::nrChunker( void )
{
    m_File = 0;

    m_ID = 0;
    m_Length = 0;
    m_Data = 0;

    m_Byte = 0;
    m_Bytes = 0;
}

////////////////////////////////////////////////////////////////////////////

nrChunker::~nrChunker( void )
{
}

////////////////////////////////////////////////////////////////////////////

bool nrChunker::Open( const char* file_name )
{
    m_File = fopen( file_name, "rb" );
    if ( m_File == 0 )
    {
        return false;
    }
    
    m_Byte = 0;
    
    fseek( m_File, 0, SEEK_END );
    m_Bytes = ftell( m_File );
    fseek( m_File, 0, SEEK_SET );
    
    m_ID = 0;
    m_Length = 0;
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

void nrChunker::Close( void )
{
    fclose( m_File );
    m_File = 0;
}

////////////////////////////////////////////////////////////////////////////

int nrChunker::Bytes( void ) const
{
    return m_Byte;
}

////////////////////////////////////////////////////////////////////////////

unsigned short nrChunker::ID( void ) const
{
    return m_ID;
}

////////////////////////////////////////////////////////////////////////////

unsigned short nrChunker::Length( void ) const
{
    return m_Length;
}

////////////////////////////////////////////////////////////////////////////

unsigned char* nrChunker::Data( void )
{
    m_Data = new unsigned char[ m_Length ];
    if ( m_Data == 0 )
    {
        fprintf( stderr, "nrChunker::Data(): Out of memory.\n" );
        assert( 0 );
        return 0;
    }
    
    if ( fread( m_Data, m_Length, 1, m_File ) != 1 )
    {
        fprintf( stderr, "nrChunker::Data(): Error reading chunk data (file may be truncated).\n" );
        delete [] m_Data;
        assert( 0 );
        return 0;
    }
    
    m_Byte += m_Length;
    
    return m_Data;
}

////////////////////////////////////////////////////////////////////////////

char* nrChunker::ReadString( void )
{
    assert( m_File );
    nrArray< char > string( 256 );
    
    int c;
    while ( ( c = getc( m_File ) ) != '\0' )
    {
        string.Add( c );
    }
    
    char* s = new char[ string.Length() + 1 ];
    
    int i;
    for ( i = 0; i < string.Length(); i++ )
    {
        s[ i ] = string[ i ];
    }
    s[ i ] = 0;
    
    m_Byte += i + 1;
    
    return s;
}

////////////////////////////////////////////////////////////////////////////

short nrChunker::ReadShort( void )
{
    assert( m_File );
    
    short s;
    
    fread( &s, 2, 1, m_File );
    
    m_Byte += 2;
    
    return s;
}

////////////////////////////////////////////////////////////////////////////

int nrChunker::ReadInt( void )
{
    assert( m_File );
    
    int i;
    
    fread( &i, 4, 1, m_File );
    
    m_Byte += 4;
    
    return i;
}

////////////////////////////////////////////////////////////////////////////

float nrChunker::ReadFloat( void )
{
    assert( m_File );
    
    float f;
    
    fread( &f, 4, 1, m_File );
    
    m_Byte += 4;
    
    return f;
}

////////////////////////////////////////////////////////////////////////////

bool nrChunker::Next( void )
{
    assert( m_File );
    
    if ( fread( &( m_ID ), 2, 1, m_File ) != 1 )
    {
        if ( feof( m_File ) )
        {
            return false;
        }
        else
        {
            fprintf( stderr, "nrChunker::Next(): %s\n", strerror( errno ) );
            return false;
        }
    }
    
    if ( fread( &( m_Length ), 4, 1, m_File ) != 1 )
    {
        fprintf( stderr, "nrChunker::Next(): Error reading chunk length (file may be truncated).\n" );
        return false;
    }
    
    // Already read 6 bytes from this chunk (ID == 2, Length == 4), so make
    // sure we've got a length of at least 6, and then subtract from the length.
    if ( m_Length < 6 )
    {
        fprintf( stderr, "nrChunker::Next(): Invalid chunk length (file may be corrupt).\n" );
        return false;
    }
    
    m_Length -= 6;
    m_Byte += 6;
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

void nrChunker::Skip( void )
{
    fseek( m_File, m_Length, SEEK_CUR );
    
    m_Byte += m_Length;
}


////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////

nrModel3DS::nrModel3DS()
{
    m_Name = 0;
}

////////////////////////////////////////////////////////////////////////////

nrModel3DS::~nrModel3DS()
{
}

////////////////////////////////////////////////////////////////////////////

#define NEW( ptr, type, num ) \
    ptr = new type[ num ]; \
    \
    if ( ptr == NULL ) \
    { \
        fprintf( stderr, "nrModel3DS::NewFromFile(): Out of memory.\n" ); \
        delete m_Model; \
        return NULL; \
    }

////////////////////////////////////////////////////////////////////////////

#undef NEW

////////////////////////////////////////////////////////////////////////////

nrModel* nrModel3DS::NewFromFile( const char* file_name )
{
    nrModel3DS l;
    
    nrChunker chunker;
    
    if ( ! chunker.Open( file_name ) )
    {
        fprintf( stderr, "nrModel3DS::NewFromFile(): Unable to open file \"%s\".\n", file_name );
        return 0;
    }
    
    nrModel* m;
    
    chunker.Next();
    if ( chunker.ID() != 0x4D4D )
    {
        fprintf( stderr, "nrModel3DS::NewFromFile(): Invalid 3DS file \"%s\".\n", file_name );
    }
    
    m = new nrModel;
    if ( m == 0 )
    {
        fprintf( stderr, "nrModel3DS::NewFromFile(): Out of memory.\n" );
    }
    else
    {
        l.m_Model = m;
        l.Chunk_4D4D( chunker );
    }
    
    chunker.Close();
    
    return m;
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_4D4D( nrChunker& chunker )
{
    // Add a default material to the model.
    m_Model->m_Materials.Add( new nrmMaterial( "default" ) );
    
    int bytes = chunker.Bytes() + chunker.Length();
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0x3D3D:    // 3D Editor chunk
            Chunk_3D3D( chunker );
            break;
            
        case 0x0002:    // 3DS version
        case 0xB000:    // Keyframer chunk
            fprintf( stderr, "nrModel3DS::Chunk_4D4D(): Chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_4D4D(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_3D3D( nrChunker& chunker )
{
    int bytes = chunker.Bytes() + chunker.Length();
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0x4000:    // Object block
            Chunk_4000( chunker );
            break;
            
        case 0xAFFF:    // Material block
            Chunk_AFFF( chunker );
            break;
            
        case 0x0100:    // One unit

        case 0x1100:    // Background bitmap
        case 0x1101:    // Use background bitmap

        case 0x1200:    // Background color
        case 0x1201:    // Use background color

        case 0x1300:    // Gradient colors
        case 0x1301:    // Use gradient

        case 0x1400:    // Shadow map bias
        case 0x1420:    // Shadow map size
        case 0x1450:    // Shadow map sample range

        case 0x1460:    // Raytrace bias
        case 0x1470:    // Raytrace on

        case 0x2100:    // Ambient color

        case 0x2200:    // Fog
            // 0x2210 : fog background
        case 0x2201:    // Use fog
        case 0x2210:    // Fog background

        case 0x2300:    // Distance queue
            // 0x2310 : Dim background
        case 0x2301:    // Use distance queue

        case 0x2302:    // Layered fog options
        case 0x2303:    // Use layered fog

        case 0x3D3E:    // Mesh version

        case 0x7001:    // Window settings
            // 0x7011 : Window description #2 ...
            // 0x7012 : Window description #1 ...
            // 0x7020 : Mesh windows ...
            fprintf( stderr, "nrModel3DS::Chunk_3D3D(): Chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_3D3D(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_4000( nrChunker& chunker )
{
    int bytes = chunker.Bytes() + chunker.Length();

    assert( m_Name == 0 );
    m_Name = chunker.ReadString();
    
    fprintf( stderr, "nrModel3DS::Chunk_4000(): Name: %s\n", m_Name );
    
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0x4100:    // Triangular mesh
            Chunk_4100( chunker );
            break;
            
        case 0x4010:    // Object hidden
        case 0x4012:    // Object doesn't cast
        case 0x4013:    // Matte object
        case 0x4015:    // External process on
        case 0x4017:    // Object doesn't receive shadows
            
        case 0x4600:    // Light
            // 0x4610 : Spotlight
            //   0x4627 : Spot raytrace
            //   0x4630 : Light shadowed
            //   0x4641 : Spot shadow map
            //   0x4650 : Spot show cone
            //   0x4651 : Spot is rectangular
            //   0x4652 : Spot overshoot
            //   0x4653 : Spot map
            //   0x4656 : Spot roll
            //   0x4658 : Spot ray trace bias
            
            // 0x4620 : Light off
            // 0x4625 : Attenuation on
            // 0x4659 : Range start
            // 0x465A : Range end
            // 0x465B : Multiplier
            
        case 0x4700:    // Camera
            fprintf( stderr, "nrModel3DS::Chunk_4000(): Chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_4000(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
    
    delete [] m_Name;
    m_Name = 0;
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_4100( nrChunker& chunker )
{
    int bytes = chunker.Bytes() + chunker.Length();
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0x4110:    // Vertices list
            Chunk_4110( chunker );
            break;
            
        case 0x4120:    // Faces description
            Chunk_4120( chunker );
            break;
            
        case 0x4130:    // Faces material list
            Chunk_4130( chunker );
            break;
            
        case 0x4140:    // Mapping coordinates list
        case 0x4150:    // Smoothing group list
            
        case 0x4160:    // Local coordinate system
        case 0x4165:    // Object color in editor
            
        case 0x4181:    // External process name
        case 0x4182:    // External process parameters
            fprintf( stderr, "nrModel3DS::Chunk_4100(): Chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_4100(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_4110( nrChunker& chunker )
{
    m_BaseVertex = m_Model->m_Positions.Length();
    m_BaseTriangle = m_Model->m_Triangles.Length();
    
    int num_vertices = chunker.ReadShort();
    
    fprintf( stderr, "nrModel3DS::Chunk_4110(): Num Vertices: %d\n", num_vertices );
    
    for ( int i = 0; i < num_vertices; i++ )
    {
        float x = chunker.ReadFloat();
        float y = chunker.ReadFloat();
        float z = chunker.ReadFloat();
        m_Model->m_Positions.Add( nrVector3( x, z, -y ) );
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_4120( nrChunker& chunker )
{
    int num_triangles = chunker.ReadShort();
    
    fprintf( stderr, "nrModel3DS::Chunk_4120(): Num Triangles: %d\n", num_triangles );
    
    for ( int i = 0; i < num_triangles; i++ )
    {
        int a = chunker.ReadShort();
        int b = chunker.ReadShort();
        int c = chunker.ReadShort();
        //int flags = chunker.ReadShort();
        
        a += m_BaseVertex;
        b += m_BaseVertex;
        c += m_BaseVertex;
        
        m_Model->m_Triangles.Add( new nrmTriangle( a, -1, -1, -1, b, -1, -1, -1, c, -1, -1, -1, m_Model ) );
    }
}

////////////////////////////////////////////////////////////////////////////

int nrModel3DS::FindMaterial( const char* name ) const
{
    for ( int i = 0; i < m_Model->NumMaterials(); i++ )
    {
        nrmMaterial* material = m_Model->m_Materials[ i ];
        
        if ( strcmp( material->m_Name, name ) == 0 )
        {
            return i;
        }
    }
    
    // Didn't find one - issue a warning.
    fprintf( stderr, "nrModel3DS::FindMaterial(): Unknown material \"%s\" (using default).\n", name );
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_4130( nrChunker& chunker )
{
    char* name = chunker.ReadString();
    int num_triangles = chunker.ReadShort();
    
    m_Group = new nrmGroup( m_Name, m_Model );
    m_Model->m_Groups.Add( m_Group );
    m_Group->m_MaterialIndex = FindMaterial( name );
    
    for ( int i = 0; i < num_triangles; i++ )
    {
        int t = chunker.ReadShort();
        m_Group->m_TriangleIndices.Add( m_BaseTriangle + t );
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_AFFF( nrChunker& chunker )
{
    int bytes = chunker.Bytes() + chunker.Length();
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0xA000:    // Material name
            Chunk_A000( chunker );
            break;
            
        case 0xA010:    // Ambient color
            m_Material->m_Ambient = Chunk_Color( chunker );
            break;
            
        case 0xA020:    // Diffuse color
            m_Material->m_Diffuse = Chunk_Color( chunker );
            break;
            
        case 0xA030:    // Specular color
            m_Material->m_Specular = Chunk_Color( chunker );
            break;
            
        case 0xA040:    // Shininess percent
            m_Material->m_Shininess = Chunk_Percent( chunker );
            break;
            
        case 0xA200:    // Texture map 1
            Chunk_Texture( chunker );
            break;
            
        case 0xA041:    // Shininess strength percent
        case 0xA050:    // Transparency percent
        case 0xA052:    // Transparency falloff percent
        case 0xA053:    // Reflection blur percent
            
        case 0xA081:    // 2 sided
        case 0xA083:    // Add trans
        case 0xA084:    // Self illum
        case 0xA085:    // Wire frame on
        case 0xA087:    // Wire thickness
        case 0xA088:    // Face map
        case 0xA08A:    // In tranc
        case 0xA08C:    // Soften
        case 0xA08E:    // Wire in units
            
        case 0xA100:    // Render type
            
        case 0xA240:    // Transparency falloff percent present
        case 0xA250:    // Reflection blur percent present
        case 0xA252:    // Bump map present (true percent)
            
        case 0xA33A:    // Texture map 2
        case 0xA210:    // Opacity map
        case 0xA230:    // Bump map
        case 0xA33C:    // Shininess map
        case 0xA204:    // Specular map
        case 0xA33D:    // Self illum. map
        case 0xA220:    // Reflection map
        case 0xA33E:    // Mask for texture map 1
        case 0xA340:    // Mask for texture map 2
        case 0xA342:    // Mask for opacity map
        case 0xA344:    // Mask for bump map
        case 0xA346:    // Mask for shininess map
        case 0xA348:    // Mask for specular map
        case 0xA34A:    // Mask for self illum. map
        case 0xA34C:    // Mask for reflection map
            fprintf( stderr, "nrModel3DS::Chunk_AFFF(): Chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_AFFF(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_A000( nrChunker& chunker )
{
    char* name = chunker.ReadString();
    
    m_Material = new nrmMaterial( name );
    
    m_Model->m_Materials.Add( m_Material );
    
    fprintf( stderr, "Chunk: 0x%04X  Material: %s\n", chunker.ID(), name );
    
    delete [] name;
}

////////////////////////////////////////////////////////////////////////////

nrColor nrModel3DS::Chunk_Color( nrChunker& chunker )
{
    nrColor c;

    int bytes = chunker.Bytes() + chunker.Length();
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0x0010:    // Color float/channel.
            {
                unsigned char* data = chunker.Data();
                float r = *( ( float* )data );  data += 4;
                float g = *( ( float* )data );  data += 4;
                float b = *( ( float* )data );  data += 4;

                c = nrColor( r, g, b );
            }
            break;
            
        case 0x0011:    // Color 8 bits/channel.
            {
                unsigned char* data = chunker.Data();
                unsigned char r = data[ 0 ];
                unsigned char g = data[ 1 ];
                unsigned char b = data[ 2 ];
                
                c = nrColor( r / 255.0f, g / 255.0f, b / 255.0f );
            }
            break;
            
        case 0x0012:    // Lin color?
        case 0x0013:    // Lin color?
            chunker.Skip();
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_Color(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
    
    return c;
}

////////////////////////////////////////////////////////////////////////////

float nrModel3DS::Chunk_Percent( nrChunker& chunker )
{
    float percent = 0;
    
    chunker.Next();
    unsigned short id = chunker.ID();
    //unsigned short length = chunker.Length();
    
    //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
    switch ( id )
    {
    case 0x0030:    // Int percentage
        {
            unsigned char* data = chunker.Data();
            int p = *( ( unsigned short* )data );  data += 2;
            percent = p / 100.0f;
        }
        break;
        
    case 0x0031:    // Float percentage
        {
            unsigned char* data = chunker.Data();
            percent = *( ( float* )data );  data += 4;
        }
        break;
        
    default:
        fprintf( stderr, "nrModel3DS::Chunk_Percent(): Unknown chunk 0x%04X, skipped.\n", id );
        chunker.Skip();
        break;
    }
    
    return percent;
}

////////////////////////////////////////////////////////////////////////////

void nrModel3DS::Chunk_Texture( nrChunker& chunker )
{
    int bytes = chunker.Bytes() + chunker.Length();
    while ( chunker.Bytes() < bytes )
    {
        chunker.Next();
        unsigned short id = chunker.ID();
        //unsigned int length = chunker.Length();
        
        //fprintf( stderr, "Chunk: 0x%04X  Length: %d bytes\n", id, length );
        switch ( id )
        {
        case 0xA300:    // Mapping filename
            {
                char* name = chunker.ReadString();
                fprintf( stderr, "Chunk: 0x%04X  Name: %s\n", chunker.ID(), name );
                delete [] name;
            }
            break;
            
        case 0xA351:    // Mapping parameters
        case 0xA353:    // Blur percent
        case 0xA354:    // V scale
        case 0xA356:    // U scale
        case 0xA358:    // U offset
        case 0xA35A:    // V offset
        case 0xA35C:    // Rotation angle
        case 0xA360:    // RGB Luma/Alpha tint 1
        case 0xA362:    // RGB Luma/Alpha tint 2
        case 0xA364:    // RGB tint R
        case 0xA366:    // RGB tint G
        case 0xA368:    // RGB tint B
            chunker.Skip();
            break;
            
        case 0x030:     // Percentage (possibly transparency of texture?)
            {
                //unsigned char* data = chunker.Data();
                //int p = *( ( unsigned short* )data );  data += 2;
            }
            break;
            
        default:
            fprintf( stderr, "nrModel3DS::Chunk_Texture(): Unknown chunk 0x%04X, skipped.\n", id );
            chunker.Skip();
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

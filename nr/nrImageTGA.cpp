////////////////////////////////////////////////////////////////////////////
//
// nrImageTGA.cpp
//
// A class for TGA (Truvision Targa) images.
//
// Only type 2 (uncompressed 16/24/32 bit) files are supported.
//
// See:  http://www.cica.indiana.edu/graphics/image_specs/tga.format.txt
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrImage.h"

#include <assert.h>

#include "nrLog.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

#pragma pack( 1 )

class nrImageTGA
{
public:

    unsigned char id_field_size;
    unsigned char colormap_type;
    unsigned char image_type_code;
    struct {
        unsigned short colormap_origin;
        unsigned short colormap_length;
        unsigned char  colormap_entry_size;
    } colormap_specification;
    struct {
        unsigned short x_origin;
        unsigned short y_origin;
        unsigned short width;
        unsigned short height;
        unsigned char  num_bits_per_pixel;
        unsigned char  descriptor_byte;
    } image_specification;
};

#pragma pack()


////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

bool nrImage::ReadTGA( const char* file_name )
{
    // open the file.
    FILE* file = fopen( file_name, "rb" );
    if ( file == NULL ) 
    {
        g_Log.Write( "nrImage::ReadTGA() : %s: Failed to open file.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // grab the header.
    nrImageTGA tga;
    if ( fread( &tga, sizeof( nrImageTGA ), 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::ReadTGA() : %s: Not a valid TGA.\n", file_name );
        return false;
    }
    
    // check the image type (only supports type 2 for now).
    if ( tga.image_type_code != 2 )
    {
        g_Log.Write( "nrImage::ReadTGA() : %s: Not a type 2 (uncompressed 16/24/32 bit) TGA file.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // (possibly) read the image identification.
    if ( tga.id_field_size > 0 )
    {
        char image_id[ 255 ];

        if ( fread( &image_id, 255, 1, file ) != 1 )
        {
            g_Log.Write( "nrImage::ReadTGA() : %s: Not a valid TGA.\n", file_name );
            return false;
        }
    }
    
    // !!! shouldn't have a colormap.
    if ( tga.colormap_type != 0 )
    {
        assert( tga.colormap_type == 0 );
        g_Log.Write( "nrImage::ReadTGA() : %s: No support for colormaps.\n", file_name );
        return false;
    }
    
    int w = tga.image_specification.width;
    int h = tga.image_specification.height;
    int d = tga.image_specification.num_bits_per_pixel / 8;
    
    // grab all the image data in one fell swoop.
    unsigned char* image = new unsigned char[ w * h * d ];
    if ( image == NULL )
    {
        g_Log.Write( "nrImage::ReadTGA() : Out of memory.\n" );
        assert( 0 );
        return false;
    }
    
    if ( fread( image, w * h * d, 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::ReadTGA() : %s: File may be truncated.\n", file_name );
        // not a fatal error, no need to return.
    }
    
    // expand 16 bit pixels into 24/32 bit pixels.
    if ( d == 2 )
    {
        int ud = d;
        
        int attr = tga.image_specification.descriptor_byte & 0x07;
        d = ( attr == 0 ) ? 3 : 4;
        
        unsigned char* expanded = new unsigned char[ w * h * d ];
        for ( int j = 0; j < h; j++ )
        {
            for ( int i = 0; i < w; i++ )
            {
                unsigned char* src = &( image[ ( j * w + i ) * ud ] );
                unsigned char* dst = &( expanded[ ( j * w + i ) * d ] );
                
                unsigned char lo = src[ 0 ];
                unsigned char hi = src[ 1 ];
                
                unsigned char b = lo & 0x1F;
                unsigned char g = ( ( lo & 0xE0 ) >> 5 ) | ( ( hi & 0x03 ) << 3 );
                unsigned char r = ( hi & 0x7C ) >> 2;
                unsigned char a = ( hi & 0x80 ) >> 7;
                
                b <<= 3;
                g <<= 3;
                r <<= 3;
                a <<= 7;
                
                if ( d == 4 )
                {
                    dst[ 0 ] = a;
                    dst[ 1 ] = b;
                    dst[ 2 ] = g;
                    dst[ 3 ] = r;
                }
                else //if ( d == 3 )
                {
                    dst[ 0 ] = b;
                    dst[ 1 ] = g;
                    dst[ 2 ] = r;
                }
            }
        }
        
        delete [] image;
        
        image = expanded;
    }
    
    // targa's are stored bgr(a) - swizzle to rgb(a).
    for ( int j = 0; j < h; j++ )
    {
        for ( int i = 0; i < w; i++ )
        {
            unsigned char* p = &( image[ ( j * w + i ) * d ] );
            
            if ( d == 4 )
            {
                unsigned char b = p[ 0 ];
                unsigned char g = p[ 1 ];
                unsigned char r = p[ 2 ];
                unsigned char a = p[ 3 ];
                p[ 0 ] = r;
                p[ 1 ] = g;
                p[ 2 ] = b;
                p[ 3 ] = a;
            }
            else if ( d == 3 )
            {
                unsigned char b = p[ 0 ];
                unsigned char g = p[ 1 ];
                unsigned char r = p[ 2 ];
                p[ 0 ] = r;
                p[ 1 ] = g;
                p[ 2 ] = b;
            }
            else if ( d == 2 )
            {


                d = 3;
            }
            else if ( d == 1 )
            {

                d = 3;
            }
        }
    }
    
    fclose( file );
    
    m_Width  = w;
    m_Height = h;
    m_Depth  = d;
    m_Image  = image;
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

bool nrImage::WriteTGA( const char* file_name ) const
{
    assert( m_Image );
    
    int w = m_Width;
    int h = m_Height;
    int d = m_Depth;
    
    FILE* file = fopen( file_name, "wb" );
    if ( file == NULL ) 
    {
        g_Log.Write( "nrImage::WriteTGA() : %s: Failed to open file.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // cobble up a tga header.
    nrImageTGA tga;
    
    tga.id_field_size = 0;
    tga.colormap_type = 0;
    tga.image_type_code = 2;
    tga.colormap_specification.colormap_origin = 0;
    tga.colormap_specification.colormap_length = 0;
    tga.colormap_specification.colormap_entry_size = 0;
    tga.image_specification.x_origin = 0;
    tga.image_specification.y_origin = 0;
    tga.image_specification.width = w;
    tga.image_specification.height = h;
    tga.image_specification.num_bits_per_pixel = d * 8;
    tga.image_specification.descriptor_byte = 0;
    if ( d == 4 )
    {
        tga.image_specification.descriptor_byte |= 8;
    }
    
    // spit out header.
    if ( fwrite( &tga, sizeof ( nrImageTGA ), 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::WriteTGA() : %s: Failed to write image.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // internal image format is rgb(a) - targa needs bgr(a).
    unsigned char* image = new unsigned char[ w * h * d ];
    if ( image == NULL )
    {
        g_Log.Write( "nrImage::WriteTGA() : Out of memory.\n" );
        assert( 0 );
        return false;
    }
    
    for ( int j = 0; j < h; j++ )
    {
        for ( int i = 0; i < w; i++ )
        {
            unsigned char* src = &( m_Image[ ( j * w + i ) * d ] );
            unsigned char* dst = &( image[ ( j * w + i ) * d ] );
            
            if ( d == 4 )
            {
                unsigned char r = src[ 0 ];
                unsigned char g = src[ 1 ];
                unsigned char b = src[ 2 ];
                unsigned char a = src[ 3 ];
                dst[ 0 ] = b;
                dst[ 1 ] = g;
                dst[ 2 ] = r;
                dst[ 3 ] = a;
            }
            else if ( d == 3 )
            {
                unsigned char r = src[ 0 ];
                unsigned char g = src[ 1 ];
                unsigned char b = src[ 2 ];
                dst[ 0 ] = b;
                dst[ 1 ] = g;
                dst[ 2 ] = r;
            }
        }
    }
    
    // spit out all the image data.
    if ( fwrite( image, w * h * d, 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::WriteTGA() : %s: Failed to write image.\n", file_name );
        assert( 0 );
        return false;
    }
    
    delete [] image;

    fflush( file );
    
    fclose( file );
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

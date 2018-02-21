////////////////////////////////////////////////////////////////////////////
//
// nrImagePCX.cpp
//
// A class for PCX (ZSoft Corporation PC Paintbrush format) images.
//
// Only version 5 files are supported.
//
// See:  http://www.cica.indiana.edu/graphics/image_specs/pcx.format.txt
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

class nrImagePCX
{
public:
    
    char  manufacturer;
    char  version;
    char  encoding;
    char  bitsperpixel;
    short xmin;
    short ymin;
    short xmax;
    short ymax;
    short hres;
    short vres;
    char  palette[ 16 * 3 ];
    char  reserved;
    char  nplanes;
    short bytesperline;
    short paletteinfo;
    short hscreensize;
    short vscreensize;
    char  filler[ 54 ];
};

#pragma pack()


////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

bool nrImage::ReadPCX( const char* file_name )
{
    unsigned char* image = NULL;
    unsigned char* decompressed = NULL;
    int w = 0;
    int h = 0;
    nrImagePCX pcx;
    
    FILE* file = fopen( file_name, "rb" );
    if ( file == NULL ) 
    {
        g_Log.Write( "nrImage::ReadPCX() : %s: Failed to open file.\n", file_name );
        goto ReadPCX_BAIL;
    }
    
    // grab header of the file.
    if ( fread( &pcx, sizeof( pcx ), 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::ReadPCX() : %s: File may be truncated.\n", file_name );
        goto ReadPCX_BAIL;
    }
    
    if ( pcx.manufacturer != 10 || pcx.encoding != 1 )
    {
        g_Log.Write( "nrImage::ReadPCX() : %s: Not a valid PCX file.\n", file_name );
        goto ReadPCX_BAIL;
    }
    
    // verify the header of the file
    if ( pcx.version != 5 )
    {
        g_Log.Write( "nrImage::ReadPCX() : %s: Only PCX Version 5 images are supported, this file is version %d.\n", file_name, pcx.version );
        goto ReadPCX_BAIL;
    }
    
    // compute width, height in pixels.
    w = ( pcx.xmax - pcx.xmin ) + 1;
    h = ( pcx.ymax - pcx.ymin ) + 1;
    
    // decompress the image data
    {
        int total_bytes = h * pcx.nplanes * pcx.bytesperline;
        
        decompressed = new unsigned char[ total_bytes ];
        
        if ( decompressed == NULL )
        {
            g_Log.Write( "nrImage::ReadPCX() : Out of memory.\n" );
            goto ReadPCX_BAIL;
        }
        
        for ( int i = 0; i < total_bytes; )
        {
            int num_bytes_to_duplicate;
            char control_byte;
            char data_byte;
            
            if ( fread( &control_byte, 1, 1, file ) != 1 )
            {
                g_Log.Write( "nrImage::ReadPCX() : %s: File may be truncated.\n", file_name );
                goto ReadPCX_BAIL;
            }
            
            if ( ( control_byte & 0xC0 ) == 0xC0 )
            {
                num_bytes_to_duplicate = ( control_byte & 0x3F );
                
                if ( num_bytes_to_duplicate == 0 )
                {
                    continue;
                }
                
                if ( fread( &data_byte, 1, 1, file ) != 1 )
                {
                    g_Log.Write( "nrImage::ReadPCX() : %s: File may be truncated.\n", file_name );
                    goto ReadPCX_BAIL;
                }
            }
            else
            {
                num_bytes_to_duplicate = 1;
                data_byte = control_byte;
            }
            
            for ( int j = 0; j < num_bytes_to_duplicate; j++ )
            {
                decompressed[ i + j ] = data_byte;
            }
            
            i += num_bytes_to_duplicate;
        }
    }
    
    // convert data to RGB format. 
    if ( pcx.nplanes == 3 )
    {
        // non-paletted image in planes (RRR...GGG...BBB...).
        image = new unsigned char[ w * h * 3 ];
        
        if ( image == NULL )
        {
            g_Log.Write( "nrImage::ReadPCX() : Out of memory.\n" );
            goto ReadPCX_BAIL;
        }
        
        unsigned char* dst = image;
        unsigned char* src = decompressed;
        
        for ( int j = 0; j < h; j++ )
        {
            for ( int i = 0; i < w; i++ )
            {
                dst[ 0 ] = src[ ( 0 * w ) + i ];
                dst[ 1 ] = src[ ( 1 * w ) + i ];
                dst[ 2 ] = src[ ( 2 * w ) + i ];
                
                dst += 3;
            }
            
            src += 3 * w;
        }
    }
    else
    {
        // paletted image (one byte index into palette per pixel).
        unsigned char palette[ 768 ];
        
        assert( pcx.version == 5 );
        
        // get the palette.
        {
            char palette_byte;
            
            fseek( file, -769, SEEK_END );
            
            if ( fread( &palette_byte, 1, 1, file ) != 1 )
            {
                g_Log.Write( "nrImage::ReadPCX() : %s: File may be truncated.\n", file_name );
                goto ReadPCX_BAIL;
            }
            
            if ( palette_byte != 12 )
            {
                g_Log.Write( "nrImage::ReadPCX() : %s: Missing or corrupt palette.\n", file_name );
                goto ReadPCX_BAIL;
            }
            
            if ( fread( palette, 768, 1, file ) != 1 )
            {
                g_Log.Write( "nrImage::ReadPCX() : %s: File may be truncated.\n", file_name );
                goto ReadPCX_BAIL;
            }
            
            // put the file pointer back to the end of the header.
            fseek( file, 128, SEEK_SET );
        }
        
        // create RGB image from the indexed image.
        {
            image = new unsigned char[ w * h * 3 ];
            
            if ( image == NULL )
            {
                g_Log.Write( "nrImage::ReadPCX() : Out of memory.\n" );
                goto ReadPCX_BAIL;
            }
            
            unsigned char* dst = image;
            unsigned char* src = decompressed;
            
            for ( int i = 0; i < w * h; i++ )
            {
                unsigned char* color = &( palette[ *src * 3 ] );
                
                dst[ 0 ] = color[ 0 ];
                dst[ 1 ] = color[ 1 ];
                dst[ 2 ] = color[ 2 ];
                
                dst += 3;
                src += 1;
            }
        }
    }
    
    m_Width  = w;
    m_Height = h;
    m_Depth  = 3;
    m_Image  = image;
    
ReadPCX_BAIL:
    
    if ( decompressed != NULL )
    {
        delete [] decompressed;
    }
    
    if ( file != NULL )
    {
        fclose( file );
    }
    
    if ( image )
    {
        return true;
    }
    else
    {
        assert( 0 );
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

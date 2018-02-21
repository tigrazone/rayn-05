////////////////////////////////////////////////////////////////////////////
//
// nrImagePPM.cpp
//
// A class for PPM (Portable Pixmap) images.
//
// Only type P6 files are supported.
//
// See:   http://www.cica.indiana.edu/graphics/image_specs/ppm.format.txt
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrImage.h"

#include "nrLog.h"

#include <assert.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

bool nrImage::ReadPPM( const char* file_name )
{
    int  w, h, d;
    char line[ 70 ];			// max line <= 70 in PPM ( per spec ).
    
    // open the file.
    FILE* file = fopen( file_name, "rb" );
    if ( file == NULL ) 
    {
        g_Log.Write( "nrImage::ReadPPM() : %s: Failed to open file.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // grab first two chars of the file and make sure that it has the
    // correct magic cookie for a raw PPM file.
    fgets( line, 70, file );
    if ( strncmp( line, "P6", 2 ) != 0 ) 
    {
        g_Log.Write( "nrImage::ReadPPM() : %s: Not a raw PPM file.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // grab the three elements in the header ( width, height, maxval ).
    int i = 0;
    while( i < 3 ) 
    {
        fgets( line, 70, file );
        if ( line[ 0 ] == '#' )		// skip comments.
        {
            continue;
        }
        if ( i == 0 )
        {
            i += sscanf( line, "%d %d %d", &w, &h, &d );
        }
        else if ( i == 1 )
        {
            i += sscanf( line, "%d %d", &h, &d );
        }
        else if ( i == 2 )
        {
            i += sscanf( line, "%d", &d );
        }
    }
    
    // grab all the image data in one fell swoop.
    unsigned char* image = new unsigned char[ w * h * 3 ];
    
    if ( image == NULL )
    {
        g_Log.Write( "nrImage::ReadPPM() : Out of memory.\n" );
        assert( 0 );
        return false;
    }
    
    if ( fread( image, w * h * 3, 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::ReadPPM() : %s: File may be truncated.\n", file_name );
        // not a fatal error, no need to return.
    }
    
    fclose( file );
    
    m_Width  = w;
    m_Height = h;
    m_Depth  = 3;
    m_Image  = image;
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

bool nrImage::WritePPM( const char* file_name ) const
{
    assert( m_Image );
    assert( m_Depth == 3 );
    
    int w = m_Width;
    int h = m_Height;
    
    FILE* file = fopen( file_name, "wb" );
    if ( file == NULL ) 
    {
        g_Log.Write( "nrImage::WritePPM() : %s: Failed to open file.\n", file_name );
        assert( 0 );
        return false;
    }
    
    // spit out binary PPM header.
    fprintf( file, "P6\n# Created by nrImage tools.\n%d %d\n255\n", w, h );
    
    // spit out all the image data.
    if ( fwrite( m_Image, w * h * 3, 1, file ) != 1 )
    {
        g_Log.Write( "nrImage::WritePPM() : %s: Failed to write image.\n", file_name );
        assert( 0 );
        return false;
    }
    
    fflush( file );
    
    fclose( file );
    
    return true;
}

////////////////////////////////////////////////////////////////////////////


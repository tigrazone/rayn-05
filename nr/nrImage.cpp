////////////////////////////////////////////////////////////////////////////
//
// nrImage.cpp
//
// A class for images.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrImage.h"

#include "nrLog.h"
#include "nrMath.h"

#include <assert.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrImage::nrImage( void )
{
    m_Width = 0;
    m_Height = 0;
    m_Depth = 0;
    m_Image = NULL;
}

////////////////////////////////////////////////////////////////////////////

nrImage::~nrImage( void )
{
    delete [] m_Image;
}

///////////////////////////////////////////////////////////////////////////

bool nrImage::CreateFromFile( const char* file_name )
{
    delete [] m_Image;
    
    char* extension = strrchr( file_name, '.' );
    
    if ( stricmp( extension, ".ppm" ) == 0 )
    {
        return ReadPPM( file_name );
    }
    else if ( stricmp( extension, ".pcx" ) == 0 )
    {
        return ReadPCX( file_name );
    }
    else if ( stricmp( extension, ".tga" ) == 0 )
    {
        return ReadTGA( file_name );
    }
    else
    {
        g_Log.Write( "nrImage::CreateFromFile() : %s: Unrecognized image format.\n", file_name );
        assert( 0 );
        return false;
    }
}

//////////////////////////////////////////////////////////////////////////

bool nrImage::CreateBlank( int width, int height, int depth )
{
    delete [] m_Image;
    
    m_Image = new unsigned char[ width * height * depth ];
    memset( m_Image, 0, width * height * depth );
    
    if ( m_Image == NULL )
    {
        g_Log.Write( "nrImage::CreateBlank() : Out of memory.\n" );
        return false;
    }
    
    m_Width  = width;
    m_Height = height;
    m_Depth  = depth;
    
    return true;
}

//////////////////////////////////////////////////////////////////////////

bool nrImage::CreateFromBytes( int width, int height, int depth, unsigned char* pixels )
{
    delete [] m_Image;
    
    m_Image = new unsigned char[ width * height * depth ];
    
    if ( m_Image == NULL )
    {
        g_Log.Write( "nrImage::CreateBlank() : Out of memory.\n" );
        return false;
    }
    
    m_Width  = width;
    m_Height = height;
    m_Depth  = depth;
    
    memcpy( m_Image, pixels, width * height * depth );
    
    return true;
}

///////////////////////////////////////////////////////////////////////////

bool nrImage::CreateChecker( int width, int height, int num_checks )
{
    if ( ! CreateBlank( width, height, 3 ) )
    {
        return false;
    }
    
    int texels_per_check_i = width / num_checks;
    int texels_per_check_j = height / num_checks;
    
    for ( int j = 0; j < height; j++ )
    {
        for ( int i = 0; i < width; i++ )
        {
            if ( nrMath::Even( j / texels_per_check_j ) )
            {
                if ( nrMath::Even( i / texels_per_check_i ) )
                {
                    SetPixel( i, j, nrPixel( 192, 192, 192 ) );
                }
                else
                {
                    SetPixel( i, j, nrPixel( 64, 64, 64 ) );
                }
            }
            else
            {
                if ( nrMath::Even( i / texels_per_check_i ) )
                {
                    SetPixel( i, j, nrPixel( 64, 64, 64 ) );
                }
                else
                {
                    SetPixel( i, j, nrPixel( 192, 192, 192 ) );
                }
            }
            
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////

bool nrImage::CreateGrid( int width, int height, int num_grids )
{
    if ( ! CreateBlank( width, height, 3 ) )
    {
        return false;
    }
    
    for ( int j = 0; j < height; j++ )
    {
        for ( int i = 0; i < width; i++ )
        {
            if ( i % num_grids == 0 || j % num_grids == 0 )
            {
                SetPixel( i, j, nrPixel( 64, 64, 64 ) );
            }
            else
            {
                SetPixel( i, j, nrPixel( 192, 192, 192 ) );
            }
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////

bool nrImage::CreateSphere( int width, int height )
{
    if ( ! CreateBlank( width, height, 3 ) )
    {
        return false;
    }

    for ( int j = 0; j < height; j++ )
    {
        for ( int i = 0; i < width; i++ )
        {
            float x = ( ( ( float )i / width  ) * 2 ) - 1.0f;
            float y = ( ( ( float )j / height ) * 2 ) - 1.0f;
			float z = ( x * x ) + ( y * y );
            
            if ( z > 1.0f )
			{
				z = 0.0f;
			}
			else
			{
                z = nrMath::Sqrt( 1.0f - z );
			}
            
            unsigned char c = ( unsigned char )( z * 255 );
            
            SetPixel( i, j, nrPixel( c, c, c ) );
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////

bool nrImage::CreateBlend( int width, int height, const nrPixel& ul, const nrPixel& ur, const nrPixel& ll, const nrPixel& lr )
{
    if ( ! CreateBlank( width, height, 3 ) )
    {
        return false;
    }
    
    for ( int j = 0; j < height; j++ )
    {
        for ( int i = 0; i < width; i++ )
        {
            float x, y;
            
            x = ( ( float )i / width  );
            y = ( ( float )j / height );
            
            nrPixel u;
            u.r = ( int )( ul.r * ( 1.0f - x ) + x * ur.r );
            u.g = ( int )( ul.g * ( 1.0f - x ) + x * ur.g );
            u.b = ( int )( ul.b * ( 1.0f - x ) + x * ur.b );
            
            nrPixel l;
            l.r = ( int )( ll.r * ( 1.0f - x ) + x * lr.r );
            l.g = ( int )( ll.g * ( 1.0f - x ) + x * lr.g );
            l.b = ( int )( ll.b * ( 1.0f - x ) + x * lr.b );
            
            nrPixel p;
            p.r = ( int )( u.r * ( 1.0f - y ) + l.r * y );
            p.g = ( int )( u.g * ( 1.0f - y ) + l.g * y );
            p.b = ( int )( u.b * ( 1.0f - y ) + l.b * y );
            
            SetPixel( i, j, p );
        }
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////

void nrImage::ScaleToPowerOfTwo( void )
{
    assert( m_Image );
    
    int w = nrMath::NextPowerOfTwo( m_Width );
    int h = nrMath::NextPowerOfTwo( m_Height );
    int d = m_Depth;
    
    int original_width = m_Width;
    int original_height = m_Height;
    
    if ( w == m_Width && h == m_Height )
    {
        // short circuit if the image is already a power of two.
        return;
    }
    
    unsigned char* image = new unsigned char[ w * h * d ];
    
    if ( image == NULL )
    {
        g_Log.Write( "nrImage::ScaleToPowerOfTwo() : Out of memory.\n" );
        return;
    }
    
    for ( int j = 0; j < h; j++ )
    {
        // add 0.5f to scale from the center of the pixel for OpenGL compatibility.
        int y = ( int )( ( ( j + 0.5f ) / h ) * original_height );
        
        for ( int i = 0; i < w; i++ )
        {
            int x = ( int )( ( ( i + 0.5f ) / w ) * original_width );
            
            for ( int k = 0; k < d; k++ )
            {
                image[ ( j * w * d ) + ( i * d ) + k ] = m_Image[ ( y * original_width * d ) + ( x * d ) + k ];
            }
        }
    }
    
    delete [] m_Image;
    
    m_Image = image;
    m_Width = w;
    m_Height = h;
}

////////////////////////////////////////////////////////////////////////////

void nrImage::AddAlphaChannel( void )
{
    assert( m_Image );
    assert( m_Depth == 3 );
    
    int w = m_Width;
    int h = m_Height;
    
    unsigned char* image = new unsigned char[ w * h * 4 ];
    
    if ( image == NULL )
    {
        g_Log.Write( "nrImage::AddAlphaChannel() : Out of memory.\n" );
        return;
    }
    
    for ( int i = 0; i < w * h; i++ )
    {
        unsigned char r = m_Image[ ( i * 3 ) + 0 ];
        unsigned char g = m_Image[ ( i * 3 ) + 1 ];
        unsigned char b = m_Image[ ( i * 3 ) + 2 ];
        unsigned char a = 255;
        
        image[ ( i * 4 ) + 0 ] = r;
        image[ ( i * 4 ) + 1 ] = g;
        image[ ( i * 4 ) + 2 ] = b;
        image[ ( i * 4 ) + 3 ] = a;
    }
    
    delete [] m_Image;
    
    m_Image = image;
    m_Depth = 4;
}

////////////////////////////////////////////////////////////////////////////

void nrImage::RemoveAlphaChannel( void )
{
    assert( m_Image );
    assert( m_Depth == 4 );
    
    int w = m_Width;
    int h = m_Height;
    
    unsigned char* image = new unsigned char[ w * h * 3 ];
    
    if ( image == NULL )
    {
        g_Log.Write( "nrImage::RemoveAlphaChannel() : Out of memory.\n" );
        return;
    }
    
    for ( int i = 0; i < w * h; i++ )
    {
        unsigned char r = m_Image[ ( i * 4 ) + 0 ];
        unsigned char g = m_Image[ ( i * 4 ) + 1 ];
        unsigned char b = m_Image[ ( i * 4 ) + 2 ];
        
        image[ ( i * 3 ) + 0 ] = r;
        image[ ( i * 3 ) + 1 ] = g;
        image[ ( i * 3 ) + 2 ] = b;
    }
    
    delete [] m_Image;
    
    m_Image = image;
    m_Depth = 3;
}

////////////////////////////////////////////////////////////////////////////

void nrImage::Mask( const nrPixel& pixel )
{
    assert( m_Image );
    
    if ( m_Depth == 3 )
    {
        AddAlphaChannel();
    }
    
    assert( m_Depth == 4 );
    
    int w = m_Width;
    int h = m_Height;
    
    for ( int i = 0; i < w * h; i++ )
    {
        unsigned char r = m_Image[ ( i * 4 ) + 0 ];
        unsigned char g = m_Image[ ( i * 4 ) + 1 ];
        unsigned char b = m_Image[ ( i * 4 ) + 2 ];
        unsigned char a;
        
        if ( r == pixel.r && g == pixel.g && b == pixel.b )
        {
            // This pixel is the transparent color, so put a 0 in the alpha channel.
            a = 0;
        }
        else
        {
            // This pixel is not the transparent color, so put 255 in the alpha channel.
            a = 255;
        }
        
        m_Image[ ( i * 4 ) + 0 ] = r;
        m_Image[ ( i * 4 ) + 1 ] = g;
        m_Image[ ( i * 4 ) + 2 ] = b;
        m_Image[ ( i * 4 ) + 3 ] = a;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrImage::SetPixel( int x, int y, const nrPixel& pixel )
{
    int p = ( y * m_Width + x ) * m_Depth;
    
    assert( p >= 0 );
    assert( p < m_Width * m_Height * m_Depth );
    
    if ( x < 0 || x >= m_Width )
    {
        return;
    }
    if ( y < 0 || y >= m_Height )
    {
        return;
    }
    
    m_Image[ p + 0 ] = pixel.r;
    m_Image[ p + 1 ] = pixel.g;
    m_Image[ p + 2 ] = pixel.b;
    
    if ( m_Depth > 3 )
    {
        m_Image[ p + 3 ] = pixel.a;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrImage::SetPixel( int x, int y, const nrColor& color )
{
    int p = ( y * m_Width + x ) * m_Depth;
    
    assert( p >= 0 );
    assert( p < m_Width * m_Height * m_Depth );
    
    if ( x < 0 || x >= m_Width )
    {
        return;
    }
    if ( y < 0 || y >= m_Height )
    {
        return;
    }
    
    m_Image[ p + 0 ] = ( unsigned int )( color.r * 255 );
    m_Image[ p + 1 ] = ( unsigned int )( color.g * 255 );
    m_Image[ p + 2 ] = ( unsigned int )( color.b * 255 );
}

////////////////////////////////////////////////////////////////////////////

void nrImage::GetPixel( int x, int y, nrPixel& pixel ) const
{
    int p = ( y * m_Width + x ) * m_Depth;
    
    assert( p >= 0 );
    assert( p < m_Width * m_Height * m_Depth );
    
    if ( x < 0 || x >= m_Width )
    {
        return;
    }
    if ( y < 0 || y >= m_Width )
    {
        return;
    }
    
    pixel.r = m_Image[ p + 0 ];
    pixel.g = m_Image[ p + 1 ];
    pixel.b = m_Image[ p + 2 ];
    if ( m_Depth > 3 )
    {
        pixel.a = m_Image[ p + 3 ];
    }
    else
    {
        pixel.a = 255;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrImage::GetPixel( int x, int y, nrColor& color ) const
{
    nrPixel pixel;

    GetPixel( x, y, pixel );
    
    color.r = ( float )pixel.r / 255.0f;
    color.g = ( float )pixel.g / 255.0f;
    color.b = ( float )pixel.b / 255.0f;
}

////////////////////////////////////////////////////////////////////////////

void nrImage::FlipVertical( void )
{
    assert( m_Image );
    
    int w = m_Width;
    int h = m_Height;
    int d = m_Depth;
    
    unsigned char* image = new unsigned char[ w * h * d ];
    
    for ( int j = 0; j < h; j++ )
    {
        unsigned char* src = &( m_Image[ j * w * d ] );
        unsigned char* dst = &(   image[ ( h - j - 1 ) * w * d ] );
        
        memcpy( dst, src, w * d );
    }
    
    delete [] m_Image;
    
    m_Image = image;
}

////////////////////////////////////////////////////////////////////////////

void nrImage::Clear( const nrPixel& background )
{
    assert( m_Image );
    
    int w = m_Width;
    int h = m_Height;
    int d = m_Depth;
    
    for ( int i = 0; i < w * h; i++ )
    {
        m_Image[ i * d + 0 ] = background.r;
        m_Image[ i * d + 1 ] = background.g;
        m_Image[ i * d + 2 ] = background.b;

        if ( d == 4 )
        {
            m_Image[ i * d + 3 ] = background.a;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

void nrImage::Copy( const nrImage& src, int dst_x, int dst_y )
{
    CopySub( src, 0, 0, src.m_Width, src.m_Height, dst_x, dst_y );
}

////////////////////////////////////////////////////////////////////////////

void nrImage::CopySub( const nrImage& src, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y )
{
    assert( m_Depth == src.Depth() );
    
    int y = dst_y;
    for ( int j = src_y; j < src_h; j++ )
    {
        if ( y >= m_Height )
        {
            continue;
        }
        
        int x = dst_x;
        for ( int i = src_x; i < src_w; i++ )
        {
            if ( x >= m_Width )
            {
                continue;
            }
            
            int pixel = ( y * m_Width + x ) * m_Depth;
            int src_pixel = ( j * src.m_Width + i ) * src.m_Depth;
            
            memcpy( &( m_Image[ pixel ] ), &( src.m_Image[ src_pixel ] ), m_Depth );
            
            x++;
        }
        
        y++;
    }
}

////////////////////////////////////////////////////////////////////////////

bool nrImage::WriteToFile( const char* file_name ) const
{
    char* extension = strrchr( file_name, '.' );
    
    if ( stricmp( extension, ".ppm" ) == 0 )
    {
        return WritePPM( file_name );
    }
    else if ( stricmp( extension, ".tga" ) == 0 )
    {
        return WriteTGA( file_name );
    }
    else
    {
        g_Log.Write( "nrImage::WriteToFile() : %s: Unsupported output format.\n", file_name );
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

int nrImage::Width( void ) const
{
    return m_Width;
}

////////////////////////////////////////////////////////////////////////////

int nrImage::Height( void ) const
{
    return m_Height;
}

////////////////////////////////////////////////////////////////////////////

int nrImage::Depth( void ) const
{
    return m_Depth;
}

///////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
// nrKernel.cpp
//
// A class for images.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrKernel.h"

#include <assert.h>

#include "nrImage.h"
#include "nrMath.h"


////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////

#define K( row, column )	m_Kernel[ ( row ) * m_Size + ( column ) ]


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrKernel::nrKernel( void )
{
    m_Size = 0;
    m_Kernel = 0;
}

////////////////////////////////////////////////////////////////////////////

nrKernel::~nrKernel( void )
{
    delete [] m_Kernel;
}

///////////////////////////////////////////////////////////////////////////

void nrKernel::CreateGaussian( int size, float sigma )
{
    m_Size = size;
    
    delete [] m_Kernel;
    
    m_Kernel = new float[ m_Size * m_Size ];
    
    float sum = 0;
    for ( int j = 0; j < m_Size; j++ )
    {
        for ( int i = 0; i < m_Size; i++ )
        {
            float g = Gaussian( i - m_Size / 2, j - m_Size / 2, sigma );
            
            K( i, j ) = g;
            
            sum += g;
        }
    }
    
    Normalize();
    
#if 0
    // Print the kernel.
    for ( j = 0; j < m_Size; j++ )
    {
        g_Log.Write( "| " );
        
        for ( int i = 0; i < m_Size; i++ )
        {
            g_Log.WriteNoTime( "%4.2f ", m_Kernel( i, j ) );
        }
        
        g_Log.WriteNoTime( "|\n" );
    }
#endif
}

///////////////////////////////////////////////////////////////////////////

void nrKernel::CreateSobel( int mode )
{
    m_Size = 3;
    
    delete [] m_Kernel;
    
    m_Kernel = new float[ m_Size * m_Size ];
    
    if ( mode == Vertical )
    {
        K( 0, 0 ) = -1.0f;	K( 0, 1 ) =  0.0f;	K( 0, 2 ) =  1.0f;
        K( 1, 0 ) = -2.0f;	K( 1, 1 ) =  0.0f;	K( 1, 2 ) =  2.0f;
        K( 2, 0 ) = -1.0f;	K( 2, 1 ) =  0.0f;	K( 2, 2 ) =  1.0f;
    }
    else
    {
        K( 0, 0 ) =  1.0f;	K( 0, 1 ) =  2.0f;	K( 0, 2 ) =  1.0f;
        K( 1, 0 ) =  0.0f;	K( 1, 1 ) =  0.0f;	K( 1, 2 ) =  0.0f;
        K( 2, 0 ) = -1.0f;	K( 2, 1 ) = -2.0f;	K( 2, 2 ) = -1.0f;
    }
    
    Normalize();
}

///////////////////////////////////////////////////////////////////////////

nrImage* nrKernel::Convolve( const nrImage* src, int mode, const nrPixel& color ) const
{
    int w = src->Width();
    int h = src->Height();
    int d = src->Depth();
    
    nrImage* dst = new nrImage;
    dst->CreateBlank( w, h, d );
    
    int ks = m_Size;
    int hks = m_Size / 2;
    
    for ( int j = 0; j < h; j++ )
    {
        for ( int i = 0; i < w; i++ )
        {
            if ( mode == Confine )
            {
                if ( j <= hks || i <= hks || j >= h - hks - 1 || i >= w - hks - 1 )
                {
                    nrPixel pixel;
                    src->GetPixel( i, j, pixel );
                    dst->SetPixel( i, j, pixel );
                    
                    continue;
                }
            }
            
            float rr = 0;
            float gg = 0;
            float bb = 0;
            float aa = 0;
            nrPixel pixel;
            
            for ( int y = -hks; y <= hks; y++ )
            {
                for ( int x = -hks; x <= hks; x++ )
                {
                    int k = ( ( y + hks ) * ks ) + ( x + hks );
                    assert( k >= 0 && k < m_Size * m_Size );
                    
                    int ii = i + x;
                    int jj = j + y;
                    
                    if ( mode == Clamp )
                    {
                        if ( ii < 0 )
                        {
                            ii = 0;
                        }
                        else if ( ii >= w )
                        {
                            ii = w - 1;
                        }
                        if ( jj < 0 )
                        {
                            jj = 0;
                        }
                        else if ( jj >= h )
                        {
                            jj = h - 1;
                        }
                        
                        src->GetPixel( ii, jj, pixel );
                    } 
                    else if ( mode == Border )
                    {
                        if ( ii < 0 || ii >= w || jj < 0 || jj >= h )
                        {
                            pixel = color;
                        }
                        else
                        {
                            src->GetPixel( ii, jj, pixel );
                        }
                    }
                    else
                    {
                        src->GetPixel( ii, jj, pixel );
                    }
                    
                    rr += ( float )pixel.r * m_Kernel[ k ];
                    gg += ( float )pixel.g * m_Kernel[ k ];
                    bb += ( float )pixel.b * m_Kernel[ k ];
                    if ( d > 3 )
                    {
                        aa += ( float )pixel.a * m_Kernel[ k ];
                    }
                }
            }
            
            pixel.r = ( unsigned char )rr;
            pixel.g = ( unsigned char )gg;
            pixel.b = ( unsigned char )bb;
            pixel.a = ( unsigned char )aa;
            
            dst->SetPixel( i, j, pixel );
        }
    }
    
    return dst;
}

///////////////////////////////////////////////////////////////////////////
// Private
///////////////////////////////////////////////////////////////////////////

float nrKernel::Gaussian( int x, int y, float sigma )
{
    float dx = 0.1f;
    float dy = 0.1f;
    
    float two_sigma_squared = 2.0f * sigma * sigma;
    
    float g = 0.0f;
    for ( float yy = y - 0.5f; yy < y + 0.5f + dy / 2; yy += dy )
    {
        for ( float xx = x - 0.5f; xx < x + 0.5f + dx / 2; xx += dx )
        {
            g += ( 1.0f / ( 2.0f * 3.14159265f * sigma * sigma ) ) * 
                nrMath::Pow( 2.718282f, -( ( x * x + y * y ) / 2.0f * sigma * sigma ) );
        }
    }
    
    return g;
}

////////////////////////////////////////////////////////////////////////////

void nrKernel::Normalize( void )
{
    int i, j;
    
    float sum = 0.0f;
    for ( j = 0; j < m_Size; j++ )
    {
        for ( i = 0; i < m_Size; i++ )
        {
            sum += K( i, j );
        }
    }
    
    if ( nrMath::Equal( sum, 0.0f ) )
    {
        return;
    }
    
    sum = 1.0f / sum;
    for ( j = 0; j < m_Size; j++ )
    {
        for ( i = 0; i < m_Size; i++ )
        {
            K( i, j ) *= sum;
        }
    }
    
}

////////////////////////////////////////////////////////////////////////////

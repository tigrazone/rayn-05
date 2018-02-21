////////////////////////////////////////////////////////////////////////////
//
// nrKernel.h
//
// A class for convolutions.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRKERNEL_H
#define NRKERNEL_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrPixel.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrImage;

class nrKernel
{
public:
    
    enum
    {
        Horizontal,
        Vertical,
    };
    
    enum
    {
        Confine,	// Kernel never extends beyond edges of the data.
        Clamp,		// Elements outside the edges are clamped to closest edge values.
        Border,		// Elements outside the edges are set to border color.
    };
    
    
public:
    
    nrKernel( void );
    ~nrKernel( void );
    
    // Create a gaussian kernel of the specified size (best if odd) and 
    // sigma.
    void CreateGaussian( int size = 5, float sigma = 1.4f );
    
    // Create a 3x3 sobel kernel to detect the class of edges specified.
    void CreateSobel( int edges = Horizontal );
    
    // Returns a new image containing the specified image convolved by the 
    // kernel.
    // The color parameter is only used in Border mode.
    nrImage* Convolve( const nrImage* image, int mode = Confine, const nrPixel& color = nrPixel( 255, 255, 255 ) ) const;
    
private:
    
    // Evaluate the gaussian function at x, y.
    float Gaussian( int x, int y, float sigma );
    
    // Normalize the kernel.
    void Normalize( void );
    
private:
    
    int m_Size;
    float* m_Kernel;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRKERNEL_H

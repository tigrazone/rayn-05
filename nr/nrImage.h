////////////////////////////////////////////////////////////////////////////
//
// nrImage.h
//
// A class for images.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRIMAGE_H
#define NRIMAGE_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColor.h"
#include "nrPixel.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrImage
{
public:
    
    nrImage( void );
    ~nrImage( void );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create a blank image of the specified dimensions. 
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error  message to the global log.
    bool CreateBlank( int width, int height, int depth = 3 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create an image read from the file specified by the filename. 
    //
    // They type of file to read is determined by the extension of the 
    // filename.
    //
    // PPM = Portable Pixmap format.
    // PCX = ZSoft Corporation PC Paintbrush format.
    // TGA = Truvision targa format.
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error message to the global log.
    bool CreateFromFile( const char* file_name );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create an image from an array of bytes.
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error message to the global log.
    bool CreateFromBytes( int width, int height, int depth, unsigned char* pixels );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create a 3 component checkerboard image of light and dark gray 
    // checks (64, 64, 64) and (192, 192, 192).
    //
    // For even check distribution, width and height should be multiples of 
    // num_checks. 
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error message to the global log.
    bool CreateChecker( int width, int height, int num_checks = 4 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create a 3 component sphere image.
    //
    // The sphere is calculated by using sqrt( 1 - d^2 ).  Where d is the 
    // distance from the center of the image to the pixel.
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error message to the global log.
    bool CreateSphere( int width, int height );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create a 3 component "grid" image.
    //
    // For an even number of grid spaces, width and height should be 
    // multiples of num_grids.
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error message to the global log.
    bool CreateGrid( int width, int height, int num_grids = 16 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Create a 3 component "blend" image.
    //
    // The blend is performed between the four corners of the image.
    //
    // If successful, returns true, otherwise returns false and writes an 
    // error message to the global log.
    bool CreateBlend( int width, int height, 
        const nrPixel& ul = nrPixel( 255, 0,   0 ), const nrPixel& ur = nrPixel( 255, 255, 0 ), 
        const nrPixel& ll = nrPixel(   0, 0, 255 ), const nrPixel& lr = nrPixel(   0, 255, 0 ) );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Scale the image to the power of two closest to but not less than the 
    // current width and height. 
    //
    // For example, a 400x129 image will be rescaled to 512x256. 
    // No filtering is used (simple point sample expansion).
    void ScaleToPowerOfTwo( void );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Add an alpha channel to the image. 
    //
    // The alpha channel added is fully opaque (i.e., all 255). 
    // The source image must have exactly three channels.
    void AddAlphaChannel( void );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Remove an alpha channel from the image.
    //
    // The source image must have exactly four channels.
    void RemoveAlphaChannel( void );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Generate a mask for the image. 
    //
    // The pixel indicates the color in the image that will be used as the 
    // mask color (i.e., all pixels of this color in the image will have an 
    // alpha of 0 in the new alpha channel - all other pixels will have an 
    // alpha of 255). 
    //
    // If the image doesn't have an alpha channel, one is added. 
    void Mask( const nrPixel& pixel );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Set a pixel value in the image. 
    //
    // Out of range x,y are clipped (i.e., the image is unchanged).
    // If the image doesn't have an alpha channel, alpha is ignored.
    void SetPixel( int x, int y, const nrPixel& pixel );
    void SetPixel( int x, int y, const nrColor& color );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Get a pixel value from the image. 
    // 
    // Out of range x,y are clipped (i.e., return parameters are unchanged).
    // If the image doesn't have an alpha channel, alpha returned is 
    // undefined.
    void GetPixel( int x, int y, nrPixel& pixel ) const;
    void GetPixel( int x, int y, nrColor& color ) const;
    
    ////////////////////////////////////////////////////////////////////////
    
    // Flip an image vertically.
    void FlipVertical( void );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Clear the image to a background color.
    //
    // The default background color is black.
    void Clear( const nrPixel& background = nrPixel( 0, 0, 0 ) );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Copy another image into this image.
    //
    // Both images must be the same depth.  The source image is clipped by 
    // the destination image.
    void Copy( const nrImage& src, int dst_x = 0, int dst_y = 0 );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Copy a portion of another image to this image.
    //
    // Both images must be the same depth.  The source image is clipped by 
    // the destination image.
    void CopySub( const nrImage& src, int src_x, int src_y, int src_w, int src_h, int dst_x = 0, int dst_y = 0 );
        
    ////////////////////////////////////////////////////////////////////////
    
    // Write the image to a file.
    //
    // If successful, returns true, otherwise returns false and writes an
    // error message to the global log.  Only PPM writing is supported.
    bool WriteToFile( const char* file_name ) const;
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the image width.
    int Width( void ) const;
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the image height.
    int Height( void ) const;
    
    ////////////////////////////////////////////////////////////////////////
    
    // Returns the image depth.
    int Depth( void ) const;
    
private:
    
    // Read a PPM file (Portable Pixmap).
    //
    // Only type P6 files (PPM raw) are supported.
    //
    // If an error occurs, an error message is logged and false is returned.
    bool ReadPPM( const char* filename );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Read a PCX file (ZSoft Corporation PC Paintbrush).
    //
    // Only version 5 files are supported.
    //
    // If an error occurs, an error message is logged and false is returned.
    bool ReadPCX( const char* filename );
    
    // Read a TGA file (Truvision Targa).
    //
    // Only type 2 (uncompressed 16/24/32 bit) files are supported.
    //
    // If an error occurs, an error message is logged and false is returned.
    bool ReadTGA( const char* filename );
    
    ////////////////////////////////////////////////////////////////////////
    
    // Write a PPM file (Portable Pixmap).
    //
    // Only type P6 files (PPM raw) are supported.
    //
    // If an error occurs, an error message is logged and false is returned.
    bool WritePPM( const char* file_name ) const;
    
    ////////////////////////////////////////////////////////////////////////
    
    // Read a TGA file (Truvision Targa).
    //
    // Only type 2 (uncompressed 16/24/32 bit) files are supported.
    //
    // If an error occurs, an error message is logged and false is returned.
    bool WriteTGA( const char* filename ) const;
    
public:
    
    int m_Width;
    int m_Height;
    int m_Depth;
    
    unsigned char* m_Image;
};


////////////////////////////////////////////////////////////////////////////

#endif  // NRIMAGE_H

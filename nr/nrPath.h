////////////////////////////////////////////////////////////////////////////
//
// nrPath.h
//
// A class for file paths.
//
// Nate Robins, March 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRPATH_H
#define NRPATH_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColor.h"
#include "nrPixel.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrPath
{
public:

	// Returns base filename (no extension).
	static const char* Basename( const char* path );
	
	// Returns directory, including trailing '/'.
	static const char* Directory( const char* path );

	// Returns (optional) drive letter, followed by ':'.
	static const char* Drive( const char* path );

	// Returns the extension, including leading '.'.
	static const char* Extension( const char* path );
	
	// Returns the filename, including the extension.
	static const char* Filename( const char* path );
	
private:

	// Split the path into parts.
	static void SplitPath( const char* path );

private:
    
    nrPath( void );
    ~nrPath( void );

};

////////////////////////////////////////////////////////////////////////////

#endif  // NRPATH_H

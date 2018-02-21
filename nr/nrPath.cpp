////////////////////////////////////////////////////////////////////////////
//
// nrPath.cpp
//
// A class for images.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrPath.h"

#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////

#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif
#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif
#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif
#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif


////////////////////////////////////////////////////////////////////////////
// Statics
////////////////////////////////////////////////////////////////////////////

static char m_Drive[ _MAX_DRIVE ];
static char m_Directory[ _MAX_DIR ];
static char m_Extension[ _MAX_EXT ];
static char m_Filename[ _MAX_FNAME ];
static char m_FilenameAndExt[ _MAX_FNAME + _MAX_EXT ];


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

const char* nrPath::Basename( const char* path )
{
	SplitPath( path );
	
	return m_Filename;
}

////////////////////////////////////////////////////////////////////////////

const char* nrPath::Directory( const char* path )
{
	SplitPath( path );
	
	return m_Directory;
}

////////////////////////////////////////////////////////////////////////////

const char* nrPath::Drive( const char* path )
{
	SplitPath( path );
	
	return m_Drive;
}

////////////////////////////////////////////////////////////////////////////

const char* nrPath::Extension( const char* path )
{
	SplitPath( path );
	
	return m_Extension;
}

////////////////////////////////////////////////////////////////////////////

const char* nrPath::Filename( const char* path )
{
	SplitPath( path );
	
	strcpy( m_FilenameAndExt, m_Filename );
	strcat( m_FilenameAndExt, m_Extension );

	return m_FilenameAndExt;
}


////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

void nrPath::SplitPath( const char* path )
{
#ifdef _WIN32
	_splitpath( path, m_Drive, m_Directory, m_Filename, m_Extension );
#else
	// !!!
#endif
}

////////////////////////////////////////////////////////////////////////////

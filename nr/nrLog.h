////////////////////////////////////////////////////////////////////////////
//
// nrLog.h
//
// A class for logging.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRLOG_H
#define NRLOG_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "nrPencil.h"


////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////

class nrLog;
extern nrLog g_Log;


////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////

typedef void ( *nrLogCB )( const char* string );


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrLog
{
public:
	
	nrLog( void );
	virtual ~nrLog( void );
	
	// Write to the log.
	void Write( const char* format, ... );
	
	// Write a timestamp to the log.
	void TimeStamp( void );
	
	// Redirect the log (NULL = use stderr).
	void Redirect( const char* file_name = NULL );
	
	// Register a callback invoked whenever Write*() is called.  The 
	// formatted string is passed to the callback.
	void OnWrite( nrLogCB on_write );
	
private:
	
	FILE* m_File;
	
	nrPencil m_Pencil;
	
	const char* m_String;

	nrLogCB m_OnWrite;
};


////////////////////////////////////////////////////////////////////////////

#endif  // NRLOG_H

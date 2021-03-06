////////////////////////////////////////////////////////////////////////////
//
// nrLog.cpp
//
// A class for logging.
//
// Nate Robins, March 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrLog.h"

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////
// Global
////////////////////////////////////////////////////////////////////////////

nrLog g_Log;


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrLog::nrLog( void )
{
	m_File = stderr;
	m_OnWrite = NULL;
}

////////////////////////////////////////////////////////////////////////////

nrLog::~nrLog( void )
{
	if ( m_File != stderr )
	{
		fclose( m_File );
	}
}

////////////////////////////////////////////////////////////////////////////

void nrLog::Write( const char* format, ... )
{
	va_list args;
	
	va_start( args, format );
	m_String = m_Pencil.Compose( format, args );
	va_end( args );
	
	fprintf( m_File, "%s", m_String );
    
	if ( m_File != stderr )
	{
		fflush( m_File );
	}
    
	if ( m_OnWrite != NULL )
	{
		m_OnWrite( m_String );
	}
}

////////////////////////////////////////////////////////////////////////////

//void nrLog::TimeStamp( void )
//{
//	SYSTEMTIME st;
//	GetLocalTime( &st );
//    
//	Write( "nrLog::nrLog() : %4d-%02d-%02d %d:%02d.\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute );
//}

////////////////////////////////////////////////////////////////////////////

void nrLog::Redirect( const char* file_name )
{
	FILE* file;
	
	if ( m_File != stderr )
	{
		fclose( m_File );
	}
	
	if ( file_name )
	{
		Write( "nrLog::Redirect() : Redirecting log to \"%s\".\n", file_name );
		
		file = fopen( file_name, "ab" );
		
		if ( file == NULL )
		{
			Write( "nrLog::Redirect() : Failed, using stderr.\n", file_name );
			
			file = stderr;
		}
	}
	else
	{
		Write( "nrLog::Redirect() : Redirecting log to stderr.\n" );
		
		file = stderr;
	}
	
	m_File = file;
}

////////////////////////////////////////////////////////////////////////////

void nrLog::OnWrite( nrLogCB on_write )
{
	Write( "nrLog::OnWrite() : Registering OnWrite callback.\n" );

	m_OnWrite = on_write;
}

////////////////////////////////////////////////////////////////////////////

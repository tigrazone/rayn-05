////////////////////////////////////////////////////////////////////////////
//
// nrCmdLine.cpp
//
// A class for parsing command line switches.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrCmdLine.h"

#include "nrLog.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////

nrCmdLineArg::nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, int& variable )
{
    m_Switch = _switch;
    m_Parameter = parameter;
    m_Default = _default;
    m_Description = description;
    m_Type = CMDLINEARG_INT;
    m_Int = &variable;
    m_Length = 0;
    m_Parsed = false;
}

////////////////////////////////////////////////////////////////////////////

nrCmdLineArg::nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, bool& variable )
{
    m_Switch = _switch;
    m_Parameter = parameter;
    m_Default = _default;
    m_Description = description;
    m_Type = CMDLINEARG_BOOL;
    m_Bool = &variable;
    m_Length = 0;
    m_Parsed = false;
}

////////////////////////////////////////////////////////////////////////////

nrCmdLineArg::nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, float& variable )
{
    m_Switch = _switch;
    m_Parameter = parameter;
    m_Default = _default;
    m_Description = description;
    m_Type = CMDLINEARG_FLOAT;
    m_Float = &variable;
    m_Length = 0;
    m_Parsed = false;
}

////////////////////////////////////////////////////////////////////////////

nrCmdLineArg::nrCmdLineArg( const char* _switch, const char* parameter, const char* _default, const char* description, char* variable, int length )
{
    m_Switch = _switch;
    m_Parameter = parameter;
    m_Default = _default;
    m_Description = description;
    m_Type = CMDLINEARG_STRING;
    m_String = variable;
    m_Length = length;
    m_Parsed = false;
}

////////////////////////////////////////////////////////////////////////////

void nrCmdLineArg::Parse( const char* parameter )
{
    switch ( m_Type )
    {
    case CMDLINEARG_INT:
        if ( m_Int )
        {
            sscanf( parameter, "%d", m_Int );
        }
        break;
    case CMDLINEARG_BOOL:
        if ( m_Bool )
        {
            *m_Bool = ( strcmp( parameter, "true" ) == 0 );
        }
        break;
    case CMDLINEARG_FLOAT:
        if ( m_Float )
        {
            sscanf( parameter, "%f", m_Float );
        }
        break;
    case CMDLINEARG_STRING:
        if ( m_String )
        {
            strncpy( m_String, parameter, m_Length - 1 );
        }
        break;
    default:
        assert( 0 );
        break;
    }
    
    m_Parsed = true;
}

////////////////////////////////////////////////////////////////////////////

nrCmdLine::nrCmdLine( nrCmdLineArg* switches, int num_switches )
{
    m_NumArgs = num_switches;
    m_Args = switches;
}

////////////////////////////////////////////////////////////////////////////

nrCmdLine::~nrCmdLine()
{
}

////////////////////////////////////////////////////////////////////////////

bool nrCmdLine::Parse( int argc, const char** argv )
{
    // First off, stick the defaults in place.
    for ( int i = 0; i < m_NumArgs; i++ )
    {
        nrCmdLineArg& s = m_Args[ i ];
        
        if ( s.m_Default )
        {
            s.Parse( s.m_Default );
        }
    }
    
    // Now parse the command line.
    int n = 1;
    while ( n < argc )
    {
        const char* _switch = argv[ n ];
        
        bool parsed = false;
        for ( int i = 0; i < m_NumArgs; i++ )
        {
            nrCmdLineArg& s = m_Args[ i ];
            
            if ( s.m_Switch && strcmp( _switch, s.m_Switch ) == 0 )
            {
                n++;
                if ( n >= argc )
                {
                    g_Log.Write( "nrCmdLine::Parse(): Malformed command line (switch requires a parameter).\n " );
                    return false;
                }
                s.Parse( argv[ n ] );
                parsed = true;
                break;
            }
        }
        
        if ( ! parsed )
        {
            for ( int i = 0; i < m_NumArgs; i++ )
            {
                nrCmdLineArg& s = m_Args[ i ];
                
                if ( s.m_Switch == 0 && ! s.m_Parsed )
                {
                    s.Parse( _switch );
                    parsed = true;
                    break;
                }
            }
        }
        
        if ( ! parsed )
        {
            g_Log.Write( "nrCmdLine::Parse(): Malformed command line (expected a switch).\n " );
            return false;
        }
        
        n++;
    }
    
    // Make sure we parsed all required parameters.
    for ( int j = 0; j < m_NumArgs; j++ )
    {
        nrCmdLineArg& s = m_Args[ j ];
        
        if ( s.m_Default == 0 && ! s.m_Parsed )
        {
            g_Log.Write( "nrCmdLine::Parse(): Malformed command line (required parameter missing).\n " );
            return false;
        }
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

void nrCmdLine::Usage( const char* program_name ) const
{
    assert( m_Args );
    
    #ifdef WIN32
    {
        // Under Windows, the program_name is the full path.  Cut that down
        // to just the basename.
        char drive[ _MAX_DRIVE ];
        char directory[ _MAX_DIR ];
        char extension[ _MAX_EXT ];
        static char filename[ _MAX_FNAME ];
        
        _splitpath( program_name, drive, directory, filename, extension );
        
        program_name = filename;
    }
    #endif

    printf( "\n" );
    printf( "usage: %s [options]", program_name );

    // Print required parameters.
    for ( int i = 0; i < m_NumArgs; i++ )
    {
        nrCmdLineArg& s = m_Args[ i ];
        
        if ( s.m_Default == 0 )
        {
            if ( s.m_Switch == 0 )
            {
                printf( " %s", s.m_Parameter );
            }
            else
            {
                printf( " %s %s", s.m_Switch, s.m_Parameter );
            }
        }
    }
    
    printf( "\n\n" );
    
    printf( " required:\n\n" );
    for ( int j = 0; j < m_NumArgs; j++ )
    {
        nrCmdLineArg& s = m_Args[ j ];
        
        if ( s.m_Default == 0 )
        {
            if ( s.m_Switch )
            {
                printf( "  %-10s %-15s %s\n", s.m_Switch, s.m_Parameter, s.m_Description );
            }
            else
            {
                printf( "  %-10s %-15s %s\n", "", s.m_Parameter, s.m_Description );
            }
        }
    }
    printf( "\n" );

    printf( " options:\n\n" );
    
    for ( int k = 0; k < m_NumArgs; k++ )
    {
        nrCmdLineArg& s = m_Args[ k ];
        
        if ( s.m_Switch && s.m_Default )
        {
            printf( "  %-10s %-15s %s (%s)\n", s.m_Switch, s.m_Parameter, s.m_Description, s.m_Default );
        }
    }

    printf( "\n\n" );
}

////////////////////////////////////////////////////////////////////////////

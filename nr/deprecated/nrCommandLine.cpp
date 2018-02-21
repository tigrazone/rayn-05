////////////////////////////////////////////////////////////////////////////
//
// nrCommandLine.cpp
//
// A class for command line parsing.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrCommandLine.h"

#include "nrLog.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////

nrCommandLine::nrCommandLine()
{
    m_NumOptions = 0;
    m_Options = 0;
    m_Arguments = 0;
}

////////////////////////////////////////////////////////////////////////////

nrCommandLine::~nrCommandLine()
{
    int i;

    for ( i = 0; i < m_NumOptions; i++ )
    {
        delete [] m_Options[ i ];
    }
    delete [] m_Options;

    for ( i = 0; i < m_NumOptions; i++ )
    {
        delete [] m_Arguments[ i ];
    }
    delete [] m_Arguments;
}

////////////////////////////////////////////////////////////////////////////

bool nrCommandLine::Create( int argc, const char** argv )
{
    int i;
    
    assert( m_NumOptions == 0 );    // Create() is being called more than once.
    
    // Count the number of options.
    for ( i = 1; i < argc; i++ )
    {
        const char* option = argv[ i ];

        if ( option[ 0 ] == '-' )
        {
            m_NumOptions++;
        }
    }
    
    // Allocate memory for the options/arguments.
    m_Options   = new char*[ m_NumOptions ];
    m_Arguments = new char*[ m_NumOptions ];
    if ( m_Options == 0 || m_Arguments == 0 )
    {
        g_Log.Write( "nrCommandLine::Create(): Out of memory.\n" );
        return false;
    }
    
    // Sort the command line into option/argument pairs & save off the 
    // response file if one was found.
    int n = 0;
    const char* response = 0;
    
    i = 1;
    while ( i < argc )
    {
        const char* option = argv[ i ];
        
        if ( option[ 0 ] == '-' )
        {
            // Save off the option name.
            m_Options[ n ] = new char[ strlen( option ) ];
            if ( m_Options[ n ] == 0 )
            {
                g_Log.Write( "nrCommandLine::Create(): Out of memory.\n" );
                return false;
            }
            strcpy( m_Options[ n ], &( option[ 1 ] ) );
            
            i++;
            
            // Save off the argument (if there is one).
            m_Arguments[ n ] = 0;
            if ( i < argc )
            {
                const char* argument = argv[ i ];
                
                if ( argument[ 0 ] != '-' && argument[ 0 ] != '@' )
                {
                    m_Arguments[ n ] = new char[ strlen( argument ) + 1 ];
                    if ( m_Arguments[ n ] == 0 )
                    {
                        g_Log.Write( "nrCommandLine::Create(): Out of memory.\n" );
                        return false;
                    }
                    strcpy( m_Arguments[ n ], argument );
                    
                    i++;
                }
            }
            
            n++;
        }
        else if ( option[ 0 ] == '@' )
        {
            assert( response == 0 );    // More than one response file on the command line.
            response = &( option[ 1 ] );
            
            i++;
        }
        else
        {
            g_Log.Write( "nrCommandLine::Create(): Malformed command line (expected an option).\n " );
            return false;
        }
    }
    
    // Handle the response file (if there is one).
    if ( response )
    {
        FILE* f = fopen( response, "rb" );
        if ( f == 0 )
        {
            g_Log.Write( "nrCommandLine::Create(): Can't open response file \"%s\".\n", response );
            return false;
        }
        
        // First, count the number of lines in the response file & make 
        // sure the option doesn't already exist.
        
        int num_lines = 0;
        char line[ 256 ];
        while ( fgets( line, 256, f ) )
        {
            const char* option = strtok( line, " \t-" );

            if ( ! Exists( option ) )
            {
                num_lines++;
            }
        }
        
        // Then, reallocate the options/arguments.
        char** options   = new char*[ m_NumOptions + num_lines ];
        char** arguments = new char*[ m_NumOptions + num_lines ];
        if ( options == 0 || arguments == 0 )
        {
            g_Log.Write( "nrCommandLine::Create(): Out of memory.\n" );
            return false;
        }
        memcpy( options, m_Options, sizeof ( char* ) * m_NumOptions );
        memcpy( arguments, m_Arguments, sizeof ( char* ) * m_NumOptions );
        delete [] m_Options;
        delete [] m_Arguments;
        m_Options = options;
        m_Arguments = arguments;
        
        // Finally, re-read the file and add the new option/argument pairs.
        rewind( f );
        n = m_NumOptions;
        while ( fgets( line, 256, f ) )
        {
            const char* option   = strtok( line, "-\t " );
            const char* argument = strtok( NULL, " \t\r\n" );
            
            if ( ! Exists( option ) )
            {
                m_Options[ n ] = new char[ strlen( option ) + 1 ];
                if ( m_Options[ n ] == 0 )
                {
                    g_Log.Write( "nrCommandLine::Create(): Out of memory.\n" );
                    return false;
                }
                strcpy( m_Options[ n ], option );
                
                if ( argument )
                {
                    m_Arguments[ n ] = new char[ strlen( argument ) + 1 ];
                    if ( m_Arguments[ n ] == 0 )
                    {
                        g_Log.Write( "nrCommandLine::Create(): Out of memory.\n" );
                        return false;
                    }
                    strcpy( m_Arguments[ n ], argument );
                }
                else
                {
                    m_Arguments[ n ] = 0;
                }
                
                n++;
            }
        }
        
        m_NumOptions = n;
        
        fclose( f );
    }
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

int nrCommandLine::NumOptions( void ) const
{
    return m_NumOptions;
}

////////////////////////////////////////////////////////////////////////////

const char* nrCommandLine::Option( int n ) const
{
    assert( n >= 0 && n < m_NumOptions );

    return m_Options[ n ];
}

////////////////////////////////////////////////////////////////////////////

const char* nrCommandLine::Argument( int n ) const
{
    assert( n >= 0 && n < m_NumOptions );

    return m_Arguments[ n ];
}


////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

bool nrCommandLine::Exists( const char* option ) const
{
    for ( int i = 0; i < m_NumOptions; i++ )
    {
        if ( strcmp( option, m_Options[ i ] ) == 0 )
        {
            return true;
        }
    }
    
    return false;
}

////////////////////////////////////////////////////////////////////////////

//#define TEST_COMMANDLINE
#ifdef TEST_COMMANDLINE

void main( void )
{
    int i;
    
    const char* argv1[] = { "program", "-option1", "argument1", "-option2", "argument2", "-option3", "argument3" };
    const char* argv2[] = { "program", "-option1", "argument1", "-option2", "argument2", "-option3" };
    const char* argv3[] = { "program", "-option1", "argument1", "-option2", "-option3", "argument3" };
    const char* argv4[] = { "program", "-option1", "-option2", "argument2", "-option3", "argument3" };
    const char* argv5[] = { "program", "-option1", "-option2", "-option3", "argument3" };
    const char* argv6[] = { "program" };
    const char* argv7[] = { "program", "@response.txt" };
    const char* argv8[] = { "program", "-option1", "@response.txt" };
    const char* argv9[] = { "program", "-option1", "argument1", "@response.txt", "-option2", "argument2" };
    
    nrCommandLine a1;
    a1.Create( 7, argv1 );
    
    g_Log.Write( "a1:\n" );
    for ( i = 0; i < a1.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a1.Option( i ), a1.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a2;
    a2.Create( 6, argv2 );

    g_Log.Write( "a2:\n" );
    for ( i = 0; i < a2.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a2.Option( i ), a2.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a3;
    a3.Create( 6, argv3 );

    g_Log.Write( "a3:\n" );
    for ( i = 0; i < a3.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a3.Option( i ), a3.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a4;
    a4.Create( 6, argv4 );

    g_Log.Write( "a4:\n" );
    for ( i = 0; i < a4.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a4.Option( i ), a4.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a5;
    a5.Create( 5, argv5 );
    
    g_Log.Write( "a5:\n" );
    for ( i = 0; i < a5.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a5.Option( i ), a5.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a6;
    a6.Create( 0, argv6 );

    g_Log.Write( "a6:\n" );
    for ( i = 0; i < a6.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a6.Option( i ), a6.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a7;
    a7.Create( 2, argv7 );

    g_Log.Write( "a7:\n" );
    for ( i = 0; i < a7.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a7.Option( i ), a7.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a8;
    a8.Create( 3, argv8 );

    g_Log.Write( "a8:\n" );
    for ( i = 0; i < a8.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a8.Option( i ), a8.Argument( i ) );
    }
    g_Log.Write( "\n" );
    
    nrCommandLine a9;
    a9.Create( 6, argv9 );

    g_Log.Write( "a9:\n" );
    for ( i = 0; i < a9.NumOptions(); i++ )
    {
        g_Log.Write( "option: %s\targument: %s\n", a9.Option( i ), a9.Argument( i ) );
    }
    g_Log.Write( "\n" );
}
#endif

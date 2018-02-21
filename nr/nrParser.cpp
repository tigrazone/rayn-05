////////////////////////////////////////////////////////////////////////////
//
// nrParser.cpp
//
// A class for parsing.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrParser.h"

#include "nrLog.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////

char* nrParser::m_TokenStrings[ TOKEN_COUNT ] =
{
    "key",
    "string",
    "float",
    "int",
    "{ (begin aggregate)",
    "} (end aggregate)",
    "< (begin list)",
    "> (end list)",
    "eof",
    "unknown",
};


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

bool nrParser::Open( const char* file_name )
{
    m_File = fopen( file_name, "rb" );
    if ( m_File == 0 )
    {
        return false;
    }
    
    m_Line = 1;

    rewind( m_File );
    fseek( m_File, 0, SEEK_END );
    int size = ftell( m_File );
    rewind( m_File );
    m_Progress.Reset( size );

    m_Error = false;
    m_Token = Tokenize();

    return true;
}

////////////////////////////////////////////////////////////////////////////

void nrParser::Close( void )
{
    fclose( m_File );
}

////////////////////////////////////////////////////////////////////////////

inline int nrParser::GetC( void )
{
    int c = getc( m_File );
    
    if ( c != EOF )
    {
        m_Progress.Update();
    }
    
    return c;
}

////////////////////////////////////////////////////////////////////////////

inline void nrParser::UnGetC( int c )
{
    ungetc( c, m_File );

    m_Progress.Update( -1 );
}

////////////////////////////////////////////////////////////////////////////

nrParser::nrToken nrParser::NextToken( void )
{
    return m_Token;
}

////////////////////////////////////////////////////////////////////////////

void nrParser::ReadToken( nrToken expecting )
{
    if ( m_Token != expecting )
    {
        ParseError( expecting );
    }
    
    m_Token = Tokenize();
}

////////////////////////////////////////////////////////////////////////////

int nrParser::ReadInt( void )
{
    if ( m_Token != TOKEN_INT )
    {
        ParseError( TOKEN_INT );
    }
    
    int value = atoi( m_Value );
    
    m_Token = Tokenize();
    
    return value;
}

////////////////////////////////////////////////////////////////////////////

float nrParser::ReadFloat( void )
{
    if ( m_Token != TOKEN_FLOAT && m_Token != TOKEN_INT )
    {
        ParseError( TOKEN_FLOAT );
    }
    
    float value = ( float )atof( m_Value );
    
    m_Token = Tokenize();
    
    return value;
}

////////////////////////////////////////////////////////////////////////////

const char* nrParser::ReadString( void )
{
    if ( m_Token != TOKEN_STRING )
    {
        ParseError( TOKEN_STRING );
    }
    
    strncpy( m_PrevValue, m_Value, 4096 );
    
    m_Token = Tokenize();
    
    return m_PrevValue;
}

////////////////////////////////////////////////////////////////////////////

const char* nrParser::ReadKey( void )
{
    if ( m_Token != TOKEN_KEY )
    {
        ParseError( TOKEN_KEY );
    }
    
    strncpy( m_PrevValue, m_Value, 4096 );
    
    m_Token = Tokenize();
    
    return m_PrevValue;
}

////////////////////////////////////////////////////////////////////////////

bool nrParser::KeyMatches( const char* key, const char* match )
{
    if ( strcmp( key, match ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrParser::ParseError( const char* error )
{
    g_Log.Write( "nrParser::ReadToken(): parse error, line %d: %s\n", m_Line, error );
    m_Error = true;
}

////////////////////////////////////////////////////////////////////////////

bool nrParser::Error( void )
{
    bool error = m_Error;
    
    m_Error = false;
    
    return error;
}


////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

void nrParser::ParseError( nrToken expected )
{
    if ( m_Value[ 0 ] )
    {
        g_Log.Write( "nrParser::ReadToken(): parse error, line %d: expected `%s', encountered `%s' (\"%s\").\n", m_Line, m_TokenStrings[ expected ], m_TokenStrings[ m_Token ], m_Value );
    }
    else
    {
        g_Log.Write( "nrParser::ReadToken(): parse error, line %d: expected `%s', encountered `%s'.\n", m_Line, m_TokenStrings[ expected ], m_TokenStrings[ m_Token ] );
    }
    m_Error = true;
}

////////////////////////////////////////////////////////////////////////////

nrParser::nrToken nrParser::Tokenize( void )
{
    int c;
    
nrParser__Tokenize__StartOver:
    
    m_Value[ 0 ] = 0;
    
    do
    {
        c = GetC();
	
        if ( c == '\n' )
        {
            m_Line++;
        }
    } while ( isspace( c ) );
    
    if ( isalpha( c ) || c == '_' )
    {
        int i = 0;
        
        do
        {
            m_Value[ i++ ] = c;
            c = GetC();
        } while ( isalnum( c ) || c == '_' );
        UnGetC( c );
        m_Value[ i ] = 0;
        
        return TOKEN_KEY;
    }
    else if ( isdigit( c ) || c == '-' || c == '+' )
    {
        int i = 0;
        bool parsing_float = false;
        
        do
        {
            m_Value[ i++ ] = c;
            c = GetC();
            
            // If the digit stream contains a period (.) or an exponent
            // (e,E), the number must be a floating point number.
            if ( c == '.' || c == 'e' || c == 'E' )
            {
                parsing_float = true;
            }
        } while ( isdigit( c ) || c == '.' || c == '-' || c == '+' || c == 'e' || c == 'E' );
        UnGetC( c );
        m_Value[ i ] = 0;
        
        if ( parsing_float )
        {
            return TOKEN_FLOAT;
        }
        else
        {
            return TOKEN_INT;
        }
    }
    else if ( c == '"' )
    {
        int i = 0;
        
        while ( ( c = GetC() ) != '"' )
        {
            m_Value[ i++ ] = c;
            
            // Possibly add some more robust error checking in here, since a
            // string could potentially go on forever (even to the EOF).
            // Probably ought to generate an error/warning if a '\n' is 
            // found.
            // !!!
            
        }
        m_Value[ i ] = 0;
        
        return TOKEN_STRING;
    }
    else if ( c == '/' || c == '#' )
    {
        do
        {
            c = GetC();
        } while ( c != '\r' && c != '\n' );
        UnGetC( c );
        
        goto nrParser__Tokenize__StartOver;
        //return TOKEN_COMMENT;
    }
    else if ( c == '{' )
    {
        return TOKEN_AGGREGATE_BEGIN;
    }
    else if ( c == '}' )
    {
        return TOKEN_AGGREGATE_END;
    }
    else if ( c == '<' )
    {
        return TOKEN_LIST_BEGIN;
    }
    else if ( c == '>' )
    {
        return TOKEN_LIST_END;
    }
    else if ( c == EOF )
    {
        return TOKEN_EOF;
    }
    
    m_Value[ 0 ] = c;
    m_Value[ 1 ] = 0;
    return TOKEN_UNKNOWN;
}

////////////////////////////////////////////////////////////////////////////

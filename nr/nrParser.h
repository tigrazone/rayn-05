////////////////////////////////////////////////////////////////////////////
//
// nrParser.cpp
//
// A class for parsing.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRPARSER_H
#define NRPARSER_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "nrProgress.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrParser
{
    
public:
    
    typedef enum
    {
        TOKEN_KEY,
        TOKEN_STRING,
        TOKEN_FLOAT,
        TOKEN_INT,
        TOKEN_AGGREGATE_BEGIN,
        TOKEN_AGGREGATE_END,
        TOKEN_LIST_BEGIN,
        TOKEN_LIST_END,
        TOKEN_EOF,
        TOKEN_UNKNOWN,
        
        TOKEN_COUNT,
    } nrToken;
    
private:
    
    static char* m_TokenStrings[ TOKEN_COUNT ];
    
public:
    
    // Open a stream (file) for parsing.
    bool Open( const char* file_name );
    
    // Close the stream (file) being parsed.
    void Close( void );

    // Returns the type of the next token in the stream.
    nrParser::nrToken NextToken( void );
    
    // Reads the expected token type from the stream.
    // 
    // If the next token in the stream is not of the expected type, a parse 
    // error is issued.
    void ReadToken( nrParser::nrToken expected );
    
    // Reads a TOKEN_INT from the stream.
    // 
    // If the next token in the stream is not of type TOKEN_INT, a parse 
    // error is issued and the value returned is undefined.
    int ReadInt( void );
    
    // Reads a TOKEN_FLOAT from the stream.
    // 
    // If the next token in the stream is of type TOKEN_INT, it will be
    // coerced into a TOKEN_FLOAT.
    // If the next token in the stream is not of type TOKEN_FLOAT, a parse 
    // error is issued and the value returned is undefined.
    float ReadFloat( void );
    
    // Reads a TOKEN_STRING from the stream.
    // 
    // If the next token in the stream is not of type TOKEN_STRING, a parse 
    // error is issued and the value returned is undefined.
    const char* ReadString( void );
    
    // Reads a TOKEN_KEY from the stream.
    // 
    // If the next token in the stream is not of type TOKEN_KEY, a parse 
    // error is issued and the value returned is undefined.
    const char* ReadKey( void );
    
    // Returns true of the key is equal to match, false otherwise.
    //
    // Does a case sensitive compare of the two strings.
    bool KeyMatches( const char* key, const char* match );
    
    // Reports a parse error.
    //
    // This function provides a facility for generating a parse error on
    // the application side (for example, if an unhandled key appears in
    // an aggregate).  This function sets the error flag.
    void ParseError( const char* error );
    
    // Returns true if there was at least one parse error.
    // 
    // The error flag is set if one or more parse errors occur.  Calling
    // this function clears the flag.
    bool Error( void );
    
private:
    
    // Report an internal parse error.
    void ParseError( nrToken expected );
    
    // Read the next token from the stream.
    nrToken Tokenize( void );
    
    // Read the next character from the stream.
    inline int GetC( void );

    // Put the character back on the stream.
    inline void UnGetC( int c );
    
private:
    
    FILE* m_File;
    int   m_Line;
    int   m_Lines;
    
    nrToken m_Token;
    char    m_Value[ 4096 ];
    char    m_PrevValue[ 4096 ];
    
    bool    m_Error;

    nrProgress m_Progress;
};

////////////////////////////////////////////////////////////////////////////


#endif  // NRPARSER_H

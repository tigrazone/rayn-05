////////////////////////////////////////////////////////////////////////////
//
// nrCommandLine.h
//
// A class for command line parsing.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRCOMMANDLINE_H
#define NRCOMMANDLINE_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrCommandLine
{
public:

    nrCommandLine();
    ~nrCommandLine();

public:
    
    // Create a command line given an argc and argv from an entry point
    // such as main().
    bool Create( int argc, const char** argv );
    
    // Return the number of options.
    int NumOptions( void ) const;
    
    // Return the n'th option.
    const char* Option( int n ) const;
    
    // Return the n'th options argument (or 0 if the option doesn't have 
    // one).
    const char* Argument( int n ) const;
    
private:

    // Returns true if the option was specified on the command line, false
    // otherwise.
    bool Exists( const char* option ) const;
    
private:
    
    int    m_NumOptions;
    char** m_Options;
    char** m_Arguments;
};


////////////////////////////////////////////////////////////////////////////

#endif  // NRCOMMANDLINE_H

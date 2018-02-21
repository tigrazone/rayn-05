////////////////////////////////////////////////////////////////////////////
//
// nrAtomScalar.h
//
// A class for atoms containing a scalar.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRATOMSCALAR_H
#define NRATOMSCALAR_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrAtom.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrAtomScalar : public nrAtom
{
public:
    
    nrAtomScalar( const char* name = 0 );
    ~nrAtomScalar( void );
    
    // Return a new scalar atom created from an integer value.
    static nrAtomScalar* NewFromInt( int scalar, const char* name = 0 );
    
    // Return a new scalar atom created from a floating point value.
    static nrAtomScalar* NewFromFloat( float scalar, const char* name = 0 );
    
    // Return a new scalar atom created from a string value.
    static nrAtomScalar* NewFromString( const char* scalar, const char* name = 0 );
    
public:
    
    union 
    {
        int    m_ScalarInt;
        float  m_ScalarFloat;
        char*  m_ScalarString;
    };
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRATOMSCALAR_H

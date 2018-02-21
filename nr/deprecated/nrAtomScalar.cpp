////////////////////////////////////////////////////////////////////////////
//
// nrAtomScalar.cpp
//
// A class for atoms containing a scalar.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrAtomScalar.h"

#include <assert.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrAtomScalar::nrAtomScalar( const char* name ) :
    nrAtom( name )
{
}

////////////////////////////////////////////////////////////////////////////

nrAtomScalar::~nrAtomScalar( void )
{
    if ( m_Type == STRING )
    {
        if ( m_ScalarString != 0 )
        {
            delete [] m_ScalarString;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

nrAtomScalar* nrAtomScalar::NewFromInt( int value, const char* name )
{
    nrAtomScalar* a = new nrAtomScalar( name );
    assert( a );    // out of memory.
    
    a->m_Type = INT;
    a->m_ScalarInt = value;
    
    return a;
}

////////////////////////////////////////////////////////////////////////////

nrAtomScalar* nrAtomScalar::NewFromFloat( float value, const char* name )
{
    nrAtomScalar* a = new nrAtomScalar( name );
    assert( a );    // out of memory.
    
    a->m_Type = FLOAT;
    a->m_ScalarFloat = value;
    
    return a;
}

////////////////////////////////////////////////////////////////////////////

nrAtomScalar* nrAtomScalar::NewFromString( const char* value, const char* name )
{
    nrAtomScalar* a = new nrAtomScalar( name );
    assert( a );    // out of memory.
    
    a->m_Type = STRING;

    if ( value != 0 )
    {
        a->m_ScalarString = new char[ strlen( value ) + 1 ];
        assert( a->m_ScalarString );    // out of memory.
        strcpy( a->m_ScalarString, name );
    }

    return a;
}

////////////////////////////////////////////////////////////////////////////

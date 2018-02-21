////////////////////////////////////////////////////////////////////////////
//
// nrAtomList.h
//
// A class for atoms containing lists.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRATOMLIST_H
#define NRATOMLIST_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"
#include "nrAtom.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrAtomList : public nrAtom
{
public:
    
    nrAtomList( const char* name = 0 );
    ~nrAtomList( void );
    
    // Return a new list atom created from an integer list.
    static nrAtomList* NewFromInt( const nrArray<int>& list, const char* name = 0 );
    
    // Return a new list atom created from a floating point list.
    static nrAtomList* NewFromFloat( const nrArray<float>& list, const char* name = 0 );
    
    // Return a new list atom created from a string list.
    static nrAtomList* NewFromString( const nrArray<char*>& list, const char* name = 0 );
    
public:
    
    nrArray<int>   m_ListInt;
    nrArray<float> m_ListFloat;
    nrArray<char*> m_ListString;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRATOMLIST_H

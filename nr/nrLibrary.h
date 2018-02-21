////////////////////////////////////////////////////////////////////////////
//
// nrLibrary.h
//
// A class for librarying.
//
// Nate Robins, March 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRLIBRARY_H
#define NRLIBRARY_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

template <class T> class nrLibrary
{
public:
    
    inline nrLibrary( void );
    inline ~nrLibrary( void );
    
    // Add an item to the library.  
	// 
	// If an item of the same name already exists in the library, the
	// item is not added again (i.e., every item in the library must
	// have a unique name.
    inline void Add( const char* key, T* item );
    
    // Remove an item from the library.
    //inline void Remove( const char* key );
    
    // Find an item in the library.
    //
    // If the item couldn't be found, 0 is returned.
    inline T* Find( const char* key ) const;
    
private:
	
    nrArray< char* > m_Keys;
    nrArray< T* > m_Items;
};

////////////////////////////////////////////////////////////////////////////

#include "nrLibrary.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRLIBRARY_H
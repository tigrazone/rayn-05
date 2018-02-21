////////////////////////////////////////////////////////////////////////////
//
// nrHash.h
//
// A hash table class.
//
// Nate Robins, March 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRHASH_H
#define NRHASH_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

template <class T> class nrHash
{
public:
    
    inline nrHash( void );
    inline ~nrHash( void );
    
    // Operations //////////////////////////////////////////////////////////
    
    // Set the size of the hash table (must be called before anything is 
    // added to the hash table).
    inline void Size( int size );
    
    // Add an item to the hash table.
    inline void Add( const char* key, T item );
    
    // Remove an item from the list (collapses the array, but does not
    // compress the array).
    //inline void Remove( int index );
    
    // Return a reference to an item in the hash table.
    inline T& operator[]( const char* key ) const;
    
    // Clear the hash table (remove all items).
    inline void Clear( void );
    
private:
    
    // Return a hash value given a key.
    inline int Hash( const char* key );
    
private:
    
    int           m_Size;
    nrArray< T >* m_Hash;
};

////////////////////////////////////////////////////////////////////////////

#include "nrHash.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRHASH_H

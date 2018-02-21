////////////////////////////////////////////////////////////////////////////
//
// nrArray.h
//
// A dynamic array class.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRARRAY_H
#define NRARRAY_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

template <class T> class nrArray
{
public:
    
    inline nrArray( int num_items = 0 );
    inline ~nrArray( void );
    
    // Operations //////////////////////////////////////////////////////////
    
    // Add an item to the (end of the) list.
    inline void Add( T item );
    
    // Remove an item from the list (collapses the array, but does not
    // compress the array).
    //inline void Remove( int index );
    
    // Get an item from the list.
    inline T Get( int index ) const;
    
    // Return a reference to an item in the list.
    inline T& operator[]( int index ) const;
    
    // Return the number of items in the array.
    inline int Length( void ) const;
    
    // Compress the array to use the minimum amount of memory necessary.
    inline void Compress( void );
    
    // Clear the array (remove all items and compress).
    inline void Clear( void );
    
    // Sort the array.
    inline void Sort( int ( *compare )( const void* a, const void* b ) );
    
private:
    
    // Expand the array.
    inline void Expand( int expansion );
    
private:
    
    int m_NumAllocated;
    int m_NumItems;
    T*  m_Items;
};

////////////////////////////////////////////////////////////////////////////

#include "nrArray.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRARRAY_H

////////////////////////////////////////////////////////////////////////////
//
// nrArray.inl
//
// A dynamic array class.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"

#include <assert.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

template <class T> inline nrArray<T>::nrArray( int num_items )
{
    m_Items = 0;
    
    Clear();
    
    if ( num_items > 0 )
    {
        Expand( num_items );
    }
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline nrArray<T>::~nrArray( void )
{
    delete [] m_Items;
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrArray<T>::Add( T item )
{
    if ( m_NumItems + 1 > m_NumAllocated )
    {
        if ( m_NumItems == 0 )
        {
            Expand( 1 );
        }
        else
        {
            Expand( m_NumItems );
        }
    }
    
    m_Items[ m_NumItems ] = item;
    
    m_NumItems++;
}

////////////////////////////////////////////////////////////////////////////

//template <class T> inline void nrArray<T>::Remove( int index )
//{
//}

////////////////////////////////////////////////////////////////////////////

template <class T> inline T nrArray<T>::Get( int index ) const
{
    assert( index >= 0 && index < m_NumItems );
    
    return m_Items[ index ];
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline T& nrArray<T>::operator[]( int index ) const
{
    assert( index >= 0 && index < m_NumItems );
    
    return m_Items[ index ];
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline int nrArray<T>::Length( void ) const
{
    return m_NumItems;
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrArray<T>::Compress( void )
{
    if ( m_NumAllocated == m_NumItems )
    {
        return;
    }
    
    m_NumAllocated = m_NumItems;
    
    T* array = new T[ m_NumAllocated ];
    assert( array != 0 );
    
    memcpy( array, m_Items, sizeof( T ) * m_NumItems );
    
    delete [] m_Items;
    m_Items = array;
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrArray<T>::Clear( void )
{
    delete [] m_Items;
    m_Items = 0;
    
    m_NumItems = 0;
    m_NumAllocated = 0;
}

////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrArray<T>::Expand( int expansion )
{
	m_NumAllocated = m_NumItems + expansion;
    
    T* array = new T[ m_NumAllocated ];
    assert( array != 0 );
    
    memcpy( array, m_Items, sizeof( T ) * m_NumItems );
    
    delete [] m_Items;
    m_Items = array;
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrArray<T>::Sort( int ( *compare )( const void* a, const void* b ) )
{
	//   int compare( const void* _a, const void* _b )
	//   {
	//      const T& a = *( T* )_a;
	//      const T& b = *( T* )_b;
	//      
	//      if ( a > b )
	//      {
	//         return 1;
	//      }
	//      else if ( b > a )
	//      {
	//         return -1;
	//      }
	//      else
	//      {
	//         return 0;
	//      }
	//   }

	qsort( m_Items, m_NumItems, sizeof ( T ), compare );
}

////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
//
// nrHash.inl
//
// A linked Hash class.
//
// Nate Robins, March 2002.
//
////////////////////////////////////////////////////////////////////////////







<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

Bah, this is broken.


<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<










////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrHash.h"

#include <assert.h>
#include <crtdbg.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

template <class T> inline nrHash<T>::nrHash( void )
{
    m_Hash = 0;
    Clear();
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline nrHash<T>::~nrHash( void )
{
    delete [] m_Hash;
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrHash<T>::Add( const char* key, T item )
{
    assert( m_Hash );
    
    int h = Hash( key );
    while ( m_Hash[ h ] != 0 )
    {
        #if defined ( _DEBUG )
        _RPT2( _CRT_WARN, "nrHash<0x%08X>::Add() : Collision (size %d)!\n", this, m_Size );
        #endif
        
        h += 7;
    }
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline T& nrHash<T>::operator[]( const char* key ) const
{
    int h = Hash( key );

    assert( m_Hash[ h ].Length() > 0 ); // Key not found in the hash table!

    if ( m_Hash[ h ].Length() > 1 )
    {
        // Search.
        for ( int i = 0; i < m_Hash[ h ].Length; i++ )
        {

        }
    }
    else
    {
        return m_Hash[ h ][ 0 ];
    }
}

////////////////////////////////////////////////////////////////////////////

template <class T> inline void nrHash<T>::Clear( void )
{
    delete [] m_Hash;
    m_Size = 0;
}

////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

template <class T> inline int nrHash<T>::Hash( const char* key )
{
    unsigned int h = 0;
    
    for ( int i = 0; i < strlen( key ); i++ )
    {
        h += key[ i ] * 128 * i;
    }
    
    h = h % m_Size;
    
    return h;
}

////////////////////////////////////////////////////////////////////////////

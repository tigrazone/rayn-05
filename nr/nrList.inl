////////////////////////////////////////////////////////////////////////////
//
// nrList.inl
//
// A linked list class.
//
// Nate Robins, April 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrList.h"

#include <assert.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

inline nrLink::nrLink( void* ptr )
{
    m_Ptr = ptr;
    m_Next = 0;
    m_Prev = 0;
}

////////////////////////////////////////////////////////////////////////////

inline nrLink::~nrLink( void )
{
}

////////////////////////////////////////////////////////////////////////////

inline nrList::nrList( void  )
{
    m_Head = 0;
    m_Tail = 0;
    m_Iter = 0;
}

////////////////////////////////////////////////////////////////////////////

inline nrList::~nrList( void )
{
    nrLink* link;
    
    for ( link = m_Head; link != 0; /* */ )
    {
        nrLink* nuke = link;
        
        link = link->m_Next;
        
        delete nuke;
    }
    
    m_Head = 0;
    m_Tail = 0;
    m_Iter = 0;
}

////////////////////////////////////////////////////////////////////////////

inline const nrLink* nrList::Head( void )
{
    m_Iter = m_Head;
    
    return Next();
}

////////////////////////////////////////////////////////////////////////////

inline const nrLink* nrList::Tail( void )
{
    m_Iter = m_Tail;
    
    return Prev();
}

////////////////////////////////////////////////////////////////////////////

inline const nrLink* nrList::Next( void )
{
    if ( m_Iter == 0 )
    {
        return 0;
    }
    else
    {
        nrLink* prev = m_Iter;
        
        m_Iter = m_Iter->m_Next;
        
        return prev;
    }
}

////////////////////////////////////////////////////////////////////////////

inline const nrLink* nrList::Prev( void )
{
    if ( m_Iter == 0 )
    {
        return 0;
    }
    else
    {
        nrLink* next = m_Iter;
        
        m_Iter = m_Iter->m_Prev;
        
        return next;
    }
}

////////////////////////////////////////////////////////////////////////////

inline void nrList::Add( nrLink* link )
{
    assert( link != 0 );
    
    if ( m_Head == 0 )
    {
        m_Head = link;
        link->m_Prev = 0;
    }
    else
    {
        m_Tail->m_Next = link;
        link->m_Prev = m_Tail;
    }
    
    m_Tail = link;
    link->m_Next = 0;
}

////////////////////////////////////////////////////////////////////////////

inline void nrList::Remove( const nrLink* link )
{
    assert( link != 0 );
    
    if ( link != m_Head && link != m_Tail )
    {
        link->m_Next->m_Prev = link->m_Prev;
        link->m_Prev->m_Next = link->m_Next;
    }
    else
    {
        if ( link == m_Head )
        {
            m_Head = link->m_Next;
            
            if ( m_Head )
            {
                m_Head->m_Prev = 0;
            }
        }
        
        if ( link == m_Tail )
        {
            m_Tail = link->m_Prev;
            
            if ( m_Tail )
            {
                m_Tail->m_Next = 0;
            }
        }
    }
    
    delete link;
}

////////////////////////////////////////////////////////////////////////////

inline void* nrList::Ptr( const nrLink* link ) const
{
    if ( link != 0 )
    {
        return link->m_Ptr;
    }
    else
    {
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

inline int nrList::Length( void )
{
    int length = 0;
    
    for ( nrLink* link = m_Head; link != 0; link = link->m_Next )
    {
        length++;
    }
    
    return length;
}

////////////////////////////////////////////////////////////////////////////

inline void nrList::Sort( int ( *compare )( const void* a, const void* b ) )
{
    nrLink* link;
    void** links;
    
    int n = 0;
    for ( link = m_Head; link != 0; link = link->m_Next )
    {
        n++;
    }
    
    if ( n <= 1 )
    {
        return;
    }
    
    links = new void*[ n ];
    
    for ( n = 0, link = m_Head; link != 0; link = link->m_Next, n++ )
    {
        links[ n ] = link->m_Ptr;
    }
    
    qsort( links, n, sizeof( void* ), compare );
    
    for ( n = 0, link = m_Head; link != 0; link = link->m_Next, n++ )
    {
        link->m_Ptr = links[ n ];
    }
    
    delete [] links;
}

////////////////////////////////////////////////////////////////////////////

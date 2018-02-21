////////////////////////////////////////////////////////////////////////////
//
// nrList.h
//
// A linked list class.
//
// Nate Robins, April 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRLIST_H
#define NRLIST_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrLink
{
    friend class nrList;
    
public:
    
    inline nrLink( void* ptr );
    inline ~nrLink( void );
    
private:
    
    void* m_Ptr;
    nrLink* m_Next;
    nrLink* m_Prev;
};

////////////////////////////////////////////////////////////////////////////

class nrList
{
public:
    
    inline nrList( void );
    inline ~nrList( void );
    
    // Operations ///////////////////////////////////////////////////////////
    
    // Add a link to the tail of the list.
    inline void Add( nrLink* link );
    
    // Remove a specific link from the list.
    inline void Remove( const nrLink* link );
    
    // Return the data pointer in the link.
    inline void* Ptr( const nrLink* link ) const;
    
    // Return the number of links in the list.
    inline int Length();
    
    // Sort the linked list.
    inline void Sort( int ( *compare )( const void* a, const void* b ) );
    
    // Iteration ////////////////////////////////////////////////////////////
    
    // Return the head link in the list (or NULL if the list is empty).
    inline const nrLink* Head( void );
    
    // Return the tail link in the list (or NULL if the list is empty).
    inline const nrLink* Tail( void );
    
    // Return the next link in the list (or NULL if at the end of the list).
    inline const nrLink* Next( void );
    
    // Return the previous link in the list (or NULL if at the beginning of the list).
    inline const nrLink* Prev( void );
    
private:
    
    nrLink* m_Head;
    nrLink* m_Tail;
    nrLink* m_Iter;
};

////////////////////////////////////////////////////////////////////////////

#include "nrList.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRLIST_H

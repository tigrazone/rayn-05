////////////////////////////////////////////////////////////////////////////
//
// nrInterval.h
//
// A class for intervals.
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRINTERVAL_H
#define NRINTERVAL_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrInterval
{
public:
    
    inline nrInterval( void );
    inline nrInterval( float minimum, float maximum );
    
    // Return true if the interval includes t (i.e., t is within the 
    // interval), false otherwise.
    inline bool Includes( float t ) const;
    
    // Return true if interval overlaps this interval, false otherwise.
    inline bool Overlaps( const nrInterval& interval ) const;
    
    // Return the interval of overlap between interval and this interval.  
    // If the overlap interval is empty, the interval [0,0] is returned.
    inline nrInterval Overlap( const nrInterval& interval ) const;
    
    // Return true if the interval is empty, false otherwise.
    inline bool Empty( void ) const;
    
public:
    
    float m_Minimum;
    float m_Maximum;
};

////////////////////////////////////////////////////////////////////////////

#include "nrInterval.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRINTERVAL_H

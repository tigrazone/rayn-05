////////////////////////////////////////////////////////////////////////////
//
// nrStopWatch.h
//
// A class for stop watch like timing.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRSTOPWATCH_H
#define NRSTOPWATCH_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrStopWatch
{
public:
    
    nrStopWatch( void );
    ~nrStopWatch( void );
    
    // Start the stopwatch.
    void Start( void );
    
    // Stop the stopwatch.
    void Stop( void );
    
    // Reset the stopwatch.
    void Reset( void );
    
    // Return the elapsed time, in seconds.
    float Elapsed( void );
    
    // Return the elapsed time in the form HHhMMmSS.Ss.
    const char* ElapsedInHMS( void );
    
private:
    
    // Return current system time.
    float Time( void );
    
private:
    
    float m_Start;
    float m_Elapsed;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRSTOPWATCH_H

////////////////////////////////////////////////////////////////////////////
//
// nrTimer.h
//
// A class for timing.
//
// Nate Robins, August 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRTIMER_H
#define NRTIMER_H


////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////

class nrTimer;
extern nrTimer g_Timer;


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrTimer
{

public:
	
	nrTimer( void );
	~nrTimer( void );
	
	// Get the time elapsed since the program started (in seconds).  This time
	// is subject to Pause() and Multiplier()'s.
	float GetTime( void );
	
	// Returns the wall clock time (in seconds).  This time is not subject to Pause()
	// or Multiplier().
	float GetWallClockTime( void );
	
	// Pause/UnPause the timer.
	void Pause( void );
	void UnPause( void );
	
	// Set a timer multiplier for artificially speeding up/slowing down
	// time.  1.0f == normal time.
	void Multiplier( float multiplier = 1.0f );
	
private:
	
	// Get the system time.
	float Time( void );
	
	// Update the timers internal time counter.
	void Update( void );
	
private:
	
	float m_Time;
    float m_LastTime;
	
	bool m_Paused;
	
	float m_Multiplier;
};


////////////////////////////////////////////////////////////////////////////

#endif  // NRTIMER_H

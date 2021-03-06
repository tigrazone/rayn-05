////////////////////////////////////////////////////////////////////////////
//
// nrProgress.inl
//
// A class for a progress meter.
//
// Nate Robins, March 2002.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrProgress.h"

#include "nrLog.h"
#include "nrMath.h"

#include <assert.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

inline nrProgress::nrProgress( void )
{
}

////////////////////////////////////////////////////////////////////////////

inline nrProgress::~nrProgress( void )
{
}

////////////////////////////////////////////////////////////////////////////

inline void nrProgress::Reset( int iterations )
{
    m_Total = iterations;
    m_Current = 0;
    m_Display = 1;
    m_Iteration = 0;
}

////////////////////////////////////////////////////////////////////////////

inline void nrProgress::Update( int delta )
{
    m_Current += delta;

    assert( m_Current <= m_Total );
    
    if ( m_Current >= m_Display || m_Current == m_Total )
    {
        g_Log.Write( "% 4d%%\r", ( int )nrMath::Round( ( float )m_Current * 100.0f / ( float )m_Total ) );
        
        m_Iteration++;
        m_Display = m_Total * m_Iteration / 100;
        
        assert( m_Iteration <= 101 );
    }
}

////////////////////////////////////////////////////////////////////////////

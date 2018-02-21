////////////////////////////////////////////////////////////////////////////
//
// nrInterval.inl
//
// A class for intervals.
//
// Nate Robins, December 2001.
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrInterval.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

inline nrInterval::nrInterval( void )
{
}

////////////////////////////////////////////////////////////////////////////

inline nrInterval::nrInterval( float minumum, float maximum )
{
    m_Minimum = minumum;
    m_Maximum = maximum;
}

////////////////////////////////////////////////////////////////////////////

inline bool nrInterval::Includes( float t ) const
{
    if ( t > m_Minimum && t < m_Maximum )
    {
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

inline bool nrInterval::Overlaps( const nrInterval& interval ) const
{
    if ( m_Minimum > interval.m_Maximum || m_Maximum < interval.m_Minimum )
    {
        return false;
    }
    else
    {
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////

inline nrInterval nrInterval::Overlap( const nrInterval& interval ) const
{
    if ( Overlaps( interval ) )
    {
        nrInterval overlap;
        
        if ( m_Minimum > interval.m_Minimum )
        {
            overlap.m_Minimum = m_Minimum;
        }
        else
        {
            overlap.m_Minimum = interval.m_Minimum;
        }
        
        if ( m_Maximum < interval.m_Maximum )
        {
            overlap.m_Maximum = m_Maximum;
        }
        else
        {
            overlap.m_Maximum = interval.m_Maximum;
        }
        
        return overlap;
    }
    else
    {
        return nrInterval( 0, 0 );
    }
}

////////////////////////////////////////////////////////////////////////////

inline bool nrInterval::Empty( void ) const
{
    if ( m_Minimum == m_Maximum )
    {
        return true;
    }
    else 
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////

//#define TEST_INTERVAL
#ifdef TEST_INTERVAL

void main( void )
{
    nrInterval a = nrInterval( 1, 3 );
    nrInterval b = nrInterval( 2, 5 );
    nrInterval c = nrInterval( 7, 11 );
    nrInterval d = nrInterval( -5, 15 );
    
    g_Log.Write( "a.Within( 2.0f )  = %s (true).\n",  a.Within( 2.0f )  ? "true" : "false" );
    g_Log.Write( "a.Within( 3.1f )  = %s (false).\n", a.Within( 3.1f )  ? "true" : "false" );
    g_Log.Write( "b.Within( 2.0f )  = %s (true).\n",  b.Within( 2.0f )  ? "true" : "false" );
    g_Log.Write( "b.Within( 1.9f )  = %s (false).\n", b.Within( 1.9f )  ? "true" : "false" );
    g_Log.Write( "c.Within( 9.0f )  = %s (true).\n",  c.Within( 9.0f )  ? "true" : "false" );
    g_Log.Write( "c.Within( 5.0f )  = %s (false).\n", c.Within( 5.0f )  ? "true" : "false" );
    g_Log.Write( "d.Within( 0.0f )  = %s (true).\n",  d.Within( 0.0f )  ? "true" : "false" );
    g_Log.Write( "d.Within( -6.0f ) = %s (false).\n", d.Within( -6.0f ) ? "true" : "false" );
    
    g_Log.Write( "a.Overlaps( b ) = %s (true).\n",  a.Overlaps( b ) ? "true" : "false" );
    g_Log.Write( "a.Overlaps( c ) = %s (false).\n", a.Overlaps( c ) ? "true" : "false" );
    g_Log.Write( "a.Overlaps( d ) = %s (true).\n",  a.Overlaps( d ) ? "true" : "false" );
    g_Log.Write( "b.Overlaps( a ) = %s (true).\n",  b.Overlaps( a ) ? "true" : "false" );
    g_Log.Write( "b.Overlaps( c ) = %s (false).\n", b.Overlaps( c ) ? "true" : "false" );
    g_Log.Write( "b.Overlaps( d ) = %s (true).\n",  b.Overlaps( d ) ? "true" : "false" );
    g_Log.Write( "c.Overlaps( a ) = %s (false).\n", c.Overlaps( a ) ? "true" : "false" );
    g_Log.Write( "c.Overlaps( b ) = %s (false).\n", c.Overlaps( b ) ? "true" : "false" );
    g_Log.Write( "c.Overlaps( d ) = %s (true).\n",  c.Overlaps( d ) ? "true" : "false" );
    g_Log.Write( "d.Overlaps( a ) = %s (true).\n",  d.Overlaps( a ) ? "true" : "false" );
    g_Log.Write( "d.Overlaps( b ) = %s (true).\n",  d.Overlaps( b )  ? "true" : "false" );
    g_Log.Write( "d.Overlaps( c ) = %s (true).\n",  d.Overlaps( c ) ? "true" : "false" );
    
    g_Log.Write( "a.Overlap( b ) = [%1.0f, %2.0f] ([2,  3]).\n", a.Overlap( b ).m_Minimum, a.Overlap( b ).m_Maximum );
    g_Log.Write( "a.Overlap( c ) = [%1.0f, %2.0f] ([0,  0]).\n", a.Overlap( c ).m_Minimum, a.Overlap( c ).m_Maximum );
    g_Log.Write( "a.Overlap( d ) = [%1.0f, %2.0f] ([1,  3]).\n", a.Overlap( d ).m_Minimum, a.Overlap( d ).m_Maximum );
    g_Log.Write( "b.Overlap( a ) = [%1.0f, %2.0f] ([2,  3]).\n", b.Overlap( a ).m_Minimum, b.Overlap( a ).m_Maximum );
    g_Log.Write( "b.Overlap( c ) = [%1.0f, %2.0f] ([0,  0]).\n", b.Overlap( c ).m_Minimum, b.Overlap( c ).m_Maximum );
    g_Log.Write( "b.Overlap( d ) = [%1.0f, %2.0f] ([2,  5]).\n", b.Overlap( d ).m_Minimum, b.Overlap( d ).m_Maximum );
    g_Log.Write( "c.Overlap( a ) = [%1.0f, %2.0f] ([0,  0]).\n", c.Overlap( a ).m_Minimum, c.Overlap( a ).m_Maximum );
    g_Log.Write( "c.Overlap( b ) = [%1.0f, %2.0f] ([0,  0]).\n", c.Overlap( b ).m_Minimum, c.Overlap( b ).m_Maximum );
    g_Log.Write( "c.Overlap( d ) = [%1.0f, %2.0f] ([7, 11]).\n", c.Overlap( d ).m_Minimum, c.Overlap( d ).m_Maximum );
    g_Log.Write( "d.Overlap( a ) = [%1.0f, %2.0f] ([1,  3]).\n", d.Overlap( a ).m_Minimum, d.Overlap( a ).m_Maximum );
    g_Log.Write( "d.Overlap( b ) = [%1.0f, %2.0f] ([2,  5]).\n", d.Overlap( b ).m_Minimum, d.Overlap( b ).m_Maximum );
    g_Log.Write( "d.Overlap( c ) = [%1.0f, %2.0f] ([7, 11]).\n", d.Overlap( c ).m_Minimum, d.Overlap( c ).m_Maximum );
    
}
#endif

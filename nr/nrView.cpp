////////////////////////////////////////////////////////////////////////////
//
// nrView.cpp
//
// A class for views.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrView.h"

#include "nrLog.h"
#include "nrParser.h"

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrView::nrView( void )
{
}

////////////////////////////////////////////////////////////////////////////

nrView::nrView( const nrVector3& eye, const nrVector3& lookat, const nrVector3& up )
{
    m_Eye = eye;
    m_Gaze = lookat - eye;
    m_Up = up;
    m_Basis.ConstructFromWV( -m_Gaze, m_Up );
    
    m_Distance = 1.0f;
    m_BottomLeft = nrVector2( -0.5f, -0.5f );
    m_TopRight = -m_BottomLeft;
}

////////////////////////////////////////////////////////////////////////////

nrView* nrView::NewFromString( const char* string )
{
    nrVector3 eye;
    nrVector3 gaze;
    nrVector3 up;
    
    if ( sscanf( string, "%f %f %f %f %f %f %f %f %f", &eye.x, &eye.y, &eye.z, &gaze.x, &gaze.y, &gaze.z, &up.x, &up.y, &up.z ) != 9 )
    {
        return 0;
    }
    
    nrView* view = new nrView( eye, gaze, up );
    if ( view == 0 )
    {
        g_Log.Write( "nrView::NewFromString(): Out of memory.\n" );
    }
    
    return view;
}

////////////////////////////////////////////////////////////////////////////

nrView* nrView::Parse( nrParser& parser )
{
    nrVector3 eye;
    nrVector3 lookat;
    nrVector3 up;
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_BEGIN );
    
    while ( parser.NextToken() == nrParser::TOKEN_KEY )
    {
        const char* key = parser.ReadKey();
        
        if ( parser.KeyMatches( key, "eye" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            eye.x = parser.ReadFloat();
            eye.y = parser.ReadFloat();
            eye.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else if ( parser.KeyMatches( key, "lookat" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            lookat.x = parser.ReadFloat();
            lookat.y = parser.ReadFloat();
            lookat.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else if ( parser.KeyMatches( key, "up" ) )
        {
            parser.ReadToken( nrParser::TOKEN_LIST_BEGIN );
            up.x = parser.ReadFloat();
            up.y = parser.ReadFloat();
            up.z = parser.ReadFloat();
            parser.ReadToken( nrParser::TOKEN_LIST_END );
        }
        else
        {
            parser.ParseError( "unknown key." );
        }
    }
    
    parser.ReadToken( nrParser::TOKEN_AGGREGATE_END );
    
    if ( parser.Error() )
    {
        return 0;
    }
    
    nrView* view = new nrView( eye, lookat, up );
    if ( view )
    {
        return view;
    }
    else
    {
        assert( 0 );    // out of memory.
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////

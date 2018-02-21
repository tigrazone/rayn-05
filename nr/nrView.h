////////////////////////////////////////////////////////////////////////////
//
// nrView.h
//
// A class for views.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRVIEW_H
#define NRVIEW_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrBasis.h"
#include "nrVector2.h"
#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrParser;

////////////////////////////////////////////////////////////////////////////

class nrView
{
public:
    
    nrView( void );
    nrView( const nrVector3& eye, const nrVector3& lookat, const nrVector3& up );
    
    // Return a new view from the description in string.
    // 
    // string has the form:
    // eyex eyey eyez  gazex gazey gazez  upx upy upz
    static nrView* NewFromString( const char* string );
    
    // Return a new view parsed from a file.  The view directive has the
    // following form:
    // 
    // view
    // {
    //   eye [ x y z ]
    //   lookat [ x y z ]
    //   up [ x y z ]
    // }
    // 
    // elements in ()'s are optional.
    static nrView* Parse( nrParser& parser );
    
public:
    
    nrVector3   m_Eye;          // Position of the eye.
    nrVector3   m_Gaze;         // Direction the eye is gazing.
    nrVector3   m_Up;           // Up direction.
    nrBasis     m_Basis;        // Orthonormal basis [eye,gaze,up]
    
    float       m_Distance;     // Distance from the eye to the image plane.
    nrVector2   m_BottomLeft;   // Bottom left of the image plane.
    nrVector2   m_TopRight;     // Top right of the image plane.
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRVIEW_H

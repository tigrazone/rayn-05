////////////////////////////////////////////////////////////////////////////
//
// nrColorRamp.cpp
//
// A class for color ramps.
//
// Nate Robins, August 2001.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrColorRamp.h"

#include "nrImage.h"
#include "nrPixel.h"
#include "nrMath.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

nrColorRamp::nrColorRamp( void )
{
	m_NumEntries = 0;
	m_ColorRamp = 0;
}

///////////////////////////////////////////////////////////////////////////

nrColorRamp::~nrColorRamp( void )
{
	delete [] m_ColorRamp;
}

///////////////////////////////////////////////////////////////////////////

void nrColorRamp::CreateFromImage( const nrImage& image )
{
	delete [] m_ColorRamp;
	
	m_NumEntries = image.Width();
	m_ColorRamp = new nrColor[ m_NumEntries ];
	
	for ( int i = 0; i < m_NumEntries; i++ )
	{
		nrPixel pixel;
		image.GetPixel( i, 0, pixel );
		m_ColorRamp[ i ].r = ( float )pixel.r / 255.0f;
		m_ColorRamp[ i ].g = ( float )pixel.g / 255.0f;
		m_ColorRamp[ i ].b = ( float )pixel.b / 255.0f;
	}
}

///////////////////////////////////////////////////////////////////////////

void nrColorRamp::CreateFromColors( const nrArray< nrColor >& colors )
{
	delete [] m_ColorRamp;
	
	m_NumEntries = colors.Length();
	m_ColorRamp = new nrColor[ m_NumEntries ];
	
	for ( int i = 0; i < m_NumEntries; i++ )
	{
		m_ColorRamp[ i ] = colors[ i ];
	}
}

///////////////////////////////////////////////////////////////////////////

nrColor nrColorRamp::Color( float value ) const
{
	value = nrMath::Clamp( value, 0.0f, 1.0f );
	
	float entry = value * ( m_NumEntries - 1 );
	
	nrColor a = m_ColorRamp[ ( int )nrMath::Floor( entry ) ];
	nrColor b = m_ColorRamp[ ( int )nrMath::Ceil( entry ) ];
	
	float t = entry - nrMath::Floor( entry );
	
	return nrColor::Lerp( a, b, t );
}

////////////////////////////////////////////////////////////////////////////

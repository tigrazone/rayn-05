////////////////////////////////////////////////////////////////////////////
//
// nrNoise.h
//
// A class for noise (perlin noise in N dimensions).
//
// Adapted from code by Ken Perlin:
//  http://mrl.nyu.edu/~perlin/doc/oscar.html#noise
// and Chris Stephenson:
//  http://www.flipcode.com/cgi-bin/msg.cgi?showThread=COTD-NoiseGenerationClass&forum=cotd&id=-1
//
// Nate Robins, February 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRNOISE_H
#define NRNOISE_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrVector2;
class nrVector3;
template <int T> class nrNoiseN;

////////////////////////////////////////////////////////////////////////////

class nrNoise
{
public:
    
    // Compute noise.
    inline static float Noise1( const float v );
    inline static float Noise2( const nrVector2& v );
    inline static float Noise3( const nrVector3& v );
    
    // Compute fractal noise (sums of octaves of noise).
    inline static float Fractal1( const float v, int octaves = 8 );
    inline static float Fractal2( const nrVector2& v, int octaves = 8 );
    inline static float Fractal3( const nrVector3& v, int octaves = 8 );
    
    // Compute turbulence (sums of absolute values of octaves of noise).
    inline static float Turbulence1( const float v, int octaves = 8 );
    inline static float Turbulence2( const nrVector2& v, int octaves = 8 );
    inline static float Turbulence3( const nrVector3& v, int octaves = 8 );
    
    // Compute marble noise (sin of a * v[0] + b * fractal noise).
    //
    // The parameter a controls how many periods of sin are computed.
    // The parameter b controls how distorted the lines are (0.0f = no distortion,
    // 1.0f = distortion 1 period in magnitude).
    //
    // It often looks better if the absolute value of this function is taken.
    inline static float Marble1( const float v, float a = 1.0f, float b = 1.0f, int octaves = 8 );
    inline static float Marble2( const nrVector2& v, float a = 1.0f, float b = 1.0f, int octaves = 8 );
    inline static float Marble3( const nrVector3& v, float a = 1.0f, float b = 1.0f, int octaves = 8 );
    
private:
    
    nrNoise( void );
    ~nrNoise( void );
    
private:
    
    static nrNoiseN<1> m_Noise1;
    static nrNoiseN<2> m_Noise2;
    static nrNoiseN<3> m_Noise3;
};

////////////////////////////////////////////////////////////////////////////

template <int N> class nrNoiseN
{
public:
	inline nrNoiseN( void );
    inline ~nrNoiseN( void );
    
public:
    // Return noise based on input vector.
    inline float Get( const float v[ N ] ) const;
    
private:
    // Recursive noise function which computes noise in N dimensions.
	inline float Get( int p[ N ], float r[ N ], float s[ N ], int cumulative_p, int n ) const;
    
    // Cubic spline interpolation.
	inline float CubicSpline( float t ) const;
    
private:
	enum
	{ 
		NUM  = 256,     // The number of gradients (must be a power of 2).
		MASK = NUM - 1, // The mask corresponding to the number of gradients (for &'ing instead of %'ing).
		PAD  = 2        // A bit of extra space in the arrays to simplify some array accesses.
	};
    
private:
	int m_Pointers[ NUM + NUM + PAD ];          // Array of random pointers into gradients array.
    float m_Gradients[ NUM + NUM + PAD ][ N ];  // Random direction vectors (gradients).
};

////////////////////////////////////////////////////////////////////////////
    
#include "nrNoise.inl"

////////////////////////////////////////////////////////////////////////////

#endif  // NRNOISE_H

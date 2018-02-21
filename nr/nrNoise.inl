////////////////////////////////////////////////////////////////////////////
//
// nrNoise.inl
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


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrNoise.h"

#include "nrMath.h"
#include "nrVector2.h"
#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Public
////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Noise1( const float v )
{
    float w[ 1 ] = { v };
    return m_Noise1.Get( w );
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Noise2( const nrVector2& v )
{
    float w[ 2 ] = { v.x, v.y };
    return m_Noise2.Get( w );
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Noise3( const nrVector3& v )
{
    float w[ 3 ] = { v.x, v.y, v.z };
    return m_Noise3.Get( w );
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Fractal1( const float v, int octaves )
{
    float n = 0.0f;
    for ( int i = 0; i < octaves; i++ )
    {
        float o = ( float )( 1 << i );

        n += Noise1( v * o ) / o;
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Fractal2( const nrVector2& v, int octaves )
{
    float n = 0.0f;
    for ( int i = 0; i < octaves; i++ )
    {
        float o = ( float )( 1 << i );

        n += Noise2( v * o ) / o;
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Fractal3( const nrVector3& v, int octaves )
{
    float n = 0.0f;
    for ( int i = 0; i < octaves; i++ )
    {
        float o = ( float )( 1 << i );

        n += Noise3( v * o ) / o;
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Turbulence1( const float v, int octaves )
{
    float n = 0.0f;
    for ( int i = 0; i < octaves; i++ )
    {
        float o = ( float )( 1 << i );

        n += nrMath::Abs( Noise1( v * o ) / o );
    }
    
    return n;
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Turbulence2( const nrVector2& v, int octaves )
{
    float n = 0.0f;
    for ( int i = 0; i < octaves; i++ )
    {
        float o = ( float )( 1 << i );

        n += nrMath::Abs( Noise2( v * o ) / o );
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Turbulence3( const nrVector3& v, int octaves )
{
    float n = 0.0f;
    for ( int i = 0; i < octaves; i++ )
    {
        float o = ( float )( 1 << i );

        n += nrMath::Abs( Noise3( v * o ) / o );
    }

    return n;
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Marble1( const float v, float a, float b, int octaves )
{
    return nrMath::Sin( 180.0f * ( a * v + b * Fractal1( v, octaves ) ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Marble2( const nrVector2& v, float a, float b, int octaves )
{
    return nrMath::Sin( 180.0f * ( a * v.x + b * Fractal2( v, octaves ) ) );
}

////////////////////////////////////////////////////////////////////////////

inline float nrNoise::Marble3( const nrVector3& v, float a, float b, int octaves )
{
    return nrMath::Sin( 180.0f * ( a * v.x + b * Fractal3( v, octaves ) ) );
}

////////////////////////////////////////////////////////////////////////////

template <int N> inline nrNoiseN<N>::nrNoiseN( void )
{
    int i;
    
    for ( i = 0; i < NUM; i++ )
    {
        int n;
        
        // Compute a random direction (gradient).
        float l = 0.0f;
        for ( n = 0; n < N; n++ )
        {
            float& g = m_Gradients[ i ][ n ];
            
            g = nrMath::Random_f( -1.0f, 1.0f );
            
            l += g * g;
        }
        
        // Normalize the gradient.
        l = 1.0f / nrMath::Sqrt( l );
        for ( n = 0; n < N; n++ )
        {
            m_Gradients[ i ][ n ] *= l;
        }
    }
    
    // Create pointers into the gradient table and randomize them.
    for ( i = 0; i < NUM; i++ )
    {
        m_Pointers[ i ] = i;
    }
    
    for ( i = 0; i < NUM; i++ )
    {
        int j = nrMath::Random_i( 0, NUM - 1 );
        int t = m_Pointers[ i ];
        
        m_Pointers[ i ] = m_Pointers[ j ];
        m_Pointers[ j ] = t;
    }
    
    // Copy the pointers and gradients to the second half of the array.
    for ( i = 0; i < NUM + PAD; i++ )
    {	
        m_Pointers[ NUM + i ] = m_Pointers[ i ];
        
        for ( int n = 0; n < N; n++ )
        {
            m_Gradients[ NUM + i ][ n ] = m_Gradients[ i ][ n ];
        }
    }
}

////////////////////////////////////////////////////////////////////////////

template <int N> inline nrNoiseN<N>::~nrNoiseN( void )
{
}

////////////////////////////////////////////////////////////////////////////

template <int N> inline float nrNoiseN<N>::Get( const float v[ N ] ) const
{
    int offsets[ N ];   // Grid (indices into gradients) offsets.
    float grid[ N ];    // The original vector clamped to a grid point.
    float r[ N ];       // Vector from grid point to the actual point.
    float t[ N ];       // Interpolation values.
    
    for ( int i = 0; i < N; i++ )
    {
        grid[ i ] = nrMath::Floor( v[ i ] );
        offsets[ i ] = ( ( int )grid[ i ] ) & MASK;
        r[ i ] = v[ i ] - grid[ i ];
        t[ i ] = CubicSpline( r[ i ] );
    }
    
    return Get( offsets, r, t, 0, 0 );
}

////////////////////////////////////////////////////////////////////////////
// Private
////////////////////////////////////////////////////////////////////////////

template <int N> inline float nrNoiseN<N>::Get( int p[ N ], float r[ N ], float t[ N ], int m, int n ) const
{
    if ( n == N )
    {
        // Base case: return dot product of gradient and vector.
        const float* g = m_Gradients[ m ];
        
        float v = 0.0f;
        for ( int i = 0; i < N; i++ )
        {
            v += r[ i ] * g[ i ];
        }
        
        return v;
    }
    else
    {
        // Recursive case: return interpolated of values.
        float a = Get( p, r, t, m_Pointers[ m + p[ n ] ], n + 1 );
        r[ n ] -= 1.0f;
        float b = Get( p, r, t, m_Pointers[ m + p[ n ] + 1 ], n + 1 );
        r[ n ] += 1.0f;
        
        return nrMath::Lerp( a, b, t[ n ] );
    }
}

////////////////////////////////////////////////////////////////////////////

template <int N> inline float nrNoiseN<N>::CubicSpline( float t ) const
{
    return t * t * ( 3.0f - 2.0f * t );
}

////////////////////////////////////////////////////////////////////////////

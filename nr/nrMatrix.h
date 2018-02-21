////////////////////////////////////////////////////////////////////////////
//
// nrMatrix.h
//
// A matrix class.
//
// Nate Robins, November 2001.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRMATRIX_H
#define NRMATRIX_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrVector3.h"


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrMatrix
{
public:
    
    // Undefined matrix.
    nrMatrix( void );
    ~nrMatrix( void );
    
    // Matrices /////////////////////////////////////////////////////////////
    
    // Return an identity matrix.
    static nrMatrix Identity( void );
    
    // Return a translation matrix.
    static nrMatrix Translation( float x, float y, float z );
    
    // Return a rotation matrix.
    static nrMatrix Rotation( float angle_in_degrees, float x, float y, float z );
    
    // Return a scale matrix.
    static nrMatrix Scale( float x, float y, float z );
    
    // Return a perspective projection matrix.
    static nrMatrix Perspective( float field_of_view_in_degrees, float aspect, float nearz, float farz );
    
    // Return an orthographic projection matrix.
    static nrMatrix Orthographic( float left, float right, float top, float bottom, float nearz, float farz );
    
    // Return a look-at matrix.
    static nrMatrix LookAt( const nrVector3& eye, const nrVector3& lookat, const nrVector3& up );
    
    // Matrix Operations ////////////////////////////////////////////////////
    
    // Return the transpose of this matrix.
    nrMatrix Transpose( void ) const;
    
    // Return the product of this matrix and m.
    nrMatrix operator*( const nrMatrix& m ) const;
    
    // Poke Operations //////////////////////////////////////////////////////
    
    // Poke the translation of this matrix (over-write current translation).
    void PokeTranslation( float x, float y, float z );
    
    // Vector Operations ////////////////////////////////////////////////////
    
    // Return the vector (with implied w = 1.0f) transformed by this matrix.
    nrVector3 Transform( const nrVector3& vector ) const;
    nrVector3 operator*( const nrVector3& vector ) const;
    
    // Return the "right" vector of this matrix.
    nrVector3 Right( void ) const;
    
    // Return the "up" vector of this matrix.
    nrVector3 Up( void ) const;
    
    // Return the "direction" vector of this matrix.
    nrVector3 Direction( void ) const;
    
private:
    
    // Matrix elements are named as e[row][column].
    
#define STORE_TRANSPOSED
#ifdef STORE_TRANSPOSED
    
    // Store transposed for OpenGL.
    float e00, e10, e20, e30;
    float e01, e11, e21, e31;
    float e02, e12, e22, e32;
    float e03, e13, e23, e33;
    
#else
    
    float e00, e01, e02, e03;
    float e10, e11, e12, e13;
    float e20, e21, e22, e23;
    float e30, e31, e32, e33;
    
#endif
};


////////////////////////////////////////////////////////////////////////////

#endif  // NRMATRIX_H

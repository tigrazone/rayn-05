////////////////////////////////////////////////////////////////////////////
//
// nrModelOBJ.h
//
// Nate Robins, November 2001.
//
// Wavefront OBJ model file format reader.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRMODELOBJ_H
#define NRMODELOBJ_H


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include <stdio.h>


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrVector2;
class nrVector3;
class nrmTriangle;
class nrmGroup;
class nrmMaterial;
class nrModel;

////////////////////////////////////////////////////////////////////////////

class nrModelOBJ
{
public:
    // Read a Wavefront OBJ model file.
    static nrModel* NewFromFile( const char* file_name );
    
private:
    nrModelOBJ();
    ~nrModelOBJ();
    
private:
    // Parse the OBJ model file.
    void Parse( void );
    
private:
    // File reading utilities.
    bool NextLine( void );
    void Loading( void );
    
    // Find utilities.
    nrmGroup* FindGroup( const char* name ) const;
    int FindMaterial( const char* name ) const;
    
private:
    nrModel* m_Model;      // Model that we're populating.
    FILE* m_File;          // File from which model is being populated.
    
    char* m_Line;          // Current line in the file.
    int m_LineNum;         // Current line number in the file.
};

////////////////////////////////////////////////////////////////////////////

#endif    // NRMODELOBJ_H

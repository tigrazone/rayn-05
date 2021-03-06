////////////////////////////////////////////////////////////////////////////
//
// nrAtom.h
//
// A class for atoms.
//
// Nate Robins, January 2002.
//
////////////////////////////////////////////////////////////////////////////

#ifndef NRATOM_H
#define NRATOM_H


////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////

class nrAtom
{
public:
    
    enum nrAtomType
    {
        UNKNOWN,
        INT,
        FLOAT,
        STRING,
    };
    
public:
    
    nrAtom( const char* name = 0 );
    ~nrAtom( void );
    
public:

    char* m_Name;
    nrAtomType m_Type;
};

////////////////////////////////////////////////////////////////////////////

#endif  // NRATOM_H

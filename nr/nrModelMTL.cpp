////////////////////////////////////////////////////////////////////////////
//
// nrModelMTL.cpp
//
// Nate Robins, January 2002.
//
// Wavefront material file format reader.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrModelMTL.h"

#include "nrModel.h"
#include "nrProgress.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////

#define LINE_SIZE        1024


////////////////////////////////////////////////////////////////////////////
// Static
////////////////////////////////////////////////////////////////////////////

static nrProgress g_Progress;


////////////////////////////////////////////////////////////////////////////
// Implementation
////////////////////////////////////////////////////////////////////////////

nrModelMTL::nrModelMTL()
{
    m_Line = new char[ LINE_SIZE ];
    assert( m_Line );
}

////////////////////////////////////////////////////////////////////////////

nrModelMTL::~nrModelMTL()
{
    delete [] m_Line;
}

////////////////////////////////////////////////////////////////////////////

bool nrModelMTL::Read( const char* material_file, nrModel* model )
{
    char* file_name = new char[ strlen( model->m_Path ) + strlen( material_file ) + 1 ];
    strcpy( file_name, model->m_Path );
    strcat( file_name, material_file );
    
    FILE* file = fopen( file_name, "rb" );
    if ( file == NULL )
    {
        fprintf( stderr, "nrModelMTL::Read(): Unable to open material file \"%s\".\n", file_name );
        delete file_name;
        return false;
    }
    
    delete file_name;
    
    rewind( file );
    fseek( file, 0, SEEK_END );
    int size = ftell( file );
    rewind( file );
    
    nrModelMTL l;
    
    l.m_File = file;
    l.m_Model = model;
    l.m_LineNum = 0;
    g_Progress.Reset( size );
    
    l.Parse();
    
    fclose( file );
    
    return true;
}

////////////////////////////////////////////////////////////////////////////

bool nrModelMTL::NextLine( void )
{
    if ( fgets( m_Line, LINE_SIZE, m_File ) == NULL )
    {
        return false;
    }
    else
    {
        m_LineNum++;
        
        g_Progress.Update( strlen( m_Line ) );
        
        return true;
    }
}

////////////////////////////////////////////////////////////////////////////

void nrModelMTL::Parse( void )
{
    nrmMaterial* material = m_Model->m_Materials[ 0 ];
    
    while ( NextLine() )
    {
        char* directive = strtok( m_Line, " \t\r\n" );
        char* line      = strtok( NULL, "\r\n" );
        
        if ( directive == NULL || line == NULL )
        {
            continue;
        }
        
        switch ( directive[ 0 ] )
        {
        case 'n':   {   // newmtl
            material = new nrmMaterial( line );
            m_Model->m_Materials.Add( material );
                    }
            break;
            
        case 'K': {        // K = color - can be one of Kd, Ka, Ks
            switch ( directive[ 1 ] )
            {
            case 'a':        {        // Ka = ambient
                float r, g, b;
                if ( sscanf( line, "%f %f %f\n", &r, &g, &b ) != 3 )
                {
                    fprintf( stderr, "nrModelMTL::Read(): Line %d - Malformed %s directive.\n", m_LineNum, directive );
                }
                else
                {
                    material->m_Ambient = nrColor( r, g, b );
                }
                              }
                break;
                
            case 'd':        {        // Kd = diffuse
                float r, g, b;
                if ( sscanf( line, "%f %f %f\n", &r, &g, &b ) != 3 )
                {
                    fprintf( stderr, "nrModelMTL::Read(): Line %d - Malformed %s directive.\n", m_LineNum, directive );
                }
                else
                {
                    material->m_Diffuse = nrColor( r, g, b );
                }
                             }
                break;
                
            case 's':        {        // Ks = specular
                float r, g, b;
                if ( sscanf( line, "%f %f %f\n", &r, &g, &b ) != 3 )
                {
                    fprintf( stderr, "nrModelMTL::Read(): Line %d - Malformed %s directive.\n", m_LineNum, directive );
                }
                else
                {
                    material->m_Specular = nrColor( r, g, b );
                }
                             }
                break;
                
            default:
                fprintf( stderr, "nrModelMTL::Read(): Line %d - Unknown color directive \"%s\" (ignored).\n", m_LineNum, directive );
                break;
            }
                  }
            break;
            
        case 'N':        // Ns = specular power (0-1000).
            float s;
            if ( sscanf( line, "%f\n", &s ) != 1 )
            {
                fprintf( stderr, "nrModelMTL::Read(): Line %d - Malformed %s directive.\n", m_LineNum, directive );
            }
            else
            {
                s = s / 1000.0f;
                material->m_Shininess = s;
            }
            break;

        case '#':       // # = comment.
        case 'i':       // illum = ?
        case 'd':       // d = ?
            break;
            
        case 'm':       // m = map_K* = texture map.
            fprintf( stderr, "nrModelMTL::Read(): Line %d - Unsupported directive \"%s\" (ignored).\n", m_LineNum, directive );
            break;

        default:
            fprintf( stderr, "nrModelMTL::Read(): Line %d - Unknown directive \"%s\" (ignored).\n", m_LineNum, directive );
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////

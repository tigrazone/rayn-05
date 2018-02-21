////////////////////////////////////////////////////////////////////////////
//
// nrModelOBJ.cpp
//
// Nate Robins, 2001.
//
// Wavefront OBJ model file format reader.
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrModelOBJ.h"
#include "nrModelMTL.h"

#include "nrModel.h"
#include "nrPath.h"
#include "nrProgress.h"
#include "nrVector2.h"
#include "nrVector3.h"
    
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>


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

nrModelOBJ::nrModelOBJ()
{
    m_Line = new char[ LINE_SIZE ];
    assert( m_Line );
}

////////////////////////////////////////////////////////////////////////////

nrModelOBJ::~nrModelOBJ()
{
    delete [] m_Line;
}

////////////////////////////////////////////////////////////////////////////

nrModel* nrModelOBJ::NewFromFile( const char* file_name )
{        
    FILE* file = fopen( file_name, "rb" );
    if ( file == 0 )
    {
        fprintf( stderr, "nrModelOBJ::NewFromFile(): Unable to open file \"%s\".\n", file_name );
        return 0;
    }
    
    nrModel* model = new nrModel;
    if ( model == 0 )
    {
        fprintf( stderr, "nrModelOBJ::NewFromFile(): Out of memory.\n" );
        return 0;
    }
    
    // Add a default group to the model.
    model->m_Groups.Add( new nrmGroup( "default", model ) );
    
    // Add a default material to the model.
    model->m_Materials.Add( new nrmMaterial( "default" ) );
	
	// Get the directory the file is in.
	const char* directory = nrPath::Directory( file_name );
	model->m_Path = new char[ strlen( directory ) + 1 ];
	strcpy( model->m_Path, directory );
    
    rewind( file );
    fseek( file, 0, SEEK_END );
    int size = ftell( file );
    rewind( file );
    
    nrModelOBJ l;
    
    l.m_File = file;
    l.m_Model = model;
    l.m_LineNum = 0;
    g_Progress.Reset( size );
    
    l.Parse();
    
    fclose( file );
    
    return model;
}

////////////////////////////////////////////////////////////////////////////

bool nrModelOBJ::NextLine( void )
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

nrmGroup* nrModelOBJ::FindGroup( const char* name ) const
{
    for ( int i = 0; i < m_Model->NumGroups(); i++ )
    {
        nrmGroup* group = m_Model->m_Groups[ i ];
        
        if ( strcmp( group->m_Name, name ) == 0 )
        {
            return group;
        }
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////

int nrModelOBJ::FindMaterial( const char* name ) const
{
    for ( int i = 0; i < m_Model->NumMaterials(); i++ )
    {
        nrmMaterial* material = m_Model->m_Materials[ i ];
        
        if ( strcmp( material->m_Name, name ) == 0 )
        {
            return i;
        }
    }
    
    // Didn't find one - issue a warning.
    fprintf( stderr, "nrModelOBJ::FindMaterial(): Line %d - Unknown material \"%s\" (using default).\n", m_LineNum, name );
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////

void nrModelOBJ::Parse( void )
{
    nrmGroup* group = FindGroup( "default" );
    int material = FindMaterial( "default" );
    
    int num_triangles = 0;
    while ( NextLine() )
    {
        char* directive = strtok( m_Line, " \t\r\n" );
        char* line      = strtok( NULL, "\r\n" );
        int   c;
        
        if ( directive == NULL || line == NULL )
        {
            continue;
        }
        
        switch ( directive[ 0 ] )
        {
        case 'v': {        // vertex - can be one of v, vn, vt
            switch ( directive[ 1 ] )
            {
            case '\0':        {        // v = vertex coord
                nrVector3 c;
                sscanf( line, "%f %f %f\n", &c.x, &c.y, &c.z );
                m_Model->m_Positions.Add( c );
                              }
                break;
                
            case 'n':        {        // vn = vertex normal
                nrVector3 n;
                sscanf( line, "%f %f %f\n", &n.x, &n.y, &n.z );
                m_Model->m_Normals.Add( n );
                             }
                break;
                
            case 't':        {        // vt = vertex texcoord
                nrVector3 t;
                sscanf( line, "%f %f\n", &t.x, &t.y );
                m_Model->m_TexCoords.Add( t );
                             }
                break;
                
            default:
                fprintf( stderr, "nrModelOBJ::Parse(): Line %d - Unknown vertex directive (ignored).\n", m_LineNum );
                break;
            }
                  }
            break;
            
        case 'f': {        // f = face - can be one of %d, %d//%d, %d/%d, %d/%d/%d
            int v0, v1, v2; v0 = v1 = v2 = 0;
            int c0, c1, c2; c0 = c1 = c2 = 0;
            int t0, t1, t2; t0 = t1 = t2 = 0;
            int n0, n1, n2; n0 = n1 = n2 = 0;
            if ( strstr( line, "//" ) )
            {
                // v//n        (vertex/normal)
                int r = 0;
                r += sscanf( line, "%d//%d%n", &v0, &n0, &c );      line += c;
                r += sscanf( line, "%d//%d%n", &v1, &n1, &c );      line += c;
                if ( r == 4 )
                {
                    while ( sscanf( line, "%d//%d%n", &v2, &n2, &c ) == 2 )
                    {
                        line += c;
                        
                        m_Model->m_Triangles.Add( new nrmTriangle( v0 - 1, c0 - 1, t0 - 1, n0 - 1, v1 - 1, c1 - 1, t1 - 1, n1 - 1, v2 - 1, c2 - 1, t2 - 1, n2 - 1, m_Model ) );
                        group->m_TriangleIndices.Add( num_triangles );
                        num_triangles++;
                        
                        v1 = v2;    n1 = n2;    t1 = t2;
                    }
                }
                else
                {
                    fprintf( stderr, "nrModelOBJ::Parse(): Line %d - Malformed face directive.\n", m_LineNum );
                }
            }
            else if ( sscanf( line, "%d/%d/%d%n", &v0, &t0, &n0, &c ) == 3 )
            {
                line += c;
                
                // v/t/n (vertex/texcoord/normal)
                int r = 3;
                r += sscanf( line, "%d/%d/%d%n", &v1, &t1, &n1, &c );        line += c;
                if ( r == 6 )
                {
                    while ( sscanf( line, "%d/%d/%d%n", &v2, &t2, &n2, &c ) == 3 )
                    {
                        line += c;
                        
                        m_Model->m_Triangles.Add( new nrmTriangle( v0 - 1, c0 - 1, t0 - 1, n0 - 1, v1 - 1, c1 - 1, t1 - 1, n1 - 1, v2 - 1, c2 - 1, t2 - 1, n2 - 1, m_Model ) );
                        group->m_TriangleIndices.Add( num_triangles );
                        num_triangles++;
                        
                        v1 = v2;    n1 = n2;    t1 = t2;
                    }
                }
                else
                {
                    fprintf( stderr, "nrModelOBJ::Parse(): Line %d - Malformed face directive.\n", m_LineNum );
                }
            }
            else if ( sscanf( line, "%d/%d%n", &v0, &t0, &c ) == 2 )
            {
                line += c;
                
                // v/t (vertex/texcoord)
                int r = 2;
                r += sscanf( line, "%d/%d%n", &v1, &t1, &c );        line += c;
                if ( r == 4 )
                {
                    while ( sscanf( line, "%d/%d%n", &v2, &t2, &c ) == 2 )
                    {
                        line += c;
                        
                        m_Model->m_Triangles.Add( new nrmTriangle( v0 - 1, c0 - 1, t0 - 1, n0 - 1, v1 - 1, c1 - 1, t1 - 1, n1 - 1, v2 - 1, c2 - 1, t2 - 1, n2 - 1, m_Model ) );
                        group->m_TriangleIndices.Add( num_triangles );
                        num_triangles++;
                        
                        v1 = v2;    n1 = n2;    t1 = t2;
                    }
                }
                else
                {
                    fprintf( stderr, "nrModelOBJ::Parse(): Line %d - Malformed face directive.\n", m_LineNum );
                }
            }
            else if ( sscanf( line, "%d%n", &v0, &c ) == 1 )
            {
                line += c;
                
                // v (vertex)
                int r = 0;
                r += sscanf( line, "%d%n", &v1, &c );        line += c;
                if ( r == 1 )
                {
                    while ( sscanf( line, "%d%n", &v2, &c ) == 1 )
                    {
                        line += c;
                        
                        m_Model->m_Triangles.Add( new nrmTriangle( v0 - 1, c0 - 1, t0 - 1, n0 - 1, v1 - 1, c1 - 1, t1 - 1, n1 - 1, v2 - 1, c2 - 1, t2 - 1, n2 - 1, m_Model ) );
                        group->m_TriangleIndices.Add( num_triangles );
                        num_triangles++;
                        
                        v1 = v2;    n1 = n2;    t1 = t2;
                    }
                }
                else
                {
                    fprintf( stderr, "nrModelOBJ::Parse(): Line %d - Malformed face directive.\n", m_LineNum );
                }
            }
                  }
            break;
            
        case 'm':   {    // m = mtllib (material library)
            nrModelMTL::Read( line, m_Model );
                    }
            break;
            
        case 'u': // u = usemtl
            material = FindMaterial( line );
            group->m_MaterialIndex = material; 
            break;
            
        case 'g': // g = group
            group = FindGroup( line );
            if ( group == 0 )
            {
                group = new nrmGroup( line, m_Model );
                m_Model->m_Groups.Add( group );
            }
            break;
            
        case 'o':
            m_Model->SetName( line );
            break;
            
        case '#':        // # = comment
        case 's':        // s = smoothing group
            break;
            
        default:
            fprintf( stderr, "nrModelOBJ::Parse(): Line %d - Unknown directive \"%s\" (ignored).\n", m_LineNum, directive );
            break;
        }
    }
    
    // Compress all of the arrays down to their minimum size.
    m_Model->m_Positions.Compress();
    m_Model->m_Colors.Compress();
    m_Model->m_Normals.Compress();
    m_Model->m_TexCoords.Compress();
    
    m_Model->m_Triangles.Compress();
    m_Model->m_FaceNormals.Compress();
    
    m_Model->m_Groups.Compress();
    
    m_Model->m_Materials.Compress();
}

////////////////////////////////////////////////////////////////////////////


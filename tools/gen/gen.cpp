////////////////////////////////////////////////////////////////////////////
//
// Gen.cpp
//
// Scene generator.
//
// Nate Robins, January 2002
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrArray.h"
#include "nrCmdLine.h"
#include "nrLog.h"
#include "nrLight.h"
#include "nrModel.h"
#include "nrProgress.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////

void model( const char* name )
{
    nrModel* m = nrModel::NewFromFile( name );
    if ( m == 0 )
    {
        g_Log.Write( "model(): Can't open model file \"%s\".\n", name );
        return;
    }
    
    g_Log.Write( "Writing scene description.\n" );
    
    nrVector3 mins, maxs;
    m->Dimensions( mins, maxs );
    
    nrVector3 center = ( maxs + mins ) / 2.0f;
    
    printf( "# %s\n", name );
    printf( "\n" );
    printf( "\n" );
    
    printf( "view {\n" );
    printf( "  eye    < %g %g %g >\n", center.x, 0.75f * maxs.y, 2.2f * nrMath::Max3( maxs.x, maxs.y, maxs.z ) );
    printf( "  lookat < %g %g %g >\n", center.x, center.y, center.z );
    printf( "  up     < %g %g %g >\n", 0.0f, 1.0f, 0.0f );
    printf( "}\n" );
    printf( "\n" );
    
    printf( "light \"headlamp\" {\n" );
    printf( "  position < %g %g %g >\n", center.x, 0.75f * maxs.y, 2.2f * nrMath::Max3( maxs.x, maxs.y, maxs.z ) );
    printf( "  color < %g %g %g >\n", 0.9f, 0.9f, 0.9f );
    printf( "}\n" );
    printf( "\n" );
    
    printf( "ambient < %g %g %g >\n", 0.2f, 0.2f, 0.2f );
    printf( "\n" );
    
    printf( "background < %g %g %g >\n", 0.2f, 0.2f, 0.2f );
    printf( "\n" );
    
    printf( "\n" );
    
    for ( int i = 0; i < m->NumMaterials(); i++ )
    {
        const nrmMaterial& material = m->Material( i );
        
        printf( "material \"%s\" {\n", material.GetName() );
        printf( " diffuse < %g %g %g >\n", material.m_Diffuse.r, material.m_Diffuse.g, material.m_Diffuse.b );
        printf( " ambient < %g %g %g >\n", material.m_Ambient.r, material.m_Ambient.g, material.m_Ambient.b );
        //printf( " specular < %g %g %g >\n", material.m_Specular.r, material.m_Specular.g, material.m_Specular.b );
        //printf( " shininess %g\n", material.m_Shininess );
        printf( "}\n" );
    }
    
    printf( "\n" );
    printf( "\n" );
    
    nrProgress progress;
    progress.Reset( m->NumTriangles() );
    
    for ( i = 0; i < m->NumGroups(); i++ )
    {
        const nrmGroup& group = m->Group( i );
        const nrmMaterial& material = group.Material();
        
        for ( int j = 0; j < group.NumTriangles(); j++ )
        {
            const nrmTriangle& t = group.Triangle( j );
            
            const nrVector3& a = t.Position( 0 );
            const nrVector3& b = t.Position( 1 );
            const nrVector3& c = t.Position( 2 );
            
            printf( "triangle {" );
            printf( " a < %g %g %g >", a.x, a.y, a.z );
            printf( " b < %g %g %g >", b.x, b.y, b.z );
            printf( " c < %g %g %g >\n", c.x, c.y, c.z );
            printf( " material \"%s\"\n", material.GetName() );
            printf( "}\n" );
            
            progress.Update();
        }
    }
    
    delete m;
}

////////////////////////////////////////////////////////////////////////////

void sphereflake( int depth )
{
}

////////////////////////////////////////////////////////////////////////////

void random( int n )
{
    float volume = 10.0f / n;
    float radius = nrMath::Pow( 0.75f * volume / 3.14159f, 1.0f / 3.0f );
    
    printf( "# %d random spheres\n", n );
    printf( "\n\n" );
    
    printf( "view {\n" );
    printf( "  eye    < %g %g %g >\n", 0.0f, 0.0f, 15.0f );
    printf( "  lookat < %g %g %g >\n", 0.0f, 0.0f, 0.0f );
    printf( "  up     < %g %g %g >\n", 0.0f, 1.0f, 0.0f );
    printf( "}\n" );
    printf( "\n\n" );
    
    printf( "ambient < %g %g %g >\n", 1.0f, 1.0f, 1.0f );
    printf( "\n\n" );
    
    for ( int i = 0; i < n; i++ )
    {
        float x = nrMath::Random_f( -5.0f, 5.0f );
        float y = nrMath::Random_f( -5.0f, 5.0f );
        float z = nrMath::Random_f( -5.0f, 5.0f );
        
        float r = nrMath::Random_f( 0.0f, 1.0f );
        float g = nrMath::Random_f( 0.0f, 1.0f );
        float b = nrMath::Random_f( 0.0f, 1.0f );
        
        printf( "sphere {" );
        printf( " center < %g %g %g >", x, y, z );
        printf( " radius %g", radius );
        printf( " color < %g %g %g > ", r, g, b );
        printf( "}\n" );
    }
}

////////////////////////////////////////////////////////////////////////////

void colorcube( int n )
{
    float radius = ( 2.0f / n ) / 2.0f;
    
    printf( "# %dx%dx%d\n", n, n, n );
    printf( "\n\n" );
    
    printf( "view {\n" );
    printf( "  eye    < %g %g %g >\n", 0.0f, 0.0f, 3.0f );
    printf( "  lookat < %g %g %g >\n", 0.0f, 0.0f, 0.0f );
    printf( "  up     < %g %g %g >\n", 0.0f, 1.0f, 0.0f );
    printf( "}\n" );
    printf( "\n\n" );
    
    float x = ( -1.0f + radius ) + ( 2 * radius ) * 1;
    float y = ( -1.0f + radius ) + ( 2 * radius ) * ( n - 2 );
    
    printf( "light {\n" );
    printf( "  position < %g %g %g >\n", x, y, 3.0f );
    printf( "  color < %g %g %g >\n", 1.0f, 1.0f, 1.0f );
    printf( "}\n" );
    printf( "\n\n" );
    
    for ( int k = 0; k < n; k++ )
    {
        for ( int j = 0; j < n; j++ )
        {
            for ( int i = 0; i < n; i++ )
            {
                float x = ( -1.0f + radius ) + ( 2 * radius ) * i;
                float y = ( -1.0f + radius ) + ( 2 * radius ) * j;
                float z = ( -1.0f + radius ) + ( 2 * radius ) * k;
                
                float r = ( float )i / ( float )( n - 1 );
                float g = ( float )j / ( float )( n - 1 );
                float b = ( float )k / ( float )( n - 1 );
                
                printf( "sphere {" );
                printf( " center < %g %g %g >", x, y, z );
                printf( " radius %g", radius );
                printf( " color < %g %g %g > ", r, g, b );
                printf( "}\n" );
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////

int main( int argc, const char** argv )
{
    char type[ 256 ];
    char args[ 256 ];
    
    nrCmdLineArg cmdlineargs[] = 
    {
        nrCmdLineArg( "-type", "<type>", 0, "type of scene to generate",     type, sizeof ( type ) ),
        nrCmdLineArg( "-args", "<args>", 0, "arguments for scene generator", args, sizeof ( args ) ),
    };
    
    nrCmdLine c( cmdlineargs, sizeof ( cmdlineargs ) / sizeof ( nrCmdLineArg ) );
    if ( ! c.Parse( argc, argv ) )
    {
        c.Usage( argv[ 0 ] );
        g_Log.Write( "    colorcube   <n>\n" );
        g_Log.Write( "       Generates a colorcube of n by n by n spheres.\n" );
        //g_Log.Write( "    sphereflake <depth>\n" );
        //g_Log.Write( "       Generates a sphereflake of depth specified.\n" );
        g_Log.Write( "    model <scale>\n" );
        g_Log.Write( "       Generates triangle list out of a model.\n" );
        g_Log.Write( "    random <n>\n" );
        g_Log.Write( "       Generates n random spheres.\n" );
        g_Log.Write( "\n" );
        g_Log.Write( "\n" );
        return 1;
    }
    
    nrArray<nrLight*> lights;
    
    // Depending on type, parse arguments, and call generator.
    if ( strcmp( "colorcube", type ) == 0 )
    {
        int n;
        
        if ( sscanf( args, "%d", &n ) != 1 )
        {
            g_Log.Write( "Invalid arguments for \"%s\".\n", type );
            c.Usage( argv[ 0 ] );
            return 1;
        }
        
        colorcube( n );
    }
    //else if ( strcmp( "sphereflake", type ) == 0 )
    //{
    //    int d;
    //    
    //    if ( sscanf( args, "%d", &d ) != 1 )
    //    {
    //        g_Log.Write( "Invalid arguments for \"%s\".\n", type );
    //        c.Usage( argv[ 0 ] );
    //        return 1;
    //    }
    //    
    //    sphereflake( d );
    //}
    else if ( strcmp( "random", type ) == 0 )
    {
        int n;
        
        if ( sscanf( args, "%d", &n ) != 1 )
        {
            g_Log.Write( "Invalid arguments for \"%s\".\n", type );
            c.Usage( argv[ 0 ] );
            return 1;
        }
        
        random( n );
    }
    else if ( strcmp( "model", type ) == 0 )
    {
        char name[ 256 ];
        
        if ( sscanf( args, "%s", name ) != 1 )
        {
            g_Log.Write( "Invalid arguments for \"%s\".\n", type );
            c.Usage( argv[ 0 ] );
            return 1;
        }
        
        model( name );
    }
    else
    {
        g_Log.Write( "Invalid type \"%s\".\n", type );
        c.Usage( argv[ 0 ] );
        return 1;
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////

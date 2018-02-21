////////////////////////////////////////////////////////////////////////////
//
// RayN.cpp
//
// Nate's Ray Tracer
//
// Nate Robins, January 2002
//
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////

#include "nrBasis.h"
#include "nrCmdLine.h"
#include "nrColor.h"
#include "nrHit.h"
#include "nrImage.h"
#include "nrInterval.h"
#include "nrLight.h"
#include "nrLog.h"
#include "nrNoise.h"
#include "nrProgress.h"
#include "nrRay.h"
#include "nrScene.h"
#include "nrStopWatch.h"
#include "nrSurfaceSphere.h"
#include "nrVector2.h"
#include "nrVector3.h"
#include "nrView.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////

struct
{
    int width;
    int height;
    char scene[ 256 ];
    char output[ 256 ];
    bool shadows;
    bool bvh;
    bool rgs;
    bool cull;
    bool sort;
    
} opt;


////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////

inline nrColor light( nrScene& scene, nrRay& ray, nrHit& hit )
{
    const nrSurface& surface = hit.Surface();
    
    const nrColor& Ga = scene.Ambient();
    
    nrVector3 p = ray.Point( hit.t );
    nrVector3 n = surface.Normal( p );
    
    nrColor Ma = surface.Material()->Ambient( p );
    nrColor Md = surface.Material()->Diffuse( p );
    
    const nrArray<nrLight*>& lights = scene.m_Lights;
    
    nrColor color = Ga + Ma;
    
    for ( int i = 0; i < lights.Length(); i++ )
    {
        const nrLight* light = lights[ i ];
        
        nrVector3 l = ( light->m_Position - p );
        
        if ( opt.shadows )
        {
            nrRay shadow_ray = nrRay( p, l );
            nrInterval shadow_interval = nrInterval( 0.0001f, 1.0f );
            nrHit shadow_hit;
            
            if ( ! scene.Hit( shadow_ray, shadow_interval, shadow_hit ) )
            {
                l = l.Unit();
                
                const nrColor& Ld = light->Color();
                
                nrColor diffuse = Md * Ld * ( n.Dot( l ) );
                
                color = color + diffuse.Clamp();
            }
            else
            {
                // In shadow, so don't add contribution from this light.
            }
        }
        else
        {
            l = l.Unit();
            
            const nrColor& Ld = light->Color();
            
            nrColor diffuse = Md * Ld * ( n.Dot( l ) );
            
            color = color + diffuse.Clamp();
        }
    }
    
    return color.Clamp();
}

////////////////////////////////////////////////////////////////////////////

inline void trace( nrScene& scene, nrImage& image )
{
    const nrBasis& onb = scene.m_View->m_Basis;
    const nrVector3& origin = scene.m_View->m_Eye;
    const nrVector2& a = scene.m_View->m_BottomLeft;
    const nrVector2& b = scene.m_View->m_TopRight;
    
    nrProgress progress;
    progress.Reset( image.Width() * image.Height() );

    for ( int j = 0; j < image.Height(); j++ )
    {
        for ( int i = 0; i < image.Width(); i++ )
        {
            float dx = ( a.x + ( b.x - a.x ) * ( float )i / ( float )( image.Width()  - 1 ) );
            float dy = ( a.y + ( b.y - a.y ) * ( float )j / ( float )( image.Height() - 1 ) );
            float dz = -scene.m_View->m_Distance;
            nrVector3 direction = onb.u * dx + onb.v * dy + onb.w * dz;
            
            nrRay ray = nrRay( origin, direction );
            nrInterval interval = nrInterval( 0.0f, 2e30f );
            nrHit hit;
            
            nrColor color;
            
            if ( scene.Hit( ray, interval, hit ) )
            {
                color = light( scene, ray, hit );
            }
            else
            {
                color = scene.Background();
            }
            
            unsigned char r = ( unsigned char )( color.r * 255 );
            unsigned char g = ( unsigned char )( color.g * 255 );
            unsigned char b = ( unsigned char )( color.b * 255 );
            image.SetPixel( i, j, nrPixel( r, g, b ) );
            
            progress.Update();
        }
    }
}

////////////////////////////////////////////////////////////////////////////

int main( int argc, const char** argv )
{
    // Enumerate the command line arguments.
    nrCmdLineArg args[] = 
    {
        nrCmdLineArg( 0,          "<scene_file>",             0, "file containing scene description",  opt.scene,  sizeof ( opt.scene ) ),
        nrCmdLineArg( "-o",       "<output_file>", "output.tga", "output image filename",              opt.output, sizeof ( opt.output ) ),
        nrCmdLineArg( "-w",       "<width>",              "256", "width of output image",              opt.width ),
        nrCmdLineArg( "-h",       "<height>",             "256", "height of output image",             opt.height ),
        nrCmdLineArg( "-shadows", "<true/false>",        "true", "generate shadow rays",               opt.shadows ),
        nrCmdLineArg( "-rgs",     "<true/false>",        "true", "generate regular grid subdivision",  opt.rgs ),
        nrCmdLineArg( "-bvh",     "<true/false>",       "false", "generate bounding volume hierarchy", opt.bvh ),
        nrCmdLineArg( "-sort",    "<true/false>",       "false", "sort (not split) surfaces (bvh)",    opt.sort ),
        nrCmdLineArg( "-cull",    "<true/false>",       "false", "cull backfacing triangles",          opt.cull ),
    };
    
    // Parse the command line.
    nrCmdLine cmdline( args, sizeof ( args ) / sizeof ( nrCmdLineArg ) );
    if ( ! cmdline.Parse( argc, argv ) )
    {
        cmdline.Usage( argv[ 0 ] );
        return 1;
    }
    if ( opt.rgs && opt.bvh )
    {
        g_Log.Write( "rayn: both -rgs and -bvh specified, using -bvh.\n" );
        opt.rgs = false;
    }
    
    // Make sure the output file can be opened for writing, before any work 
    // is done.
    FILE* f = fopen( opt.output, "wb" );
    if ( f == 0 )
    {
        g_Log.Write( "Unable to open output file for writing \"%s\".\n", opt.output );
        return 1;
    }
    fclose( f );
    
    // Read the scene file.
    nrScene scene;
    nrStopWatch stopwatch;
    {
        g_Log.Write( "Reading scene file \"%s\".\n", opt.scene );
        stopwatch.Reset();
        stopwatch.Start();

	    scene.CullBackfaces( opt.cull );
        if ( ! scene.Parse( opt.scene ) )
        {
            return 1;
        }

        stopwatch.Stop();
        g_Log.Write( "%s (%g seconds).\n", stopwatch.ElapsedInHMS(), stopwatch.Elapsed() );
    }
    
    // Create a bounding volume hierarchy.
    if ( opt.bvh )
    {
        g_Log.Write( "Building bounding volume hierarchy.\n" );
        stopwatch.Reset();
        stopwatch.Start();
        
        scene.CreateBVH( opt.sort );
        
        stopwatch.Stop();
        g_Log.Write( "%s (%g seconds).\n", stopwatch.ElapsedInHMS(), stopwatch.Elapsed() );
    }
    else if ( opt.rgs )
    {
        g_Log.Write( "Building regular grid subdivision.\n" );
        stopwatch.Reset();
        stopwatch.Start();
        
        scene.CreateRGS();
        
        stopwatch.Stop();
        g_Log.Write( "%s (%g seconds).\n", stopwatch.ElapsedInHMS(), stopwatch.Elapsed() );
    }
    
    // Create a blank image to start with.
    nrImage image;
    image.CreateBlank( opt.width, opt.height );
    
    // Ray trace, dude.
    g_Log.Write( "Raytracing scene.\n" );
    stopwatch.Reset();
    stopwatch.Start();
    
    #if 0
    {
        for ( int j = 0; j < image.Height(); j++ )
        {
            for ( int i = 0; i < image.Width(); i++ )
            {
                nrColor c;
                
                const float m = 4.0f;
                //float n = nrNoise::Marble2( nrVector2( m * i / image.Width(), m * j / image.Height() ), 1.0f );
                //float n = nrNoise::Turbulence2( nrVector2( m * i / image.Width(), m * j / image.Height() ) );
                float n = nrNoise::Fractal2( nrVector2( m * i / image.Width(), m * j / image.Height() ) );
                
                //n = Clamp( n * 1.333f, -1.0f, 1.0f );
                //n = nrMath::Abs( n );
                n = ( n + 1 ) / 2;
                
                c.r = n;
                c.g = n;
                c.b = n;
                
                image.SetPixel( i, j, c );
            }
        }
    }
    #else
    {
        trace( scene, image );
    }
    #endif
    
    stopwatch.Stop();
    g_Log.Write( "%s (%g seconds).\n", stopwatch.ElapsedInHMS(), stopwatch.Elapsed() );
    
    // Output the image.
    g_Log.Write( "Writing image to \"%s\".\n", opt.output );
    
    if ( strstr( opt.output, ".ppm" ) )
    {
        image.FlipVertical();
    }
    image.WriteToFile( opt.output );
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////

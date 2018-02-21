
#include "nrColor.h"
#include "nrCmdLine.h"
#include "nrLog.h"

/*
    See: http://astronomy.swin.edu.au/~pbourke/terrain/planets/

   Return a colour from one of a number of colour ramps
   type == 1  blue -> cyan -> green -> magenta -> red
           2  blue -> red
           3  grey scale
           4  red -> yellow -> green -> cyan -> blue -> magenta -> red
           5  green -> yellow
           6  green -> magenta
           7  blue -> green -> red -> green -> blue
           8  white -> black -> white
           9  red -> blue -> cyan -> magenta
          10  blue -> cyan -> green -> yellow -> red -> white
          11  dark brown -> lighter brown (Mars colours, 2 colour transition)
          12  3 colour transition mars colours
          13  landscape colours, green -> brown -> yellow
          14  yellow -> red
          15  blue -> cyan -> green -> yellow -> brown -> white
   v should lie between vmin and vmax otherwise it is clipped
   The colour components range from 0 to 1
*/
nrColor GetColour( float v, float vmin, float vmax, int type )
{
   float dv,vmid;
   nrColor c = nrColor( 1, 1, 1 );
   nrColor c1,c2,c3;
   float ratio;

   if (v < vmin)
      v = vmin;
   if (v > vmax)
      v = vmax;
   dv = vmax - vmin;

   switch (type) {
   case 1:
      if (v < (vmin + 0.25f * dv)) {
         c.r = 0;
         c.g = 4 * (v - vmin) / dv;
         c.b = 1;
      } else if (v < (vmin + 0.5f * dv)) {
         c.r = 0;
         c.g = 1;
         c.b = 1 + 4 * (vmin + 0.25f * dv - v) / dv;
      } else if (v < (vmin + 0.75f * dv)) {
         c.r = 4 * (v - vmin - 0.5f * dv) / dv;
         c.g = 1;
         c.b = 0;
      } else {
         c.r = 1;
         c.g = 1 + 4 * (vmin + 0.75f * dv - v) / dv;
         c.b = 0;
      }
      break;
   case 2:
      c.r = (v - vmin) / dv;
      c.g = 0;
      c.b = (vmax - v) / dv;
      break;
   case 3:
      c.r = (v - vmin) / dv;
      c.b = c.r;
      c.g = c.r;
      break;
   case 4:
      if (v < (vmin + dv / 6.0f)) {
         c.r = 1;
         c.g = 6 * (v - vmin) / dv;
         c.b = 0;
      } else if (v < (vmin + 2.0f * dv / 6.0f)) {
         c.r = 1 + 6 * (vmin + dv / 6.0f - v) / dv;
         c.g = 1;
         c.b = 0;
      } else if (v < (vmin + 3.0f * dv / 6.0f)) {
         c.r = 0;
         c.g = 1;
         c.b = 6 * (v - vmin - 2.0f * dv / 6.0f) / dv;
      } else if (v < (vmin + 4.0f * dv / 6.0f)) {
         c.r = 0;
         c.g = 1 + 6 * (vmin + 3.0f * dv / 6.0f - v) / dv;
         c.b = 1;
      } else if (v < (vmin + 5.0f * dv / 6.0f)) {
         c.r = 6 * (v - vmin - 4.0f * dv / 6.0f) / dv;
         c.g = 0;
         c.b = 1;
      } else {
         c.r = 1;
         c.g = 0;
         c.b = 1 + 6 * (vmin + 5.0f * dv / 6.0f - v) / dv;
      }
      break;
   case 5:
      c.r = (v - vmin) / (vmax - vmin);
      c.g = 1;
      c.b = 0;
      break;
   case 6:
      c.r = (v - vmin) / (vmax - vmin);
      c.g = (vmax - v) / (vmax - vmin);
      c.b = c.r;
      break;
   case 7:
      if (v < (vmin + 0.25f * dv)) {
         c.r = 0;
         c.g = 4 * (v - vmin) / dv;
         c.b = 1 - c.g;
      } else if (v < (vmin + 0.5f * dv)) {
         c.r = 4 * (v - vmin - 0.25f * dv) / dv;
         c.g = 1 - c.r;
         c.b = 0;
      } else if (v < (vmin + 0.75f * dv)) {
         c.g = 4 * (v - vmin - 0.5f * dv) / dv;
         c.r = 1 - c.g;
         c.b = 0;
      } else {
         c.r = 0;
         c.b = 4 * (v - vmin - 0.75f * dv) / dv;
         c.g = 1 - c.b;
      }
      break;
   case 8:
      if (v < (vmin + 0.5f * dv)) {
         c.r = 2 * (v - vmin) / dv;
         c.g = c.r;
         c.b = c.r;
      } else {
         c.r = 1 - 2 * (v - vmin - 0.5f * dv) / dv;
         c.g = c.r;
         c.b = c.r;
      }
      break;
   case 9:
      if (v < (vmin + dv / 3)) {
         c.b = 3 * (v - vmin) / dv;
         c.g = 0;
         c.r = 1 - c.b;
      } else if (v < (vmin + 2 * dv / 3)) {
         c.r = 0;
         c.g = 3 * (v - vmin - dv / 3) / dv;
         c.b = 1;
      } else {
         c.r = 3 * (v - vmin - 2 * dv / 3) / dv;
         c.g = 1 - c.r;
         c.b = 1;
      }
      break;
   case 10:
      if (v < (vmin + 0.2f * dv)) {
         c.r = 0;
         c.g = 5 * (v - vmin) / dv;
         c.b = 1;
      } else if (v < (vmin + 0.4f * dv)) {
         c.r = 0;
         c.g = 1;
         c.b = 1 + 5 * (vmin + 0.2f * dv - v) / dv;
      } else if (v < (vmin + 0.6f * dv)) {
         c.r = 5 * (v - vmin - 0.4f * dv) / dv;
         c.g = 1;
         c.b = 0;
      } else if (v < (vmin + 0.8f * dv)) {
         c.r = 1;
         c.g = 1 - 5 * (v - vmin - 0.6f * dv) / dv;
         c.b = 0;
      } else {
         c.r = 1;
         c.g = 5 * (v - vmin - 0.8f * dv) / dv;
         c.b = 5 * (v - vmin - 0.8f * dv) / dv;
      }
      break;
   case 11:
      c1.r = 200 / 255.0f; c1.g =  60 / 255.0f; c1.b =   0 / 255.0f;
      c2.r = 250 / 255.0f; c2.g = 160 / 255.0f; c2.b = 110 / 255.0f;
      c.r = (c2.r - c1.r) * (v - vmin) / dv + c1.r;
      c.g = (c2.g - c1.g) * (v - vmin) / dv + c1.g;
      c.b = (c2.b - c1.b) * (v - vmin) / dv + c1.b;
      break;
   case 12:
      c1.r =  55 / 255.0f; c1.g =  55 / 255.0f; c1.b =  45 / 255.0f;
      /* c2.r = 200 / 255.0; c2.g =  60 / 255.0; c2.b =   0 / 255.0; */
      c2.r = 235 / 255.0f; c2.g =  90 / 255.0f; c2.b =  30 / 255.0f;
      c3.r = 250 / 255.0f; c3.g = 160 / 255.0f; c3.b = 110 / 255.0f;
      ratio = 0.4f;
      vmid = vmin + ratio * dv;
      if (v < vmid) {
         c.r = (c2.r - c1.r) * (v - vmin) / (ratio*dv) + c1.r;
         c.g = (c2.g - c1.g) * (v - vmin) / (ratio*dv) + c1.g;
         c.b = (c2.b - c1.b) * (v - vmin) / (ratio*dv) + c1.b;
      } else {
         c.r = (c3.r - c2.r) * (v - vmid) / ((1-ratio)*dv) + c2.r;
         c.g = (c3.g - c2.g) * (v - vmid) / ((1-ratio)*dv) + c2.g;
         c.b = (c3.b - c2.b) * (v - vmid) / ((1-ratio)*dv) + c2.b;
      }
      break;
   case 13:
      c1.r =   0 / 255.0f; c1.g = 255 / 255.0f; c1.b =   0 / 255.0f;
      c2.r = 255 / 255.0f; c2.g = 150 / 255.0f; c2.b =   0 / 255.0f;
      c3.r = 255 / 255.0f; c3.g = 250 / 255.0f; c3.b = 240 / 255.0f;
      ratio = 0.3f;
      vmid = vmin + ratio * dv;
      if (v < vmid) {
         c.r = (c2.r - c1.r) * (v - vmin) / (ratio*dv) + c1.r;
         c.g = (c2.g - c1.g) * (v - vmin) / (ratio*dv) + c1.g;
         c.b = (c2.b - c1.b) * (v - vmin) / (ratio*dv) + c1.b;
      } else {
         c.r = (c3.r - c2.r) * (v - vmid) / ((1-ratio)*dv) + c2.r;
         c.g = (c3.g - c2.g) * (v - vmid) / ((1-ratio)*dv) + c2.g;
         c.b = (c3.b - c2.b) * (v - vmid) / ((1-ratio)*dv) + c2.b;
      }
      break;
   case 14:
      c.r = 1;
      c.g = (v - vmin) / dv;
      c.b = 0;
      break;
   case 15:
      if (v < (vmin + 0.25f * dv)) {
         c.r = 0;
         c.g = 4 * (v - vmin) / dv;
         c.b = 1;
      } else if (v < (vmin + 0.5f * dv)) {
         c.r = 0;
         c.g = 1;
         c.b = 1 - 4 * (v - vmin - 0.25f * dv) / dv;
      } else if (v < (vmin + 0.75f * dv)) {
         c.r = 4 * (v - vmin - 0.5f * dv) / dv;
         c.g = 1;
         c.b = 0;
      } else {
         c.r = 1;
         c.g = 1;
         c.b = 4 * (v - vmin - 0.75f * dv) / dv;
      }
      break;
   }
   return(c);
}

#include <stdio.h>

int main( int argc, const char** argv )
{
    int type;
    int size;
    
    nrCmdLineArg args[] = 
    {
        nrCmdLineArg( "-type", "<type>", "15", "type of colormap", type ),
        nrCmdLineArg( "-size", "<args>",  "8", "size of colormap", size ),
    };
    
    nrCmdLine c( args, sizeof ( args ) / sizeof ( nrCmdLineArg ) );
    if ( ! c.Parse( argc, argv ) )
    {
        c.Usage( argv[ 0 ] );
        g_Log.Write( " type == 1  blue -> cyan -> green -> magenta -> red\n" );
        g_Log.Write( "         2  blue -> red\n" );
        g_Log.Write( "         3  grey scale\n" );
        g_Log.Write( "         4  red -> yellow -> green -> cyan -> blue -> magenta -> red\n" );
        g_Log.Write( "         5  green -> yellow\n" );
        g_Log.Write( "         6  green -> magenta\n" );
        g_Log.Write( "         7  blue -> green -> red -> green -> blue\n" );
        g_Log.Write( "         8  white -> black -> white\n" );
        g_Log.Write( "         9  red -> blue -> cyan -> magenta\n" );
        g_Log.Write( "        10  blue -> cyan -> green -> yellow -> red -> white\n" );
        g_Log.Write( "        11  dark brown -> lighter brown (Mars colours, 2 colour transition)\n" );
        g_Log.Write( "        12  3 colour transition mars colours\n" );
        g_Log.Write( "        13  landscape colours, green -> brown -> yellow\n" );
        g_Log.Write( "        14  yellow -> red\n" );
        g_Log.Write( "        15  blue -> cyan -> green -> yellow -> brown -> white\n" );
        g_Log.Write( "\n\n" );
        return 1;
    }
    
	printf( "     ramp { # \"type=%d\"\n", type );
    
	for ( int i = 0; i < size; i++ )
	{
		float t = ( float )i / ( float )size;
		nrColor c = GetColour( t, 0, 1, type );
		
		printf( "       < %f %f %f >\n", c.r, c.g, c.b );
	}
    
	printf ( "     }\n" );
    
    return 0;
}

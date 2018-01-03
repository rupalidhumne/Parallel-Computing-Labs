#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define FALSE 0
#define TRUE  1


#define devPath "C:\\students\\2017\\2017rdhumne\\Documents\\Parallel\\Ray\\Raytracing.txt" /// for program development


FILE *devFile, *ppmFile;

#define printDevFile 0  /// print development file, use 0 to speed up pgm execution by skipping unnecessary printing



#define xRes 640
#define yRes 480

#define xCoordAdjust -.20  
#define yCoordAdjust  .00  
#define nColors 3

#define R 0
#define G 1
#define B 2

#define BLACK 0x00  /// black pixel RGB value
#define WHITE 0xFF  /// white pixel RGB value



#define showLineWidth 0



#define yBegPPMDisplay 0
#define yEndPPMDisplay yRes

#define nSpheres 3  /// setting to 3 eliminates large tan sphere

int* pixelPtr; /// xRes x yRes array of pixels



#define pixel(i,j,k) *( pixelPtr + i + j*xRes + k*xRes*yRes ) /// pixel[i][j][k]



typedef struct
{
    int valid; /// indicates if values are valid

    double min, max; /// min, max reach factors

} ReachFactors;

typedef struct
{
    double x, y, z;

    int sphereIndex; /// sphere having min reach factor

    double  minReachFactor; /// when ray[x][y] is multiplied by its min reach factor, the resulting vector will reach from the eye to a sphere
                            
} XYZ; /// 3D vector

XYZ ray[xRes][yRes]; /// normalized directional vector for each pixel

#define horizon .50  /// normalized y-value of horizon

XYZ eye = { .50, horizon, -1.00 };

XYZ lightSrc = { 0.00 , 1.25 , -.50 };

typedef struct
{
    unsigned char r, g, b;

} RGB;

RGB topBackgroundColor; /// background color above horizon
RGB botBackgroundColor; /// background color below horizon

typedef struct
{
    XYZ center;
    RGB color;

    double radius;

} Sphere;

Sphere sphere[nSpheres];

XYZ difference( XYZ u , XYZ v )
{
    /// returns u - v

    XYZ diff;

    diff.x = u.x - v.x;
    diff.y = u.y - v.y;
    diff.z = u.z - v.z;

    return diff;
}

double dotProduct( XYZ u, XYZ v )
{
    /// returns u * v

    return u.x * v.x + u.y * v.y + u.z * v.z;
}

double sumOfSquares( XYZ xyz )
{
    

    return pow( xyz.x, 2 ) + pow( xyz.y, 2 ) + pow( xyz.z, 2 );
}

double magnitude( XYZ xyz )
{
    

    return sqrt( sumOfSquares(xyz) );
}

void printResolution()
{
    fprintf( devFile, "\n%8sY res%4d, X res%4d\n", "", yRes, xRes );
}

void printEye()
{
    fprintf( devFile, "\n%21s\n\n", "Eye" );

    fprintf( devFile, " %4s%4s%6s%6s%6s\n\n", "", "", " x ", " y ", " z " );
    fprintf( devFile, " %4s%4s%6.2f%6.2f%6.2f\n\n", "", "", eye.x, eye.y, eye.z );
}

void printRays()
{
    
    printEye();

    fprintf( devFile, "%8s%13s\n\n", "Pixel", "Ray" );
    fprintf( devFile, "%4s%4s %6s%6s%6s%6s\n\n", "y", "x", " x ", " y ", " z ", "mag" );

    int x, y;

    for( y=0; y<yRes; y++ )
    {
        for( x=0; x<xRes; x++ )

            fprintf( devFile, "%4d%4d %6.2f%6.2f%6.2f%6.2f\n", y, x, ray[x][y].x, ray[x][y].y, ray[x][y].z, magnitude( ray[x][y] ) );

        if( y < yRes-1 ) fprintf( devFile, "\n" );
    }
}

void printMinReachFactors()
{
    

    printEye();

    fprintf( devFile, "%8s%18s\n\n", "Pixel", "Min reach factor" );
    fprintf( devFile, "%4s%4s%8s%10s\n\n", "y", "x", "Sphere", "Factor" );

    int x, y;

    for( y=0; y<yRes; y++ )
    {
        for( x=0; x<xRes; x++ )

            if( ray[x][y].sphereIndex >= 0 )

                fprintf( devFile, "%4d%4d %7d%10.2f\n", y, x, ray[x][y].sphereIndex, ray[x][y].minReachFactor );

            else

                fprintf( devFile, "%4d%4d %18s\n", y, x, "No valid factors" );

        if( y < yRes-1 ) fprintf( devFile, "\n" );
    }
}

void printPixels()
{
    

    fprintf( devFile, "\n%8s%15s\n\n", "Pixel", "Colors" );

    fprintf( devFile, "%4s%4s %6s%6s%6s\n\n", "y", "x", "R ", " G ", " B " );

    int x, y;

    for( y=0; y<yRes; y++ )
    {
        for( x=0; x<xRes; x++ )

            fprintf( devFile, "%4d%4d %6d%6d%6d\n", y, x, pixel(x,y,R), pixel(x,y,G), pixel(x,y,B) );

        if( y < yRes-1 ) fprintf( devFile, "\n" );
    }
}

void printPixelsInPPMFormat()
{
    

    fprintf( ppmFile, "P3\n" );
    fprintf( ppmFile, "%d %d\n", xRes+showLineWidth, yEndPPMDisplay-yBegPPMDisplay );
    fprintf( ppmFile, "255\n" );

    int x, y, i;

    for( y=yBegPPMDisplay; y<yEndPPMDisplay; y++ )
    {
        if( showLineWidth > 0 )
        {
            int firstPixelComponent = WHITE * (y%2); /// WHITE == 0xFF == white, 0 == black

            for( i=0; i<showLineWidth; i++ )

                fprintf( ppmFile, "%4d%4d%4d\n", firstPixelComponent, firstPixelComponent, firstPixelComponent );

           
        }

        for( x=0; x<xRes; x++ )

            fprintf( ppmFile, "%4d%4d%4d\n", pixel(x,y,R), pixel(x,y,G), pixel(x,y,B) );

           
    }
}

double normalizeXYCoord( int XYCoord )
{
    return XYCoord / fmin(xRes,yRes);
}

void calcRays()
{
   

    int x, y;

    for( x=0; x<xRes; x++ )

        for( y=0; y<yRes; y++ )
        {
            

            ray[x][y].x = normalizeXYCoord(x) + xCoordAdjust;
            ray[x][y].y = normalizeXYCoord(y) + yCoordAdjust;

            ray[x][y].z = 0; /// z is zero since image is initially 2D

           

            ray[x][y].x -= eye.x;
            ray[x][y].y -= eye.y;
            ray[x][y].z -= eye.z;

           

            double rayMag = magnitude( ray[x][y] );

            ray[x][y].x /= rayMag;
            ray[x][y].y /= rayMag;
            ray[x][y].z /= rayMag;
        }
}

ReachFactors calcReachFactors( XYZ ray, XYZ center, double radius ) /// center and radius of sphere
{
    

    XYZ diff = difference( eye, center ); /// calc eye-center

    

    double a, b, c;

    a = 1; 

    b = 2 * dotProduct( ray, diff );

    c = sumOfSquares(diff) - pow( radius, 2 );

    

    double determinant = pow(b,2) - 4*c; /// b^2 - 4*a*c, where a==1

    ReachFactors reach;

    if( determinant < 0 ) /// ray doesn't hit sphere
    {
        reach.valid = FALSE;
    }
    else
    {
        reach.valid = TRUE;

        reach.max = ( -b + sqrt(determinant) ) / 2; /// denominator is 2*a, where a==1

        reach.min = ( -b - sqrt(determinant) ) / 2;
    }

    return reach;
}

void calcMinReachFactors()
{
   

    int x, y, i;

    for( x=0; x<xRes; x++ )

        for( y=0; y<yRes; y++ )
        {
            int sphereIndex = -1; /// init to invalid value

            double minReachFactor;

            

            for( i=0; i<nSpheres; i++ )
            {
                ReachFactors reach = calcReachFactors( ray[x][y], sphere[i].center, sphere[i].radius );

                if( !reach.valid ) continue; /// if invalid reach factors, continue to next sphere

                /// set min to reach.min or reach.max, whichever has smallest absolute value

                double min = ( abs(reach.min) <= abs(reach.max) ) ? reach.min : reach.max;

                if( sphereIndex < 0 || abs(min) < abs(minReachFactor) )
                {
                    sphereIndex = i;
                    minReachFactor = min;
                }
            }

            ray[x][y].sphereIndex = sphereIndex;

            ray[x][y].minReachFactor = minReachFactor;
        }
}

void colorPixels()
{
    int x, y;

    for( x=0; x<xRes; x++ )

        for( y=0; y<yRes; y++ )
        {
            if( ray[x][y].sphereIndex < 0 )
            {
                /// invalid sphere index, set color to background color

                if( normalizeXYCoord(y) < horizon )
                {
                    /// y coordinate is above horizon

                    pixel(x,y,R) = topBackgroundColor.r;
                    pixel(x,y,G) = topBackgroundColor.g;
                    pixel(x,y,B) = topBackgroundColor.b;
                }
                else
                {
                    /// y coordinate is below horizon

                    pixel(x,y,R) = botBackgroundColor.r;
                    pixel(x,y,G) = botBackgroundColor.g;
                    pixel(x,y,B) = botBackgroundColor.b;
                }
            }
            else
            {
                

                pixel(x,y,R) = sphere[ ray[x][y].sphereIndex ].color.r;
                pixel(x,y,G) = sphere[ ray[x][y].sphereIndex ].color.g;
                pixel(x,y,B) = sphere[ ray[x][y].sphereIndex ].color.b;
            }
        }
}

void init()
{
    /// set background color above horizon to black

    topBackgroundColor.r = topBackgroundColor.g = topBackgroundColor.b = BLACK;

    /// set background color below horizon to tan (peru)

    botBackgroundColor.r = 205; /// 0xCD
    botBackgroundColor.g = 133; /// 0x85
    botBackgroundColor.b =  63; /// 0x3F

    /// blue ball

    sphere[0].center.x = .50;
    sphere[0].center.y = .50;
    sphere[0].center.z = .50;

    sphere[0].radius = 0.25;

    sphere[0].color.r =   0;
    sphere[0].color.g =   0;
    sphere[0].color.b = 255; /// 0xFF

    /// green ball

    sphere[1].center.x = 1.00;
    sphere[1].center.y =  .50;
    sphere[1].center.z = 1.00;

    sphere[1].radius = 0.25;

    sphere[1].color.r =   0;
    sphere[1].color.g = 255; /// 0xFF
    sphere[1].color.b =   0;

    /// red ball

    sphere[2].center.x = 0.00;
    sphere[2].center.y = 0.25; 
    sphere[2].center.z = 1.25;

    sphere[2].radius = .50;

    sphere[2].color.r = 255; /// 0xFF
    sphere[2].color.g =   0;
    sphere[2].color.b =   0;

    /// tan sphere

    /// this sphere isn't used if nSpheres < 4

    sphere[3].center.x =      .50;
    sphere[3].center.y = 20000.00; /// teacher value: -20000.00
    sphere[3].center.z =      .50;

    sphere[3].radius   = 19500.25; /// teacher value: 20000.00

                             /// tan (peru)
    sphere[3].color.r = 205; /// 0xCD
    sphere[3].color.g = 133; /// 0x85
    sphere[3].color.b =  63; /// 0x3F
}

int main()
{
    init();

    pixelPtr = (int*) malloc( xRes*yRes*nColors * sizeof(int) ); /// xRes x yRes pixels, RGB for each pixel (res = resolution)

    //if( printDevFile ) devFile = fopen( devPath, "w" ); /// for program development

    ppmFile = fopen( "allgreen.ppm" , "w" ) ;
   

    //if( printDevFile ) printResolution();

    calcRays();

    //if( printDevFile ) printRays();

    calcMinReachFactors();

    //if( printDevFile ) printMinReachFactors();
        colorPixels();
    
    fprintf( ppmFile , "P3\n" ) ;
    fprintf( ppmFile, "%d %d\n" , xRes , yRes ) ;
    fprintf( ppmFile , "255\n" ) ;
    int x, y;
    for( y = 0 ; y < yRes ; y++ )
   {
      for( x = 0 ; x < xRes ; x++)
      {
           fprintf( ppmFile , "%d %d %d\n" ,
          pixel(x,y,R) , pixel(x,y,G) , pixel(x,y,B) ) ;
      }
   }
    //if( printDevFile ) printPixels();

    //printPixelsInPPMFormat();

    //if( printDevFile ) 
        //fclose(devFile);

    fclose(ppmFile);

    return 0;
}

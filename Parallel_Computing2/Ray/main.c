#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

//#define verifyPath "C:\\Documents\\Parallel\\Ray\\Rayverify.txt" /// for program verification
//#define imagePath  "C:\\Documents\\Parallel\\Ray\\Rayimage.txt"  /// for input to image program
 FILE* fout ;
//FILE *verifyFile, *imageFile;

#define FALSE 0
#define TRUE  1

/// xRes x yRes pixels, RGB for each pixel (res = resolution)

#define xRes 480     /// 480
#define yRes 640     /// 640
#define nColors 3
#define R 0
#define G 1
#define B 2

#define nBalls 4

int* pixelPtr; /// xRes x yRes array of pixels

/// my compiler won't generate correct array code for a 3D array, so I must use pointers
/// the following macro makes the pointer code brief

#define pixel(i,j,k) *( pixelPtr + i + j*xRes + k*xRes*yRes ) /// pixel[i][j][k]

typedef struct
{
    int valid; /// indicates if values are valid

    double min, max; /// min, max reach factors

} ReachFactors;

typedef struct
{
    double x, y, z;

    int ballIndex; /// ball having min reach factor

    double  minReachFactor; /// when a ray is multiplied by this factor, the resulting vector will reach from the eye to a ball
                            /// function calcMinReachFactors calculates the min reach factor across the balls (min in absolute value)
} XYZ; /// 3D vector

XYZ ray[xRes][yRes]; /// normalized ray for each pixel

XYZ eye = {  .50 ,  .50 , -1.00 };

XYZ lightSrc = { 0.00 , 1.25 , -.50 };

typedef struct
{
    unsigned char r, g, b;

} RGB;

RGB cantReachColor; /// this color is used when ray from eye can't reach any ball

typedef struct
{
    XYZ center;
    RGB color;

    double radius;

} Object;

Object ball[4]; /// ball[0] is platform on which the three balls rest

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
    /// returns sum of squares of 3D vector components

    return pow( xyz.x, 2 ) + pow( xyz.y, 2 ) + pow( xyz.z, 2 );
}

double magnitude( XYZ xyz )
{
    /// returns magnitude (length) of 3D vector

    return sqrt( sumOfSquares(xyz) );
}

void printPixelsForImagePgm()
{
    /// prints pixels for input to image program

    fout = fopen( "shade.ppm" , "w" ) ;
    
    fprintf( fout, "P3\n" );
    fprintf( fout, "%d %d\n", xRes, yRes );
    fprintf( fout, "255\n" );

    int x, y;

    for( x=0; x<xRes; x++ )
    {

        for( y=0; y<yRes; y++)
        {

            fprintf( fout, "%d %d %d\n" , pixel(x,y,R), pixel(x,y,G), pixel(x,y,B) );
        }
    }

            /// fprintf( imageFile, "%4d%4d  %4d%4d%4d\n", x, y, pixel(x,y,R), pixel(x,y,G), pixel(x,y,B) ); /// for program verification
   close( fout ) ;
}

//void printResolution()
//{
    //fprintf( verifyFile, "\n%8sX res%4d, Y res%4d\n", "", xRes, yRes );
//}

/*void printEye()
{
    fprintf( verifyFile, "\n%21s\n\n", "Eye" );

    fprintf( verifyFile, " %4s%4s%6s%6s%6s\n\n", "", "", " x ", " y ", " z " );
    fprintf( verifyFile, " %4s%4s%6.2f%6.2f%6.2f\n\n", "", "", eye.x, eye.y, eye.z );
}
 */
/*
void printPixels()
{
    /// prints pixels for program development

    fprintf( verifyFile, "\n%23s\n\n", "Pixels" );

    fprintf( verifyFile, "%4s%4s %6s%6s%6s\n\n", "x", "y", "R ", " G ", " B " );

    int x, y;

    for( x=0; x<xRes; x++ )
    {
        for( y=0; y<yRes; y++)

            fprintf( verifyFile, "%4d%4d %6d%6d%6d\n", x, y, pixel(x,y,R), pixel(x,y,G), pixel(x,y,B) );

        if( x < xRes-1 ) fprintf( verifyFile, "\n" );
    }
}
*/
/*void printRays()
{
    /// prints rays for program development

    printEye();

    fprintf( verifyFile, "%21s\n\n", "Rays" );
    fprintf( verifyFile, "%4s%4s %6s%6s%6s%6s\n\n", "x", "y", " x ", " y ", " z ", "mag" );

    int x, y;

    for( x=0; x<xRes; x++ )
    {
        for( y=0; y<yRes; y++)

            fprintf( verifyFile, "%4d%4d %6.2f%6.2f%6.2f%6.2f\n", x, y, ray[x][y].x, ray[x][y].y, ray[x][y].z, magnitude( ray[x][y] ) );

        if( x < xRes-1 ) fprintf( verifyFile, "\n" );
    }
}
 */
/*
void printMinReachFactors()
{
    /// prints min reach factors for program development

    printEye();

    fprintf( verifyFile, "%28s\n\n", "Min reach factors" );
    fprintf( verifyFile, "%4s%4s%8s%10s\n\n", "x", "y", "Ball", "Factor" );

    int x, y, i;

    for( x=0; x<xRes; x++ )
    {
        for( y=0; y<yRes; y++)

            if( ray[x][y].ballIndex >= 0 )

                fprintf( verifyFile, "%4d%4d %7d%10.2f\n", x, y, ray[x][y].ballIndex, ray[x][y].minReachFactor );

            else

                fprintf( verifyFile, "%4d%4d %18s\n", x, y, "No valid factors" );

        if( x < xRes-1 ) fprintf( verifyFile, "\n" );
    }
}
*/
void calcRays()
{
    /// for each pixel in xRes x yRes pixel array, calculate normalized ray from eye to pixel

    int x, y;

    for( x=0; x<xRes; x++ )
    {
        for( y=0; y<yRes; y++)

            /// normalize both x and y coordinates by xres
        {
            ray[x][y].x = x / (double)xRes; /// x is in (0,1) = (0,480/480)
            ray[x][y].y = y / (double)xRes; /// y is in (0,1.3) = (0,640/480) = (0,4/3)
            ray[x][y].z = 0;                /// z is zero because image is initially 2D

            /// subtract eye coordinates for distance from eye

            ray[x][y].x -= eye.x;
            ray[x][y].y -= eye.y;
            ray[x][y].z -= eye.z;

            /// normalize ray to magnitude (length) of one

            double rayMag = magnitude( ray[x][y] );

            ray[x][y].x /= rayMag;
            ray[x][y].y /= rayMag;
            ray[x][y].z /= rayMag;
        }
    }
}

ReachFactors calcReachFactors( XYZ ray, XYZ center, double radius ) /// center and radius of ball
{
    /// calculates reach factor to get from eye to ball, reach is directed, so may be negative

    /// this computation was derived by solving for T in the following four equations

    /// x = eye.x + T * ray.x
    /// y = eye.y + T * ray.y
    /// z = eye.z + T * ray.z

    /// (x-center.x)^2 + (y-center.y)^2 + (z-center.z)^2 = radius^2 /// equation for sphere

    /// the result yields the quadratic formula, with a, b, c determined by the foregoing equations

    XYZ diff = difference( eye, center ); /// calc eye-center

    /// a, b, c are the variables commonly used in the quadratic formula

    double a, b, c;

    a = 1; /// a==1 since a = square of length of ray, whose length was normalized to 1
           /// in general, a = sumOfSquares(ray)

    b = 2 * dotProduct( ray, diff );

    c = sumOfSquares(diff) - pow( radius, 2 );

    /// if quadratic determinant is non-negative, ray hits ball, so return reach factor, which is expressed in quadratic formula

    double determinant = pow(b,2) - 4*c; /// b^2 - 4*a*c, where a==1

    ReachFactors reach;

    if( determinant < 0 ) /// ray doesn't hit ball
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
    /// for each ray[x][y], calculates min reach factor across the four balls (min in absolute value)

    int x, y, i;

    for( x=0; x<xRes; x++ )

        for( y=0; y<yRes; y++)
        {
            int ballIndex = -1; /// init to invalid value

            double minReachFactor;

            /// for ray[x][y], find min reach factor across the four balls (min in absolute value)

            for( i=0; i<nBalls; i++ )
            {
                ReachFactors reach = calcReachFactors( ray[x][y], ball[i].center, ball[i].radius );

                if( !reach.valid ) continue; /// if invalid reach factors, continue to next iteration of inner loop

                /// set min to reach.min or reach.max, whichever has smallest absolute value

                double min = ( abs(reach.min) <= abs(reach.max) ) ? reach.min : reach.max;

                if( i==0 )
                {
                    /// if i==0, init values

                    ballIndex = i; /// record ball
                    minReachFactor = min;

                    continue; /// continue to next iteration of inner loop
                }

                if( abs(min) < abs(minReachFactor) )
                {
                    ballIndex = i; /// record ball
                    minReachFactor = min;
                }
            }

            ray[x][y].ballIndex = ballIndex;

            ray[x][y].minReachFactor = minReachFactor;
        }
}

void colorPixels()
{
    int x, y;

    for( x=0; x<xRes; x++ )

        for( y=0; y<yRes; y++)
        {
            if( ray[x][y].ballIndex < 0 ) /// if invalid index, set color to cantReachColor
            {
                pixel(x,y,R) = cantReachColor.r;
                pixel(x,y,G) = cantReachColor.g;
                pixel(x,y,B) = cantReachColor.b;
            }
            else
            {
                pixel(x,y,R) = ball[ ray[x][y].ballIndex ].color.r;
                pixel(x,y,G) = ball[ ray[x][y].ballIndex ].color.g;
                pixel(x,y,B) = ball[ ray[x][y].ballIndex ].color.b;
            }
        }
}

void init()
{
    /// set cantReachColor to black

    cantReachColor.r = cantReachColor.g = cantReachColor.b = 0;

    /// platform on which the three balls rest

    ball[0].center.x =       .50;
    ball[0].center.y = -20000.00;
    ball[0].center.z =       .50;

    ball[0].radius   =  20000.25;

                           /// med brown (peru)
    ball[0].color.r = 205; /// 0xCD
    ball[0].color.g = 133; /// 0x85
    ball[0].color.b =  63; /// 0x3F

    /// blue ball

    ball[1].center.x = .50;
    ball[1].center.y = .50;
    ball[1].center.z = .50;

    ball[1].radius = 0.25;

    ball[1].color.r =   0;
    ball[1].color.g =   0;
    ball[1].color.b = 255; /// 0xFF

    /// green ball

    ball[2].center.x = 1.00;
    ball[2].center.y =  .50;
    ball[2].center.z = 1.00;

    ball[2].radius = 0.25;


    ball[2].color.r =   0;
    ball[2].color.g = 255; /// 0xFF
    ball[2].color.b =   0;

    /// red ball

    ball[3].center.x = 0.00;
    ball[3].center.y =  .75;
    ball[3].center.z = 1.25;

    ball[3].radius = .50;

    ball[3].color.r = 255; /// 0xFF
    ball[3].color.g =   0;
    ball[3].color.b =   0;
}

int main()
{
    init();

    pixelPtr = (int*) malloc( xRes*yRes*nColors * sizeof(int) ); /// xRes x yRes pixels, RGB for each pixel (res = resolution)

    //verifyFile = fopen( verifyPath, "w" ); /// for program verification
    //imageFile  = fopen( imagePath, "w" );  /// for input to image program

    //printResolution();

    calcRays();

    //printRays();

    calcMinReachFactors();

    //printMinReachFactors();

    colorPixels();

    //printPixels();

    printPixelsForImagePgm();

    //fclose(verifyFile);
   // fclose(imageFile);

    return 0;
}

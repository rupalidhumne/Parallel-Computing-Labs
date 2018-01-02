#include <stdio.h>
#include <math.h>
#define N 640
#define M 480
FILE* fout ;

int x,y;
double ex=0.50;
double ey=0.50;
double ez=-1.00;
   
double lx=0.00;
double ly=1.25;
double lz=-0.50;

double floorX=0.0;
double floorY=0.333333;
double floorZ=0.0;

double spheres[3][7] = {{0.5,0.5,0.5,0.25,0,0,255},
{1.00,0.5,1.00,0.25,0,255,0},{0,0.75,1.25,0.5,255,0,0} };
int floor[3] = {255,205,0};
int rgb[N][M][3] ; // red-green-blue for each pixel
int main()
{
    int sphere ;

   int xs, ys, zs; //when dealing with the sphere
   double centerx, centery, centerz,r;
   double px, py, pz;
    double distances[4];  //either you or the one below you are going out of bounds
  double distancesBack[4];
   
	double rx, ry, rz, rmag, startx, starty, startz, rlx, rly, rlz, rlmag, a, b, c;
        int needShadow;
    for( y = 0 ; y < N ; y++ ) //window
   {
      for( x = 0 ; x < M ; x++) //window
      {	 
         needShadow=0; //0 means false
         px=(1.0*x)/(1.0*N); //rescaling
         py=1-(1.0*y)/(1.0*N);
         //printf("px: %f\n", px);
         //printf("py: %f\n", py);
         
         pz=0; 
         
        rx=px-ex;
        ry=py-ey;
        rz=pz-ez;
        
        rmag= sqrt((rx*rx)+(ry*ry)+(rz*rz));
        
        rx/=rmag;
        ry/=rmag;
        rz/=rmag;
        
        
        int minIndex;
        double minVal;
    return 0;
}
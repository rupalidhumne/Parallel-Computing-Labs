
#include <stdio.h>
#include <math.h>
//
#define M 640 //rows
#define N 480 //columns

int x,y;
double ex=0.50;
double ey=0.50;
double ez=-1.00;
   
double lx=0.00;
double ly=1.25;
double lz=-0.50;



   double spheres[4][7] = {{0.500000,-20000.00,0.5,20000.25,205,133,63},
        {0.5,0.5,0.5,0.25,0,0,255},
{1.00,0.5,1.00,0.25,0,255,0},{0,0.75,1.25,0.5,255,0,0} };
   
    int rgb[N][M][3] ; // red-green-blue for each pixel
int calcMin(double* dist)
{
	
	int x;
	int minIndex=-1;
	for(x=0; x<4; x++)
		{
			//printf("Tvalue: %f\n", dist[x]);
			if(dist[x]>0 && ( minIndex < 0 || dist[x] < dist[minIndex]))
				{
					
					minIndex=x;
				}
		}
	return minIndex;
}

double* findABC(double* abc, double rx, double ry, double rz, double centerx, double centery, double centerz, double r, double ex, double ey, double ez)
{
    double a, b, c;
    a=1;
    b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
    abc[0]=a;
    abc[1]=b; 
    abc[2]=c;
    return abc;
}
double calcT(double disc, double a, double b, double c)
{
    double Tval;
    if(disc>0)
        {
                Tval=((-1*b)-sqrt(disc))/(2*a);
                //T= printf("T %f\n", T);

        }
    else if(disc<0)
        {
                Tval=-1.0;
        }
    return Tval;
}
void calcColor(int sphere, int shadow, int y, int x)
{
    if(shadow==1)
    {
        if(sphere==-1)
        {
            rgb[y][x][0] = (int)(spheres[sphere][4] * 0.5);
            rgb[y][x][1] = (int)(spheres[sphere][5] * 0.5);
            rgb[y][x][2] = (int)(spheres[sphere][6] * 0.5); 
        }
        else
        {
            rgb[y][x][0] = (int)(spheres[sphere][4] * 0.1);
            rgb[y][x][1] = (int)(spheres[sphere][5] * 0.1);
            rgb[y][x][2] = (int)(spheres[sphere][6] * 0.1); 
        }
    }
    else
    {
        if(sphere==-1)
        {
            rgb[y][x][0] = spheres[sphere][4];
            rgb[y][x][1] = spheres[sphere][5];
            rgb[y][x][2] = spheres[sphere][6];
        }
    }
	
}
 double dotProduct(double x1,double x2,double y1,double y2,double z1,double z2)
    {
        return (x1*x2) + (y1*y2) + (z1*z2);
    }
int main(void)
{

   int sphere ;

   int xs, ys, zs; //when dealing with the sphere
   double centerx, centery, centerz,r;
   double px, py, pz;
    double distances[4];  //either you or the one below you are going out of bounds
  double distancesBack[4];
   
	double rx, ry, rz, rmag, startx, starty, startz, rlx, rly, rlz, rlmag, a, b, c;
        int needShadow;

   //
   FILE* fout ;
   //
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
         for(sphere=0; sphere<4; sphere++) //distance per each sphere
			{
                            centerx=spheres[sphere][0];
                            centery=spheres[sphere][1]; 
                            centerz=spheres[sphere][2];
                            r=spheres[sphere][3];
                            //printf("r: %f\n", r);
                            
                            double abc[3]; //new array for each sphere
                            
                            findABC(abc, rx, ry, rz, centerx, centery, centerz, r, ex, ey, ez);
                            //printf("a: %f\n", a);
                            //printf("b: %f\n", b);
                            //printf("c: %f\n", c);
                            double T;
                            a = abc[0];
                            b = abc[1];
                            c=abc[2];
                            //printf("a: %f\n", a);
                            //printf("b: %f\n", b);
                            //printf("c: %f\n", c);
                            double disc= (b*b)-(4*a*c);
                            
                            T= calcT(disc, a, b, c);
                            distances[sphere]=T;
                             //printf("T: %f\n", T);
                            
                        }
	minIndex=calcMin(distances); 
        
        if(minIndex==0)
        {
            minVal=-100.0;
        }
        else
        {
          minVal=distances[minIndex];
        }
        //printf("minIndex: %d\n", minIndex); //for someReason minIndex is never r g or b 
       
        //printf("minVal: %f\n", minVal);
        
        double normalX, normalY, normalZ, normalMag; 
        if(minIndex==0)
                {
                        
                        //printf("floorY: %f\n", floorY);
                        //printf("ey: %f\n", ey);
                        //printf("ry: %f\n", ry);
                        printf("Reached\n");
                        startx=(rx*minVal)+ex; //actual distance of minT
                        starty=(ry*minVal)+ey; 
                        startz=(rz*minVal)+ez;
                        
                        //printf("startx: %f\n", startx);
                        //printf("starty: %f\n", starty);
                        //printf("startz: %f\n", startz);
                        
                        normalX = 0;
                        normalY = 1;
                        normalZ = 0;

                }
        else
                {

                        startx=(rx*minVal)+ex; //actual distance of minT
                        starty=(ry*minVal)+ey; 
                        startz=(rz*minVal)+ez;
                        
                        normalX = (startx  - spheres[minIndex][0])/spheres[minIndex][3]; //x-z
                        normalY = (starty - spheres[minIndex][1])/spheres[minIndex][3];
                        normalZ = (startz - spheres[minIndex][2])/spheres[minIndex][3];
                        
                        normalMag = sqrt((startx*startx) + (starty*starty) + (startz*startz));
                        normalX /= normalMag;
                        normalY /= normalMag;
                        normalZ /= normalMag;
                       // printf("nx: %f\n", normalX);
                       // printf("ny: %f\n", normalY);
                        //printf("nz: %f\n", normalZ);
                        
                        startx += 0.0000000001*normalX;
                        starty += 0.0000000001*normalY;
                        startz += 0.0000000001*normalZ;

                }
                rlx=lx-startx; //light-sphere
                rly=ly-starty; //light-sphere
                rlz=lz-startz; //light-sphere
				
		rlmag= sqrt((rlx*rlx)+(rly*rly)+(rlz*rlz));
				
				
                rlx/=rlmag;
                rly/=rlmag;
                rlz/=rlmag;
		
                int sphereBack;
                
                for(sphereBack=0; sphereBack<4; sphereBack++) 
                        {
                                
                            centerx=spheres[sphereBack][0];
                            centery=spheres[sphereBack][1]; 
                            centerz=spheres[sphereBack][2];
                            r=spheres[sphereBack][3];
                            
                            double abc[3]; //new array for each sphere
                            
                            findABC(abc, rx, ry, rz, centerx, centery, centerz, r, ex, ey, ez);
                            double TBack;
                            a = abc[0];
                            b = abc[1];
                            c=abc[2];
                            double disc= (b*b)-(4*a*c);
                            TBack= calcT(disc, a, b, c);
                            distancesBack[sphereBack]=TBack;
                        }

                        
                          if (minIndex == 0 || minIndex==-1)
                            {
                               //printf("Need Shadow: %d : %f \n", needShadow, ry); 
                                if (ry>=0)
                                {
                                    
                                    rgb[y][x][0] = 0;
                                    rgb[y][x][1] = 0;
                                    rgb[y][x][2] = 0;
                                }
                                else
                                {
                                    //printf("Need Shadow: %d : %f \n", needShadow, ry); 
                                    if(needShadow==1) //NEVER REACHES
                                    {
                                        rgb[y][x][0] = (int)(spheres[0][4]*0.8);
                                        rgb[y][x][1] = (int)(spheres[0][5]*0.8);
                                        rgb[y][x][2] = (int)(spheres[0][6]*0.8);
                                    }
                                    else
                                    {

                                        rgb[y][x][0] = spheres[0][4];
                                        rgb[y][x][1] = spheres[0][5];
                                        rgb[y][x][2] = spheres[0][6];
                                    }
                                }
                            }
                          else
                          {
                              double dot = dotProduct(normalX, rlx, normalY, rly, normalZ, rlz);
                              if(needShadow==1)
                                {
                                  //calcColor(minIndex, 1, y, x);
                                  rgb[y][x][0] = (int)(spheres[minIndex][4]*0.4);
                                  rgb[y][x][1] = (int)(spheres[minIndex][5]*0.4);
                                  rgb[y][x][2] = (int)(spheres[minIndex][6]*0.4);
                                }
                              else
                                {   
                                    if (dot>0.0)
                                    {
                                    rgb[y][x][0] = (int)(spheres[minIndex][4] *dot);
                                    rgb[y][x][1] = (int)(spheres[minIndex][5] *dot);
                                    rgb[y][x][2] = (int)(spheres[minIndex][6] *dot);
                                    }
                                    else
                                    {
                                        
                                            rgb[y][x][0] = (int)(spheres[minIndex][4] *-dot);
                                            rgb[y][x][1] = (int)(spheres[minIndex][5] *-dot);
                                            rgb[y][x][2] = (int)(spheres[minIndex][6] *-dot);
                                        
                                    }
                                }
                              
                          }
                   if(rgb[y][x][0]>255 || rgb[y][x][1]>255 || rgb[y][x][2]>255)
                    {
                        rgb[y][x][0]=0;
                        rgb[y][x][1]=0;
                        rgb[y][x][2]=255;
                    }

                }
            
          }
   fout = fopen( "allgreen.ppm" , "w" ) ;
   //
   fprintf( fout , "P3\n" ) ;
   fprintf( fout , "%d %d\n" , M , N ) ;
   fprintf( fout , "255\n" ) ;
   //
   for( y = 0 ; y < N ; y++ )
   {
      for( x = 0 ; x < M ; x++)
      {
         fprintf( fout , "%d %d %d\n" ,
          rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
      }
   }
   close( fout ) ;
   //
   return 0 ;
}

//
// end of file
//


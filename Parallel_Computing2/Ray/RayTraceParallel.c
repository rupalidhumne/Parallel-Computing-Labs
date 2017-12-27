//
// OpenMP demo
//
// gcc -fopenmp omp_demo.c
//
#include <omp.h>
#include <stdio.h>
//
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



   double spheres[4][7] = {{0.50,-20000.00,0.5,20000.25,205,133,63},
        {0.5,0.5,0.5,0.25,0,0,255},
{1.00,0.5,1.00,0.25,0,255,0},{0,0.75,1.25,0.5,255,0,0} };
   
    int rgb[N][M][3] ; // red-green-blue for each pixel
    
    void calcColor()
    {
        needShadow=0; //0 means false
         px=(1.0*x)/(1.0*N); //rescaling
         py=1-((1.0*y)/(1.0*N));
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
                            double T;
                            double abc[3]; //new array for each sphere
                            centerx=spheres[sphere][0];
                            centery=spheres[sphere][1]; 
                            centerz=spheres[sphere][2];
                            r=spheres[sphere][3];
                            //printf("r: %f\n", r);
                            
                            
                            
                            findABC(abc, rx, ry, rz, centerx, centery, centerz, r, ex, ey, ez);
                            //printf("a: %f\n", a);
                            //printf("b: %f\n", b);
                            //printf("c: %f\n", c);
                            
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
        
        if(minIndex!=0)
        {
           minVal=distances[minIndex];
             //just to make sure not shadowed
       }
        else
        {
            minVal=-10000.0;
        }
        //printf("minIndex: %d\n", minIndex); //for someReason minIndex is never r g or b 
       
        //printf("minVal: %f\n", minVal);
        
        double normX, normY, normZ, normMag;
        startx=(rx*minVal)+ex; //actual distance of minT
        starty=(ry*minVal)+ey; 
        startz=(rz*minVal)+ez;
        if(minIndex==0)
                {
                        
                        normX = 0;
                        normY = 1;
                        normZ = 0;

                }
        else
                {

                        //normal vector
                        normX = (startx  - spheres[minIndex][0])/spheres[minIndex][3]; 
                        normY = (starty - spheres[minIndex][1])/spheres[minIndex][3];
                        normZ = (startz - spheres[minIndex][2])/spheres[minIndex][3];
                        
                        normMag = sqrt((startx*startx) + (starty*starty) + (startz*startz));
                        
                        normX /= normMag;
                        normY /= normMag;
                        normZ /= normMag;
                       // printf("nx: %f\n", normalX);
                       // printf("ny: %f\n", normalY);
                        //printf("nz: %f\n", normalZ);
                        
                        startx += 0.00000000000000001*normX;
                        starty += 0.00000000000000001*normY;
                        startz += 0.00000000000000001*normZ;

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
                              double TBack; 
                               double abc[3]; //new array for each sphere
                            centerx=spheres[sphereBack][0];
                            centery=spheres[sphereBack][1]; 
                            centerz=spheres[sphereBack][2];
                            r=spheres[sphereBack][3];
                            
                           
                            
                            findABC(abc, rlx, rly, rlz, centerx, centery, centerz, r, lx, ly, lz);
                           
                            a = abc[0];
                            b = abc[1];
                            c=abc[2];
                            double disc= (b*b)-(4*a*c);
                            TBack= calcT(disc, a, b, c);
                            distancesBack[sphereBack]=TBack;
                        }
                
                int needShadowFloor;
                int u;
                
                for(u=1; u<4; u++)
                {
                    
                    if(distancesBack[u]>0.0)
                    {
                        printf("U: %d DistanceBack %f \n", u, distancesBack[u]);    
                        needShadow=1; //hit minIndexBack 
                            
                    }
                }
                //SHADOW FOR THE FLOOR and my spheres
                          if (needShadow==0) 
                            {
                               if(minIndex==0)   
                               {
                                   rgb[y][x][0] = spheres[0][4];
                                   rgb[y][x][1] = spheres[0][5];
                                   rgb[y][x][2] = spheres[0][6];
                               }
                               else
                               {
                                   double dot = (normX *rlx) + (normY *rly) + (normZ*rlz);
    

                                  if (dot<=0.0)
                                        {
                                            rgb[y][x][0] = (int)(spheres[minIndex][4] *dot*-1);
                                                rgb[y][x][1] = (int)(spheres[minIndex][5] *dot*-1);
                                                rgb[y][x][2] = (int)(spheres[minIndex][6] *dot*-1);
                                        }
                                  if(dot>0.0)
                                        {

                                          rgb[y][x][0] = (int)(spheres[minIndex][4] *dot);
                                        rgb[y][x][1] = (int)(spheres[minIndex][5] *dot);
                                        rgb[y][x][2] = (int)(spheres[minIndex][6] *dot);      

                                        }
                                   
                               }

                            }
                          else // shadow on the sphere itself
                          {
                              if(minIndex==0)
                              {
                                        rgb[y][x][0] = (int)(spheres[0][4] * 0.7);
                                        rgb[y][x][1] = (int)(spheres[0][5] * 0.7);
                                        rgb[y][x][2] = (int)(spheres[0][6] * 0.7);

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
    }
//
int main(int argc,char* argv[])
{
	int tid, nthreads, j, total;
	//
	int count[N];
        nt sphere ;

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
	//
	//
	omp_set_num_threads(5);
	
	#pragma omp parallel for private(tid,x) //each will get different value of x
	//
	for( y = 0 ; y < N ; y++ ) //window
        {
            for( x = 0 ; x < M ; x++) //window
            {	 
		
                  //tid=omp_get_thread_num();
                   calcColor(x,y);
            }
	}
	//
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
	return 0;
}
//
// end of file
//
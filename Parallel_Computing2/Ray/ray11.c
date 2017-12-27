//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
//
#define M 640 //rows
#define N 480 //columns
	int x,y;
double ex=0.50;
double ey=0.50;
double ez=-1.00;

double ax=0.5;
double ay=-20000.00;
 double az= 0.50 ;
 double ar= 20000.25 ;
     
   double bx= 0.50 ;
   double by= 0.50 ;
   double bz= 0.50 ;
    double br= 0.25 ;
   
   double cx=1.00;
   double cy=0.50;
   double cz=1.00;
   double cr= 0.25 ;
   
   double dx=0.00;
   double dy=0.75;
   double dz = 1.25;
   double dr= 0.50 ;
   
double lx=0.00;
double ly=1.25;
double lz=-0.50;

double floorY=0.33333333;
double a,b,c, a1, b1,c1, T;

   int colorX, colorY, colorZ; //rgb values of the color 
    //index of sphere that it will hit
   
   	double distances[4];  //either you or the one below you are going out of bounds
   	double distancesBack[4];
   
    int rgb[N][M][3] ; // red-green-blue for each pixel
int calcMin()
{
	
	int x;
	int minIndex=-1;
	for(x=0; x<4; x++)
		{
			//printf("Tvalue: %f\n", distances[x]);
			if(distances[x]>0 && ( minIndex < 0 || distances[x] < distances[minIndex]))
				{
					
					minIndex=x;
				}
		}
	return minIndex;
}
void calcColor(int sphere, int y, int x)
{
	 
	 //rgb[y][x][0] = 0   ; // red
      //rgb[y][x][1] = 255 ; // green
      //rgb[y][x][2] = 0   ; // blue
      if(sphere==0)
		{
			colorX =    205    ; // color is Peru
			colorY=    133    ;
			colorZ =     63    ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==1)
		{
			
			colorX =    0   ; 
			colorY=    0    ;
			colorZ =     255    ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==2)
		{
			
			colorX =    0   ; 
			colorY=    255   ;
			colorZ =    0   ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==3)
		{
			
			colorX =    255  ; 
			colorY=    0  ;
			colorZ =    0  ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
}
void calcShadow(int sphere, int y, int x)
{
	if(sphere==0)
		{
			colorX =   20.5   ; // color is Peru
			colorY=    13.3   ;
			colorZ =     6.3   ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==1)
		{
			
			colorX =    0   ; 
			colorY=    0    ;
			colorZ =     25.5   ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==2)
		{
			
			colorX =    0   ; 
			colorY=    25.5   ;
			colorZ =    0   ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==3)
		{
			
			colorX =    25.5  ; 
			colorY=    0  ;
			colorZ =    0  ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
}	
int main(void)
{

      //
   int sphere ;

   int xs, ys, zs; //when dealing with the sphere
   double centerx, centery, centerz,r;
   double px, py, pz;
   
	double rx, ry, rz, rmag, startx, starty, startz, rlx, rly, rlz, rlmag, needShadow;

   //
   FILE* fout ;
   //
   for( y = 0 ; y < N ; y++ ) //window
   {
      for( x = 0 ; x < M ; x++) //window
      {
		 //printf("y: %d \n", y);
		 //printf("x: %d \n", x);
		 
         needShadow=0;
         px=(1.0*x)/(1.0*N); //rescaling
		 py=1-(1.0*y)/(1.0*N);

		 pz=0; 
		 rx=px-ex;
		 ry=py-ey;
		 rz=pz-ez;
		 //printf("rx: %f\n", rx);
		 //printf("ry: %f\n", ry);
		 //printf("rz: %f\n", rz);
		 rmag= sqrt((rx*rx)+(ry*ry)+(rz*rz));
		 //printf("rmag: %f\n", rmag);
		 rx/=rmag;
		 ry/=rmag;
		 rz/=rmag; //unit vector with length 1, ray with just a direction
		 //printf("rx: %f\n", rx);
		 //printf("ry: %f\n", ry);
		 //printf("rz: %f\n", rz);
		 
		
		 int minDistance;
         for(sphere=0; sphere<4; sphere++) //distance per each sphere
			{
				 double T;
				 
				 
				if(sphere==0)
					{
						centerx=ax;
						centery=ay; 
						centerz=az;

						r=ar;

					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    

					}
				else if(sphere ==1)
					{
						centerx=bx;
						centery=by; 
						centerz=bz;

						r=br;

					     a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    

					}
				else if(sphere==2)
					{
						centerx=cx;
						centery=cy; 
						centerz=cz;

						r=cr;

					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    
					    

					}
				else if(sphere==3)
					{
						centerx=dx;
						centery=dy; 
						centerz=dz;

						r=dr;

					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					   //printf("a: %f\n", a);
						//printf("b: %f\n", b);
						//printf("c: %f\n", c);

					}

				double deter= (b*b)-(4*a*c);
				//printf("deter %f\n", deter); 
 
				if(deter>0)
					{
						//printf("reached\n");
						T=((-1*b)+sqrt((b*b)-(4*a*c)))/(2*a);
						T= printf("T %f\n", T);

					}
				else
					{
						T=-1.0;
					}
				distances[sphere]=T;	
				//printf("sphere: %d\n", sphere);
				
			}
			
				minDistance=calcMin();
				//printf("minDistance: %d\n", minDistance);

				
				
				

				if(minDistance==-1)
					{
						double Tfloor = floorY-ey/ry;

						startx=(rx*Tfloor)+ex; 
						starty=(ry*Tfloor)+ey; 
						startz=(rz*Tfloor)+ez;

					}
				else
					{

						startx=(rx*T)+ex; 
						starty=(ry*T)+ey; 
						startz=(rz*T)+ez;

					}
				
				
				rlx=lx-startx; //light-sphere
				rly=ly-starty; //light-sphere
				rlz=lz-startz; //light-sphere
				
				rlmag= sqrt((rlx*rlx)+(rly*rly)+(rlz*rlz));
				
				
				rlx/=rlmag;
				rly/=rlmag;
				rlz/=rlmag;
				
				int sphereBack;
				//printf("minDistance: %d\n", minDistance);
				
				if(minDistance>=0) 
					{
						for(sphereBack=0; sphereBack<4; sphereBack++) 
							{
								double TBack;
								if(sphere==0)
								{
									centerx=ax;
									centery=ay; 
									centerz=az;
									r=ar;
									
									a1=1;
								   b1= (2*lx*rlx)+(2*ly*rly)+(2*lz*rlz)-(2*rlx*centerx)-(2*rly*centery)-(2*rlz*centerz);
									c1=(lx*lx)-(2*lx*centerx)+(centerx*centerx)+(ly*ly)-(2*ly*centery)+(centery*centery)+(lz*lz)-(2*lz*centerz)+(centerz*centerz)-(r*r);
								}
								else if(sphere ==1)
									{
										centerx=bx;
										centery=by; 
										centerz=bz;
										r=br;
										
										 a1=1;
										 b1= (2*lx*rlx)+(2*ly*rly)+(2*lz*rlz)-(2*rlx*centerx)-(2*rly*centery)-(2*rlz*centerz);
										 c1=(lx*lx)-(2*lx*centerx)+(centerx*centerx)+(ly*ly)-(2*ly*centery)+(centery*centery)+(lz*lz)-(2*lz*centerz)+(centerz*centerz)-(r*r);

									}
								else if(sphere==2)
									{
										centerx=cx;
										centery=cy; 
										centerz=cz;
										r=cr;
										
										 a1=1;
										 b1= (2*lx*rlx)+(2*ly*rly)+(2*lz*rlz)-(2*rlx*centerx)-(2*rly*centery)-(2*rlz*centerz);
										 c1=(lx*lx)-(2*lx*centerx)+(centerx*centerx)+(ly*ly)-(2*ly*centery)+(centery*centery)+(lz*lz)-(2*lz*centerz)+(centerz*centerz)-(r*r);

									}
								else if(sphere==3)
									{
										centerx=dx;
										centery=dy; 
										centerz=dz;
										r=dr;
										
										a1=1;
										b1= (2*lx*rlx)+(2*ly*rly)+(2*lz*rlz)-(2*rlx*centerx)-(2*rly*centery)-(2*rlz*centerz);
										c1=(lx*lx)-(2*lx*centerx)+(centerx*centerx)+(ly*ly)-(2*ly*centery)+(centery*centery)+(lz*lz)-(2*lz*centerz)+(centerz*centerz)-(r*r);

									}
							double deterBack= (b1*b1)-(4*a1*c1);
							if(deterBack>0)
								{
									TBack=((-1*b1)-sqrt((b1*b1)-(4*a1*c1)))/(2*a1);
								}
							else
								{
									TBack=-1.0;
								}
							distancesBack[sphereBack]=TBack;	
							//printf("sphereBack: %d\n", sphereBack);
							
									
						}
					int u;
					for(u=0; u<4; u++)
						{
							if(distancesBack[u]>0 && u!=minDistance) //if it hit a sphere and the sphere hit is not the same sphere as it's on
								{
									needShadow=1;
									break;
								}
						}

							if(needShadow==1)
								{
									calcShadow(minDistance, y, x);
								}
							else
								{
									calcColor(minDistance, y, x); //minDistance = sphere 
								}
					
				}
						
				minDistance=0;
				int e;
				for(e=0; e<4; e++)
					{
						distances[x]=0;
						distancesBack[x]=0;
					}
      }
   }
   //
   //
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
   return 0 ;
}

//
// end of file
//

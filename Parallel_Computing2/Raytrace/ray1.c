//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
//
#define M 640 //rows
#define N 480 //columns
double ex=0.50;
double ey=0.50;
double ez=-1.00;

double lx=0.00;
double ly=1.25;
double lz=-0.50;

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
   

   double a,b,c, T, a1, b1, c1;
   int colorX, colorY, colorZ; //rgb values of the color 
    //index of sphere that it will hit
   	int x,y;
   	double distances[4];
   	double distancesBack[4];
   
    int rgb[N][M][3] ; // red-green-blue for each pixel
int calcMin()  //the index
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
int calcMinBack()  //the index STOPPED HERE: EITHER A) RETURN INDEX OF "MIN DISTANCE" FROM SPHERE TO LIGHT SOURCE (TO FIND RIGHT SPHERE TO SHADOW) OR B) RETURN NEGATIVE 1 MEANING DOESN'T HIT ANYTHING ON THE WAY BACK
{
	
	int x;
	int needShadow = -1;
	for(x=0; x<4; x++)
		{
			//printf("Tvalue: %f\n", distances[x]);
			if(distancesBack[x]>0 && ( needShadow < 0 || distances[x] < distances[needShadow]))
				{
					
					needShadow=x;
				}
		}
	return needShadow; //will either return -1 (doesn't hit anything) or will return 0,1,2,3
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
	 
	 //rgb[y][x][0] = 0   ; // red
      //rgb[y][x][1] = 255 ; // green
      //rgb[y][x][2] = 0   ; // blue
      if(sphere==0)
		{
			colorX =    150   ; // color is Peru
			colorY=    100   ;
			colorZ =     50    ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==1)
		{
			
			colorX =    0   ; 
			colorY=    0    ;
			colorZ =     150   ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==2)
		{
			
			colorX =    0   ; 
			colorY=    150   ;
			colorZ =    0   ;
			rgb[y][x][0]=colorX;
			rgb[y][x][1]=colorY;
			rgb[y][x][2]=colorZ;
		}
	if(sphere==3)
		{
			
			colorX =    150  ; 
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
   int sphere, sphereBack ;

   int xs, ys, zs; //when dealing with the sphere
   double centerx, centery, centerz,r;
   double px, py, pz;
   
	double rx, ry, rz, rmag, rlx, rly, rlz, rlmag;

   //
   FILE* fout ;
   //
   for( y = 0 ; y < N ; y++ ) //window
   {
      for( x = 0 ; x < M ; x++) //window
      {
          //distance to each sphere
         
         px=(1.0*x)/(1.0*N); //rescaling
		 py=1-(1.0*y)/(1.0*N);
		 pz=0; 
		 
		 rx=px-ex;
		 //printf("rx: %f\n", rx);
		 ry=py-ey;
		 //printf("ry: %f\n", ry);
		 rz=pz-ez;
		 //printf("rz: %f\n", rz);
		

		 rmag= sqrt((rx*rx)+(ry*ry)+(rz*rz));
		 //printf("rmag: %f\n", rmag);

		 rx/=rmag;
		 ry/=rmag;
		 rz/=rmag; //unit vector with length 1, ray with just a direction
		
		//back to light source
		rlx=px-lx;
		//printf("rlx: %f\n", rlx);
		rly=py-ly;
		//printf("rly: %f\n", rlx);
		rlz=pz-lz;
		//printf("rlz: %f\n", rlx);
	
		
		rlmag= sqrt((rlx*rlx)+(rly*rly)+(rlz*rlz));
		//printf("rlmag: %f\n", rlmag);
		
		 rlx/=rlmag;
		 rly/=rlmag;
		 rlz/=rlmag; 
		 //printf("rx: %f\n", rlx);
		 //printf("ry: %f\n", rly);
		 //printf("rz: %f\n", rlz);

		
		 int minDistance, minDistanceBack;
         for(sphere=0; sphere<4; sphere++) //distance per each sphere
			{
				 double T, Tback;
				 
				 
				if(sphere==0)
					{
						centerx=ax;
						centery=ay; 
						centerz=az;
						
						r=ar;
						
					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					     //printf("a: %f\n", a);
						//printf("b: %f\n", b);
						//printf("c: %f\n", c);
					    
					    
					    a1=1;
					   b1= (2*lx*rlx)+(2*ly*rly)+(2*lz*rlz)-(2*rlx*centerx)-(2*rly*centery)-(2*rlz*centerz);
					    c1=(lx*lx)-(2*lx*centerx)+(centerx*centerx)+(ly*ly)-(2*ly*centery)+(centery*centery)+(lz*lz)-(2*lz*centerz)+(centerz*centerz)-(r*r);
					     //printf("a1: %f\n", a1);
						//printf("b1: %f\n", b1);
						//printf("c1: %f\n", c1);
	
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

					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    
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

					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    
					     a1=1;
					   b1= (2*lx*rlx)+(2*ly*rly)+(2*lz*rlz)-(2*rlx*centerx)-(2*rly*centery)-(2*rlz*centerz);
					    c1=(lx*lx)-(2*lx*centerx)+(centerx*centerx)+(ly*ly)-(2*ly*centery)+(centery*centery)+(lz*lz)-(2*lz*centerz)+(centerz*centerz)-(r*r);

					}

				double deter= (b*b)-(4*a*c);
				double deter1= (b1*b1)-(4*a1*c1);
				if(deter>0)
					{

						T=((-1*b)+sqrt((b*b)-(4*a*c)))/(2*a);
					}
				else
					{
						T=-1.0;
					}
				if(deter1>0)
					{

						Tback=((-1*b1)+sqrt((b1*b1)-(4*a1*c1)))/(2*a1);
					}
				else
					{
						Tback=-1.0;
					}
				distances[sphere]=T;
				distancesBack[sphere]=Tback;	
				
				
			}

				minDistance=calcMin();//min distance gives you the index of the min val, index corresponds to a sphere
				if(minDistance>-1)
				{
					minDistanceBack=calcMinBack(); //can either return a -1 (did not hit anything on the way back) or the value of the index of the sphere it hit;
				}
				if(minDistanceBack==-1)
				{
					calcColor(minDistance, y, x);
				}
				else
					{
						calcShadow(minDistance, y, x);
					}
				T=0;
				minDistance=0;
				minDistanceBack=0;
				
				
				int e;
				for(e=0; e<4; e++)
					{
						distances[x]=0;
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

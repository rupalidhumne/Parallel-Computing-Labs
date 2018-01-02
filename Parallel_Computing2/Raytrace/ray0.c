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
   

   double a,b,c, T;
   int colorX, colorY, colorZ; //rgb values of the color 
    //index of sphere that it will hit
   	int x,y;
   	double distances[4];
   
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
int main(void)
{

   //
   int sphere ;

   int xs, ys, zs; //when dealing with the sphere
   double centerx, centery, centerz,r;
   double px, py, pz;
   
	double rx, ry, rz, rmag;

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
		 ry=py-ey;
		 rz=pz-ez;

		 rmag= sqrt((rx*rx)+(ry*ry)+(rz*rz));

		 rx/=rmag;
		 ry/=rmag;
		 rz/=rmag; //unit vector with length 1, ray with just a direction
		
		 
		
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
					    //printf("a: %f\n", a);
						//printf("b: %f\n", b);
						//printf("c: %f\n", c);
					}
				else if(sphere ==1)
					{
						centerx=bx;
						centery=by; 
						centerz=bz;
						//printf("centerx: %f\n", centerx);
						//printf("centery: %f\n", centery);
						//printf("centerz: %f\n", centerz);
						r=br;
						//printf("r: %f\n", r);
					     a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    
					}
				else if(sphere==2)
					{
						centerx=cx;
						centery=cy; 
						centerz=cz;
						//printf("centerx: %f\n", centerx);
						//printf("centery: %f\n", centery);
						//printf("centerz: %f\n", centerz);
						r=cr;
						//printf("r: %f\n", r);
					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    //printf("a: %f\n", a);
						//printf("b: %f\n", b);
						//printf("c: %f\n", c);
					}
				else if(sphere==3)
					{
						centerx=dx;
						centery=dy; 
						centerz=dz;
						//printf("centerx: %f\n", centerx);
						//printf("centery: %f\n", centery);
						//printf("centerz: %f\n", centerz);
						r=dr;
						//printf("r: %f\n", r);
					    a=1;
					   b = (2*ex*rx)+(2*ey*ry)+(2*ez*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
					    c=(ex*ex)-(2*ex*centerx)+(centerx*centerx)+(ey*ey)-(2*ey*centery)+(centery*centery)+(ez*ez)-(2*ez*centerz)+(centerz*centerz)-(r*r);
					    //printf("a: %f\n", a);
						//printf("b: %f\n", b);
						//printf("c: %f\n", c);
					}
				//if(rx==0 && ry==0)
					//{//
						//double deter= (b*b)-(4*a*c);
						//printf("determinant: %f\n", deter);
					//}
				double deter= (b*b)-(4*a*c);
				//printf("deter %f\n", deter); 
				if(deter>0)
					{
						//hits = 1;
						//printf("hit");
						//printf("index %d\n", sphere);
						T=((-1*b)+sqrt((b*b)-(4*a*c)))/(2*a);
						printf("Tvalue: %f\n", T);
						//printf("a: %f\n", a);
						//printf("b: %f\n", b);
						//printf("c: %f\n", c);
					}
				else
					{
						T=-1.0;
					}
				distances[sphere]=T;	
				
				//printf("Tvalue: %f\n", distances[sphere]);
			}
				//if(distances[0]>0 && distances[3]>0)
				//	{
							//printf("I found a nonperu color\n");
							//printf("index %d\n", sphere);
							//printf("Tvalue: %f\n", distances[0]);
							//printf("Tvalue: %f\n", distances[3]);
					//}
				//90% sure breaks here
				minDistance=calcMin();//min distance gives you the index of the min val, index corresponds to a sphere
				//printf("minDistance: %d \n", minDistance);
				//printf("minDistance: %d \n", minDistance);
				calcColor(minDistance, y, x);
				T=0;
				minDistance=0;
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

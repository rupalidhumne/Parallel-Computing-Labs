#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
// 
#include "mpi.h"
// 

#include <sys/time.h>
#include <assert.h>
double MinRe = -2.0; //window zoom vals
double MaxRe = 2.0;
double MinIm = -1.5;
double MaxIm = 1.5;
int    pause = 0 ;
int max_iterations=100  ; 
int w=600;
int h=600;
float coords[N*4]; //real coords and then complex coords
float colorSteps[N];
void drawString( char* s )
{
   int k = 0 ;
   //
   while( s[k] != '\0' )
   {
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,s[k]);
      //
      ++k ;
   }
}

int main( int argc , char* argv[] ) //each worker gets a column of pixels and returns a column of steps
{
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(w,h);
   glutInitWindowPosition(100,100);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
   
   glutPostRedisplay();
   return 0;
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)w, (GLsizei)h); //this should be the same as Ortho2D in the future!!!
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*w,0.0,1.0*h); // always a square
   glMatrixMode(GL_MODELVIEW);
}
void doWork(float rReal, float cReal, float rComp, float cComp)
{
		 x=0.0;
         y=0.0;
         i=0;
         float xtemp;
         while(x*x + y*y<2*2 && i<max_iterations)
         {
            xtemp= x*x -y*y +rval;
            y=2*x*y +cval;
            x=xtemp;
            i=i+1;
         }
         return i;
}
void displayfunc()
{
   int  rank;
   int  size;
   MPI_Status status;
   double tic , toc ;
   int  tag  =0;
   
   //
   // other variables
   //
   
   
   //double kill = -0.1;
   tic=gettime();
   MPI_Init(&argc, &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
  
   
   srand( clock() );
   
   int blah=0;
   
   if( rank == 0 ) //manager
   {
       
   glClear(GL_COLOR_BUFFER_BIT); // white
   float x=0.0;
   float y=0.0;
   float x0;
   int rx;
   int ry;
   float y0;
   int index;
   int j;
   int k;
   int numCol;
   int numWorker;
   int max_iterations=100;
   float xtemp;
   int steps=0;
   int ImageHeight=h-1;
   int ImageWidth=w-1;

	
	
		//{
			/* 
			 *pseudocode:
			 * for a column in the array
			 * traverse the rows
			 * take each x y coordinate and map to complex plane
			 * add to an array of stuff for the worker
			 * send that array to a worker until max workers reached
			 * pass mpi array by doing mpi(variable without ampercent, array.length, double etc)
			*/
			for(ry=0; ry<size; ry++) //IMPORTANT, NEED TO MAKE IT SO THAT COORDS RESETS EVERY TIME YOU SEND IT TO A NEW WORKER, INDEX NEES TO RESET TOO
				{
						
						for(rx=0; rx<N; rx++)
							{
								coords[index]=rx; //send both real and imaginary parts over
								index++;
								coords[index]=ry;
								index++;
								x0= MinRe + rx*(MaxRe-MinRe)/(ImageWidth); //the a part of a+bi 
								coords[index]=x0;
								index++;
								y0=MaxIm - ry*(MaxIm-MinIm)/(ImageHeight); //point on complex plane bi
								coords[index]=y0;
								index++;
								
							}
						MPI_Send( coords, 4*N , MPI_FLOAT , numWorker , tag , MPI_COMM_WORLD ) ;
						numCol++; 
					}
					
				}
      while(numCol<N)
      {
          /*pseudocode
           * while you still have columns left (numCol<n)
           * receive the array from a worker
           * traverse the array and map the colors for each
           * then make a new array blah
           * for the next column (tracked by numCol), get all the points 
           * map to complex numbers
           * store in the array and send to a worker
           * 
           * */
          MPI_Recv( colorSteps , N*3 , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
          s1 = status.MPI_SOURCE ; 
          for(j=0; j<N; j++)
			{
				rx=colorSteps[j];
				ry=colorSteps[j+1];
				steps=colorSteps[j+2];
				if(x*x + y*y<2*2) //if over max
				  {
					 glColor3f( 0.0 , 0.0 , 0.0 ) ; 
				  }
				  else
				  {
					glColor3f(0.3*i, 0.5*i, 0.2*i);
				  }
					 glBegin(GL_POINTS);
					 glVertex2f(rx,ry);
					 glEnd();
		   }
		   for(i=0; i<N; i++) //needs to be edited
			{
				index++;
				coords[index]=ry;
				index++;
				x0= MinRe + rx*(MaxRe-MinRe)/(ImageWidth); //the a part of a+bi 
				coords[index]=x0;
				index++;
				y0=MaxIm - ry*(MaxIm-MinIm)/(ImageHeight); //point on complex plane bi
				coords[index]=y0;
				index++;
			}
			
          MPI_Send( coords , 4*N , MPI_DOUBLE , s1 , tag , MPI_COMM_WORLD ) ;
         
      }
       
      for( k = 1 ; k < size; k++ ) //for every worker
      {
          MPI_Recv( &workerResult , 1 , MPI_FLOAT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
          s2 = status.MPI_SOURCE ;
          x0=30000;
          y0=30000;
          MPI_Send( &x0, &y0 , 1 , MPI_DOUBLE , s2 , tag , MPI_COMM_WORLD ) ;
      }
    }
    else
		{
			 int y;
			 int z;
			 int steps;
			 float rReal, cReal, rComp, cComp;
       		while(1)
			   {
				  MPI_Recv( coords , 4*N , MPI_FLOAT, 0 , tag , MPI_COMM_WORLD , &status ); 
				   /*pseudocode
					* receive coordinates
					* traverse through the coordinates
					* take each and do the "step function" 
					* add the steps to an array 
					* and send back
					* if the array = blah, break
				*/
				while(y<4*N) //600 coords per col but passed both real coords and mapped ones
					{
						rReal=coords[y];
						y++;
						cReal=coords[y];
						y++;
						rComp=coords[y]; //the row val and the col val is the next cell
						y++;
						cComp=coords[y];
						y++;
						steps = doWork(rReal, cReal, rComp, cComp); //color steps is formatted 
						colorSteps[z]=rReal;
						z++;
						colorSteps[z]=cReal;
						z++;
						colorSteps=steps;
						z++;
					}
					
				  MPI_Send( colorSteps , N*3 , MPI_INT, 0 , tag , MPI_COMM_WORLD) ;
				
			}
		}
   
   
   MPI_Finalize() ;
   toc=gettime();
   printf("Time: %0.16f seconds\n" , toc - tic ) ;


}


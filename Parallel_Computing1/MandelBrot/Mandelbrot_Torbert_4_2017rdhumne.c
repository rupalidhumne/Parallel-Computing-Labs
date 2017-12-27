#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
// 
#include "mpi.h"

//rather than passing arrays back and forth, just pass the array of colors back, you know everything else as long as you know what column number you're on
#include <sys/time.h>
#include <assert.h>
double MinRe=-2.0;
double MaxRe=2.0;
double MinIm=-1.5;
double MaxIm=1.5;
double dimensions[6];
int pause = 0 ;
int max_iterations=100; 
int N=600;
int s1, s2;
int steps[600];
int ImageHeight=599;
int ImageWidth=599;
   int  size;
   MPI_Status status;
    int  rank;
    int numCol;
   int  tag  =0;
   int loop=1;
   int kill;
   double xval;
   double yval;


void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)N, (GLsizei)N); //this should be the same as Ortho2D in the future!!!
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*N,0.0,1.0*N); // always a square
   glMatrixMode(GL_MODELVIEW);
}
void displayfunc()
{
    
    dimensions[0]=MinRe;
    dimensions[1]=MaxRe;
    dimensions[2]=MinIm;
    dimensions[3]=MaxIm;
    dimensions[4]=xval;
    dimensions[5]=yval;
    glClear(GL_COLOR_BUFFER_BIT); // white` 

    int e, w, j, k, y;
    numCol=0;
    int count;
    for(w=1; w<size; w++)
    {
        MPI_Send( &numCol, 1 , MPI_INT , w, tag , MPI_COMM_WORLD ) ;
        MPI_Send( dimensions, 6 , MPI_DOUBLE , w, tag , MPI_COMM_WORLD ) ;
        numCol+=1;
    }
    printf("loop entered %d times\n", loop);
    while(numCol<N)
    {
        MPI_Recv( steps , N , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
        s1 = status.MPI_SOURCE ; 
        for(e=0; e<N; e++)
        {
            count=steps[e];
            if(count==max_iterations) //potential problem spott because manipulating x and y in the plot part and its needed here (made it global though)
                {
                       glColor3f( 0.0 , 0.0 , 0.0 ) ; 
                }
            else
                {
                      double tmp=1.0*count;
                      glColor3f(0.3*tmp, 0.5*tmp, 0.2*tmp);
                }
                       glBegin(GL_POINTS);
                       glVertex2f(e,numCol);
                       glEnd();
        }
       
       MPI_Send( &numCol, 1 , MPI_INT , s1 , tag , MPI_COMM_WORLD ) ;
       MPI_Send( dimensions, 6 , MPI_DOUBLE , s1, tag , MPI_COMM_WORLD ) ;
       numCol+=1;
    }
    glutSwapBuffers();
     kill=-1;
    for(k=1; k<size; k++ )
    {
        MPI_Recv( steps , N , MPI_INT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
        s2 = status.MPI_SOURCE ; 
        for(y=0; y<N; y++)
        {
            count=steps[y];
            if(count==max_iterations) //potential problem spott because manipulating x and y in the plot part and its needed here (made it global though)
                {
                       glColor3f( 0.0 , 0.0 , 0.0 ) ; 
                }
            else
                {
                      double tmp=1.0*count;
                      glColor3f(0.3*tmp, 0.5*tmp, 0.2*tmp);
                }
                       glBegin(GL_POINTS);
                       glVertex2f(y, numCol);
                       glEnd();
        }
        glutSwapBuffers();
        
	numCol=-1;
	
       MPI_Send( &numCol , 1 , MPI_INT, s2 , tag , MPI_COMM_WORLD ) ;
       MPI_Send( dimensions, 6 , MPI_DOUBLE , s2, tag , MPI_COMM_WORLD ) ;
       //printf("%d", numCol);
       
       
    }
     glutSwapBuffers();
    loop+=1;
    //glutSwapBuffers();
    printf("loop successfully exited\n");

}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
       yscr = N-yscr;
      if(state==GLUT_DOWN)
      {
         
         xval= MinRe + xscr*(MaxRe-MinRe)/(N); //the a part of a+bi 
         yval=MaxIm - yscr*(MaxIm-MinIm)/(N);
         double Re_factor = (MaxRe-MinRe)/4;
		double Im_factor = (MaxIm-MinIm)/4;
		 MinRe = xval - (Re_factor);
		printf(" MinRe:%f\n", MinRe);
		MaxRe = xval + (Re_factor);
		printf(" MaxRe:%f\n", MaxRe);
		MinIm = yval-(Im_factor); //y part of thing y over
		printf(" MinIm:%f\n", MinIm);
		MaxIm = yval+ (Im_factor); //y to right
		printf(" MaxIm:%f\n", MaxIm);
         
                glutPostRedisplay();
	 }
   }
   else if(button==GLUT_RIGHT_BUTTON)
   {
      yscr = N-yscr;
      if(state==GLUT_DOWN)
      {
         ///recenter and then shrink or increase
         
         
         xval= MinRe + (MaxRe-MinRe)/(N)/xscr; //the a part of a+bi 
         yval=MaxIm - (MaxIm-MinIm)/(N)/yscr;
         double Re_factor = (MaxRe-MinRe)*4; //might be divided by instead
		double Im_factor = (MaxIm-MinIm)*4;
		MinRe = xval - (Re_factor);
		printf(" MinRe:%f\n", MinRe);
		MaxRe = xval + (Re_factor);
		printf(" MaxRe:%f\n", MaxRe);
		MinIm = yval-(Im_factor); //y part of thing y over
		printf(" MinIm:%f\n", MinIm);
		MaxIm = yval+(Im_factor); //y to right
		printf(" MaxIm:%f\n", MaxIm);
         glutPostRedisplay();
         
      }
   }
}

int main( int argc , char* argv[] ) //each worker gets a column of pixels and returns a column of steps
{
	
   //tic=gettime();
   MPI_Init(&argc, &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
   if( rank == 0 ) //manager
   {
	   glutInit(&argc,argv);	   
	   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	   glutInitWindowSize(N,N);
	   glutInitWindowPosition(100,50);
	   glutCreateWindow("");
           glClearColor(1.0,1.0,1.0,0.0);
	   glShadeModel(GL_SMOOTH);
	   
	   glutReshapeFunc(reshapefunc);
	   glutDisplayFunc(displayfunc);
	   glutMouseFunc(mousefunc);
           glutIdleFunc(NULL);
	   glutMainLoop();
	   
    }
    else
        {
                 //printf("a: %d\n", a);
                 float rReal, cReal, rComp, cComp;
                 
            while(1)
                   {
                        
                        
			MPI_Recv( &numCol , 1 , MPI_INT , MPI_ANY_SOURCE  , tag , MPI_COMM_WORLD , &status ) ; 
                        MPI_Recv( dimensions , 6 , MPI_DOUBLE , MPI_ANY_SOURCE  , tag , MPI_COMM_WORLD , &status ) ; 
                        MinRe=dimensions[0];
                        MaxRe=dimensions[1];
                        MinIm=dimensions[2];
                        MaxIm=dimensions[3];
                        xval=dimensions[4];
                        yval=dimensions[5];
			if(numCol==-1)
                        {
                            printf("reached");
			    break;
                        }
                        int rx, ry, t, i;
                        float x0;
                        float y0;
                        float x, y,xtemp;
                        y0=MaxIm - numCol*(MaxIm-MinIm)/(ImageHeight) +yval;
                        for(rx=0; rx<N; rx++)
                        {
                             x0= MinRe + rx*(MaxRe-MinRe)/(ImageWidth)+xval; //the a part of a+bi 
                             x=0.0;
                             y=0.0;
                             i=0;
                             xtemp=0.0;
                             while(x*x + y*y<2*2 && i<max_iterations)
                             {
                                xtemp= x*x -y*y +x0;
                                y=2*x*y +y0;
                                x=xtemp;
                                i=i+1;
                             }
                                 steps[rx]=i;
                        }
                        MPI_Send( steps , N , MPI_INT, 0 , tag , MPI_COMM_WORLD) ;
                   }
      }
     MPI_Finalize() ; 
     return 0;
}
   
   





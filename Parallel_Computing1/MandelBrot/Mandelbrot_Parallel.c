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
int pause = 0 ;
int max_iterations=100; 
//int w=600;
//int h=600;
int N=600;

int s1, s2;
float colorSteps[1800];
float coords[2400]; //real coords and then complex coords
 int steps;
  
   int  size;
   MPI_Status status;
    int  rank;

   int  tag  =0;

void keyfunc(unsigned char key,int xscr,int yscr)
{
	if(key=='q')
	{
		MPI_Finalize();
		exit(0);
	}
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)N, (GLsizei)N); //this should be the same as Ortho2D in the future!!!
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*N,0.0,1.0*N); // always a square
   glMatrixMode(GL_MODELVIEW);
}
int doWork(float rReal, float cReal, float rComp, float cComp)
{
	float x=0.0; //use in finding color and solving color
	float y=0.0;
        int i=0;
         float xtemp;
		//printf("rReal: %f cReal: %f, rComp: %f cComp: %f\n", rReal, cReal, rComp, cComp);
         while(x*x + y*y<2*2 && i<max_iterations)
         {
             //printf("Reached\n");
             
            xtemp= x*x -y*y +rComp;
            y=2*x*y +cComp;
            x=xtemp;
            i=i+1;
            
            
         }
         printf("i: %d\n", i);
		//printf("i: %d\n", i);
         return i;
}
void displayfunc()
{
	glClear(GL_COLOR_BUFFER_BIT); // white`  
	glutSwapBuffers();
	   float x0;
	   int rx;
	   int ry;
	   float y0;
	   int j;
	   int k;
	   int e, w;
	   int numCol;
	   float xtemp;
	   int ImageHeight=N-1;
	   int ImageWidth=N-1;

    
    
    for(w=1; w<size; w++) //first three workers starts at 1 while numCol will start at 0
            {
                    
                printf("col num:%d\n", (w-1));
                int index=0;
                y0=MaxIm - (w-1)*(MaxIm-MinIm)/(ImageHeight); //point on complex plane bi
                printf("y0:%f", y0);
                    for(rx=0; rx<N; rx++)
                            {
                                    //printf("rx:%d", rx);
                                    coords[index]=rx; //send both real and imaginary parts over
                                    index++;
                                    //printf("ry:%d", numCol);
                                    coords[index]=(w-1);
                                    index++;
                                    x0= MinRe + rx*(MaxRe-MinRe)/(ImageWidth); //the a part of a+bi 
                                    //printf("x0:%f", x0);
                                    coords[index]=x0;
                                    index++;

                                    coords[index]=y0;
                                    index++;

                            }
                    printf("Sending Col %d to worker %d\n", w-1, w);
                    //numCol++; 
                    MPI_Send( coords, 4*N , MPI_FLOAT , w, tag , MPI_COMM_WORLD ) ;
                    //MPI_Finalize() ; 





        }
        
       numCol=w-1;
   
      
      while(numCol<N) 
      {
          //printf("Reached\n");
          int index=0;
          MPI_Recv( colorSteps , N*3 , MPI_FLOAT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ; //getting coordinate and step
          s1 = status.MPI_SOURCE ; 
         
          for(j=0; j<N*3; j=j+3) //going to every third to get the next coord
			{
                            rx=colorSteps[j];
                            ry=colorSteps[j+1];
                            steps=colorSteps[j+2];
                            if(steps==max_iterations) //potential problem spott because manipulating x and y in the plot part and its needed here (made it global though)
                              {
                                     glColor3f( 0.0 , 0.0 , 0.0 ) ; 
                              }
                              else
                              {
                                    double tmp=1.0*steps;
                                    glColor3f(0.3*tmp, 0.5*tmp, 0.2*tmp);
                              }
                                     glBegin(GL_POINTS);
                                     glVertex2f(rx,ry);
                                     glEnd();
             }
             glutSwapBuffers();
                y0=MaxIm - numCol*(MaxIm-MinIm)/(ImageHeight); //point on complex plane bi
                for(e=0; e<N; e++) 
                            {
								coords[index]=e; //send both real and imaginary parts over
								index++;
								coords[index]=numCol; //ry is the same as numcol
								index++;
								x0= MinRe + e*(MaxRe-MinRe)/(ImageWidth); //the a part of a+bi 
								coords[index]=x0;
								index++;
								coords[index]=y0;
								index++;
                            }
                  //numCol++;
			
          MPI_Send( coords , 4*N , MPI_FLOAT , s1 , tag , MPI_COMM_WORLD ) ;
          numCol++;
         
      }
      for( k = 1 ; k < size; k++ ) //for every worker
      {
          MPI_Recv( colorSteps , N*3 , MPI_FLOAT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
          s2 = status.MPI_SOURCE ;
          for(j=0; j<N*3; j=j+3) //plotting the row
			{
                            rx=colorSteps[j];
                            ry=colorSteps[j+1];
                            steps=colorSteps[j+2];
                            if(steps=max_iterations) 
                              {
                                     glColor3f( 0.0 , 0.0 , 0.0 ) ; 
                              }
                              else
                              {
                                double tmp=1.0*steps;
                                    glColor3f(0.3*tmp, 0.5*tmp, 0.2*tmp);
                              }
                                     glBegin(GL_POINTS);
                                     glVertex2f(rx,ry);
                                     glEnd();
            }
            glutSwapBuffers();
          coords[0]=-3000000000;
          MPI_Send( coords , 4*N , MPI_FLOAT, s2 , tag , MPI_COMM_WORLD ) ;
      }
}
int main( int argc , char* argv[] ) //each worker gets a column of pixels and returns a column of steps
{

	
   //tic=gettime();
   MPI_Init(&argc, &argv ) ;
   MPI_Comm_size( MPI_COMM_WORLD , &size ) ; // same
   MPI_Comm_rank( MPI_COMM_WORLD , &rank ) ; // different
  int numCol = 0;
   
   
   if( rank == 0 ) //manager
   {
       
	   
	   glutInit(&argc,argv);
	   
	   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	   glutInitWindowSize(N,N);
	   glutInitWindowPosition(100,50);
	   glutCreateWindow("");
	   glutCreateWindow("");
           glClearColor(1.0,1.0,1.0,0.0);
	   glShadeModel(GL_SMOOTH);
	   
	   glutReshapeFunc(reshapefunc);
	   glutDisplayFunc(displayfunc);
           glutIdleFunc(NULL);
           glutKeyboardFunc(keyfunc);
           
	   //glutPostRedisplay(); //possible problem spot
	    
	   glutMainLoop();
	   
    }
    else
        {
			
                 
                 int a=0;
                 printf("a: %d\n", a);
                 int z=0;

                 float rReal, cReal, rComp, cComp;
            while(1)
                   {
						 if(coords[0]<-300000000)  
						 {
							 break;
						 }
							MPI_Recv( coords , 4*N , MPI_FLOAT, 0 , tag , MPI_COMM_WORLD , &status ); 
							
							   /*pseudocode
									* receive coordinates
									* traverse through the coordinates
									* take each and do the "step function" 
									* add the steps to an array 
									* and send back
									* if the array = blah, break
							*/
					
					while(a<4*N) 
							{
									//printf("a: %d\n", a);
									//printf("Reached\n");
									rReal=coords[a];
									a++;
									cReal=coords[a];
									a++;
									rComp=coords[a]; //the row val and the col val is the next cell
									a++;
									cComp=coords[a];
									a++;
									steps = doWork(rReal, cReal, rComp, cComp); //color steps is formatted //STEPS IS RETURNING ZERO
									//printf("i: %d\n", i);
									printf("steps: %d\n", z);
									colorSteps[z]=rReal;
									z++;
									colorSteps[z]=cReal;
									z++;
									colorSteps[z]=steps;
									z++;
									//printf("z: %d\n", z);
									
							}

							MPI_Send( colorSteps , N*3 , MPI_FLOAT, 0 , tag , MPI_COMM_WORLD) ;

              }
      }
     MPI_Finalize() ; 
     return 0;
}
   
   




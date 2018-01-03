#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 
#include <GL/glut.h>
#include <math.h>
double MinRe = -2.0; //window zoom vals
double MaxRe = 2.0;
double MinIm = -1.5;
double MaxIm = 1.5;
int    pause = 0   ; 
int w=600;
int h=600;
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

void displayfunc() 
{
   glClear(GL_COLOR_BUFFER_BIT); // white
   float x=0.0;
   float y=0.0;
   float x0;
   int rx;
   int ry;
   float y0;
   int i;
   int max_iterations=100;
   float xtemp;
   int ImageHeight=h-1;
   int ImageWidth=w-1;
   
   for(ry=0; ry<h; ry++) //pixel on frame
   {
	 //printf("%d ry:", ry);
      y0=MaxIm - ry*(MaxIm-MinIm)/(ImageHeight); //point on complex plane bi
      
      //printf(" y0:%f\n", y0);
       for(rx=0; rx<w; rx++) //want to plot stuff in real, width is the real frame
      { 
         //printf("%d rx:", rx);
         x0= MinRe + rx*(MaxRe-MinRe)/(ImageWidth); //the a part of a+bi 
         
         //printf("x0:%f\n", x0);
         x=0.0;
         y=0.0;
         i=0;
         while(x*x + y*y<2*2 && i<max_iterations)
         {
            xtemp= x*x -y*y +x0;
            y=2*x*y +y0;
            x=xtemp;
            i=i+1;
         }
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
    
   }

   glutSwapBuffers(); 
   //glFlush();// single buffering... call glFlush();
}
void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*w,0.0,1.0*h); // always a square
   glMatrixMode(GL_MODELVIEW);
}



void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         //recenter and then shrink or increase
         double x0;
         double y0;
         //glColor3f(1.0, 0.0, 0.0);
         //glVertex2f(xscr,yscr);
         //sleep(10);
         x0= MinRe + xscr*(MaxRe-MinRe)/(w); //the a part of a+bi 
         y0=MaxIm - yscr*(MaxIm-MinIm)/(h);
         double Re_factor = (MaxRe-MinRe)/2;
		double Im_factor = (MaxIm-MinIm)/2;
		 MinRe = x0 - (Re_factor*0.5);
		printf(" MinRe:%f\n", MinRe);
		MaxRe = x0 + (Re_factor*0.5);
		printf(" MaxRe:%f\n", MaxRe);
		MinIm = y0-(Im_factor*0.5); //y part of thing y over
		printf(" MinIm:%f\n", MinIm);
		MaxIm = y0+ (Im_factor*0.5); //y to right
		printf(" MaxIm:%f\n", MaxIm);
         glutPostRedisplay();
         
         //displayfunc();
         //glutPostRedisplay();
	 }
   }
   else if(button==GLUT_RIGHT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         ///recenter and then shrink or increase
         double x0;
         double y0;
         x0= MinRe + (MaxRe-MinRe)/(w)/xscr; //the a part of a+bi 
         y0=MaxIm - (MaxIm-MinIm)/(h)/yscr;
         double Re_factor = (MaxRe-MinRe)*2; //might be divided by instead
		double Im_factor = (MaxIm-MinIm)*2;
		MinRe = x0 - (Re_factor/0.5);
		printf(" MinRe:%f\n", MinRe);
		MaxRe = x0 + (Re_factor/0.5);
		printf(" MaxRe:%f\n", MaxRe);
		MinIm = y0-(Im_factor/0.5); //y part of thing y over
		printf(" MinIm:%f\n", MinIm);
		MaxIm = y0+(Im_factor/0.5); //y to right
		printf(" MaxIm:%f\n", MaxIm);
         glutPostRedisplay();
         
         //displayfunc();
         //glutPostRedisplay();
         
      }
   }
}

void keyfunc(unsigned char key,int xscr,int yscr)
{
   if( key == ' ' )
   {
      pause = !pause ;
   }
   else if( key == 'q' )
   {
      exit( 0 ) ;
   }
}


int main(int argc,char* argv[])
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
   glutMouseFunc(mousefunc);
   glutKeyboardFunc(keyfunc);
   //
   glutMainLoop();
   //
   return 0;
}
// 
// end of file
//

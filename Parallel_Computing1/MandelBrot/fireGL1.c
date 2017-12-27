
// 
// to compile type... make 
// uses file named... Makefile
// 
// a.out: fireGL.c
//     gcc -lGL -lGLU -lglut fireGL.c
// 
// tab character '\t' before gcc
// 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// 
#include <GL/glut.h>
// 
#define N     600
#define TREE    1 
#define FIRE    2 
#define SPARK   3 
#define BURNT   8 
#define EMPTY   0 
// 
char   t[N][N]     ; 
int    pause = 0   ; 
int    step        ; 
double prob = 0.60 ; 
// 


void drawString( char* s ) //from original material
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
/*
   GLUT_BITMAP_8_BY_13
   GLUT_BITMAP_9_BY_15
   GLUT_BITMAP_TIMES_ROMAN_10
   GLUT_BITMAP_TIMES_ROMAN_24
   GLUT_BITMAP_HELVETICA_10
   GLUT_BITMAP_HELVETICA_12
   GLUT_BITMAP_HELVETICA_18
*/
void conv( int n , char* str )
{
   int pow10 ;
   int j = 0 ;
   //
   if( n < 0 )
   {
      n = ( -n ) ; // possible bug at max negative int
      //
      str[0] = '-' ;
      //
      j = 1 ;
   }
   //
   pow10 = 1 ;
   while( pow10 <= n/10 ) pow10 *= 10 ;
   //
   while( pow10 > 0 )
   {
      str[j] = '0' + ( n / pow10 ) ;
      //
      n %= pow10 ;
      //
      ++j ;
      //
      pow10 /= 10 ;
   }
   //
   str[j] = '\0' ;
}
void displayfunc()
{
   int    x  , y, count,iter, 
   double z, prevz, real;
   //
   double x1 , y1 ;
   double x2 , y2 ;
   double x3 , y3 ;
   double pr, pi;
   double newRe, newIm, oldRe, oldIm;   //real and imaginary parts of new and old z
    double zoom = 1, moveX = -0.5, moveY = 0; //you can change these to zoom and change position
    ColorRGB color; //the RGB color value for the pixel
    int maxIterations = 256;//after how much iterations the function should stop

   //
   char stepstr[20]; // overkill... 20 digits
   //
   glClear(GL_COLOR_BUFFER_BIT); // white
   //
   for( x = 0 ; x < N ; x++ )
   {
      for( y = 0 ; y < N ; y++ )
      { 
        
          //NEW CODE
          pr = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
         pi = (y - h / 2) / (0.5 * zoom * h) + moveY;
        newRe = newIm = oldRe = oldIm = 0; //these should start at 0,0
        int i;
         for(i=0; i<maxIterations; i++)
            {
                //remember value of previous iteration
                oldRe = newRe;
                oldIm = newIm;
                //the actual iteration, the real and imaginary part are calculated
                newRe = oldRe * oldRe - oldIm * oldIm + pr;
                newIm = 2 * oldRe * oldIm + pi;
                //if the point is outside the circle with radius 2: stop
                if((newRe * newRe + newIm * newIm) > 4)
                    break;
            }

                    color = HSVtoRGB(ColorHSV(i, 255, 255 * (i < maxIterations)));
                    //draw the pixel

}
         //OLD CODE
         /*
         if( t[y][x] == TREE ) 
         {
            glColor3f( 0.0 , 1.0 , 0.0 ) ; // green
         }
         else if( t[y][x] == FIRE )
         {
            glColor3f( 1.0 , 0.0 , 0.0 ) ; // red
         }
         else if( t[y][x] == BURNT )
         {
            glColor3f( 0.0 , 0.0 , 0.0 ) ; // black
         }
         else  // t[y][x] == EMPTY
         {
            glColor3f( 0.6 , 0.3 , 0.0 ) ; // brown
         }
         * */
         //
         glBegin(GL_POINTS);
         glVertex2f(x,y);
         glEnd();
      }
   }
   //
   glColor3f( 0.0 , 0.0 , 0.0 ) ; // black
   x1 = 0.825*N ; y1 = 0.875*N ;
   x2 = 0.950*N ; y2 = 0.875*N ;
   x3 = 0.950*N ; y3 = 0.950*N ;
   glBegin(GL_TRIANGLES);
   glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
   glEnd();
   x1 = 0.825*N ; y1 = 0.875*N ;
   x2 = 0.825*N ; y2 = 0.950*N ;
   x3 = 0.950*N ; y3 = 0.950*N ;
   glBegin(GL_TRIANGLES);
   glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3);
   glEnd();
   //
   glColor3f( 1.0 , 1.0 , 1.0 ) ; // white
   glRasterPos2f(0.85*N,0.9*N);
   conv( step , stepstr ) ;
   drawString(stepstr);
   //
   glutSwapBuffers(); // single buffering... call glFlush();
}
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;
	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}
	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );
	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}

void reshapefunc(int wscr,int hscr)
{
   glViewport(0,0,(GLsizei)N,(GLsizei)N);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,1.0*N,0.0,1.0*N); // always a square
   glMatrixMode(GL_MODELVIEW);
}
void mousefunc(int button,int state,int xscr,int yscr)
{
   if(button==GLUT_LEFT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         init( prob ) ;
         fire( ) ;
      }
   }
   else if(button==GLUT_RIGHT_BUTTON)
   {
      if(state==GLUT_DOWN)
      {
         srand( time(NULL) ) ;
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
   glutInitWindowSize(N,N);
   glutInitWindowPosition(100,50);
   glutCreateWindow("");
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   srand( 12345 ) ; // arbitrary random seed
   //
   init( prob ) ;
   fire( ) ;
   //
   glutIdleFunc(idlefunc);
   glutDisplayFunc(displayfunc);
   glutReshapeFunc(reshapefunc);
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
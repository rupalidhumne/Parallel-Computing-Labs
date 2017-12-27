
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
//
double pi=3.1415926;
//
int    w=640,h=480;
double    G = (6.67e-5);
//
double rho,phi,theta;
double up=1.0;
//
double xc,yc,zc;
double xe,ye,ze;
//
int xtemp, ytemp;
//
typedef struct Body
{
  double x, y, z;
  double m;
  double vx, vy, vz;
  double q;
  double ax, ay, az;
  double fx, fy, fz;
} Body;
Body body[2];
//
double force(Body b1, Body b2)
{
  double force;
  double xx = (b1.x-b2.x)*(b1.x-b2.x);
  double yy = (b1.y - b2.y)*(b1.y - b2.y);
  double zz = (b1.z - b2.z)*(b1.z - b2.z);
  force = G*(b1.m)*(b2.m)/((xx + yy + zz)*sqrt((xx + yy + zz)));
  return force;
}
void calcForce(Body body[])
{
  double f[2][2];
  int i, j;
  //Get f-overall 
  for(i = 0; i < 2; ++i)
  {
    for(j = 0; j < 2; ++j)
    {
      if(i == j)
      {
	f[i][j] = 0;
      }
      else
      {
	f[i][j] = force(body[i], body[j]);
      }
    }
  }
  //Split f into fx, fy, fz
  for(i = 0; i < 2; ++i)
  {
    body[i].fx = body[i].fy = body[i].fz = 0;
    for(j = 0; j < 2; ++j)
    {
      if(i != j)
      {
	double totalf = f[i][j];
	body[i].fx += -1* (body[i].x - body[j].x) * totalf; 
	body[i].fy += -1* (body[i].y - body[j].y) * totalf;
	body[i].fz += -1* (body[i].z - body[j].z) * totalf;
      }
    }
  }
  //Calculate movements components
  for(i = 0; i < 2; ++i)
  {
    //x
    body[i].ax = body[i].fx/body[i].m;
    body[i].vx += body[i].ax;
    body[i].x += body[i].vx;
    //y
    body[i].ay = body[i].fy/body[i].m;
    body[i].vy += body[i].ay;
    body[i].y += body[i].vy;
    //z
    body[i].az = body[i].fz/body[i].m;
    body[i].vz += body[i].az;
    body[i].z += body[i].vz;
  }
  
  
}
//
Body addBody(double x, double y, double z, double m)
{
  Body temp;
  temp.x = x;
  temp.y = y;
  temp.z = z;
  temp.m = m;
  temp.vx = temp.vy = temp.vz = 0;
  temp.ax = temp.ay = temp.az = 0;
  temp.q = 0;
  return temp;
}
void display(void)
{
   double t, size;
   int i;
   //
   glClear(GL_COLOR_BUFFER_BIT); // clear the screen
   glColor3f(0.0,0.0,0.0);
   //
   for(i = 0; i < 2; ++i)
   {
   glTranslated(body[i].x, body[i].y, body[i].z);
   
   if(body[i].m > 50)
   {
     size = 5;
   }
   else
   {
     size = 1;
   }
   glutWireSphere(size, 10, 10);
   glTranslated(-body[i].x, -body[i].y, -body[i].z);
   }
   //
   glutSwapBuffers();
}
void look()
{
   xe=xc+rho*sin(theta)*sin(phi); // y
   ye=yc+rho*cos(theta);          // z
   ze=zc+rho*sin(theta)*cos(phi); // x
   //
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(xe,ye,ze, xc,yc,zc, 0.0,up,0.0);
}
void idle(void)
{
   //phi+=0.01;
   //
   //usleep(100000000000000);
   calcForce(body);
   look();
   glutPostRedisplay();
}
void mouse(int button,int state,int xscr,int yscr)
{
  if(button == GLUT_LEFT_BUTTON)
    if(state == GLUT_DOWN)
    {
      xtemp = xscr;
      ytemp = yscr;
    }
  if(button==3)
  {
    rho -= 0.5;
  }
  else if(button==4)
  {
    rho += 0.5;
  }
}
void motion(int xscr,int yscr)
{
  int dx = xscr - xtemp;
  int dy = yscr - ytemp;
  xtemp = xscr;
  ytemp = yscr;
  if(dx > 0)
  {
    phi += 0.03;
  }
  else if(dx < 0)
  {
    phi -= 0.03;
  }
  if (dy > 0)
  {
    theta += 0.03;
  }
  else if(dy < 0)
  {
    theta -= 0.03;
  }
}
void mouse_wheel(int wheel,int direction,int xscr,int yscr)
{
}
void keyfunc(unsigned char key,int xscr,int yscr)
{
   if(key=='q')
   {
      exit(0);
   }
}
void reshape(int wscr,int hscr)
{
   GLfloat aspect_ratio;
   //
   w=wscr;
   h=hscr;
   aspect_ratio=(GLfloat)w/(GLfloat)h;
   glViewport(0,0,(GLsizei)w,(GLsizei)h);
   //
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0,aspect_ratio,0.1,500.0);
   //
   look();
}
int main(int argc,char* argv[])
{  
   rho   = 25;
   phi   = 0.0;
   theta = 0.5*pi;
   body[0] = addBody(0, 0, 0, 100);
  // body[1] = addBody(10, 10, 0, 1);
   body[1] = addBody(10, 0, 0, 1);
   double xx = (body[0].x-body[1].x)*(body[0].x-body[1].x);
  double yy = (body[0].y-body[1].y)*(body[0].y - body[1].y);
  double zz = (body[0].z-body[1].z)*(body[0].z-body[1].z);
   body[1].vy = sqrt(G*body[0].m/(zz + xx + yy));
   //
   xc=yc=zc=0.0;
   //
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(w,h);
   glutInitWindowPosition(100,50);
   glutCreateWindow("N-Body Problem");
   //
   glClearColor(1.0,1.0,1.0,0.0);
   glShadeModel(GL_SMOOTH);
   //
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutMouseWheelFunc(mouse_wheel);
   glutKeyboardFunc(keyfunc);
   glutReshapeFunc(reshape);
   //
   glutMainLoop();
   //
   return 0;
}
//
// end of file
//

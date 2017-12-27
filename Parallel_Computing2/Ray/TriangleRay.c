#include <stdio.h>
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

double spheres[2][7] = { {1.00,0.5,1.00,0.25,0,255,0},{0,0.75,1.25,0.5,255,0,0}};

double ground[7]={0.50,-20000.00,0.5,20000.25,205,133,63};
double pyramid[4][3][3]={
    {{0.500000 0.666667 0.166667},{0.602062 0.416667 0.268729}, {0.360580 0.416667 0.204024}},
    {{0.500000 0.666667 0.166667}, {0.360580 0.416667 0.204024}, {0.537357 0.416667 0.027247}},
    {{0.500000 0.666667 0.166667}, {0.537357 0.416667 0.027247}, {0.602062 0.416667 0.268729}},
    {{0.602062 0.416667 0.268729}, {0.537357 0.416667 0.027247}, {0.360580 0.416667 0.204024}}};
//4 triangles, each triangle has three points
double pyramidColor[3]={0, 0, 255};
//turn a triangle into a plane by multiplying by its normal vector
double* findABCSphere(double* abc, double rx, double ry, double rz, double centerx, double centery, double centerz, double r, double tx, double ty, double tz)
{
    double a, b, c;
    a=1;
    b = (2*tx*rx)+(2*ty*ry)+(2*tz*rz)-(2*rx*centerx)-(2*ry*centery)-(2*rz*centerz);
    c=(tx*tx)-(2*tx*centerx)+(centerx*centerx)+(ty*ty)-(2*ty*centery)+(centery*centery)+(tz*tz)-(2*tz*centerz)+(centerz*centerz)-(r*r);
    abc[0]=a;
    abc[1]=b; 
    abc[2]=c;
    return abc;
}
double calcTSphere(double disc, double a, double b, double c)
{
    double Tval;
    if(disc>0.0)
        {
                Tval=((-1*b)-sqrt(disc))/(2*a);
                //T= printf("T %f\n", T);

        }
    else if(disc<0.0)
        {
                Tval=-1.0;
        }
    return T;
}
int main(void)
{
    FILE* fout ;
    int sphere ;

   int xs, ys, zs; //when dealing with the sphere
   double centerx, centery, centerz,r;
   double px, py, pz;
    double distances[4];  //either you or the one below you are going out of bounds
  double distancesBack[4];
  double shape;
  int minIndex;
	double rx, ry, rz, rmag, startx, starty, startz, rlx, rly, rlz, rlmag, a, b, c;
        
    for( y = 0 ; y < N ; y++ ) //window
   {
      for( x = 0 ; x < M ; x++) //window
      {	 
          
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
        for(shape=0; shape<3; shape++) //distance per each sphere
			{
                            if(shape<2)
                            {
                                double T;
                                double abc[3]; //new array for each sphere
                                centerx=spheres[sphere][0];
                                centery=spheres[sphere][1]; 
                                centerz=spheres[sphere][2];
                                r=spheres[sphere][3];

                                findABCSphere(abc, rx, ry, rz, centerx, centery, centerz, r, ex, ey, ez);

                                a = abc[0];
                                b = abc[1];
                                c=abc[2];

                                double disc= (b*b)-(4*a*c);

                                T= calcTSphere(disc, a, b, c);
                                distances[shape]=T;
                                
                            }
                            else //try hitting the pyramid and all four planes
                            {
                                int plane;
                                for(int plane =0; plane<4; plane++)
                                {
                                    //take the dot product with teh shit
                                    //then turn the shit into the shit
                                }
                            }
                        }
      
        
      }
}
}



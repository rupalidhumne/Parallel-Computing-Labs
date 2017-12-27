#include <stdio.h>
#include <math.h>
//
#define WIDTH 640
#define HIGHT 480
#define true 1
#define false 0
typedef struct Vector
{
    double a;
    double b;
    double c;
} Vector;

typedef struct Point
{
    double a;
    double b;
    double c;
} Point;
//
Point eye;

Point light;


Point floorP;


// double ex = 0.5;
// double ey = 0.5;
// double ez = -1.0;
// double lx=0.000000;
// double ly=1.000000;
// double lz=-0.500000;
int sphereCount = 3;
//double floorY = 0.3333333;
double spheres[3][7] = {{0.500000,0.500000,0.166667,0.166667,0,0,255},
        {0.833333,0.500000,0.500000,0.166667,0,255,0},
{0.333333,0.666667,0.666667,0.333333,255,0,0}};
int floorC[3] = {255,205,0};
    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    // double* abcGen(double cx, double cy, double cz, double R, Vector* r, double ox, double oy, double oz, double* array)
    // {
    //     double a = 1;
    //     double b = -2*cx*r->a -2*cy*r->b - 2*cz*r->c +2*ox*r->a +2*oy*r->b + 2*oz*r->c;
    //     double c = cx*cx - 2*cx*ox + ox*ox + cy*cy - 2*cy*oy + oy*oy + cz*cz - 2*cz*oz + oz*oz - R*R;
    //     array[0] = a;
    //     array[1] = b;
    //     array[2] = c;
    //     return array;
    // }
    double* abcGen(Point* center,  double R, Vector* r, Point* origin, double* array)
    {
        double a = 1;
        double b = -2*center->a*r->a -2*center->b*r->b - 2*center->c*r->c +2*origin->a*r->a +2*origin->b*r->b + 2*origin->c*r->c;
        double c = center->a*center->a - 2*center->a*origin->a + origin->a*origin->a + center->b*center->b - 2*center->b*origin->b + origin->b*origin->b + center->c*center->c - 2*center->c*origin->c + origin->c*origin->c - R*R;
        array[0] = a;
        array[1] = b;
        array[2] = c;
        return array;
    }
    double tQuadValue(double* array)
    {

        double a = array[0];
        double b = array[1];
        double c = array[2];
        double discrim = b*b - 4*a*c;
        if (discrim < 0)
        {
            return -1;
        }
        else if (discrim > 0)
        {
            double solution = (-b - sqrt(discrim))/(2*a);
            return solution;
        }
    }
    double* minFinder(double* array, double* result)
    {
        double currMinTVal = -100;
        double minSphere = -1;
        int sphereNum = 0;
        while(sphereNum<sphereCount)
        {
            double temp = array[sphereNum];
            if (temp < 0)
            {
                sphereNum = sphereNum + 1;
            }
            else
            {
                if (currMinTVal == -100 && minSphere == -1)
                {
                    currMinTVal = temp;
                    minSphere = sphereNum;
                }
                else
                {
                    if (temp < currMinTVal)
                    {
                        currMinTVal = temp;
                        minSphere = sphereNum;
                    }
                }
                sphereNum ++;
            }
        }
        //double result[2];
        result[0] = currMinTVal;
        result[1] = minSphere;
        return result;
    }
    double tLinearValue(Vector* r, double ey, double floorY)
    {
        double minTFloor = (floorY-ey)/r->b;
        return minTFloor;
    }
    double dot(Vector* vec1, Vector* vec2)
    {
        return (vec1->a*vec2->a) + (vec1->b*vec2->b) + (vec1->c*vec2->c);
    }
    double normalizeVector(Vector* v)
    {
        double mag = sqrt(v->a*v->a + v->b*v->b + v->c*v->c);
        v->a = v->a/mag;
        v->b = v->b/mag;
        v->c = v->c/mag;
    }
    //void findColor(Point* start, Vector* dir, int* color)
    //{
        //return 0;
    //}
    int main(void)
    {
        eye.a = 0.5;
        eye.b = 0.5;
        eye.c = -1.0;

        light.a = 0.0;
        light.b = 1.0;
        light.c = -0.5;

        floorP.a = 0.0;
        floorP.b = 0.333333;
        floorP.c = 0.0;
        int rgb[HIGHT][WIDTH][3] ;
        int y , x ;
        FILE* fout ;
        for( y = 0 ; y < HIGHT ; y++ )
        {
            for( x = 0 ; x < WIDTH ; x++)
            {
                //Scale pizels to coordinates
                Point pixel;
                pixel.a = (x + 0.5)/HIGHT;
                pixel.b = 1- (y +0.5)/HIGHT;
                pixel.c = 0;

                //create eye to pixel ray
                Vector r;
                r.a = pixel.a - eye.a;
                r.b = pixel.b - eye.b;
                r.c = pixel.c - eye.c;

                normalizeVector(&r);
                //Check the spheres the ray hits
                int sphereNum = 0;
                double tValArray[sphereCount];
                int lightIntersect = false;
                while(sphereNum < sphereCount)
                {
                    Point c1;
                    c1.a = spheres[sphereNum][0];
                    c1.b = spheres[sphereNum][1];
                    c1.c = spheres[sphereNum][2];
                    //printf("%f %f %f\n",c1.a, c1.b, c1.c );
                    double R = spheres[sphereNum][3];
                    //Solve for T
                    double abcArr[3];
                    abcGen(&c1,R,&r,&eye, abcArr);
                    double tVal = tQuadValue(abcArr);
                    //Find current point using T
                    tValArray[sphereNum] = tVal;
                    sphereNum ++;
                }
                //Find min T value
                double answer[2];
                minFinder(tValArray, answer);
                double currMinTVal = answer[0];
                if(answer[1]>-0.5) answer[1] += 0.5;
                int minSphere = (int)answer[1];
                //Create light ray from sphere to light
                Vector normal;
                Point current;

                if (minSphere== -1)
                {
                double floorT = tLinearValue(&r, eye.b,  floorP.b );
                current.a = eye.a + (floorT)*r.a;
                current.b = eye.b + (floorT)*r.b;
                current.c = eye.c + (floorT)*r.c;
                // printf("%f %f %f\n",current.a, current.b, current.c );

                normal.a = 0;
                normal.b = 1;
                normal.c = 0;
                normalizeVector(&normal);
                
            }
            else
            {
                current.a = eye.a + (currMinTVal)*r.a;
                current.b = eye.b + (currMinTVal)*r.b;
                current.c = eye.c + (currMinTVal)*r.c;
                normal.a = (current.a - spheres[minSphere][0])/spheres[minSphere][3];
                normal.b = (current.b - spheres[minSphere][1])/spheres[minSphere][3];
                normal.c = (current.c - spheres[minSphere][2])/spheres[minSphere][3];
                normalizeVector(&normal);

                current.a += 0.001*normal.a;
                current.b += 0.001*normal.b;
                current.c += 0.001*normal.c;
                
            }
                Vector lr;
                lr.a = light.a - current.a;
                lr.b = light.b - current.b;
                lr.c = light.c - current.c;
                normalizeVector(&lr);

                int sphereNum2 = 0;
                double tValArray2[sphereCount];
                while(sphereNum2<sphereCount)
                {
                    Point cl;
                    cl.a = spheres[sphereNum2][0];
                    cl.b = spheres[sphereNum2][1];
                    cl.c = spheres[sphereNum2][2];
                    // printf("%f %f %f\n",cl.a, cl.b, cl.c );
                    double Rl = spheres[sphereNum2][3];
                    //Solve for T again, this time it is the length of the ray that hits the light
                    double abcArrLight[3];
                    abcGen(&cl,Rl,&lr,&current, abcArrLight);
                    double tValLight = tQuadValue(abcArrLight);
                    tValArray2[sphereNum2] = tValLight;
                    //if(tValLight!=-1) printf("%f\n",  tValLight );
                    sphereNum2++;
                }
                //If the light intersects any sphere then the point is in shadow
                int w = 0;
                for (w = 0; w < sphereCount; ++w)
                {
                    if (tValArray2[w] > 0 && w != minSphere) //!= -1)
                    {
                        lightIntersect = true;
                        break;
                    }
                }
                if (currMinTVal == -100 && minSphere == -1)
                {
                    if (r.b>=0)
                    {
                        rgb[y][x][0] = 0;
                        rgb[y][x][1] = 0;
                        rgb[y][x][2] = 0;
                    }
                    else
                    {
                        double modValX = fmod(current.a, .5);
                        if (current.a <0)
                        {
                            modValX = -modValX;
                        }
                        double modValZ = fmod(current.c, .5);
                        if (current.c <0)
                        {
                            modValZ = -modValZ;
                        }
                        if(lightIntersect==true)
                        {
                            if ((modValX<.2&&modValZ<.2)||(modValX>=.2&&modValZ>=.2))
                            {
                                rgb[y][x][0] = (int)(floorC[0] * 0.5);
                                rgb[y][x][1] = (int)(floorC[1] * 0.5);
                                rgb[y][x][2] = (int)(floorC[2] * 0.5);
                            }
                            else
                            {
                                rgb[y][x][0] = (int)(floorC[0] * 0.5+50);
                                rgb[y][x][1] = (int)(floorC[1] * 0.5+50);
                                rgb[y][x][2] = (int)(floorC[2] * 0.5+50);
                            }
                           
                        }
                        else
                        {
                            if ((modValX < .2 && modValZ < .2)||(modValX >= .2 && modValZ >= .2))
                            {
                                rgb[y][x][0] = floorC[0];
                                rgb[y][x][1] = floorC[1];
                                rgb[y][x][2] = floorC[2];
                            }
                            else
                            {
                                rgb[y][x][0] = floorC[0]+50;
                                rgb[y][x][1] = floorC[1]+50;
                                rgb[y][x][2] = floorC[2]+50;
                            }
                            
                        }
                    }
                }
                else
                {
                     double shadeDotProduct = dot(&normal, &lr);

                    if(lightIntersect==true)
                    {
                        rgb[y][x][0] = (int)(spheres[minSphere][4]*0.1);
                        rgb[y][x][1] = (int)(spheres[minSphere][5]*0.1);
                        rgb[y][x][2] = (int)(spheres[minSphere][6]*0.1);
                    }
                    else
                    {   
                        if (shadeDotProduct>0)
                        {
                        rgb[y][x][0] = (int)(spheres[minSphere][4] *shadeDotProduct);
                        rgb[y][x][1] = (int)(spheres[minSphere][5] *shadeDotProduct);
                        rgb[y][x][2] = (int)(spheres[minSphere][6] *shadeDotProduct);
                        }
                        // else
                        // {
                        // rgb[y][x][0] = (int)(spheres[minSphere][4] *-shadeDotProduct);
                        // rgb[y][x][1] = (int)(spheres[minSphere][5] *-shadeDotProduct);
                        // rgb[y][x][2] = (int)(spheres[minSphere][6] *-shadeDotProduct);
                        // }
                    }
                }
                
            }
        }
        fout = fopen( "shade.ppm" , "w" ) ;
        fprintf( fout , "P3\n" ) ;
        fprintf( fout , "%d %d\n" , WIDTH , HIGHT ) ;
        fprintf( fout , "255\n" ) ;
        //
        for( y = 0 ; y < HIGHT ; y++ )
        {
            for( x = 0 ; x < WIDTH ; x++)
            {
                if (rgb[y][x][0]>255)
                {
                   rgb[y][x][0]=255;
                }
                if (rgb[y][x][1]>255)
                {
                   rgb[y][x][1]=255;
                }
                if (rgb[y][x][2]>255)
                {
                   rgb[y][x][2]=255;
                }
                fprintf( fout , "%d %d %d\n" ,
                rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
            }
        }
        close( fout ) ;
        //
        return 0 ;
    }

//
// gcc -fopenmp ompDemo.c
//
#include <stdio.h>
#include <omp.h> 
#include <math.h>
int main() //start from column zero everywhere you want to start from 1
{
    int n=8;
    int A[9] = {0,1,1,2,3,5,2,1,2} ;
     int B[4][9] = { 0 } ;
     int C[4][9]={0};
    int i;
    omp_set_num_threads(n) ;
    #pragma omp parallel private(i)
   {
      i = omp_get_thread_num()+1;
       // printf("i: %d\n", i);
      //
      B[0][i] = A[i] ;
      
   }
    
    int h, k;
        //omp_set_num_threads(3);//end value ;//do 
        //#pragma omp parallel private(h)//index
        for(h=1; h<=3; h++) //log(8) base 2 is 3
        {
            printf("h: %d\n", h);
            omp_set_num_threads(n/pow(2,h));//end value ; 
            
            
            #pragma omp parallel private(k)
            {
                
                k = omp_get_thread_num()+1 ;
                printf("k: %d\n", k);
                B[h][k]=B[h-1][2*k-1]+B[h-1][2*k];
                    
            }
            
        }
     int r,c;
        for(r=0; r<4; r++)
        {
            for(c=1; c<9; c++)
            {
                printf("%d", B[r][c]);
                
            }
            printf("\n");
        }
        int l, m;
        for(l=4; l>=0; l--)
        {
            omp_set_num_threads(n/pow(2,l));//end value ; 
            #pragma omp parallel private(m)//index swapped even and odds
            {
                m = omp_get_thread_num()+1 ;
                if(m%2==0) 
                {
                    C[l][m]=C[l+1][m/2];
                    
                }
                else if(m==1)
                {
                    C[l][1]=B[l][1];
                     
                }
                else  
                {
                     C[l][m]=C[l+1][(m-1)/2] + B[l][m];
                }
            }
        }
        int p;
        int x, y;

   
            for(x=1; x<=8; x++)
            {
                printf("C:%d", C[0][x]);
            }
       
 
        //omp_set_num_threads(n);//end value ;
        //#pragma omp parallel private(p)//index
        /*
        {
             p = omp_get_thread_num() ;
             if(A[i]==1)
             {
                 Answer[i]=A[i]*(i-1);

             }
        }
         */
        
        
}
//
// end of file
//
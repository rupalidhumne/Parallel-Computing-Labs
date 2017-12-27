#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <omp.h>

// set variable named 'exp' in main in order to set array size n

void print( int A[], int n )
{
	// prints array A having n elements

	int nPerLine=15;

	int i;

	for( i=0; i<n; i++ )
	{
		printf( "%4d", A[i] );

		if( (i+1) % nPerLine == 0 ) printf("\n");
	}

	printf("\n");
}

void init( int A[], int B[], int n )
{
	// inits arrays A and B one odd one even no same #s

	int i;

	for( i=0; i<n; i++ )
	{
		A[i] = 2*i;

		B[i] = 2*i + 1;
	}
}

int getRank( int A[], int i, int B[], int loBnd, int hiBnd ) //BINARY SEARH
{

	int nElements = hiBnd-loBnd; 

	if( nElements == 1 ) 
        {
            
            return A[i] > B[loBnd]; // if false will return lower index val if a in b, else (true) will return greater index val
        }

	int midPt = loBnd + nElements/2; 

	if( A[i] <= B[midPt] ) // lower part of B

		return getRank( A, i, B, loBnd, midPt ); //recursion with lower part
        else
        {
            return nElements/2 + getRank( A, i, B, midPt, hiBnd ); //else it would be in the upper bound, recursion
        }
}

int main()
{
	

	int exp = 2; //2^2^this exponent

	int n = (int) pow( 2, pow(2,exp) ); //2^2^n or log2log2n

	printf("\n");


	int* A = (int*) malloc(   n * sizeof(int) ); //3 arrays with random numbers (one odd one even going to 2^2^2 (exp==2))
	int* B = (int*) malloc(   n * sizeof(int) );
	int* C = (int*) malloc( 2*n * sizeof(int) );

	init( A, B, n );

	int i;
       
	printf( "Ranks of A elements in B:\n\n"); //one less than vals in B
        
	for( i=0; i<n; i++ )
	
		printf( "A[%4d ]: %4d, rank in B: %4d\n", i, A[i], getRank( A, i, B, 0, n ) );

	printf("\n");

	printf( "Ranks of B elements in A:\n\n"); //one greater than vals in A

	for( i=0; i<n; i++ )
	
		printf( "B[%4d ]: %4d, rank in A: %4d\n", i, B[i], getRank( B, i, A, 0, n ) );

	printf("\n");
         //ranks (indices) placed in 0123blah bah blah
      
	
	
	double startTime = omp_get_wtime(); /// time in sec since fixed point in past
        //surplus log is the "solution to the ranking problem" 
        //extend it into the "merge problem" by sorting the found indices into the new array c
        //for i->n n pardo
        //do binary search for all a in b
        //then binary for all b in a
        //and add to the c thing in right order
	//#pragma omp parallel 
	//{
		#pragma omp parallel for

		for( i=0; i<n; i++ )
		{
			C[ i + getRank( A, i, B, 0, n ) ] = A[i];  //adds all A (evens) in their spots
                        //printf( " C array:\n" ); print( C, 2*n ); printf("\n");
		}

		#pragma omp parallel for
		
		for( i=0; i<n; i++ )
		{
			C[ i + getRank( B, i, A, 0, n ) ] = B[i];  //adds all B (odds) in their spots
		}
	//}

	double endTime = omp_get_wtime(); /// time in sec since fixed point in past

	printf( "Merged array C:\n" ); print( C, 2*n ); printf("\n");
	
	printf( "Exec time (ms): %.3f\n", (endTime-startTime)*1000 );

	printf("\n");
}

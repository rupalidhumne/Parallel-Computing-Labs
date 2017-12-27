
#include <stdio.h>
// 
#include "mpi.h"
// 

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

const int FALSE = 0;
const int TRUE  = 1;
int  k , j, a, s1, s2;
double prob;
           
   //double result;
   float workerResult;

// values of treeNote state

#define N 0 // N: no tree
#define T 1 // T: tree
#define F 2 // F: tree on fire
#define B 3 // B: burned tree

#define treeNodePtr(i,j) ( *(treeArray + i*nCols + j) )

typedef struct TreeNodeStruc TreeNode;

struct TreeNodeStruc
{
    int state;     // N (no tree), T (tree), F (on fire), B (burned)
    int row, col;  // location in tree array

    TreeNode *next; // list link
};

int nRows=100;
int nCols=100;
int nTrials=200;


TreeNode **treeArray;

TreeNode *BHead, *BTail, *FHead, *FTail;  // B-list head and tail, F-list head and tail

void printTreeArray();
void printList(TreeNode *head, char *heading);
char getStateChar(int state);
void waitForUser(int wait);

void makeTree()
{

    BHead = BTail = FHead = FTail = NULL;



	treeArray = (TreeNode**)malloc(nRows*nCols*sizeof(TreeNode*));

	//srand(clock()); // initialize seed

	// srand(1);

	int i, j;

	for (i = 0; i < nRows; i++)

		for (j = 0; j < nCols; j++)
		{
			

			treeNodePtr(i,j) = (TreeNode*)malloc(sizeof(TreeNode));

			treeNodePtr(i,j)->row = i;
			treeNodePtr(i,j)->col = j;

			treeNodePtr(i,j)->next = NULL;

			if (rand() / (double)RAND_MAX <= prob)
                        {
                            //printf("Make tree  prob: %f",prob);
                            treeNodePtr(i,j)->state = T; // prob = probability of T vs. N
                        }
                            

				
                                   
			else
				treeNodePtr(i,j)->state = N;
		}
}

void putList(TreeNode **headPtr, TreeNode **tailPtr, TreeNode *node) // pass head and tail pointers by reference
{
	

	if ( *headPtr == NULL) // list is empty

		*headPtr = node;

	else // list is not empty

		treeNodePtr( (*tailPtr)->row, (*tailPtr)->col )->next = node; // treeArray[ (*tailPtr)->row ][ (*tailPtr)->col ]

	*tailPtr = node;
}

void baseStep()
{
    
	int i;

    for( i=0; i<nRows; i++ )
    {
       

		if ( treeNodePtr(i,0)->state == T )
        {
			treeNodePtr(i,0)->state = B; // treeArray[i][0]->state

			putList( &BHead, &BTail, treeNodePtr(i,0) ); // treeArray[i][0]
        };
    }
}

int checkNeighbor(int i, int j)
{
    

	int done = TRUE;

	if (i >= 0 && i<nRows && j >= 0 && j<nCols && treeNodePtr(i,j)->state == T) //not on border and tree
	{
		treeNodePtr(i,j)->state = F; // treeArray[i][j]->state

		putList( &FHead, &FTail, treeNodePtr(i,j) ); // treeArray[i][j]

		done = FALSE;
	};

	return done;
}

int contStep()
{
    

    int done = TRUE;

	TreeNode* node;

    for( node=BHead; node != NULL; node=node->next )
    {
        // set done to false if any node is changed

        int i = node->row;
        int j = node->col;

        // ***
        BHead = node ;
        free( BHead );
        BHead = NULL ;
        // ***
        
        // in following statements, checkNeighbor must be on LHS to be sure it is called

        done = checkNeighbor( i, j-1 ) && done; // left neighbor
        done = checkNeighbor( i, j+1 ) && done; // right neighbor

        done = checkNeighbor( i-1, j ) && done; // up neighbor
        done = checkNeighbor( i+1, j ) && done; // down neighbor
    }

    return done;
}

void connectLists()
{
	
	TreeNode* node;

	for( node=FHead; node != NULL; node=node->next )

		node->state = B;

	if( BHead == NULL ) // B-list is empty

		BHead = FHead;

	else

		treeNodePtr(BTail->row, BTail->col)->next = FHead; // treeArray[BTail->row][BTail->col]->next

	BTail = FTail;

	FHead = FTail = NULL;
}

char getStateChar(int state)
{
	

	switch(state)
	{
		case N: return '*'; 
		case T: return 'T';
		case F: return 'F';
		case B: return 'B';
	}
	return '\0'; // requested by compiler
}

void printTreeArray()
{

	printf("\n");

	int i, j;

	for( i=0; i<nRows; i++)
	{
		for( j=0; j<nCols; j++)

			printf( "%c", getStateChar( treeNodePtr(i,j)->state ) ); // treeArray[i][j]->state

		printf("\n");
	}
}

void printList( TreeNode *head, char* heading )
{
	// print B-list or F-list

	printf("\n%s\n", heading );

	TreeNode* node;

	for( node=head; node != NULL; node = node->next)

		printf( "i: %d, j: %d, state: %c\n", node->row, node->col, getStateChar(node->state) );
}


double gettime()
{
	double t ;
	//
	struct timeval* ptr = (struct timeval*)malloc( sizeof(struct timeval) ) ;
	//
	gettimeofday( ptr , NULL ) ; // second argument is time zone... NULL
	//
	t = ptr->tv_sec * 1000000.0 + ptr->tv_usec ;
	//
	free( ptr ) ;
	//
	return t / 1000000.0 ;
}
float doWork()
{
       
    int totalIterations = 0, nIterations;
    //double tic, toc;
    //tic= gettime() ;

	int i;

	for( i=0; i<nTrials; i++ )
	{
		//printf( "\nTrial %d, prob: %.2f\n", i, prob );

		makeTree();
               //printf("Total Iterations: %d" , totalIterations ) ;

		//printTreeArray();
		baseStep();
		//printTreeArray();

		int done;

		nIterations = 0;

		do
		{

			done = contStep();
			connectLists();
			if( !done ) 
                            //printTreeArray();

			nIterations++;

		} 
                while( !done );

		totalIterations += nIterations;

		//printf( "\nTrial %d, prob: %.2f, # iterations: %d\n", i, prob, nIterations );
		//printf( "\n* = no tree, T = live tree, B = burned tree\n\n" );

	}
        
        //printf("Time: %0.16f seconds\n" , toc - tic ) ;
	//printf( "\n# rows: %d, # cols: %d, # trials: %d, prob: %.2f\n", nRows, nCols, nTrials, prob );

	//printf( "\nTotal # iterations: %d, mean # iterations/trial: %.1f\n\n", totalIterations, totalIterations / (float) nTrials );
       //printf("Total Iterations: %d" , totalIterations ) ;
       //printf("nTrials: %d" , nTrials ) ;
        float retval= totalIterations / (float) nTrials ;
        //totalIterations=0;
        //printf("Ret Val: %f" , retval ) ;
       return retval;
}


int main( int argc , char* argv[] )
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
       
       for( j = 1 ; j < size; j++ ) //for every worker
      {
          
          
          //printf("Before Sent: %f",prob);
          
           MPI_Send( &prob , 1 , MPI_DOUBLE , j , tag , MPI_COMM_WORLD ) ;
           // printf("After Sent: %f",prob);
          
          //printf("Before Increment: %f",prob);
            prob+=0.01;
            //printf(" Prob: %f", prob);
            //printf("Prob: %f", )
          //printf("After Increment prob: %f",prob);
           
      }
        
      while(prob<=1.0)
      {
          if(prob==1.0)
          {
              printf("reached");
          }
          //printf(" Prob: %f", prob);
          //printf("Prob: ", %);
          MPI_Recv( &workerResult , 1 , MPI_FLOAT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
            s1 = status.MPI_SOURCE ;
          //printf( " Answer: %d %d %20.16f\n" , s1 , size , workerResult ) ;
          MPI_Send( &prob , 1 , MPI_DOUBLE , s1 , tag , MPI_COMM_WORLD ) ;
          //if(prob==0.9)
          //{
              //printf("reached");
              //break;
          //}
          prob+=0.01;
          //printf("Status:  %d", s1);
       //printf(" Prob: %f", prob);
         
      }
       
      for( k = 1 ; k < size; k++ ) //for every worker
      {
          MPI_Recv( &workerResult , 1 , MPI_FLOAT , MPI_ANY_SOURCE , tag , MPI_COMM_WORLD , &status ) ;
          //printf("Reached");
          s2 = status.MPI_SOURCE ;
          //printf( " Answer: %d %d %20.16f\n" , s2 , size , workerResult ) ;
          prob=-1.0;
          MPI_Send( &prob , 1 , MPI_DOUBLE , s2 , tag , MPI_COMM_WORLD ) ;
      }
      //if(prob==1)
          //{
              //for( a = 1 ; a < size; a++ ) //for every worker
                //{
                    //MPI_Send( &kill, 1 , MPI_DOUBLE , a , tag , MPI_COMM_WORLD);
               // }
          //}
   
       
   
    }
   
   else
   {
       
       
       while(1)
       {
          MPI_Recv( &prob , 1 , MPI_DOUBLE , 0 , tag , MPI_COMM_WORLD , &status ); //error spot
           //printf("Received Prob: %f",prob);
          
          if(prob<0.0)
          {
             break; 
          }
         
          workerResult = doWork();
          //printf("%f\n",  workerResult/100);
           //printf("Worker Result for %f, prob: %f" , workerResult,prob);
          //printf("%f Work = ", workerResult);
          MPI_Send( &workerResult , 1 , MPI_FLOAT , 0 , tag , MPI_COMM_WORLD) ;
        
       }
       
   }

   MPI_Finalize() ;
   toc=gettime();
   printf("Time: %0.16f seconds\n" , toc - tic ) ;

   return 0;
}


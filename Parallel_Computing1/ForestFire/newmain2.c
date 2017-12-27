#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

const int FALSE = 0;
const int TRUE  = 1;

// values of treeNote state

#define N 0 // N: no tree
#define T 1 // T: tree
#define F 2 // F: tree on fire
#define B 3 // B: burned tree

// treeNodePtr(i,j) == treeArray[i][j]
// can't use treeArray[i][j] because array dimensions are determined at runtime

#define treeNodePtr(i,j) ( *(treeArray + i*nCols + j) )

typedef struct TreeNodeStruc TreeNode;

struct TreeNodeStruc
{
    int state;     // N (no tree), T (tree), F (on fire), B (burned)
    int row, col;  // location in tree array

    TreeNode *next; // list link
};

int nRows, nCols, nTrials;

double prob;

// treeArray is a pointer to an array of TreeNode pointers
// treeArray[i][j] is a pointer to a TreeNode

TreeNode **treeArray;

TreeNode *BHead, *BTail, *FHead, *FTail;  // B-list head and tail, F-list head and tail

void printTreeArray();
void printList(TreeNode *head, char *heading);
char getStateChar(int state);
void waitForUser(int wait);

void makeTree()
{
    // makes array of tree nodes

    // init B-list and F-list head and tail pointers

    BHead = BTail = FHead = FTail = NULL;

    // allocate  and initialize array of tree nodes and, with probability prob, assign T vs. N

	treeArray = (TreeNode**)malloc(nRows*nCols*sizeof(TreeNode*));

	srand(clock()); // initialize seed

	// srand(1);

	int i, j;

	for (i = 0; i < nRows; i++)

		for (j = 0; j < nCols; j++)
		{
			// treeNodePtr(i,j) == treeArray[i][j] == pointer to TreeNode

			treeNodePtr(i,j) = (TreeNode*)malloc(sizeof(TreeNode));

			treeNodePtr(i,j)->row = i;
			treeNodePtr(i,j)->col = j;

			treeNodePtr(i,j)->next = NULL;

			if (rand() / (double)RAND_MAX <= prob)

				treeNodePtr(i,j)->state = T; // prob = probability of T vs. N

			else

				treeNodePtr(i,j)->state = N;
		}
}

void putList(TreeNode **headPtr, TreeNode **tailPtr, TreeNode *node) // pass head and tail pointers by reference
{
	// puts node at end of list

	if ( *headPtr == NULL) // list is empty

		*headPtr = node;

	else // list is not empty

		treeNodePtr( (*tailPtr)->row, (*tailPtr)->col )->next = node; // treeArray[ (*tailPtr)->row ][ (*tailPtr)->col ]

	*tailPtr = node;
}

void baseStep()
{
    // for each T-node in column 0, change to B-node and put node on B-list

	int i;

    for( i=0; i<nRows; i++ )
    {
        // if node is T-node, change to B-node and put node on B-list

		if ( treeNodePtr(i,0)->state == T )
        {
			treeNodePtr(i,0)->state = B; // treeArray[i][0]->state

			putList( &BHead, &BTail, treeNodePtr(i,0) ); // treeArray[i][0]
        };
    }
}

int checkNeighbor(int i, int j)
{
	// if neighbor is T-node, change neighbor  to F=node and put neighbor on F-list

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
    // for each B-node, check all four rectangular neighbors: left, right, up, down

    int done = TRUE;

	TreeNode* node;

    for( node=BHead; node != NULL; node=node->next )
    {
        // set done to false if any node is changed

        int i = node->row;
        int j = node->col;

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
	// in F-list nodes, change F-state to B-state, then connect F-list to B-list change all fire to burned and add to burned

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
	// returns letter for state

	switch(state)
	{
		case N: return '*'; //star makes it easy to verify accuracy at end of of execution
		case T: return 'T';
		case F: return 'F';
		case B: return 'B';
	}
	return '\0'; // requested by compiler
}

void printTreeArray()
{
	// print tree array, showing letters for state values

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

void waitForUser( int wait )
{
	if( wait )
        {
            printf("Type enter to continue");
            getchar();
        }
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
void main()
{
    int nTrials, totalIterations = 0, nIterations;

	printf( "Please enter # rows: " );
	scanf("%d", &nRows );

	printf( "Please enter # columns: " );
	scanf( "%d", &nCols );

	printf( "Please enter # trials: " );
	scanf( "%d", &nTrials );

	printf( "Please enter probability of tree: " );
	scanf( "%lf", &prob );

	printf( "\n# rows: %d, # cols: %d, # trials: %d, prob: %.2f\n", nRows, nCols, nTrials, prob );

	
    double tic, toc;
    tic= gettime() ;

	int i;

	for( i=0; i<nTrials; i++ )
	{
		printf( "\nTrial %d, prob: %.2f\n", i, prob );

		makeTree();

		printTreeArray();

		baseStep();

		printTreeArray();

		int done;

		nIterations = 0;

		do
		{
		
			done = contStep();
			connectLists();

			if( !done ) 
                            printTreeArray();

			nIterations++;

		} while( !done );

		totalIterations += nIterations;

		printf( "\nTrial %d, prob: %.2f, # iterations: %d\n", i, prob, nIterations );
	}
        toc=gettime();
        printf("Time: %0.16f seconds\n" , toc - tic ) ;
	printf( "\n# rows: %d, # cols: %d, # trials: %d, prob: %.2f\n", nRows, nCols, nTrials, prob );

	printf( "\nTotal # iterations: %d, mean # iterations/trial: %.1f\n\n", totalIterations, totalIterations / (float) nTrials );

}//waitForUser(TRUE);

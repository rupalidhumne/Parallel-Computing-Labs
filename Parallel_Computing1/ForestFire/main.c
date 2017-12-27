
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int row;
	int col ;
        struct Node* next ;
        
} 
ListNode;

ListNode* list_create(int row, int col)
{
	ListNode *node;
	if(!(node=malloc(sizeof(ListNode*)))) 
            return NULL;
	node->row=row;
        node->col=col;
	node->next=NULL;
	return node;
}
ListNode* find(int row, int col, ListNode* notBurned)
{
	while(notBurned) 
        {
		if(notBurned->row==row && notBurned->col ==col)
                    return notBurned;
		notBurned=notBurned->next;
	}
	return NULL;
}
ListNode* insert(ListNode* node, int row, int col)
{
	ListNode* newnode;
        newnode=list_create(row,col);
        newnode->next = node->next;
        node->next = newnode;
	return newnode;
}
void printMat(char matrix[N][N])
{
    int row, columns;
    for (row=0; row<sizeof(matrix); row++)
    {
        for(columns=0; columns<sizeof(matrix); columns++)
             printf("%d", matrix[row][columns]);
        printf("\n");
     }
}
int main() 
{
    ListNode* burned = NULL ;
    ListNode* notBurned = NULL ;
    float prob=0;
    #define  N 5
    char matrix[N][N];
    printf("Please input an probability value: ");
    scanf("%f", &prob);
    int r, c;
    double treeprob;
        for(r=0; r<N; r++)
        {
            for(c=0; c<N; c++)
            {
                treeprob = ((double)rand()/(double)RAND_MAX);
                if(treeprob<=prob)
                {
                    matrix[r][c]='*'; //tree
                }
                else 
                {
                    matrix[r][c]='+'; //not tree
                }
            }
        }
    printMat(matrix);
    int z;
    for(z=0; z<N; z++)
    {
        if(matrix[z][0]=='*')
        {
            matrix[z][0] = 'F'; 
        }
        
    }
    printMat(matrix);
    int x, y; 
    for(x=0; x<N; x++) // add to burned and not burned
    {
        for(y=0; y<N; y++)
        {
            if(matrix[x][y]=='F')
            {
                ListNode* newnode;
                newnode= list_create(x,y);
                newnode->next = burned->next;
                burned->next =newnode;
                burned=newnode;
                
            }
            else
            {
                ListNode* newnode;
                newnode= list_create(x,y);
                newnode->next = notBurned->next;
                notBurned->next =newnode;
                notBurned=newnode;
            }
        }
    }
    while(burned!=NULL)
    {
        ListNode  *removed = *burned;            // save old head for freeing.
                   // list empty? then do nothing.
        *burned = removed->next;                   // advance head to second node.
                          // free old head
        
       ListNode* neighbor;
       int row = removed->row; 
       int col = removed-> col;
       free(removed);
       matrix[row][col]='-';
       if(matrix[row+1][col] =='*')
       {
           neighbor=find(row,col,notBurned);
           insert(burned,neighbor);
           matrix[row+1][col]='F';
       }
       if(matrix[row][col+1] =='*')
       {
           neighbor=find(row,col,notBurned);
           insert(burned,neighbor);
           matrix[row+1][col]='F';
       }
       if(matrix[row-1][col] =='*')
       {
           neighbor=find(row,col,notBurned);
           insert(burned,neighbor);
           matrix[row+1][col]='F';
       }
       if(matrix[row][col-1] =='*')
       {
          neighbor=find(row,col,notBurned);
          insert(burned,neighbor);
          matrix[row+1][col]='F';
       }
        printMat(matrix);
    }
    
   
}



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int true = 1, false=0;

typedef struct NodeStruct NodeType; //defines type for all node things

struct NodeStruct
{
	char symbol;	
	int count; // num occurrences
	float prob;  // probability = count/(message length)

	int* codeArray; // array for the code
	int  level;     // tree level of node 
	
	NodeType* sNext; 
	NodeType* tNext; 

	NodeType* left; 
	NodeType* right;
};


void printNode(NodeType* node, int printLevel, int currentLevel)
{


	if (node == NULL) 
            return;

	if (currentLevel == printLevel)
	{
		int i;
		int symNode = (node->left == NULL) && (node->right == NULL);

		if ((int)node->symbol == 10) printf("%4d%6s%10d%10.6f", printLevel, "\\n", node->count, node->prob);

		else printf("%4d%6c%10d%10.6f", printLevel, node->symbol, node->count, node->prob);

		if (node->left != NULL)
			
			if ((int)(node->left)->symbol == 10) printf("%4s%7d", "\\n", (node->left)->count);
		
			else printf("%4c%7d", (node->left)->symbol, (node->left)->count);

		else // node left == NULL

			if (symNode && node->level >= 0)
			{
			

				printf(" code:  ");

				for (i = 0; i < node->level; i++) 
                                    printf("%d", node->codeArray[i]);
			}
				
			else printf("%11s", " ");

		if (node->right != NULL)
			
			if ((int)(node->right)->symbol == 10) 
                            printf("%5s%7d\n", "\\n", (node->right)->count);

			else  
                            printf("%5c%7d\n", (node->right)->symbol, (node->right)->count);

		else 
                    printf("\n");
	}
	else
	{
		printNode(node->left, printLevel, currentLevel + 1);

		printNode(node->right, printLevel, currentLevel + 1);
	}
}
void updateSList(NodeType** headPtr, char symbol) 
{
	// searches for symbol 
	// if symbol is in list
	// else creates node 

	NodeType* prior;
	NodeType* current;

	
	
	for (prior = NULL, current = *headPtr; current != NULL && symbol > current->symbol; prior = current, current = current->sNext);

	// now current == NULL or symbol <= current->symbol

	if (current != NULL && symbol == current->symbol)
	{
		// symbol in list

		current->count++;
		return;
	}

	// symbol is not in list

	NodeType* newNode = (NodeType*) malloc(sizeof(NodeType));

	newNode->symbol = symbol;
	newNode->count = 1;
	newNode->level = -1;

	newNode->tNext = NULL;
	newNode->left = NULL;
	newNode->right = NULL;

	// insert symbol before current node 

	newNode->sNext = current;

	if (prior != NULL) 
            prior->sNext = newNode;

	if (*headPtr == current) 
            *headPtr = newNode;
}

NodeType* findSymNode(NodeType* head, char symbol)
{
	// finds symbol in symbol list

	NodeType* current;

	for (current = head; current != NULL && current->symbol != symbol; current = current->sNext);

	

	return current;
}


void sortSListByCount(NodeType** headPtr) 
{
	// sorts symbol list

	int i, j, nElements=0;
	NodeType *prior, *current, *next, *nextNext;

	

	for (current = *headPtr; current != NULL; current = current->sNext)

		nElements++;

	//bubble sort

	for (i = 0; i < nElements; i++)
	{
		

		for (prior = NULL, current = *headPtr, j = i; current != NULL && j < nElements; j++)
		{
			next = current->sNext;

			if (next != NULL && current->count > next->count)
			{
				

				if (*headPtr == current) *headPtr = next;

				else prior->sNext = next;

				nextNext = next->sNext;

				next->sNext = current;

				current->sNext = nextNext;

				prior = next; 
			}
			else
			{
				

				prior = current;
				
				current = current->sNext;
			}
		}
	}
}

NodeType* makeTList(NodeType* sHead)
{
	// makes tree list 

	NodeType *current;

	for (current = sHead; current != NULL; current = current->sNext)

		current->tNext = current->sNext;

	return sHead;
}

NodeType* getTNode(NodeType** headPtr) 
{
	

	NodeType* current = *headPtr;

	if ( current != NULL ) *headPtr = current->tNext;

	return current;
}

void putTNode(NodeType** headPtr, NodeType* node) 
{
	// inserts node into  list 

	NodeType *prior, *current;

	for (prior = NULL, current = *headPtr; current != NULL && node->count > current->count; prior = current, current = current->tNext);

	// now current == NULL or node->count <= current->count

	// insert symbol before current node 

	node->tNext = current;

	if (prior != NULL) prior->tNext = node;

	if (*headPtr == current) *headPtr = node;
}

NodeType* makeTree(NodeType** tHeadPtr) 
{
	// makes Huffman coding tree using tree list

	NodeType *left, *right, *parent;

	while (true)
	{
		left = getTNode(tHeadPtr);
		right = getTNode(tHeadPtr);

		if (right == NULL) 
                    break; // left node is root

		// create parent node for left and right nodes

		parent = (NodeType*)malloc(sizeof(NodeType));

		parent->symbol = '\0'; 

		parent->count = left->count + right->count;
		parent->prob = left->prob + right->prob;

		parent->level = -1;

		parent->sNext = NULL; 
		parent->tNext = NULL;

		parent->left = left;
		parent->right = right;

		putTNode(tHeadPtr, parent);

		
	}

	return left; // root node
}

int calcTreeDepth(NodeType* node)
{
	

	if (node == NULL) return 0;

	return 1 + (int)fmax( calcTreeDepth(node->left), calcTreeDepth(node->right) );
}

void setCodes(NodeType* node, int* codeArray, int level)
{
	// sets Huffman code for each symbol node

	int i;

	if (node->left == NULL && node->right == NULL)
	{
		

		node->level = level;
		
		node->codeArray = (int*)malloc((level)*sizeof(int));

		for (i = 0; i < level; i++)
		
			node->codeArray[i] = codeArray[i];

		return;
	}

	if (node->left != NULL)
	{
		codeArray[level] = 0;
		setCodes(node->left, codeArray, level + 1);
	}

	if (node->right != NULL)
	{
		codeArray[level] = 1;
		setCodes(node->right, codeArray, level + 1);
	}
}

int main()
{
	// read message from file create Huffman and display and then decode
  
	FILE *inFile, *outFile;



	NodeType* sHead = NULL; // symbol list
	NodeType* tHead = NULL; // tree list
	NodeType* root  = NULL; // Huffman  tree

	int  treeDepth; // # levels in tree
	int* codeArray; // Huffman code array


	char symbol;

	inFile = fopen("encodeIN.txt", "r");

	printf("\nPlain text stuff:\n\n");

	while (fread(&symbol, sizeof(char), 1, inFile)) // read to end of file
	{
		 printf("%c", symbol);

		updateSList(&sHead, symbol); 
	}

	fclose(inFile);

	printf("\n");

	sortSListByCount(&sHead); 

	tHead = makeTList(sHead);

	root = makeTree(&tHead); 

	treeDepth = calcTreeDepth(root);

	codeArray = (int*)malloc((treeDepth-1)*sizeof(int));

	setCodes(root, codeArray, 0);


	inFile  = fopen("encodeIN.txt", "r");
	outFile = fopen("Encoded.txt", "w");

	printf("\n\nEncoded:\n\n");

	while (fread(&symbol, sizeof(char), 1, inFile)) 
	{
		

		NodeType* symNode = findSymNode(sHead, symbol);


		 printf("%c ", symNode->symbol);

		int i;

		for (i = 0; i < symNode->level; i++)
		{
			fputc((char)(symNode->codeArray[i]+'0'), outFile);

			printf("%d", symNode->codeArray[i]);
		}

		printf("\n");
	}

	fclose(inFile);
	fclose(outFile);


	char codeBit;

	inFile  = fopen("Encoded.txt", "r");
	outFile = fopen("encodeOUT.txt", "w");
	
	printf("\n\nDecoded:\n\n");

	NodeType* current = root;
	
	do
	{
		if (!fread(&codeBit, sizeof(char), 1, inFile)) 
                    break; 
                if(codeBit=='0')
                {
                    current = current->left;
                }
                else
                {
                   current = current->right; 
                }
		if (current->left == NULL && current->right == NULL)
		{
			fputc(current->symbol, outFile);

			 printf("%c", current->symbol);

			current = root;
		}

	} 
        while(true);
        

	 printf("\n");

	fclose(inFile);
	fclose(outFile);

	return 0;
}

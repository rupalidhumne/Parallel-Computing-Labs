#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
   FILE* fin = fopen( "decodeMe.txt" , "r" ) ;
   char ch;
   char tree[100100];
   for(int x=0; x<sizeof(tree); x++)
   {
      tree[x]='\0';
   }           
   int numbytes;
   char symbol;
   char msg;
   char *answer;
   for(int j=0; j<29; j++)
   {
      if(j<28)
      {
         numbytes = fread( &symbol , sizeof(char) , 1 , fin ) ;
         int k=1; //root
         while(ch !='\n')
         {
            numbytes = fread( &ch , sizeof(char) , 1 , fin ) ;
            if(ch=='\n')
               break;
            else if(ch=='0')
            {
               k=2*k;
            }
            else if (ch=='1')
            {
               k=(2*k)+1;
            }      
         }
         tree[k]=symbol;
      } 
      if( j==29)
      {
         int p=1;
         while( msg!='\n')
         {
            
            numbytes = fread( &msg , sizeof(char) , 1 , fin ) ; //read entire string fix
            if(msg!='\0')
            {
               if(msg=='0')
               {
                  p=2*p;
               }
               else
               {
                  p=2*p+1;
               }   
               
            }
            else
            {
               answer+=tree[k]+"";
            }   
              
         }     
                       
   }               
   }
   printf(answer);
   return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
int main()
{
   //at end when have msg each letter is 8 bits, in ascii, so ascii bits(num letters times 8) -huffmanbits (num 1s and zeroes read in)when translating/ascii bits
   
    int encode = 0;
    int decode=0;
    FILE* file = fopen( "decodeMe.txt" , "r" );
    int dummy;

   char ch;
   char tree[100100];
   int x;
   int freq[256]; //  256 = 2^8 = max # ASCII symbols, which use 8-bit coding

   int i;
   
   for (i = 0; i < 256; i++) freq[i] = 0; // PB: need to init freq array

   for(x=1; x<100100; x++)
   {
      tree[x]='\0';
   } 
   char symbol;
   char msg;
   int k;
   int j;
   for (j = 0; j<28; j++) 
   {
      k=1;
       fread( &symbol , sizeof(char) , 1 , file ) ;
         while(1)
         {
            fread( &ch , sizeof(char) , 1 , file ) ;
            if(ch=='0')
            {
              
               k=2*k;
            }
            else if (ch=='1')
            {
               k=(2*k)+1;
            }
            if(ch =='\n')
              break;
         }
         
      tree[k]=symbol;
   }

         int p=1;
         while(1)
         {
            if( !fread( &msg , sizeof(char) , 1 , file) ) 
                break; //  end of file

            if(msg =='\n') 
                break;

                if(msg=='0')
               {
                   //printf("%c", msg);
                   p=2*p;
                   
                   encode++;
                   //printf("%d", p);
               }
               else
               {
                   //printf("%c", msg);
                   p=(2*p)+1;
                   encode++;
                   //printf("%d", p);
               }

               if(tree[p]!='\0') //if symbol spot
            {
                printf("%c", tree[p]);
                freq[(int)tree[p]]++; //have to make it a int thing //frequency for each letter
                decode++;
                p=1;
            }
            
         }
         
		 int nChars = decode; 

         decode=decode*8;

         double ratio=(decode-encode)/(float)decode;

		 //printf("\n\n# characeers in message: %d", nChars); // PB: this value is useful
		 //printf("\n\nbit ratio encode/decode: %.2f", encode / (float)decode); // PB: this value is useful
		 printf("\nCompression ratio: %f", ratio);

		 double sum = 0.0;

		

		 // exceeded --> compression ration = decoded bits = 8*8- (encoded bits = num 0s and 1s)/48
                 // exceeded--> shannon e4 x1 c1 d2 so for each (4) proability = num/total letters (8))
                 //sum +=freq[p] * -log2(probability) so for e it would be 4*-log2(1/2) = 4*-(-1 = 4) x or c it would be 1* -log2(1/8)) = 4*--3=12

         for(x=0; x<256; x++) //num ascii    
         {
             if(freq[x]!=0)
             {
                double p = freq[x] / (double)nChars; //frequency of letter over total number of levels -log2probability times frequency

                sum += freq[x] * -log2(p);

                //meanNBits += p * -log2(p);
             }             
         }

		// printf("\n\nmean # bits = sum over p * -log2(p): %.2f\n", meanNBits); // this value is what you want
		 
		 printf("\nShannon: %d\n\n", (int)sum); // this value doesn't make sense
		
		 // PB: ths following was the original code:
		 /*
		 int t;

		 for (t = 0; t<256; t++)  // PB: Used t instead of x
		 {
			 if (freq[x] != 0)
			 {
				 double p = (double)freq[x] / nChars;
				 int lthing = (-1)*(freq[x])*(log(2) / log(p));  // PB: log2(z) = log(z)/log(2)
				 sum += lthing;
			 }
		 }
		 */

   return 0;
}
//int shannon()
//{
    //sum for all characters 1-28 of the negative frequency times log base 2 probability (freq that letter/total number of decoded letter)
    //for every character i find the number of times it appears and do -freq(log2)freq/
//

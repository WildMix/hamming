
/***********************************************************************
* FILENAME :        hamming.c             
*
* DESCRIPTION :
*       generates a word for error correction. 
*
* PUBLIC FUNCTIONS :
*       int     FM_CompressFile( FileHandle )
*       int     FM_DecompressFile( FileHandle )
*
* NOTES :
*       
*       The program is not yet finished
*		Needs to check the operation and simulate an error
*
*       
* 
* AUTHOR :    Luigi Gesualdo        START DATE :    20 Nov 19
*
* CHANGES :
*
**/


#include <stdio.h>
#include <stdlib.h>
#include <hamming.h>
#include <math.h>
#include <string.h>
int oscio[30];
int matrix [30][30];


int main()
{
	int word [8] = {0,1,0,1,1,1,0,0};
	int vett[8], vett2[8], i;
	createWord(word,vett,8);
	for (i = 0; i < 12; i ++)
		printf(" %d ",vett[i]);
	printf("\n");
	createfinalWord(vett,vett2,8);
	for (i = 0; i < 12; i ++)
		printf(" %d ",vett2[i]);
	printf("\n");
 
 }

void dec2bin(int n, int a)  // save the value in binary of n in an array with size a
{
   int c, k, i = 0, j = 0, Temp;
   a = a -1;

for (c = a; c >= 0; c--)
	  {
		k = n >> c;
	 
		if (k & 1){
		  oscio[c] = 1;
		}
		else{
		  oscio[c] = 0;
		}
	  }

   i = 0, j = a ;
   while (i < j) 
   {
      Temp = oscio[i];
      oscio[i] = oscio[j];
      oscio[j] = Temp;
      i++;             
      j--;         
   }  
   
}

// create a matrix with the value in binary of every single index
void createMatrix (int righe, int colonne)
{
	int i, j;
	
	for (i = 0; i < righe; i++)
	{
		dec2bin(i+1,colonne);
		for (j = 0; j < colonne; j++)
		{
			if ( oscio[j] == 1)
				matrix[i][j] = 1;
			else
				matrix[i][j] = 0;
		}
	}
}



int power (int base, int exponent)  // the value of the power of base at exponent
{
    int result = 1;
    while (exponent != 0)
    {
        result *= base;
        --exponent;
    }
return result;
}




int HammingDistance (int * word1, int* word2)  // the number of 1 of the XOR operation of word1 and word2
{
	int i = 0, dist = 0;
	while (i<8)
	{
		if (word1[i] != word2[i])
			dist ++;
		i++;
	}
	
	return dist;
}


int findParity (int N)	//  for the Hamming law, give the number of the bit of parity necessarly to check an error in a word of N bit 
{
	int i = 0, parity;
	int sum, pow;
	while (1)
	{
		sum = N + i + 1;
		pow = power(2,i);
		if (sum <= pow)
		{
			parity = i;
			break;
		}
			
		i++;
	}
	return parity;
}


int countparity (int number,int righe,int colonne) // how many 1 there are in the matrix at position number
{
	int i , pos = 0;
	if ( number > colonne)
		return 0;					// errore, number è troppo grande 
	for (i = 0; i < righe; i ++)
		if (matrix[i][number] == 1)
			pos++;
	
	return pos;
}

void findPosition(int vett[],int number,int righe, int colonne)  //memorize in vett all index of the bit of the word that the bit of parity 'number' control
{
	int j = 0, i;
	for (i = 0; i < righe; i ++){
		if (matrix[i][number] == 1){
				vett[j] = i + 1;
				j++;
		}
		
	}
}



int ispowof2 (int n)  // if n is a power of 2 return 1, else 0
{ 
 if (n == 0) 
    return 0; 
  while (n != 1) 
  { 
      if (n%2 != 0) 
         return 0; 
      n = n/2; 
  } 
  return 1; 
} 
  		




void createWord (int word[],int neword[],int data)  // create the word with the right size, in place of the bit of parity there's 12 
{
	int parity = findParity(data);
	int m = data + parity;
	int i = 0, j = 0, temp, a;
	
	while (i<m)
	{
		if (ispowof2(i+1) == 1)
		{
			neword[i] = 12;
		}
		else
		{
			if (word[j] == 1)
				neword[i] = 1;
			else
				neword[i] = 0;
			j++;
		}
	
	i++;
	}
	

}


int postoValue (int vett[],int n,int righe,int colonne)  // the value of the n bit of parity
{
	int count = 0, a,i;
	int max = countparity(n,righe,colonne);
	int vector[max];
	
	
	findPosition(vector,n,righe,colonne);
	for (i = 0; i < max; i ++){
		a = vector[i] - 1;
		if (vett[a] == 1)
				count++;
	}
	if (count%2 == 0)
		return 0;  // if is even must be 0
	return 1;      // else, 1
}


void createfinalWord (int word[],int neword[],int data)  // create the final word with the bit of parity
{
	int parity = findParity(data);
	int m = data + parity;
	int i = 0, j = 0, temp, a;
	createMatrix(m,parity);
	
	for (i = 0; i < m; i ++)
	{
		if (ispowof2(i+1) == 1)
		{
			if (postoValue(word,i,m,parity) == 1)
				word[i] = 1;
			else
				word[i] = 0;
		}
			
	}

	for (i = 0; i < m; i ++)
	{
		if (word[i] == 1)
			neword[i] = 1;
		else
			neword[i] = 0;
	}
	
}

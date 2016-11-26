/*
 *
 *
 * VM helper functions
 *
 * Author : Willie Stevenson
 *
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "vm.h"
#include "util.h"

void displayCharArray(char* IR, int size)
{
	for (int i = 0; i < size; ++i)
	{
		printf("%c", IR[i]);
	}
}

void displayMemory(char memory[][6], int iSize, int jSize)
{
	for (int i = 0; i < iSize; ++i)
	{	
		if (i%10 == 0)
		{
			printf("%d0: ", i/10);
		}
		else
		{
			printf(" ");
		}
		for (int j = 0; j < jSize; ++j)
		{
			printf("%c", memory[i][j]);
		}
		if ((i+1) % 10 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
}

int getOpCode(char* IR)
{
	int result = (charToInt(IR[0])*10) + charToInt(IR[1]);

	if (result > 37 || result < 0)
	{
		// printf("Opcode not valid\n");
		// printf("Opcode %c%c was converted to %d.\n", IR[0], IR[1], result);
		// printf("\nexiting . . . . . . \n");

		// exit(1);
	}
	
	return result;
}

int charToInt(char c)
{
	return (int)(c - 48);
}

/*
 * converts whatever's floating around in the ac to characters  
 */
void intToCharArray(int number, char* destArr)
{
	for(int j = 0; j < 6; j++) {
		if (j < 2)
		{
			destArr[j] = 'Z';
		}
		else
		{
			destArr[j] = 'X';
		}
    }

    int i = 5;

    while (number > 0)
    {
        destArr[i] = (number % 10) + '0';
        number = number / 10;
        i--;
    }

    for (int k = 0; k < 6; k++)
    {
    	if (destArr[k] == 'X')
    	{
    		destArr[k] = 0 + '0';
    	}
    }
}


// for testing purposes
//testing for string literal -> changed to 6 instead of using sizeof(sourceArray)
void charArrayCopy(char* sourceArr, char* destArr)
{
	for (int i = 0; i < 6; ++i)
	{
		destArr[i] = sourceArr[i];
	}
}

void loadProgramIntoMemory(Vm* vm, char* filename)
{

	// Step 1 Read file into VM memory. Assume the file name is program2.
	// int fp = open(filename, O_RDONLY); //always check the return value. 

	FILE *fp;

    fp = fopen (filename, "r");


	if (fp < 0) //error in read 
	{	
		printf("Could not open file\n");
	    exit(-1);
	}

	char f[1];

	//read in the first line of the program
	int lines_read = 0;
	int ret = fread (vm->memory[lines_read], 1, 6 ,fp); //returns number of characters read`
	ret = fread (f, 1, 1, fp); 
	lines_read++;
    while (ret > 0)
    { 
      	ret = fread (vm->memory[lines_read], 1, 6 ,fp); //returns number of characters read`
      	ret = fread (f, 1, 1, fp);
		
      	//if the first character is a 'Z' then you are reading data. 
		//No more program code so break out of loop
		// if(vm->memory[lines_read][0] == 'Z') {
		// 	charArrayCopy("XXXXXX", vm->memory[lines_read]);
		// 	break;
		// }
		lines_read++;
	}

	fclose(fp);
}
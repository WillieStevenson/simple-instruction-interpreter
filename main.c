/*
 *
 *
 * This is the runner file
 *
 * Author : Willie Stevenson
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "util.h"

int main(int argc, char *argv[])
{
	printf("%s\n", "Reading In Data program...\n");

	Vm* vm = newVm(); 

	loadProgramIntoMemory(vm, "test-programs/fib.txt");
	displayVm(vm);
	displayVmFinal(vm);
	printf("%s\n", "Executing program in memory . . .\n");
	startVM(vm);

	free(vm);

	printf("%s\n", "Completed.");
}

/*
 *
 *
 * This is the heart of the VM 
 *
 * Author : Willie Stevenson
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "vm.h"
#include "util.h"
#include "instructions.h"


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - VM PRIVATE FUNCTIONS - - - - - - - - - -*/

void nextInstruction(Vm* vm)
{
    charArrayCopy(vm->memory[vm->PC], vm->IR);
    displayVm(vm);
    vm->PC++;
}

void setPointer(Vm* vm, int p, int value){
    switch(p){
        case 0:
            vm->P0 = value;
            break;
        case 1:
            vm->P1 = value;
            break;
        case 2:
            vm->P2 = value;
            break;
        case 3:
            vm->P3 = value;
            break;
    }
}

void setRegister(Vm* vm, int r, int value){
    switch(r){
        case 0:
            vm->R0 = value;
            break;
        case 1:
            vm->R1 = value;
            break;
        case 2:
            vm->R2 = value;
            break;
        case 3:
            vm->R3 = value;
            break;
    }
}

int getPointer(Vm* vm, int p){
    int value = 0;
    
    switch(p){
        case 0:
            value = vm->P0;
            break;
        case 1:
            value = vm->P1;
            break;
        case 2:
            value = vm->P2;
            break;
        case 3:
            value = vm->P3;
            break;
    }

    return value;
}

int getRegister(Vm* vm, int r){
    int value = 0;  

    switch(r){
        case 0:
            value = vm->R0;
            break;
        case 1:
            value = vm->R1;
            break;
        case 2:
            value = vm->R2;
            break;
        case 3:
            value = vm->R3;
            break;
    }

    return value;
}

/*
 * obtains the pointer/register number from operand1 if in the form Pn/Rn, where n is the register number.
 * if not in this form and in the form XX, where X are integers as characters, these will be converted to integers. 
 *
 */

int opToInt(const char * op){
    int sum = 0;

    if(op[0] != 'P' || op[0] != 'R'){
        sum += (int)(op[0] -48) *10;    
    }
    sum += (op[1]-48);

    return sum;
}

/*
 * location contains the starting position of the opcode or either operand in the instruction
 * returns a 2 character subset array containing either the opcode, operand1, or operand2  
 */
char * getOp(int location, char * IR){
    // static char op[2];
    // op[0] = IR[location];
    // op[1] = IR[location+1];

    // return op;

    return IR + location;
}

int powerFunction(int base, int power){

    int result = 1;

    for (int i = 0; i < power; i++){
        result *= base;
    }

    return result;
}

/*
 * returns the sum as an integer of all opcodes that contain successive operands  XXXX,
 * however ignores 'X' characters if need be.
 * 
 * assumes opcode is always first two instruction indicies, instruction size stays at 6 for now
 */

int charArrayToInt(int start, int IRSize, char const * IR){
    int sum = 0;

    for(int i = start; i < IRSize; i++){
        if (IR[0] == 'Z' && IR[1] == 'Z')
        {
            if (i > 1 && IR[i] != 'X')
            {
                sum += (charToInt(IR[i]) * powerFunction(10,IRSize-1-i));
            }
        }
        else
        {
            if(IR[i] != 'X'){
                sum += (charToInt(IR[i]) * powerFunction(10,IRSize-1-i));
            }  
        } 
    }



    return sum;
    
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */


Vm* newVm()
{
	Vm* vm = malloc(sizeof(Vm));

	for (int i = 0; i < 6; ++i)
	{
		vm->IR[i] = 'X';
	}

    vm->PC = 0;
    vm->P0 = 0;
    vm->P1 = 0;
    vm->P2 = 0;
    vm->P3 = 0;
    vm->R0 = 0; 
    vm->R1 = 0;
    vm->R2 = 0;
    vm->R3 = 0;

    vm->SP = 0;

    vm->ACC = 0;

    for (int i = 0; i < 1; ++i)
    {
    	vm->PSW[i] = 'X';
    }

    for (int i = 0; i < 100; ++i)
    {
    	for (int j = 0; j < 6; ++j)
    	{
    		vm->memory[i][j] = 'X';
    	}
    }

    vm->opcode = 0;

	return vm;
}

void displayVm(Vm* vm)
{
	printf("PC = %d ", vm->PC);
	printf("IR = ");
	displayCharArray(vm->IR, 6);
	printf(";  ");
	printf("%s\n", INSTRUCTION_NAMES[getOpCode(vm->IR)]);
}

void displayVmFinal(Vm* vm)
{
    printf("\nCPU:\n");
    printf("PC=%d SP=%d ACC=%d PSW=%s", vm->PC, vm->SP, vm->ACC, vm->PSW);
    printf(" IR=");
    displayCharArray(vm->IR, 6);
    printf("\n");
    printf("\nREGISTERS:\n");
    printf("P0=%d P1=%d P2=%d P3=%d\n", vm->P0, vm->P1, vm->P2, vm->P3);
    printf("R0=%d R1=%d R2=%d R3=%d\n", vm->R0, vm->R1, vm->R2, vm->R3);
    printf("");
    printf("\nMEMORY:\n");
    displayMemory(vm->memory, 100, 6);
    printf("--------------------------------------------------------------\n");
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*- - - - - - - - - - VM FUNCTIONS - - - - - - - - - - - */

/*  
 * Load Pointer Immediate
 * Pn <- XX, X: {0 .. 9}.
 * 
 * 00 Pn XX
 *
 * Integer value XX is loaded into Pointer Register n
 */
void o0(Vm* vm){
    setPointer(vm, charToInt(vm->IR[3]),opToInt(getOp(4, vm->IR)));
}

/*  
 * Add to Pointer Immediate
 * Pn <- Pn + XX, X: {0 .. 9}.
 * 
 * 01 Pn XX
 *
 * Pointer incremented by integer XX
 */
void o1(Vm* vm){
    setPointer(vm, charToInt(vm->IR[3]), getPointer(vm, charToInt(vm->IR[3])) + opToInt(getOp(4,vm->IR)));
}

/*  
 * Subtract From Pointer Immediate
 * Pn <- Pn - XX, X: {0 .. 9}.
 * 
 * 02 Pn XX
 *
 * Pointer decremented by integer XX
 */
void o2(Vm* vm){
    setPointer(vm, charToInt(vm->IR[3]), getPointer(vm, charToInt(vm->IR[3])) - opToInt(getOp(4,vm->IR)));
}

/*  
 * Load Accumulator Immediate
 * AC <- XXXX, X: {0 .. 9}.
 * 
 * 03 XX XX
 *
 * Load interger value XXXX into accumulator
 */
void o3(Vm* vm){
    vm->ACC = charArrayToInt(2, 6,vm->IR);
}

/*  
 * Load Accumulator Register Addressing
 * AC <- M(Pn).
 * 
 * 04 Pn --
 *
 * Load accumulator with contents of memory adderess contained in Pn, n{0..3}
 */
void o4(Vm* vm){
    vm->ACC = charArrayToInt(0, 6, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]); 
}

/*  
 * Load Accumulator Register Addressing
 * AC <- M(XX).
 * 
 * 05 XX --
 *
 * Load accumulator with contents of memory adderess contained in Pn, n{0..3}
 */
void o5(Vm* vm){
    vm->ACC = charArrayToInt(0, 6, vm->memory[charArrayToInt(2,4,vm->IR)]); 
}

/*  
 * Store Accumulator Register Addresing
 * M(Pn) <- AC.
 * 
 * 06 Pn --
 *
 * Store contents of accumulator to the memory location held in Pn
 */
void o6(Vm* vm){
    intToCharArray(vm->ACC, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]);
}

/*  
 * Store Accumulator Direct Addressing
 * M(XX) <- AC.
 * 
 * 07 XX --
 *
 * Store contents of AC into memory location XX, X{0..9}
 */
void o7(Vm* vm){
    intToCharArray(vm->ACC, vm->memory[charArrayToInt(2,4,vm->IR)]);
}

/*  
 * Store Register to memory: Register Addressing
 * M(Pn) <- Rn.
 * 
 * 08 Rn Pn
 *
 * Store contents of Register Rn into memory address pointed to by Pn, n{0..3}
 */
void o8(Vm* vm){
    intToCharArray(getRegister(vm, charToInt(vm->IR[3])), vm->memory[getPointer(vm, charToInt(vm->IR[5]))]);
}

/*  
 * Store Register to memory: Direct Addressing
 * M(XX) <- Rn.
 * 
 * 09 Rn XX
 *
 * Store contents of Register n, n{0..3} to memory location XX, X{0..9}
 */
void o9(Vm* vm){
    intToCharArray(getRegister(vm, charToInt(vm->IR[3])), vm->memory[charArrayToInt(4,6,vm->IR)]);             
}

/*  
 * Load Register from memory: Register Addressing
 * Rn <- M(Pn).
 * 
 * 10 Rn Pn
 *
 * Load Register Rn with the contents of memory location pointed to by Pn, n{0..3}
 */
void o10(Vm* vm){
    setRegister(vm, charToInt(vm->IR[3]), charArrayToInt(0,6,vm->memory[getPointer(vm, charToInt(vm->IR[5]))]));     
}

/*  
 * Load Register from memory: Direct Addressing
 * Rn <- M(XX).
 * 
 * 11 Rn XX
 *
 * Load Register Rn, n{0..3}, with the contents of memory location XX, X{0..9}
 */
void o11(Vm* vm){
    setRegister(vm, charToInt(vm->IR[3]), charArrayToInt(0,6,vm->memory[charArrayToInt(4,6,vm->IR)]));         
}

/*  
 * Load Register R0 Immediate
 * R0 <- XXXX.
 * 
 * 12 XX XX
 *
 * Load Register R0 with integer value XXXX, X{0..9}
 */
void o12(Vm* vm){
    setRegister(vm, 0, charArrayToInt(2,6,vm->IR));
}

/*  
 * Reister to Register Transfer
 * Rn <- [Rp].
 * 
 * 13 Rn Rp
 *
 * Rn is assigned the contents of Rp, n, p{0..3}
 */
void o13(Vm* vm){
    setRegister(vm, charToInt(vm->IR[3]), getRegister(vm, charToInt(vm->IR[5])));
}

/*  
 * Load Accumulator from Register
 * ACC <- [Rn].
 * 
 * 14 Rn 
 *
 * Accumulator assigned the contents of Rn, n{0..3}
 */
void o14(Vm* vm){
    vm->ACC = getRegister(vm, charToInt(vm->IR[3]));
}

/*  
 * Load Register from Accumulator
 * Rn <- [Acc].
 * 
 * 15 Rn
 *
 * Register Rn, n{0..3} assigned the contents of the accumulator
 */
void o15(Vm* vm){
    setRegister(vm, charToInt(vm->IR[3]),vm->ACC);  
}

/*  
 * Add Accumulator Immediate
 * AC <- AC + XXXX, X{0..9}.
 * 
 * 16 XX XX
 */
void o16(Vm* vm){
    vm->ACC += charArrayToInt(2,6,vm->IR);
}

/*  
 * Subtract Accumulator Immediate
 * AC <- AC - XXXX, X{0..9}.
 * 
 * 17 XX XX
 */
void o17(Vm* vm){
    vm->ACC -= charArrayToInt(2,6,vm->IR);
}


/*  
 * Add contents of Register to Accumulator
 * AC <- AC + Rn, n{0..3}.
 * 
 * 18 Rn --
 */
void o18(Vm* vm){
    vm->ACC += getRegister(vm, charToInt(vm->IR[3]));
}

/*  
 * Subtract contents of Register from Accumulator
 * AC <- AC - Rn, n{0..3}.
 * 
 * 19 Rn --
 */
void o19(Vm* vm){
    vm->ACC -= getRegister(vm, charToInt(vm->IR[3]));
}

/*  
 * Add Accumulator Register Addressing 
 * AC <- AC + M(Pn).
 * 
 * 20 Pn -- 
 *
 * Add contents of memory pointed to by Pn, n{0..3} to AC.
 */
void o20(Vm* vm){
    vm->ACC += charArrayToInt(0, 6, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]);
}

/*  
 * Add Accumulator Direct Addressing 
 * AC <- AC + M(XX).
 * 
 * 21 XX -- 
 *
 * Add contents of memory location XX to accumulator.
 */
void o21(Vm* vm){
    vm->ACC += charArrayToInt(0, 6, vm->memory[charArrayToInt(2,4,vm->IR)]);
}

/*  
 * Subtract from Accumulator Register Addressing 
 * AC <- AC - M(Pn).
 * 
 * 22 Pn -- 
 *
 * Subtract from accumulator contents of memory location XX
 */
void o22(Vm* vm){
    vm->ACC -= charArrayToInt(0, 6, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]);
}

/*  
 * Subtract from Accumulator Direct Addressing 
 * AC <- AC - M(XX), X{0..9}.
 * 
 * 23 XX -- 
 *
 * Subtract from accumulator contents of memory location XX
 */
void o23(Vm* vm){
    vm->ACC -= charArrayToInt(0, 6, vm->memory[charArrayToInt(2,4,vm->IR)]);
}

/*  
 * Compare Equal Register Addressing 
 * If AC == M(Pn) then PSW[0] = T else PSW[0] = F.
 * 
 * 24 Pn -- 
 *
 * If contents of accumulator are equal to contents of memory location pointed to by Pn,
 * n{0..3}, then byte 0 of the PSW set to T, else set to F.
 */
void o24(Vm* vm){
    int value = charArrayToInt(0, 6, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]);
    if(vm->ACC == value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Less Register Addressing 
 * If AC < M(Pn) then PSW[0] = T else PSW[0] = F.
 * 
 * 25 Pn -- 
 *
 * If contents of Accumulator are less than the contents of memory location pointed to by Pn, n{0..3},
 * then byte 0 of the PSW set to T, else set to F.
 */
void o25(Vm* vm){
    int value = charArrayToInt(0, 6, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]);
    if(vm->ACC < value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Greater Register Addressing 
 * If AC > M(Pn) then PSW[0] = T else PSW[0] = F.
 * 
 * 26 Pn -- 
 *
 * If contents of Accumulator are greater than the contents of memory location pointed to by Pn, n{0..3},
 * then byte 0 of the PSW set to T, else set to F.
 */
void o26(Vm* vm){
    int value = charArrayToInt(0, 6, vm->memory[getPointer(vm, charToInt(vm->IR[3]))]);
    if(vm->ACC > value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Greater Immediate 
 * If AC > XXXX,X{0..9}, then PSW[0] = T else PSW[0] = F 
 * 
 * 27 XX XX 
 *
 * If contents of Accumulator are greater than the value XXXX,
 * then byte 0 of the PSW set to T, else set to F.
 */
void o27(Vm* vm){
    int value = charArrayToInt(2,6,vm->IR);
    if(vm->ACC > value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Equal Immediate 
 * If AC == XXXX, X{0..9}, then PSW[0] = T else PSW[0] = F.
 * 
 * 28 XX XX 
 *
 * Compare contents of accumulator to integer
 */
void o28(Vm* vm){
    int value = charArrayToInt(2,6,vm->IR);
    if(vm->ACC == value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Less Immediate  
 * If AC < XXXX, X{0..9}, then PSW[0] = T else PSW[0] = F.
 * 
 * 29 XXXX
 *
 * Compare contents of accumulator to integer
 */
void o29(Vm* vm){
    int value = charArrayToInt(2,6,vm->IR);
    if(vm->ACC < value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}


/*  
 * Compare Register Equal 
 * If ACC == [Rn] then PSW[0] = T else PSW[0] = F.
 * 
 * 30 Rn  
 *
 * Compare contents of accumulator to contents in register Rn, n{0..3}
 */
void o30(Vm* vm){
    int value = getRegister(vm, charToInt(vm->IR[3]));
    if(vm->ACC == value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Register Less 
 * If ACC < [Rn], then PSW[0] = T else PSW[0] = F.
 * 
 * 30 Rn  
 *
 * Compare contents of accumulator to contents in register Rn, n{0..3}
 */
void o31(Vm* vm){
    int value = getRegister(vm, charToInt(vm->IR[3]));
    if(vm->ACC < value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Compare Register Greater 
 * If ACC > [Rn], then PSW[0] = T else PSW[0] = F.
 * 
 * 32 Rn  
 *
 * Compare contents of accumulator to contents in register Rn, n{0..3}
 */
void o32(Vm* vm){
    int value = getRegister(vm, charToInt(vm->IR[3]));
    if(vm->ACC > value){
        vm->PSW[0] = 'T';   
    }else{
        vm->PSW[0] = 'F';   
    }
}

/*  
 * Branch Conditional 
 * If PSW[0] = T then PC = XX, X{0..9}
 * 
 * 33 XX --  
 */
void o33(Vm* vm){
    if(vm->PSW[0] == 'T'){
        vm->PC = charArrayToInt(2,4,vm->IR);
    }
}

/*  
 * Branch Conditional 
 * If PSW[0] = F then PC = XX, X{0..9}
 * 
 * 34 XX --   
 */
void o34(Vm* vm){
    if(vm->PSW[0] == 'F'){
        vm->PC = charArrayToInt(2,4,vm->IR);
    }
}

/*  
 * Branch Unconditional 
 * PC == XX, X{0..9}
 * 
 * 35 XX --   
 */
void o35(Vm* vm){
    vm->PC = charArrayToInt(2,4,vm->IR);
}

/*  
 * HALT   
 */
void o99(Vm* vm){
    // HALT
    // call print memory function
    displayVmFinal(vm);

    exit(0);
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*  
 * START THE VM 
 */
void startVM(Vm* vm)
{
    vm->PC = 0;

    while (charArrayToInt(0,2,vm->IR) != 99)
    {
        nextInstruction(vm);

        switch (charArrayToInt(0,2,vm->IR)) 
        {
            case 0:
                o0(vm);
                break;
            case 1: 
                o1(vm);
                break;
            case 2:
                o2(vm);
                break;
            case 3:
                o3(vm);
                break;
            case 4:
                o4(vm);
                break;
            case 5:
                o5(vm);
                break;
            case 6:
                o6(vm);
                break;
            case 7:
                o7(vm);
                break;
            case 8:
                o8(vm);
                break;
            case 9:
                o9(vm);
                break;
            case 10:
                o10(vm);
                break;
            case 11:
                o11(vm);
                break;
            case 12:
                o12(vm);
                break;
            case 13:
                o13(vm);
                break;
            case 14:
                o14(vm);
                break;
            case 15:
                o15(vm);
                break;
            case 16:
                o16(vm);
                break;
            case 17:
                o17(vm);
                break;
            case 18:
                o18(vm);
                break;
            case 19:
                o19(vm);
                break;
            case 20:
                o20(vm);
                break;
            case 21:
                o21(vm);
                break;
            case 22:
                o22(vm);
                break;
            case 23:
                o23(vm);
                break;
            case 24:
                o24(vm);
                break;
            case 25:
                o25(vm);
                break;
            case 26:
                o26(vm);
                break;
            case 27:
                o27(vm);
                break;
            case 28:
                o28(vm);
                break;
            case 29:
                o29(vm);
                break;
            case 30:
                o30(vm);
                break;
            case 31:
                o31(vm);
                break;
            case 32:
                o32(vm);
                break;
            case 33:
                o33(vm);
                break;
            case 34:
                o34(vm);
                break;
            case 35:
                o35(vm);
                break;
            case 99:
                o99(vm);
                break;
            default:
                // Code
                break;
        }
        displayVmFinal(vm);
    }
}
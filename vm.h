/*
 *
 *
 * This is the heart of the VM 
 *
 * Author : Willie Stevenson
 *
 *
 */

#ifndef VM_H
#define VM_H

typedef struct vm {
    char IR[6];
    short int PC;

    short int P0;
    short int P1;
    short int P2;
    short int P3;

    int R0; 
    int R1;
    int R2;
    int R3;

    int SP;

    //int gap_between_program_space_and_storage = 10;
    int ACC;
    char PSW[2];
    char memory[100][6] ; //this is the program memory for first program
    short int opcode; //nice to know what we are doing
} Vm;

Vm* newVm();
void displayVm(Vm*);
void displayVmFinal(Vm*);
void startVM(Vm*);


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*- - - - - - - - - - VM FUNCTIONS - - - - - - - - - - - */

void o0(Vm* vm);
void o1(Vm* vm);
void o2(Vm* vm);
void o3(Vm* vm);
void o4(Vm* vm);
void o5(Vm* vm);
void o6(Vm* vm);
void o7(Vm* vm);
void o8(Vm* vm);
void o9(Vm* vm);
void o10(Vm* vm);
void o11(Vm* vm);
void o12(Vm* vm);
void o13(Vm* vm);
void o14(Vm* vm);
void o15(Vm* vm);
void o16(Vm* vm);
void o17(Vm* vm);
void o18(Vm* vm);
void o19(Vm* vm);
void o20(Vm* vm);
void o21(Vm* vm);
void o22(Vm* vm);
void o23(Vm* vm);
void o24(Vm* vm);
void o25(Vm* vm);
void o26(Vm* vm);
void o27(Vm* vm);
void o28(Vm* vm);
void o29(Vm* vm);
void o30(Vm* vm);
void o31(Vm* vm);
void o32(Vm* vm);
void o33(Vm* vm);
void o34(Vm* vm);
void o35(Vm* vm);
void o36(Vm* vm, int pid, int sys_call);
void o37(Vm* vm);
void o99(Vm* vm);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - -*/



#endif /* VM_H */
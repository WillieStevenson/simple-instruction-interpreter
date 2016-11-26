/*
 *
 *
 * Function detail strings 
 *
 * Author : Willie Stevenson
 *
 *
 */

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

const char* INSTRUCTION_NAMES[38] = {
	"Load Pointer Immediate",
	"Add to Pointer Immediate",
	"Subtract From Pointer Immediate",
	"Load Accumulator Immediate",
	"Load Accumulator Register Addressing",
	"Load Accumulator Direct Addressing",
	"Store Accumulator Register Addressing",
	"Store Accumulator Direct Addressing",
	"Store Register to memory: Register Addressing",
	"Store Register to memory: Direct Addressing",
	"Load Register from memory: Register Addressing",
	"Load Register from memory: Direct Addressing",
	"Load Register R0 Immediate",
	"Register to Register Transfer",
	"Load Accumulator from Register",
	"Load Register from Accumulator",
	"Add Accumulator Immediate",
	"Subtract Accumulator Immediate",
	"Add contents of Register to Accumulator",
	"Subtract contents of Register from Accumulator",
	"Add Accumulator Register Addressing",
	"Add Accumulator Direct Addressing",
	"Subtract from Accumulator Register Addressing",
	"Subtract form Accumulator Direct Addressing",
	"Compare Equal Register Addressing",
	"Compare Less Register Addressing",
	"Compare Greater Register Addressing",
	"Compare Greater Immediate",
	"Compare Equal Immediate",
	"Compare Less Immediate",
	"Compare Register Equal",
	"Compare Register Less",
	"Compare Register Greater",
	"Branch Conditional",
	"Branch Conditional",
	"Branch Unconditional",
	"Halt"
};



#endif /* INSTRUCTIONS_H */
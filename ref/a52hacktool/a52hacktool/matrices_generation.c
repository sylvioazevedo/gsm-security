/*============================================================================*
 *                                                                            *
 *                             matrices_generation.c                          *
 *                                                                            *
 *============================================================================*
 *                                                                            *
 * Part of A52HackTool                                                        *
 *                                                                            *
 * Copyright © 2011   -   Nicolas Paglieri   &   Olivier Benjamin             *
 * All rights reserved.                                                       *
 *                                                                            *
 * Contact Information:  nicolas.paglieri [at] ensimag.fr                     *
 *                       olivier.benjamin [at] ensimag.fr                     *
 *                                                                            *
 *============================================================================*
 *                                                                            *
 * This file may be used under the terms of the GNU General Public License    *
 * version 3 as published by the Free Software Foundation.                    *
 * See <http://www.gnu.org/licenses/> or GPL.txt included in the packaging of *
 * this file.                                                                 *
 *                                                                            *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE    *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  *
 *                                                                            *
 *============================================================================*/

 /**
  * @file matrices_generation.c
  * @brief Implementation of first attack phase: matrices dictionary generation
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #30
  * @date 01/12/2011
  * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "utils.h"

#include "matrices_generation.h"
#include "code.h"
#include "keygen.h"

/**
 * \struct GenerationArgs
 * \brief Set of arguments related to a thread in a multithreaded generation context
 *
 * GenerationArgs contains the bounds of data generation,
 * along with the file name to write the generated data subset to.
 */
struct GenerationArgs {
	int lowindex;       //!< Index to start the generation form (inclusive)
	int highindex;      //!< Last index to be processed (exclusive)
	char filename[255]; //!< Path of the file to write the generated data to
};




// vvvv   The following 4 functions are separated on purpose (because register ranges differ)   vvvv

/**
 * \fn void matrices_generation_clockR1(byte R1[R1_BITS][1+R1_SIMPLE_VARS])
 * \brief Clock register R1 contents
 *
 * \param[in, out] R1 Register to clock
 */
void matrices_generation_clockR1(byte R1[R1_BITS][1+R1_SIMPLE_VARS]) {
	byte tmp[1+R1_SIMPLE_VARS];
	memset(tmp, 0, (1+R1_SIMPLE_VARS)*sizeof(byte));
	XOR_CHARARRAYS(tmp, R1[R1_SHIFTTAP_1], 1+R1_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R1[R1_SHIFTTAP_2], 1+R1_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R1[R1_SHIFTTAP_3], 1+R1_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R1[R1_SHIFTTAP_4], 1+R1_SIMPLE_VARS);
	memmove(R1[1], R1[0], (R1_BITS-1)*(1+R1_SIMPLE_VARS)*sizeof(byte));
	memcpy( R1[0], tmp,   (1+R1_SIMPLE_VARS)*sizeof(byte));
}

/**
 * \fn void matrices_generation_clockR2(byte R2[R2_BITS][1+R2_SIMPLE_VARS])
 * \brief Clock register R2 contents
 *
 * \param[in, out] R2 Register to clock
 */
void matrices_generation_clockR2(byte R2[R2_BITS][1+R2_SIMPLE_VARS]) {
	byte tmp[1+R2_SIMPLE_VARS];
	memset(tmp, 0, (1+R2_SIMPLE_VARS)*sizeof(byte));
	XOR_CHARARRAYS(tmp, R2[R2_SHIFTTAP_1], 1+R2_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R2[R2_SHIFTTAP_2], 1+R2_SIMPLE_VARS);
	memmove(R2[1], R2[0], (R2_BITS-1)*(1+R2_SIMPLE_VARS)*sizeof(byte));
	memcpy( R2[0], tmp,   (1+R2_SIMPLE_VARS)*sizeof(byte));
}

/**
 * \fn void matrices_generation_clockR3(byte R3[R3_BITS][1+R3_SIMPLE_VARS])
 * \brief Clock register R3 contents
 *
 * \param[in, out] R3 Register to clock
 */
void matrices_generation_clockR3(byte R3[R3_BITS][1+R3_SIMPLE_VARS]) {
	byte tmp[1+R3_SIMPLE_VARS];
	memset(tmp, 0, (1+R3_SIMPLE_VARS)*sizeof(byte));
	XOR_CHARARRAYS(tmp, R3[R3_SHIFTTAP_1], 1+R3_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R3[R3_SHIFTTAP_2], 1+R3_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R3[R3_SHIFTTAP_3], 1+R3_SIMPLE_VARS);
	XOR_CHARARRAYS(tmp, R3[R3_SHIFTTAP_4], 1+R3_SIMPLE_VARS);
	memmove(R3[1], R3[0], (R3_BITS-1)*(1+R3_SIMPLE_VARS)*sizeof(byte));
	memcpy( R3[0], tmp,   (1+R3_SIMPLE_VARS)*sizeof(byte));
}

/**
 * \fn void matrices_generation_clockR4(byte R4[])
 * \brief Clock register R4 contents
 *
 * \param[in, out] R4 Register to clock
 */
void matrices_generation_clockR4(byte R4[]) {
	byte tmp = R4[R4_SHIFTTAP_1] ^ R4[R4_SHIFTTAP_2];
	memmove(R4+1, R4, (R4_BITS-1)*sizeof(byte));
	R4[0] = tmp;
}

// ^^^^  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .  ^^^^




/**
 * \fn void matrices_generation_clockingUnit(byte R1[R1_BITS][1+R1_SIMPLE_VARS], byte R2[R2_BITS][1+R2_SIMPLE_VARS], byte R3[R3_BITS][1+R3_SIMPLE_VARS], byte R4[R4_BITS], int clockAll)
 * \brief Performs register clocking according to the rules of the Clocking Unit
 *
 * \param[in, out] R1 First LFSR
 * \param[in, out] R2 Second LFSR
 * \param[in, out] R3 Third LFSR
 * \param[in, out] R4 Fourth LFSR
 * \param[in] clockAll When non-zero, bypass Clocking Unit decision: always clock (initialization phase)
 */
void matrices_generation_clockingUnit(byte R1[R1_BITS][1+R1_SIMPLE_VARS], \
                                      byte R2[R2_BITS][1+R2_SIMPLE_VARS], \
                                      byte R3[R3_BITS][1+R3_SIMPLE_VARS], \
                                      byte R4[R4_BITS], int clockAll) {

	byte maj = MAJORITY(R4[R4_CLOCKTAP_R1], R4[R4_CLOCKTAP_R2], R4[R4_CLOCKTAP_R3]);
	if (clockAll || maj==R4[R4_CLOCKTAP_R1])
		matrices_generation_clockR1(R1);
	if (clockAll || maj==R4[R4_CLOCKTAP_R2])
		matrices_generation_clockR2(R2);
	if (clockAll || maj==R4[R4_CLOCKTAP_R3])
		matrices_generation_clockR3(R3);
	matrices_generation_clockR4(R4);
}




/**
 * \fn void matrices_generation_doubleProduct(const byte a1[], const byte a2[], byte out[])
 * \brief Performs a double product of two vectors of variables
 *
 * We assume here that the given vectors are correctly formed, meaning that all the present
 * variables either belong to the same LFSR simple set, or represent the constant value "1".
 *
 * \param[in]  a1 First Vector
 * \param[in]  a2 Second Vector
 * \param[out] out Product of a1 and a2
 */
void matrices_generation_doubleProduct(const byte a1[], const byte a2[], byte out[]) {

	memset(out, 0, REGS_TOTAL_VARS*sizeof(byte));

	for (int i=0 ; i<REGS_TOTAL_VARS ; ++i) {
		if (!a1[i]) continue;
		if (i>=REGS_SIMPLE_VARS) i=REGS_TOTAL_VARS-1; // skip double variables

		for (int j=0 ; j<REGS_TOTAL_VARS ; ++j) {
			if (!a2[j]) continue;
			if (j>=REGS_SIMPLE_VARS) j=REGS_TOTAL_VARS-1; // skip double variables

			// The double products are ordered by increasing LFSR id, then by indexes pair
			// Identical operands (eg. x0.x0) are not represented in the variables array (simplified)
			// Products including const "1" are also simplified
			// x0.x1
			// x0.x2
			// x0.x3
			// x0.[...]
			// x0.x18
			// x1.x2
			// x1.x3
			// x1.[...]
			// x1.x18
			// x2.x3
			// ...

			unsigned int v1 = MIN(i,j);
			unsigned int v2 = MAX(i,j);
			// DEBUG("double product %d %d: ", v1, v2);
			unsigned int vprod = 0;

			if (v1==v2) {
				out[v1] ^= 1;
				// DEBUG("into single (%d)\n",v1);
				continue; // nothing else to be done here (single term)
			} else { // v1 < v2

				if (v1 == REGS_TOTAL_VARS-1) { // constant "1"
					out[v2] ^= 1;
					// DEBUG("by constant 1 (%d)\n",v2);
				} else if (v2 == REGS_TOTAL_VARS-1) {
					out[v1] ^= 1;
					// DEBUG("by constant 1 (%d)\n",v1);
				} else {
					if (v1 < R1_SIMPLE_VARS) { // variables belong to LFSR R1
						vprod = (REGS_SIMPLE_VARS-1) + (R1_SIMPLE_VARS-2)*v1 - v1*(v1-1)/2 + v2;
					} else if (v1 < R1_SIMPLE_VARS+R2_SIMPLE_VARS) { // variables belong to LFSR R2
						v1 -= R1_SIMPLE_VARS;
						v2 -= R1_SIMPLE_VARS;
						vprod = (REGS_SIMPLE_VARS+R1_PROD_VARS-1) + (R2_SIMPLE_VARS-2)*v1 - v1*(v1-1)/2 + v2;
					} else if (v1 < R1_SIMPLE_VARS+R2_SIMPLE_VARS+R3_SIMPLE_VARS) { // variables belong to LFSR R3
						v1 -= R1_SIMPLE_VARS + R2_SIMPLE_VARS;
						v2 -= R1_SIMPLE_VARS + R2_SIMPLE_VARS;
						vprod = (REGS_SIMPLE_VARS+R1_PROD_VARS+R2_PROD_VARS-1) + (R3_SIMPLE_VARS-2)*v1 - v1*(v1-1)/2 + v2;
					} else {
						DEBUG("Error: found double variable (#%d) in source vectors. Returning inconsistent result", v1);
						return;
					}
					out[vprod] ^= 1;
					// DEBUG("processed %d\n",vprod);
				}
			}


		}
	}

}




/**
 * \fn void matrices_generation_getOutBit(byte R1[R1_BITS][1+R1_SIMPLE_VARS], byte R2[R2_BITS][1+R2_SIMPLE_VARS], byte R3[R3_BITS][1+R3_SIMPLE_VARS], byte out[REGS_TOTAL_VARS])
 * \brief Returns the current out keystream bit (here a set of xored variables) corresponding to a particular LFSR state
 *
 * \param[in]  R1 First LFSR
 * \param[in]  R2 Second LFSR
 * \param[in]  R3 Third LFSR
 * \param[out] out Set of variables describing the current out keystream bit
 */
void matrices_generation_getOutBit(byte R1[R1_BITS][1+R1_SIMPLE_VARS], \
                                   byte R2[R2_BITS][1+R2_SIMPLE_VARS], \
                                   byte R3[R3_BITS][1+R3_SIMPLE_VARS], \
                                   byte out[REGS_TOTAL_VARS]) {

	// The majority function can here be defined as a xored product:  maj(a,b,c) = a.b ^ a.c ^ c.a

	byte op1[REGS_TOTAL_VARS];
	byte op2[REGS_TOTAL_VARS];
	byte op3[REGS_TOTAL_VARS];
	byte sum[REGS_TOTAL_VARS];
	byte prod[REGS_TOTAL_VARS];

	#define NULLIFY(op)      do {memset(op, 0, REGS_TOTAL_VARS*sizeof(byte));} while(0)

	NULLIFY(out);
	NULLIFY(op1); NULLIFY(op2); NULLIFY(op3); NULLIFY(sum); NULLIFY(prod);


	// First we need to transform operands from specific LFSR type to global type
	// discarding the first index (const "1") during the copy, translating it to the end of the set

	#define LOADR1(op,tap,x) do {NULLIFY(op);                                                      \
	                             memcpy((op), R1[(tap)]+sizeof(byte), R1_SIMPLE_VARS*sizeof(byte));\
	                             op[REGS_TOTAL_VARS-1] = R1[(tap)][0] ^ ((x)&1);                   \
	                         } while(0)
	#define LOADR2(op,tap,x) do {NULLIFY(op);                                                      \
	                             memcpy((op)+R1_SIMPLE_VARS, R2[(tap)]+sizeof(byte), R2_SIMPLE_VARS*sizeof(byte));\
	                             op[REGS_TOTAL_VARS-1] = R2[(tap)][0] ^ ((x)&1);                   \
	                         } while(0)
	#define LOADR3(op,tap,x) do {NULLIFY(op);                                                      \
	                             memcpy((op)+R1_SIMPLE_VARS+R2_SIMPLE_VARS, R3[(tap)]+sizeof(byte), R3_SIMPLE_VARS*sizeof(byte));\
	                             op[REGS_TOTAL_VARS-1] = R3[(tap)][0] ^ ((x)&1);                   \
	                         } while(0)


	// R1 RELATED PROCESSING - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	LOADR1(op1, R1_OUTTAP_1, 0);
	LOADR1(op2, R1_OUTTAP_2, 1); // xor 1
	LOADR1(op3, R1_OUTTAP_3, 0);

	matrices_generation_doubleProduct(op1, op2, prod);
	matrices_generation_doubleProduct(op2, op3, sum);
	XOR_CHARARRAYS(sum, prod, REGS_TOTAL_VARS);
	matrices_generation_doubleProduct(op1, op3, prod);
	XOR_CHARARRAYS(sum, prod, REGS_TOTAL_VARS);

	LOADR1(op1, (R1_BITS-1), 0);
	XOR_CHARARRAYS(sum, op1, REGS_TOTAL_VARS);

	memcpy(out, sum, REGS_TOTAL_VARS*sizeof(byte));


	// R2 RELATED PROCESSING - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	LOADR2(op1, R2_OUTTAP_1, 0);
	LOADR2(op2, R2_OUTTAP_2, 0);
	LOADR2(op3, R2_OUTTAP_3, 1); // xor 1

	matrices_generation_doubleProduct(op1, op2, prod);
	matrices_generation_doubleProduct(op2, op3, sum);
	XOR_CHARARRAYS(sum, prod, REGS_TOTAL_VARS);
	matrices_generation_doubleProduct(op1, op3, prod);
	XOR_CHARARRAYS(sum, prod, REGS_TOTAL_VARS);

	LOADR2(op1, (R2_BITS-1), 0);
	XOR_CHARARRAYS(sum, op1, REGS_TOTAL_VARS);

	XOR_CHARARRAYS(out, sum, REGS_TOTAL_VARS);


	// R3 RELATED PROCESSING - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	LOADR3(op1, R3_OUTTAP_1, 1); // xor 1
	LOADR3(op2, R3_OUTTAP_2, 0);
	LOADR3(op3, R3_OUTTAP_3, 0);

	matrices_generation_doubleProduct(op1, op2, prod);
	matrices_generation_doubleProduct(op2, op3, sum);
	XOR_CHARARRAYS(sum, prod, REGS_TOTAL_VARS);
	matrices_generation_doubleProduct(op1, op3, prod);
	XOR_CHARARRAYS(sum, prod, REGS_TOTAL_VARS);

	LOADR3(op1, (R3_BITS-1), 0);
	XOR_CHARARRAYS(sum, op1, REGS_TOTAL_VARS);

	XOR_CHARARRAYS(out, sum, REGS_TOTAL_VARS);

	#undef NULLIFY
	#undef LOADR1
	#undef LOADR2
	#undef LOADR3

}




/**
 * \fn void matrices_generation_processKeystreamEqns(const byte initialR4[], byte keystream[][REGS_TOTAL_VARS], const int len)
 * \brief Generates the desired amount of keystream dependancy equations (skipping first 99cycles) considering one initial R4 state
 *
 * \param[in]  initialR4 Inferred initial value of the fourth LFSR
 * \param[out] keystream Keystream dependancy equations to be generated
 * \param[in]  len Desired amount of keystream dependancy equations
 */
void matrices_generation_processKeystreamEqns(const byte initialR4[], byte keystream[][REGS_TOTAL_VARS], const int len) {

	memset(keystream, 0, len*REGS_TOTAL_VARS*sizeof(byte));

	// For registers R1 R2 & R3, the first index (0) represents the constant "1"
	byte R1[R1_BITS][1+R1_SIMPLE_VARS];
	byte R2[R2_BITS][1+R2_SIMPLE_VARS];
	byte R3[R3_BITS][1+R3_SIMPLE_VARS];
	byte R4[R4_BITS];

	memset(R1, 0, R1_BITS*(1+R1_SIMPLE_VARS)*sizeof(byte));
	memset(R2, 0, R2_BITS*(1+R2_SIMPLE_VARS)*sizeof(byte));
	memset(R3, 0, R3_BITS*(1+R3_SIMPLE_VARS)*sizeof(byte));
	memcpy(R4, initialR4, R4_BITS*sizeof(byte));


	// Initial R4 validity test
	if (R4[R4_INITIAL_CONST_POS] != 1) {
		DEBUG("ERROR: Processing keystream from invalid R4 vector (bit #%d must be set to 1): Returning 0\n",R4_INITIAL_CONST_POS);
		return;
	}


	// LFSR variables & constants initialization
	for (int i=0 ; i<R1_BITS ; ++i)
		R1[ i ][ (i==R1_INITIAL_CONST_POS) ? 0 : ((i>R1_INITIAL_CONST_POS) ? i : (i+1)) ] = 1;
	for (int i=0 ; i<R2_BITS ; ++i)
		R2[ i ][ (i==R2_INITIAL_CONST_POS) ? 0 : ((i>R2_INITIAL_CONST_POS) ? i : (i+1)) ] = 1;
	for (int i=0 ; i<R3_BITS ; ++i)
		R3[ i ][ (i==R3_INITIAL_CONST_POS) ? 0 : ((i>R3_INITIAL_CONST_POS) ? i : (i+1)) ] = 1;


	// First 99cycles of pre-processing (output discarded): in fact, discard 98
	for (int i=0 ; i<100 ; ++i) {
		matrices_generation_clockingUnit(R1, R2, R3, R4, 0);
	}


	// Next cycles: output placed in keystream
	byte tmp[REGS_TOTAL_VARS];

	for (int i=0 ; i<len ; ++i) {

		matrices_generation_getOutBit(R1, R2, R3, tmp); // bit export
		memcpy(keystream[i], tmp, REGS_TOTAL_VARS*sizeof(byte));

		matrices_generation_clockingUnit(R1, R2, R3, R4, 0);
	}

}




/**
 * \fn int matrices_generation_getRealVariableIndex(unsigned int Reg, unsigned int index)
 * \brief Processes the correct location of a variable inside a LFSR from the variable index
 *
 * Only variables were recorded in equations, thus there is a small gap in the location of the
 * constant "1" in the LFSR. This function is mandatory to allow a transparent access to the array.
 *
 * \param[in] Reg Considered register id. May be (1), (2) or (3)
 * \param[in] index Variable index
 * \return Fixed-up location inside the LFSR
 */
int matrices_generation_getRealVariableIndex(unsigned int Reg, unsigned int index) {
	switch (Reg) {
		case 1:
			return (index<R1_INITIAL_CONST_POS) ? index : index+1;
		case 2:
			return (index<R2_INITIAL_CONST_POS) ? index : index+1;
		case 3:
			return (index<R3_INITIAL_CONST_POS) ? index : index+1;
		default:
			DEBUG("Error: Invalid Register Identifier: Returning 0");
			return 0;
	}
}




/**
 * \fn byte matrices_generation_solveEquation(byte eqn[], byte R1[], byte R2[], byte R3[])
 * \brief Returns the result of the evaluation of an expression with the given values of initial variables (for test purpose)
 *
 * \param[in] eqn Considered expression
 * \param[in] R1 Contents of the first LFSR
 * \param[in] R2 Contents of the second LFSR
 * \param[in] R3 Contents of the third LFSR
 * \return Value of the expression described by eqn, after substituting LFSR values for variables
 */
byte matrices_generation_solveEquation(byte eqn[], byte R1[], byte R2[], byte R3[]) {

	byte res = 0;

	// Processing single vars
	for (int i=0 ; i<R1_SIMPLE_VARS ; ++i) {
		if (eqn[i])
			res ^= R1[matrices_generation_getRealVariableIndex(1,i)] & 1;
	}
	for (int i=0 ; i<R2_SIMPLE_VARS ; ++i) {
		if (eqn[i+R1_SIMPLE_VARS])
			res ^= R2[matrices_generation_getRealVariableIndex(2,i)] & 1;
	}
	for (int i=0 ; i<R3_SIMPLE_VARS ; ++i) {
		if (eqn[i+R1_SIMPLE_VARS+R2_SIMPLE_VARS])
			res ^= R3[matrices_generation_getRealVariableIndex(3,i)] & 1;
	}

	// Processing terminal "1" constant
	if (eqn[REGS_TOTAL_VARS-1])
		res ^= 1;

	// Processing double vars
	int i=REGS_SIMPLE_VARS; // iterator for convenience
	for (int v1=0 ; v1<R1_SIMPLE_VARS ; ++v1) {
		for (int v2=v1+1 ; v2<R1_SIMPLE_VARS ; ++v2) {
			if (eqn[i])
				res ^= (R1[matrices_generation_getRealVariableIndex(1,v1)] & R1[matrices_generation_getRealVariableIndex(1,v2)]) & 1;
			++i;
		}
	}
	for (int v1=0 ; v1<R2_SIMPLE_VARS ; ++v1) {
		for (int v2=v1+1 ; v2<R2_SIMPLE_VARS ; ++v2) {
			if (eqn[i])
				res ^= (R2[matrices_generation_getRealVariableIndex(2,v1)] & R2[matrices_generation_getRealVariableIndex(2,v2)]) & 1;
			++i;
		}
	}
	for (int v1=0 ; v1<R3_SIMPLE_VARS ; ++v1) {
		for (int v2=v1+1 ; v2<R3_SIMPLE_VARS ; ++v2) {
			if (eqn[i])
				res ^= (R3[matrices_generation_getRealVariableIndex(3,v1)] & R3[matrices_generation_getRealVariableIndex(3,v2)]) & 1;
			++i;
		}
	}

	return res & 1;
}




// Documentation in header file
void getR4fromIndex(const int index, byte R4[R4_BITS]) {
	memset(R4, 0, R4_BITS);
	for (int k=0 ; k < R4_INITIAL_CONST_POS ; ++k)
		R4[k] = (index>>k)&1;
	R4[R4_INITIAL_CONST_POS] = 1;
	for (int k=R4_INITIAL_CONST_POS ; k < (R4_BITS-1) ; ++k)
		R4[k+1] = (index>>k)&1;
}




/**
 * \fn int matrices_generation_exportMatrices(const char* filename, const int lowindex, const int highindex)
 * \brief Thread generation method
 *
 * \param[in] filename Path of the file to write the generated data to
 * \param[in] lowindex Index to start the generation form (inclusive)
 * \param[in] highindex Last index to be processed (exclusive)
 * \return Number of generated matrices
 */
int matrices_generation_exportMatrices(const char* filename, const int lowindex, const int highindex) {

    printf("File name: %d", lowindex);
    
	FILE* destfile = fopen(filename, "w+");
    
	if (!destfile) {
        DEBUG("Error: failed to open %s\nAborting operation", filename);
        return 0;
    }

	// Code Matrix & Parity-Check Matrix needed to process Resolution Matrices
	byte G[SOURCEWORD_LENGTH][CODEWORD_LENGTH];
	processFullEncodingGMatrix(G);
	byte H[SYNDROME_LENGTH][CODEWORD_LENGTH];
	processFullEncodingHMatrix(G, H);

	// Since the (R4_INITIAL_CONST_POS)-th bit in R4 is constant value "1" whatever happens,
	// we are going to use an incremential vector of (R4_BITS-1) bits that will be merged with
	// the constant to generate the 2^(R4_BITS-1) initialization vectors, each generating 1 matrix

	byte R4[R4_BITS];
	memset(R4, 0, (R4_BITS)*sizeof(byte));
	R4[R4_INITIAL_CONST_POS] = 1; // This element will never be modified

	// Set of keystream equations obtained from register initial state
	byte keystreamEqns[EQN_SYSTEM_SIZE][REGS_TOTAL_VARS];
	// Resolution matrix
	byte HS[NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH][REGS_TOTAL_VARS];

	// The way we traverse all possible values for R4 have no importance. We chose the faster way,
	// which consists of using some mirrored version of i to fill the variable bits of R4.

	for (int i=lowindex ; i<highindex ; ++i) {

		if (i%128==0) {
			printf("Thread #%d:  \tGenerating matrix #%d \t", lowindex/THREAD_CHUNKSIZE, i);
			PROGRESSBAR((i-lowindex)*100/(highindex-lowindex));
		}

		// R4 Setting
		for (int k=0 ; k < R4_INITIAL_CONST_POS ; ++k)
			R4[k] = (i>>k)&1;
		// R4[R4_INITIAL_CONST_POS] is already set to 1
		for (int k=R4_INITIAL_CONST_POS ; k < (R4_BITS-1) ; ++k)
			R4[k+1] = (i>>k)&1;

		matrices_generation_processKeystreamEqns(R4, keystreamEqns, EQN_SYSTEM_SIZE);

		// We process HS via local sub-products of H with parts of keystreamEqns
		for (int i=0 ; i<NEEDED_ENCRYPTED_MESSAGES ; ++i) {
			BINPRODUCT_MATRIX_MATRIX(H, keystreamEqns+i*CODEWORD_LENGTH, HS+i*SYNDROME_LENGTH, SYNDROME_LENGTH, REGS_TOTAL_VARS, CODEWORD_LENGTH);
		}

		// Export to file
		byte buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		for (int k=0 ; k<BUFFER_SIZE ; ++k) {
			buffer[k] = ((HS[0][8*k  ]&1) << 7)
			          | ((HS[0][8*k+1]&1) << 6)
			          | ((HS[0][8*k+2]&1) << 5)
			          | ((HS[0][8*k+3]&1) << 4)
			          | ((HS[0][8*k+4]&1) << 3)
			          | ((HS[0][8*k+5]&1) << 2)
			          | ((HS[0][8*k+6]&1) << 1)
			          | ((HS[0][8*k+7]&1)     );
		}
		if (fwrite(buffer, sizeof(byte), BUFFER_SIZE, destfile) != BUFFER_SIZE) {
			DEBUG("Error: couldn't write out matrix #%d to file", i);
			return (i-lowindex);
		}


	}

	fclose(destfile);

	printf("Thread #%d:  \tAll Matrices Generated [%d-%d] \t", lowindex/THREAD_CHUNKSIZE, lowindex, highindex);
	PROGRESSBAR(100);

	return (highindex-lowindex);

}




/**
 * \fn void* matrices_generation_launchExport(void* data)
 * \brief Thread initializer
 *
 * \param[in] data Pointer to the thread's arguments
 * \return NULL
 */
void* matrices_generation_launchExport(void* data) {
	
    struct GenerationArgs *args = data;
    
	matrices_generation_exportMatrices(args->filename, args->lowindex, args->highindex);
	return NULL;
}




// Documentation in header file
int exportAllMatrices(const char* filename) {

	time_t datetime = time(NULL);
	struct tm *local = localtime(&datetime);
	DEBUG("Matrices Generation started on %s", asctime(local));

	pthread_t *t = malloc(PROCESSING_THREADS*sizeof(pthread_t));
	struct GenerationArgs args[PROCESSING_THREADS];

	// Thread & Arguments Creation
	for (int i=0 ; i<PROCESSING_THREADS ; ++i) {

		sprintf(args[i].filename, "bin/tmp0x%08X.bin", i);
		args[i].lowindex  =   i   * THREAD_CHUNKSIZE;
		args[i].highindex = (i+1) * THREAD_CHUNKSIZE;

		if (pthread_create(&t[i], NULL, matrices_generation_launchExport, &args[i])) {
			DEBUG("Unable to create thread #%d\nKilling process.", i);
			exit(1);
		}
	}


	// Thread Joining
	for (int i=0 ; i<PROCESSING_THREADS ; ++i) {
		pthread_join(t[i], NULL);
	}
	free(t);


	// Files Merging
	datetime = time(NULL);
	local = localtime(&datetime);
	DEBUG("Starting Merge on %s", asctime(local));

	FILE* destfile = fopen(filename, "wb");
	for (int i=0 ; i<PROCESSING_THREADS ; ++i) {

		FILE* sourcefile = fopen(args[i].filename, "rb");

		byte buffer[BUFFER_SIZE];
		for (int k=0 ; k<THREAD_CHUNKSIZE ; k++) {

			memset(buffer, 0, BUFFER_SIZE);
			if (fread(buffer, sizeof(byte), BUFFER_SIZE, sourcefile) != BUFFER_SIZE) {
				DEBUG("Error: Unable to read matrix #%d in file %s", k, args[i].filename);
				return 1;
			}
			if (fwrite(buffer, sizeof(byte), BUFFER_SIZE, destfile) != BUFFER_SIZE) {
				DEBUG("Error: couldn't write out matrix #%d to destination file", k);
				return 1;
			}

		}

		fclose(sourcefile);

		if (remove(args[i].filename)) {
			DEBUG("Warning: file '%s' couldn't be deleted. Please remove it manually.", args[i].filename);
		}

	}
	fclose(destfile);

	datetime = time(NULL);
	local = localtime(&datetime);
	DEBUG("File Generated on %s", asctime(local));
	return 0;

}




// Documentation in header file
int matrices_generation_test() {

	byte secretKey[SECRETKEY_BITS] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	byte frameId[FRAMEID_BITS] = {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	// Original values of registers R1, R2 & R3 after keysetup (without the 99 discarded cycles)
	byte R1[R1_BITS] = {1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1,1};
	byte R2[R2_BITS] = {0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1};
	byte R3[R3_BITS] = {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1};

	// Original value of R4 after keysetup (without the 99 discarded cycles)
	byte initialR4[R4_BITS] = {1,1,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0};
	// From it, we generate equations on the bits of R1, R2 & R3
	byte keystreamEqns[228][REGS_TOTAL_VARS];
	matrices_generation_processKeystreamEqns(initialR4, keystreamEqns, 228);

	// Original keystream given by A5/2 cipher
	byte keystream[228];
	memset(keystream, 0, 228*sizeof(byte));
	keysetup(secretKey, frameId);
	getKeystream(keystream, 228);

	// We check that each equation we found coincide with the original data
	for (int i=0 ; i<228 ; ++i) {
		byte tmp = matrices_generation_solveEquation(keystreamEqns[i], R1, R2, R3);
		if (keystream[i] != tmp) {
			DEBUG("Self-check aborted: a discrepancy was found in the equations.");
			return 1;
		}
	}

	DEBUG("Self-check succeeded: the equations are all right");
	return 0;

}

/*============================================================================*
 *                                                                            *
 *                              keysetup_reverse.c                            *
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
  * @file keysetup_reverse.c
  * @brief Implementation of last attack phase on A5/2: keysetup reversal
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #8
  * @date 24/06/2011
  * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#include "keysetup_reverse.h"




/**
 * \struct keysetup_reverse_LFSRSet_t
 * \brief Set of all LFSR's variables
 *
 * This structure establishes the correspondance between the LFSR values and the keystream unknowns.
 * The last index of the second dimension (+1) represents the constant "1".
 * Other indexes represent standard variables.
 */
typedef struct {
	byte R1[R1_BITS][SECRETKEY_BITS+1]; //!< R1 expressed from keystream unknowns
	byte R2[R2_BITS][SECRETKEY_BITS+1]; //!< R2 expressed from keystream unknowns
	byte R3[R3_BITS][SECRETKEY_BITS+1]; //!< R3 expressed from keystream unknowns
	byte R4[R4_BITS][SECRETKEY_BITS+1]; //!< R4 expressed from keystream unknowns
} keysetup_reverse_LFSRSet_t;

//! Pointer to a set of LFSR's variables
typedef keysetup_reverse_LFSRSet_t * keysetup_reverse_LFSRSet;




/**
 * \fn void keysetup_reverse_clockRegs(keysetup_reverse_LFSRSet LFSRs)
 * \brief Clock all registers (during the keysetup phase, the clocking unit is unused).
 *
 * \param[in, out] LFSRs Pointer to the set of LFSR's variables to clock
 */
void keysetup_reverse_clockRegs(keysetup_reverse_LFSRSet LFSRs) {

	byte carry[SECRETKEY_BITS+1];

	// R1
	memset(carry, 0, (SECRETKEY_BITS+1)*sizeof(byte));
	XOR_CHARARRAYS(carry, LFSRs->R1[13], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R1[16], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R1[17], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R1[18], SECRETKEY_BITS+1);
	memmove(LFSRs->R1[1], LFSRs->R1[0], (R1_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
	memcpy(LFSRs->R1[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

	// R2
	memset(carry, 0, (SECRETKEY_BITS+1)*sizeof(byte));
	XOR_CHARARRAYS(carry, LFSRs->R2[20], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R2[21], SECRETKEY_BITS+1);
	memmove(LFSRs->R2[1], LFSRs->R2[0], (R2_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
	memcpy(LFSRs->R2[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

	// R3
	memset(carry, 0, (SECRETKEY_BITS+1)*sizeof(byte));
	XOR_CHARARRAYS(carry, LFSRs->R3[ 7], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R3[20], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R3[21], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R3[22], SECRETKEY_BITS+1);
	memmove(LFSRs->R3[1], LFSRs->R3[0], (R3_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
	memcpy(LFSRs->R3[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

	// R4
	memset(carry, 0, (SECRETKEY_BITS+1)*sizeof(byte));
	XOR_CHARARRAYS(carry, LFSRs->R4[11], SECRETKEY_BITS+1);
	XOR_CHARARRAYS(carry, LFSRs->R4[16], SECRETKEY_BITS+1);
	memmove(LFSRs->R4[1], LFSRs->R4[0], (R4_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
	memcpy(LFSRs->R4[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

}




// Documentation in header file
int reverseKeysetup(const byte R1[R1_BITS], const byte R2[R2_BITS], const byte R3[R3_BITS],        \
                    const byte R4[R4_BITS], const byte frameId[FRAMEID_BITS],                      \
                    byte secretKey[SECRETKEY_BITS]) {

	memset(secretKey, 0, SECRETKEY_BITS*sizeof(byte));

	keysetup_reverse_LFSRSet LFSRs = malloc(sizeof(keysetup_reverse_LFSRSet_t));
	memset(LFSRs, 0, sizeof(keysetup_reverse_LFSRSet_t));


	// -------------------------------------------------------
	// Keysetup steps mirroring using secret key variables...
	// -------------------------------------------------------

	byte Kc[SECRETKEY_BITS+1];

	for (int i=0 ; i<SECRETKEY_BITS ; ++i) {
		keysetup_reverse_clockRegs(LFSRs);
		memset(Kc, 0, (SECRETKEY_BITS+1)*sizeof(char));
		Kc[i] = 1;
		XOR_CHARARRAYS(LFSRs->R1[0], Kc, SECRETKEY_BITS+1);
		XOR_CHARARRAYS(LFSRs->R2[0], Kc, SECRETKEY_BITS+1);
		XOR_CHARARRAYS(LFSRs->R3[0], Kc, SECRETKEY_BITS+1);
		XOR_CHARARRAYS(LFSRs->R4[0], Kc, SECRETKEY_BITS+1);
	}

	for (int i=0 ; i<FRAMEID_BITS ; ++i) {
		keysetup_reverse_clockRegs(LFSRs);
		if (frameId[i] == 1) {
			LFSRs->R1[0][SECRETKEY_BITS] ^= 1;
			LFSRs->R2[0][SECRETKEY_BITS] ^= 1;
			LFSRs->R3[0][SECRETKEY_BITS] ^= 1;
			LFSRs->R4[0][SECRETKEY_BITS] ^= 1;
		}
	}
	memset(LFSRs->R1[15], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R1[15][SECRETKEY_BITS] = 1;
	memset(LFSRs->R2[16], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R2[16][SECRETKEY_BITS] = 1;
	memset(LFSRs->R3[18], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R3[18][SECRETKEY_BITS] = 1;
	memset(LFSRs->R4[10], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R4[10][SECRETKEY_BITS] = 1;


	// -------------------------------------------------------
	// Linear System Generation...
	// -------------------------------------------------------

	// Initial state of R1->R4 LFSRs is determined by the previous deciphering step (given as param)
	byte regStatus[REGS_BITS];
	memset(regStatus, 0, REGS_BITS*sizeof(byte));
	memcpy(regStatus,                         R1, R1_BITS*sizeof(byte));
	memcpy(regStatus+R1_BITS,                 R2, R2_BITS*sizeof(byte));
	memcpy(regStatus+R1_BITS+R2_BITS,         R3, R3_BITS*sizeof(byte));
	memcpy(regStatus+R1_BITS+R2_BITS+R3_BITS, R4, R4_BITS*sizeof(byte));

	// Concatenate in a line the 4 matrices of equations into a single linear system,
	// thus changing data representation [line/col]=>[col/line]
	byte linearSystem[REGS_BITS][SECRETKEY_BITS];
	memset(linearSystem, 0, (REGS_BITS)*SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R1_BITS ; ++i)
		memcpy(linearSystem[i], LFSRs->R1[i], SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R2_BITS ; ++i)
		memcpy(linearSystem[i+R1_BITS], LFSRs->R2[i], SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R3_BITS ; ++i)
		memcpy(linearSystem[i+R1_BITS+R2_BITS], LFSRs->R3[i], SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R4_BITS ; ++i)
		memcpy(linearSystem[i+R1_BITS+R2_BITS+R3_BITS], LFSRs->R4[i], SECRETKEY_BITS*sizeof(char));

	// -------------------------------------------------------
	// Construction step done. Now entering resolution...
	// -------------------------------------------------------

	// Detach last line (line representing ones), xor with provided result from previous decrypting
	// step, and simply solve the system of 64 unknown quantities and 19+22+23+17 = 81 equations
	// (4 of them are just 0=0, some others are dependant...)
	for (int i=0 ; i<R1_BITS ; ++i)
		regStatus[i] ^= LFSRs->R1[i][SECRETKEY_BITS];
	for (int i=0 ; i<R2_BITS ; ++i)
		regStatus[i+R1_BITS] ^= LFSRs->R2[i][SECRETKEY_BITS];
	for (int i=0 ; i<R3_BITS ; ++i)
		regStatus[i+R1_BITS+R2_BITS] ^= LFSRs->R3[i][SECRETKEY_BITS];
	for (int i=0 ; i<R4_BITS ; ++i)
		regStatus[i+R1_BITS+R2_BITS+R3_BITS] ^= LFSRs->R4[i][SECRETKEY_BITS];

	// Gauss Elimination
	byte headFactor = 0;
	for (int col=0 ; col<SECRETKEY_BITS ; ++col) {
		for (int line=col ; line<REGS_BITS ; ++line) {
			// Pivot finding
			headFactor = linearSystem[line][col];
			if (headFactor) {
				// Line Swap if necessary
				if (line!=col) {
					byte temp;
					for (int c=0 ; c<SECRETKEY_BITS ; ++c) {
						temp = linearSystem[col][c];
						linearSystem[col][c]  = linearSystem[line][c];
						linearSystem[line][c] = temp;
					}
					temp = regStatus[col];
					regStatus[col] = regStatus[line];
					regStatus[line] = temp;
				}
				// Elimination
				for (int l=col+1 ; l<REGS_BITS ; ++l) {
					if (linearSystem[l][col]) {
						for (int c=col; c<SECRETKEY_BITS; ++c) {
							linearSystem[l][c] ^= linearSystem[col][c];
						}
						regStatus[l] ^= regStatus[col];
					}
				}
				break;
			}
		}
		if (!headFactor) {
			DEBUG("Error: Not a Cramer System, returning all 0");
			free(LFSRs);
			return 1;
		}
	}

	// Solution finding (starting from line #SECRETKEY_BITS, assuming we have a Cramer System)
	for (int line=SECRETKEY_BITS-1 ; line>=0 ; --line) {
		secretKey[line] = regStatus[line];
		// propagate new result amongst above equations
		for (int l=line-1 ; l>=0 ; --l) {
			regStatus[l] ^= linearSystem[l][line] & secretKey[line];
			linearSystem[l][line] = 0;
		}
	}

	// -------------------------------------------------------
	// Solution Found
	// -------------------------------------------------------

	free(LFSRs);
	return 0;

}




// Documentation in header file
int keysetup_reverse_test() {

	byte secretKey[SECRETKEY_BITS];

	// The frameId is totally public
	byte frameId[FRAMEID_BITS] = {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	// Initial state of R1->R4 LFSRs is determined after keysetup (before the discarded 99 cycles)
	byte R1[R1_BITS] = {1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1,1};
	byte R2[R2_BITS] = {0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1};
	byte R3[R3_BITS] = {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1};
	byte R4[R4_BITS] = {1,1,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0};
	reverseKeysetup(R1, R2, R3, R4, frameId, secretKey);

	// This secret key is the standard secret key that generates the values above
	byte verifiedSecretKey[SECRETKEY_BITS] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

	// We check that the secret key we processed is really the same as the one used to generate sample data:
	for (int i=0 ; i<SECRETKEY_BITS ; ++i) {
		if (secretKey[i] != verifiedSecretKey[i]) {
			DEBUG("Self-check aborted: a discrepancy was found comparing the produced secret key and the verified one:");
			DUMP_CHAR_VECTOR(secretKey, SECRETKEY_BITS, "Computed secretKey");
			DUMP_CHAR_VECTOR(verifiedSecretKey, SECRETKEY_BITS, "Verification");
			return 1;
		}
	}
	DEBUG("Self-check succeeded: the produced secret key matches the verified one");
	return 0;

}

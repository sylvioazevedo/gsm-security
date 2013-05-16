/*============================================================================*
 *                                                                            *
 *                                   keygen.c                                 *
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
  * @file keygen.c
  * @brief Implementation of A5/2 keystream generation.
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #10
  * @date 24/06/2011
  *
  * Here are located the methods used for simulating the behavior of the A5/2 keystream generator.
  * Four LFSRs are used...
  * TODO: Write some more documentation
  *
  * */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#include "keygen.h"


//@{
//! LFSRs Storage (as global variables for easier exploitation)
byte R1[R1_BITS];
byte R2[R2_BITS];
byte R3[R3_BITS];
byte R4[R4_BITS];
//@}



/**
 * \fn void keygen_clockReg(const int index)
 * \brief Clock the specified register
 *
 * \param[in] index Register to be clocked
 */
void keygen_clockReg(const int index) {
	byte carry = 0;
	switch(index) {
	case 1:
		carry ^= R1[R1_SHIFTTAP_1] ^ R1[R1_SHIFTTAP_2] ^ R1[R1_SHIFTTAP_3] ^ R1[R1_SHIFTTAP_4];
		memmove(R1+1, R1, (R1_BITS-1)*sizeof(byte));
		R1[0] = carry;
		break;
	case 2:
		carry ^= R2[R2_SHIFTTAP_1] ^ R2[R2_SHIFTTAP_2];
		memmove(R2+1, R2, (R2_BITS-1)*sizeof(byte));
		R2[0] = carry;
		break;
	case 3:
		carry ^= R3[R3_SHIFTTAP_1] ^ R3[R3_SHIFTTAP_2] ^ R3[R3_SHIFTTAP_3] ^ R3[R3_SHIFTTAP_4];
		memmove(R3+1, R3, (R3_BITS-1)*sizeof(byte));
		R3[0] = carry;
		break;
	case 4:
		carry ^= R4[R4_SHIFTTAP_1] ^ R4[R4_SHIFTTAP_2];
		memmove(R4+1, R4, (R4_BITS-1)*sizeof(byte));
		R4[0] = carry;
		break;
	default:
		DEBUG("Error: Trying to clock invalid LFSR #%d", index);
		break;
	}
}




/**
 * \fn void keygen_clockingUnit(int clockAll)
 * \brief Performs register clocking according to the rules of the Clocking Unit
 *
 * \param[in] clockAll When non-zero, bypass Clocking Unit decision: always clock (initialization phase)
 */
void keygen_clockingUnit(int clockAll) {
	byte maj = MAJORITY(R4[R4_CLOCKTAP_R1], R4[R4_CLOCKTAP_R2], R4[R4_CLOCKTAP_R3]);
	if (clockAll || maj==R4[R4_CLOCKTAP_R1])
		keygen_clockReg(1);
	if (clockAll || maj==R4[R4_CLOCKTAP_R2])
		keygen_clockReg(2);
	if (clockAll || maj==R4[R4_CLOCKTAP_R3])
		keygen_clockReg(3);
	keygen_clockReg(4);
}




/**
 * \fn byte keygen_getOutBit()
 * \brief Returns the current out keystream bit
 */
byte keygen_getOutBit() {
	return R1[R1_BITS-1] ^ R2[R2_BITS-1] ^ R3[R3_BITS-1]
	     ^ MAJORITY(R1[R1_OUTTAP_1], 1 ^ R1[R1_OUTTAP_2], R1[R1_OUTTAP_3])
	     ^ MAJORITY(R2[R2_OUTTAP_1], R2[R2_OUTTAP_2], 1 ^ R2[R2_OUTTAP_3])
	     ^ MAJORITY(1 ^ R3[R3_OUTTAP_1], R3[R3_OUTTAP_2], R3[R3_OUTTAP_3]);
}




// Documentation in header file
void keysetup(const byte Kc[SECRETKEY_BITS], const byte frameId[FRAMEID_BITS]) {

	memset(R1, 0, R1_BITS*sizeof(byte));
	memset(R2, 0, R2_BITS*sizeof(byte));
	memset(R3, 0, R3_BITS*sizeof(byte));
	memset(R4, 0, R4_BITS*sizeof(byte));

	for (int i=0; i<SECRETKEY_BITS; ++i) {
		keygen_clockingUnit(1);
		R1[0] ^= Kc[i];
		R2[0] ^= Kc[i];
		R3[0] ^= Kc[i];
		R4[0] ^= Kc[i];
	}

	for (int i=0; i<FRAMEID_BITS; ++i) {
		keygen_clockingUnit(1);
		R1[0] ^= frameId[i];
		R2[0] ^= frameId[i];
		R3[0] ^= frameId[i];
		R4[0] ^= frameId[i];
	}

	R1[R1_INITIAL_CONST_POS] = 1;
	R2[R2_INITIAL_CONST_POS] = 1;
	R3[R3_INITIAL_CONST_POS] = 1;
	R4[R4_INITIAL_CONST_POS] = 1;

	// First 99 cycles of pre-processing (output discarded)
	for (int i=0 ; i<100 ; ++i) {
		keygen_clockingUnit(0);
	}
}




// Documentation in header file
void getKeystream(byte keystream[], const int len) {

	memset(keystream, 0, len*sizeof(byte));

	for (int i=0; i<len; i++) {
		keystream[i] = keygen_getOutBit();
		keygen_clockingUnit(0);
	}
}




// Documentation in header file
int keygen_test() {

	byte Kc[SECRETKEY_BITS] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	byte frameId[FRAMEID_BITS] = {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	// This keystream is the standard keystream that should be obtained considering the values above
	byte verifiedKeystream[228] = {1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,1,0,1,1,0,0,1,0,0,1,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0};



	byte keystream[228];
	memset(keystream, 0, 228*sizeof(byte));

	keysetup(Kc, frameId);
	getKeystream(keystream, 228);


	for (int i=0 ; i<228 ; ++i) {
		if (keystream[i] != verifiedKeystream[i]) {
			DEBUG("Self-check aborted: a discrepancy was found comparing the produced keystream and the verified one");
			return 1;
		}
	}

	DEBUG("Self-check succeeded: the produced keystream matches the verified one");
	return 0;
}

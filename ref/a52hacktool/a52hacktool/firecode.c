/*============================================================================*
 *                                                                            *
 *                                  firecode.c                                *
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
  * @file firecode.c
  * @brief Implementation of first encoding phase: Fire code (CRC)
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #9
  * @date 24/06/2011
  * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "firecode.h"
#include "const_code.h"



//! Bit Representation of the Fire polynomial
char polynomial[FIRE_POLYNOMIAL_LENGTH] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1};

//! Bit mirrored representation of the Fire polynomial
char polynomial_mirrored[FIRE_POLYNOMIAL_LENGTH] = {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1};




// Documentation in header file
void processFireGMatrix(byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH]) {

	memset(G, 0, FIRE_SOURCE_LENGTH*FIRE_CODE_LENGTH*sizeof(byte));

	for (int i=0 ; i<FIRE_SOURCE_LENGTH ; ++i) {
		memcpy(G[i]+i, polynomial_mirrored, 41*sizeof(byte));
	}

	// We assume the standard form of the matrix (1 diag first):
	// 1xxxxxxx...
	// 01xxxxxx...
	// 001xxxxx...
	// 0001xxxx...
	// ...........
	// We now proceed a Gauss elimination
	for (int col=0 ; col<FIRE_SOURCE_LENGTH ; ++col) {
		for (int line=(col-1) ; line>=0 ; --line) {
			if (G[line][col]) {
				for (int c=col ; c<FIRE_CODE_LENGTH ; ++c)
					G[line][c] ^= G[col][c];
			}
		}
	}

}




// Documentation in header file
void processFireHMatrix(byte H[FIRE_CRC_LENGTH][FIRE_CODE_LENGTH], byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH]) {

	memset(H, 0, FIRE_CRC_LENGTH*FIRE_CODE_LENGTH*sizeof(byte));

	for (int i=0 ; i<FIRE_CRC_LENGTH ; ++i) {
		for (int j=0; j<FIRE_SOURCE_LENGTH; ++j) {
			H[i][j] = G[j][i+FIRE_SOURCE_LENGTH];
		}
		H[i][FIRE_SOURCE_LENGTH+i] = 1;
	}

}




// Documentation in header file
void reverseFire(byte in[FIRE_CODE_LENGTH], byte out[FIRE_SOURCE_LENGTH]){

	memset(out, 0, FIRE_SOURCE_LENGTH*sizeof(byte));

	byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH];
	processFireGMatrix(G);

	byte H[FIRE_CRC_LENGTH][FIRE_CODE_LENGTH];
	processFireHMatrix(H, G);

	// Matrix Multiplication (code decoding)
	byte decoded[FIRE_SOURCE_LENGTH];
	memset(decoded, 0, FIRE_SOURCE_LENGTH*sizeof(byte));

	for (int i=0 ; i<FIRE_CRC_LENGTH ; ++i) {
		for (int j=0 ; j<FIRE_CODE_LENGTH ; ++j) {
			decoded[i] ^= in[j] & H[i][j];
		}
		// This prevented from having a non null syndrome, since non codewords make it return all 0s
		if (decoded[i]) {
			if(DEBUG_INFO) printf("Error checking CRC\n");
		}
	}
	if(DEBUG_INFO) DUMP_CHAR_VECTOR(decoded, FIRE_CRC_LENGTH, "decoding check (must be all 0)");
	memcpy(out, in, FIRE_SOURCE_LENGTH*sizeof(byte));
}

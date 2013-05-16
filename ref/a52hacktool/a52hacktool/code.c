/*============================================================================*
 *                                                                            *
 *                                    code.c                                  *
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
  * @file code.c
  * @brief Implementation of full coding & decoding process
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #17
  * @date 01/12/2011
  * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#include "code.h"
#include "firecode.h"
#include "const_code.h"
#include "convolution.h"
#include "interleaving.h"




// Documentation in header file
void processFullEncodingGMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH]) {

	memset(G, 0, FIRE_SOURCE_LENGTH*INTERLEAVING_LENGTH*sizeof(byte));

	byte FireG[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH];
	processFireGMatrix(FireG);

	byte ConvolutionG[CONVOLUTION_SOURCE_LENGTH][CONVOLUTION_CODE_LENGTH];
	processConvolutionGMatrix(ConvolutionG);

	byte InterleavingG[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH];
	processInterleavingGMatrix(InterleavingG);

	byte FireConvolution[FIRE_SOURCE_LENGTH][CONVOLUTION_CODE_LENGTH];
	memset(FireConvolution, 0, FIRE_SOURCE_LENGTH*CONVOLUTION_CODE_LENGTH*sizeof(byte));

	for (int line=0 ; line<FIRE_SOURCE_LENGTH ; ++line)
		for (int col=0 ; col<CONVOLUTION_CODE_LENGTH ; ++col)
			for (int i=0 ; i<CONVOLUTION_SOURCE_LENGTH ; ++i)
				if (i<FIRE_CODE_LENGTH)
					FireConvolution[line][col] ^= ((FireG[line][i]) & (ConvolutionG[i][col]));

	for (int line=0 ; line<FIRE_SOURCE_LENGTH ; ++line)
		for (int col=0 ; col<INTERLEAVING_LENGTH ; ++col)
			for (int i=0 ; i<INTERLEAVING_LENGTH ; ++i)
				G[line][col] ^= ((FireConvolution[line][i]) & (InterleavingG[i][col]));

}




// Documentation in header file
void processFullEncodingGSystematicMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH],  \
										  byte L[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH],   \
										  byte P[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH]) {

	byte headFactor = 0;
	CHAR_IDENTITY(L, FIRE_SOURCE_LENGTH);
	CHAR_IDENTITY(P, INTERLEAVING_LENGTH);

	for (int line=0; line<FIRE_SOURCE_LENGTH; ++line) {
		for (int col=line; col<INTERLEAVING_LENGTH; ++col) {

			headFactor = G[line][col];
			if (headFactor) {
				if (line!=col) {
					byte temp;
					for (int l=0; l<FIRE_SOURCE_LENGTH; ++l) {
						temp = G[l][line];
						G[l][line]  = G[l][col];
						G[l][col] = temp;
					}

					for (int l=0; l<INTERLEAVING_LENGTH; ++l) {
						temp = P[l][line];
						P[l][line]  = P[l][col];
						P[l][col] = temp;
					}
				}

				for (int l=line+1; l<FIRE_SOURCE_LENGTH; ++l) {
					if (G[l][line]) {
						for (int c=0; c<INTERLEAVING_LENGTH; ++c) {
							G[l][c] ^= G[line][c];
						}

						for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
							L[l][c] ^= L[line][c];
						}
					}
				}

				break;
			}

		}
	}

	for (int line=0; line<FIRE_SOURCE_LENGTH; ++line) {
		for (int col=line+1; col<FIRE_SOURCE_LENGTH; ++col) {
			if(G[line][col]){
				for (int c=line+1; c<INTERLEAVING_LENGTH; ++c) {
						G[line][c] ^= G[col][c];
				}
				for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
						L[line][c] ^= L[col][c];
				}
			}
		}
	}


}




// Documentation in header file
void processFullEncodingHSystematicMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH],                       \
										  byte H[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH]) {

	memset(H, 0, (INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH)*INTERLEAVING_LENGTH*sizeof(byte));

	for (int i=0 ; i<INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH ; ++i) {
		for (int j=0; j<FIRE_SOURCE_LENGTH; ++j) {
			H[i][j] = G[j][i+FIRE_SOURCE_LENGTH];
		}
		H[i][FIRE_SOURCE_LENGTH+i] = 1;
	}
}




// Documentation in header file
void processFullEncodingHMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH],                       \
								byte H[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH]) {

	byte HP_I[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH];
	byte L[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH];
	byte P[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH];
	byte P_I[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH];
	byte temp_G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH];

	memcpy(temp_G, G, FIRE_SOURCE_LENGTH*INTERLEAVING_LENGTH*sizeof(byte));

	processFullEncodingGSystematicMatrix(temp_G, L, P);
	processFullEncodingHSystematicMatrix(temp_G, H);
	//Invert permutation matrix P into P_I
	for (int line=0 ; line<INTERLEAVING_LENGTH; ++line) {
		for (int col=0 ; col<INTERLEAVING_LENGTH; ++col) {
				P_I[line][col] = P[col][line];
			}
	}
	BINPRODUCT_MATRIX_MATRIX(H, P_I, HP_I, (INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH), INTERLEAVING_LENGTH, INTERLEAVING_LENGTH);

	memcpy(H, HP_I, (INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH)*INTERLEAVING_LENGTH);

}




// Documentation in header file
void InvertMatrixL( byte L  [FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH], \
					byte L_I[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH]) {

	byte headFactor = 0;
	CHAR_IDENTITY(L_I, FIRE_SOURCE_LENGTH);

	for (int col=0; col<FIRE_SOURCE_LENGTH; ++col) {
		for (int line=col; line<FIRE_SOURCE_LENGTH; ++line) {


			headFactor = L[line][col];
			if (headFactor) {
				if (line!=col) {
					byte temp;
					for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
						temp = L[line][c];
						L[line][c]  = L[col][c];
						L[col][c] = temp;
					}
					for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
						temp = L_I[line][c];
						L_I[line][c]  = L_I[col][c];
						L_I[col][c] = temp;
					}
				}

				for (int l=line+1; l<FIRE_SOURCE_LENGTH; ++l) {
					if (L[l][col]) {
						for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
							L[l][c] ^= L[col][c];
						}
						for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
							L_I[l][c] ^= L_I[col][c];
						}
					}
				}
				break;
			}
		}
	}

	for (int line=0; line<FIRE_SOURCE_LENGTH; ++line) {
		for (int col=line+1; col<FIRE_SOURCE_LENGTH; ++col) {
			if(L[line][col]){
				for (int c=line+1; c<FIRE_SOURCE_LENGTH; ++c) {
						L[line][c] ^= L[col][c];
				}
				for (int c=0; c<FIRE_SOURCE_LENGTH; ++c) {
						L_I[line][c] ^= L_I[col][c];
				}
			}
		}
	}
}




// Documentation in header file
void processFullDecoding(byte codeword[CODEWORD_LENGTH], byte sourceword[SOURCEWORD_LENGTH]) {

	memset(sourceword, 0, SOURCEWORD_LENGTH*sizeof(byte));

	// Interleave decoder
	byte deinterleavedMessage[INTERLEAVING_LENGTH]; // right padding of 24 bits
	reverseInterleaving(codeword, deinterleavedMessage);

	// Convolutional decoder
	byte deconvolutedMessage[CONVOLUTION_SOURCE_LENGTH]; // right padding of 28 bits
	reverseConvolution(deinterleavedMessage, deconvolutedMessage);

	// Remove the 4 additional zeroes
	byte predeFiredMessage[FIRE_CODE_LENGTH];
	memcpy(predeFiredMessage, deconvolutedMessage, FIRE_CODE_LENGTH*sizeof(byte));

	// Fire decoder
	reverseFire(predeFiredMessage, sourceword);

}




// Documentation in header file
int code_test() {

	byte G[SOURCEWORD_LENGTH][CODEWORD_LENGTH];
	byte originalMessage[SOURCEWORD_LENGTH];
	byte encodedMessage[CODEWORD_LENGTH];
	byte decodedMessage[SOURCEWORD_LENGTH];

	// Random generation of a Source Word
	for (int k=0 ; k<SOURCEWORD_LENGTH ; ++k) {
		originalMessage[k] = rand() & 1;
	}

	// Targets initialization
	memset(encodedMessage, 0, CODEWORD_LENGTH  *sizeof(byte));
	memset(decodedMessage, 0, SOURCEWORD_LENGTH*sizeof(byte));

	// Encoding
	processFullEncodingGMatrix(G);
	BINPRODUCT_VECTOR_MATRIX(originalMessage, G, encodedMessage, SOURCEWORD_LENGTH, CODEWORD_LENGTH);

	// Decoding
	processFullDecoding(encodedMessage, decodedMessage);

	// We check that the message was recovered correctly:
	for (int i=0 ; i<SOURCEWORD_LENGTH ; ++i) {
		if (originalMessage[i] != decodedMessage[i]) {
			DEBUG("Self-check aborted: a discrepancy was found comparing the original message and the decoded one:");
			DUMP_CHAR_VECTOR(decodedMessage, SOURCEWORD_LENGTH, "Decoded Message");
			DUMP_CHAR_VECTOR(originalMessage, SOURCEWORD_LENGTH, "Verification");
			return 1;
		}
	}
	DEBUG("Self-check succeeded: the decoded message matches the original one");
	return 0;
}

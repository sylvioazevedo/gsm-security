/*============================================================================*
 *                                                                            *
 *                                   attack.c                                 *
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
  * @file attack.c
  * @brief Implementation of A52 attack
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #28
  * @date 01/12/2011
  * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#include "utils.h"

#include "attack.h"

#include "code.h"
#include "keygen.h"
#include "matrices_generation.h"
#include "keysetup_reverse.h"



//! RAM Storage for Resolution Matrices
unsigned int** ALLMATRICES;




/**
 * \fn void attack_redispatchLFSRdata(const byte LFSRState[REGS_TOTAL_VARS-1], byte R1[R1_BITS], byte R2[R2_BITS], byte R3[R3_BITS])
 * \brief Translates a compact LFSR representation (only including variables) into separate registers (adding initial "1" constants)
 *
 * \param[in]  LFSRState Compact LFSR representation
 * \param[out] R1 First LFSR
 * \param[out] R2 Second LFSR
 * \param[out] R3 Third LFSR
 */
void attack_redispatchLFSRdata(const byte LFSRState[REGS_TOTAL_VARS-1], byte R1[R1_BITS], byte R2[R2_BITS], byte R3[R3_BITS]) {

	memcpy(R1, LFSRState, R1_INITIAL_CONST_POS);
	memcpy(R1+R1_INITIAL_CONST_POS+1, LFSRState+R1_INITIAL_CONST_POS, R1_SIMPLE_VARS-R1_INITIAL_CONST_POS);
	R1[R1_INITIAL_CONST_POS] = 1;

	memcpy(R2, LFSRState+R1_SIMPLE_VARS, R2_INITIAL_CONST_POS);
	memcpy(R2+R2_INITIAL_CONST_POS+1, LFSRState+R1_SIMPLE_VARS+R2_INITIAL_CONST_POS, R2_SIMPLE_VARS-R2_INITIAL_CONST_POS);
	R2[R2_INITIAL_CONST_POS] = 1;

	memcpy(R3, LFSRState+R1_SIMPLE_VARS+R2_SIMPLE_VARS, R3_INITIAL_CONST_POS);
	memcpy(R3+R3_INITIAL_CONST_POS+1, LFSRState+R1_SIMPLE_VARS+R2_SIMPLE_VARS+R3_INITIAL_CONST_POS, R3_SIMPLE_VARS-R3_INITIAL_CONST_POS);
	R3[R3_INITIAL_CONST_POS] = 1;

}




/**
 * \fn int attack_checkDoubleVars(const byte LFSRState[REGS_TOTAL_VARS-1])
 * \brief Checks that all double variables values are consistent with single variables ones
 *
 * \param[in] LFSRState Compact LFSR representation
 * \return 0 if the state is consistent, non-zero otherwise
 */
int attack_checkDoubleVars(const byte LFSRState[REGS_TOTAL_VARS-1]) {
	int i=REGS_SIMPLE_VARS;
	for (int v1=0 ; v1<R1_SIMPLE_VARS ; ++v1)
		for (int v2=v1+1 ; v2<R1_SIMPLE_VARS ; ++v2)
			if (LFSRState[i++] != (LFSRState[v1] & LFSRState[v2]))
				return 1;
	for (int v1=0 ; v1<R2_SIMPLE_VARS ; ++v1)
		for (int v2=v1+1 ; v2<R2_SIMPLE_VARS ; ++v2)
			if (LFSRState[i++] != (LFSRState[v1+R1_SIMPLE_VARS] & LFSRState[v2+R1_SIMPLE_VARS]))
				return 1;
	for (int v1=0 ; v1<R3_SIMPLE_VARS ; ++v1)
		for (int v2=v1+1 ; v2<R3_SIMPLE_VARS ; ++v2)
			if (LFSRState[i++] != (LFSRState[v1+R1_SIMPLE_VARS+R2_SIMPLE_VARS] & LFSRState[v2+R1_SIMPLE_VARS+R2_SIMPLE_VARS]))
				return 1;
	return 0;
}




// Documentation in header file
void freeRAM() {
	if (!ALLMATRICES) return;
	for (int i=0 ; i<TOTAL_MATRICES ; ++i) {
		free(ALLMATRICES[i]);
		ALLMATRICES[i] = NULL;
	}
	free(ALLMATRICES);
	ALLMATRICES = NULL;
}




// Documentation in header file
int initializeRAM(const char* filename) {

	if (ALLMATRICES) {
		DEBUG("Dictionary already initialized. Please free it by calling freeRAM(); before reloading data");
		return 1;
	}

	DEBUG("Loading matrices dictionary...");

	FILE* sourcefile = fopen(filename, "rb");
	if (!sourcefile) {
		DEBUG("Error: failed to open '%s'. Aborting operation", filename);
		return 1;
	}

	ALLMATRICES = (unsigned int**) malloc(TOTAL_MATRICES*sizeof(unsigned int*));
	if (!ALLMATRICES) {
		DEBUG("Unable to allocate enough RAM for direct RAM attack.");
		return 1;
	}
	for (int i=0 ; i<TOTAL_MATRICES ; ++i) {
		// We need to allocate extra space at the end of the matrix lines (there is a half-loaded integer at the end of each line)
		ALLMATRICES[i] = (unsigned int*) malloc(NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH*RESOLUTION_MATRIX_INT_WIDTH*sizeof(unsigned int));
		if (!ALLMATRICES[i]) {
			DEBUG("Unable to allocate enough RAM for direct RAM attack.");
			for (int j=i-1 ; j>=0 ; --j) {
				free(ALLMATRICES[j]);
			}
			free(ALLMATRICES);
			return 1;
		}
	}

	byte buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);

	for (int i=0 ; i<TOTAL_MATRICES ; ++i) {
		if (fread(buffer, sizeof(byte), BUFFER_SIZE, sourcefile) != BUFFER_SIZE) {
			DEBUG("Error: Unable to load matrix #%d from file '%s'", i, filename);
			fclose(sourcefile);
			freeRAM();
			return 1;
		}

		// Direct storage in integer representation for faster later use
		int k=0;
		for (int l=0 ; l<NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH ; ++l) {
			for (int c=0 ; c<RESOLUTION_MATRIX_INT_WIDTH-1 ; ++c) {
				ALLMATRICES[i][RESOLUTION_MATRIX_INT_WIDTH*l+c] = (buffer[k] << 24) | (buffer[k+1] << 16) | (buffer[k+2] << 8) | buffer[k+3];
				k = k+4;
			}
			// The last int of each line will only be half-filled with data
			// The last bit of meaningful data is duplicated to the very last bit of container for later optimization
			ALLMATRICES[i][RESOLUTION_MATRIX_INT_WIDTH*(l+1)-1] = (buffer[k] << 24) | (buffer[k+1] << 16) | (buffer[k+1] & 1);
			k = k+2;
		}
	}

	fclose(sourcefile);
	DEBUG("Dictionary Loaded");
	return 0;
}




/**
 * \fn int attack_decipherSecretKey(cipherTextArgs* ctArgs, int lowindex, int highindex, int* keyFound, byte secretKey[SECRETKEY_BITS])
 * \brief Thread attack method
 *
 * \param[in]  ctArgs Problem to solve
 * \param[in]  lowindex Index to start the search form (inclusive)
 * \param[in]  highindex Last index to be analyzed (exclusive)
 * \param[in]  keyFound Solution found flag
 * \param[out] secretKey Storage for a potential solution
 * \return 0 if the attack is thread-locally successful, non-zero otherwise
 */
int attack_decipherSecretKey(cipherTextArgs* ctArgs, int lowindex, int highindex, int* keyFound, byte secretKey[SECRETKEY_BITS]) {


	// DEBUG("Thread #%d:  \tInitializing Decryption...", lowindex/THREAD_CHUNKSIZE);

	byte *cipherText1 = ctArgs->cipherText1;
	byte *cipherText2 = ctArgs->cipherText2;
	byte *cipherText3 = ctArgs->cipherText3;
	byte *frameId     = ctArgs->frameId;

	// Code Matrix
	byte G[SOURCEWORD_LENGTH][CODEWORD_LENGTH];
	processFullEncodingGMatrix(G);

	// Code Parity-Check Matrix
	byte H[SYNDROME_LENGTH][CODEWORD_LENGTH];
	processFullEncodingHMatrix(G, H);

	// Base syndrome
	byte originalSyndrome[NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH];
	memset(originalSyndrome, 0, NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH*sizeof(byte));
	BINPRODUCT_MATRIX_VECTOR(H, cipherText1, originalSyndrome,                   SYNDROME_LENGTH, CODEWORD_LENGTH);
	BINPRODUCT_MATRIX_VECTOR(H, cipherText2, originalSyndrome+  SYNDROME_LENGTH, SYNDROME_LENGTH, CODEWORD_LENGTH);
	BINPRODUCT_MATRIX_VECTOR(H, cipherText3, originalSyndrome+2*SYNDROME_LENGTH, SYNDROME_LENGTH, CODEWORD_LENGTH);


	// DEBUG("Thread #%d:  \tLaunching Attack on range [%d-%d]", lowindex/THREAD_CHUNKSIZE, lowindex, highindex);

	for (int index=lowindex ; (index<highindex) && (!*keyFound) ; ++index) {

		int skipCycle = 0;

		// if (index % (1<<9) == 0) {
			// printf("Thread #%d:  \tTrying to decrypt message using matrix #%d \t", lowindex/THREAD_CHUNKSIZE, index);
			// PROGRESSBAR((index-lowindex)*100/(highindex-lowindex));
		// }


		// We load the Resolution Matrix designated by this index
		unsigned int HS[NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH][RESOLUTION_MATRIX_INT_WIDTH];
		memcpy(HS, ALLMATRICES[index], NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH*RESOLUTION_MATRIX_INT_WIDTH*sizeof(unsigned int));


		// The corresponding syndrome is calculated (from the original, processed during initialization)
		byte syndrome[NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH];
		// we xor the "last column" (in bit representation) of HS (representing "1" constant) with the processed syndrome.
		// NB: we duplicated this data to the very last bit of the int when loading RAM to prevent from shifting everytime.
		for (int i=0 ; i<SYNDROME_LENGTH*NEEDED_ENCRYPTED_MESSAGES ; ++i) {
			syndrome[i] = originalSyndrome[i] ^ (HS[i][RESOLUTION_MATRIX_INT_WIDTH-1] & 1);
		}


		// Now we have the correct linear system.
		// We proceed to a Gauss Elimination, resulting in a Lower Triangular Matrix
		byte headFactor = 0;
		int lineref = (NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH-1);
		for (int col=REGS_TOTAL_VARS-2 ; col>=0 ; --col) {
			for (int line=lineref ; line>=0 ; --line) {
				// Pivot finding
				headFactor = (HS[line][col/32] >> (31-(col%32))) & 1;

				if (headFactor) {
					// Line Swap if necessary
					if (line!=lineref) {
						unsigned int tempa[RESOLUTION_MATRIX_INT_WIDTH];
						memcpy(tempa,       HS[lineref], RESOLUTION_MATRIX_INT_WIDTH*sizeof(unsigned int));
						memcpy(HS[lineref], HS[line],    RESOLUTION_MATRIX_INT_WIDTH*sizeof(unsigned int));
						memcpy(HS[line],    tempa,       RESOLUTION_MATRIX_INT_WIDTH*sizeof(unsigned int));
						byte temp         = syndrome[lineref];
						syndrome[lineref] = syndrome[line];
						syndrome[line]    = temp;
					}
					// Elimination
					for (int l=lineref-1 ; l>=0 ; --l) {
						if ((HS[l][col/32] >> (31-(col%32))) & 1) {
							for (int c=col/32 ; c>=0 ; --c) {
								HS[l][c] ^= HS[lineref][c];
							}
							syndrome[l] ^= syndrome[lineref];
						}
					}
					break;
				}
			}
			if (!headFactor) {
				// This is not really a "wrong matrix" case, we just have not enough equations to prove correctness
				// DEBUG("Wrong Matrix: no enough equations");
				skipCycle = 1;
				break;
			}
			--lineref;
		}
		if (skipCycle) continue;


		// Once we arrive here, we got exactly SYNDROME_EMPTY_EQUATIONS empty lines (0 == ?).
		// The line #(SYNDROME_EMPTY_EQUATIONS+1) will be exactly
		//     HS[SYNDROME_EMPTY_EQUATIONS+1][0] = 0x80000000 (0b10000000000000000000000000000000)
		for (lineref=0 ; !(syndrome[lineref]) && (lineref<=SYNDROME_EMPTY_EQUATIONS) ; ++lineref);
		if (lineref != SYNDROME_EMPTY_EQUATIONS+1) {
			// DEBUG("Wrong Matrix: Bad Equation 0 = 1");
			continue;
		}


		// Here we find the solution (LFSRs initial state)
		byte LFSRState[REGS_TOTAL_VARS-1];
		memset(LFSRState, 0, REGS_TOTAL_VARS-1);
		// We assume here that the matrix is at least in lower triangular form
		for (int i=lineref ; i<NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH ; ++i) {
			LFSRState[i-lineref] = syndrome[i];
			// propagate new result amongst equations below
			for (int l=i+1 ; l<NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH ; ++l) {
				syndrome[l] ^= ((HS[l][(i-lineref)/32] >> (31-((i-lineref)%32))) & 1) & LFSRState[i-lineref];
				HS[l][(i-lineref)/32] &= ~(1 << (31-((i-lineref)%32)));
			}
		}


		// We check if the solution is consistent
		if (attack_checkDoubleVars(LFSRState)) {
			// DEBUG("Wrong Matrix: Double Vars Mismatch");
			continue;
		}


		// Redispatch data into separate registers
		byte R1[R1_BITS];
		byte R2[R2_BITS];
		byte R3[R3_BITS];
		attack_redispatchLFSRdata(LFSRState, R1, R2, R3);

		byte R4[R4_BITS];
		getR4fromIndex(index, R4);


		// Key Setup reversal, providing us with the secret key
		if (reverseKeysetup(R1, R2, R3, R4, frameId, secretKey)) {
			// DEBUG("Wrong Matrix: Unable to reverse keysetup");
			continue;
		}

		// DEBUG("Thread #%d:  \tSecret key found: Attack succeeded", lowindex/THREAD_CHUNKSIZE);
		*keyFound = lowindex/THREAD_CHUNKSIZE+1; // +1 to avoid message from thread #0 to be discarded
		return 0;

	}


	if (*keyFound) {
		// DEBUG("Thread #%d:  \tComputation aborted: Key found by another thread", lowindex/THREAD_CHUNKSIZE);
	} else {
		// DEBUG("Thread #%d:  \tAttack failed", lowindex/THREAD_CHUNKSIZE);
	}
	return 1;
}




/**
 * \fn void* attack_launchAttack(void* data)
 * \brief Thread initializer
 *
 * \param[in] data Pointer to the thread's arguments
 * \return NULL
 */
void* attack_launchAttack(void* data) {
	threadArgs *args = data;
	attack_decipherSecretKey(args->ctArgs, args->lowindex, args->highindex, args->keyFound, args->secretKey);
	return NULL;
}



// Documentation in header file
int attack(cipherTextArgs* ctArgs, byte secretKey[SECRETKEY_BITS]) {

	memset(secretKey, 0, SECRETKEY_BITS);

	if (!ALLMATRICES) {
		DEBUG("Dictionary not initialized, unable to proceed with the attack");
		return 1;
	}

	time_t datetime = time(NULL);
	struct tm *local = localtime(&datetime);
	DEBUG("Attack started on %s", asctime(local));

	pthread_t *t = malloc(PROCESSING_THREADS*sizeof(pthread_t));
	threadArgs args[PROCESSING_THREADS];

	int keyFound = 0; // Will store the index of the thread that found the key

	// Thread & Arguments Creation
	for (int i=0 ; i<PROCESSING_THREADS ; ++i) {

		args[i].ctArgs    = ctArgs;
		args[i].lowindex  =   i   * THREAD_CHUNKSIZE;
		args[i].highindex = (i+1) * THREAD_CHUNKSIZE;
		args[i].keyFound  = &keyFound;
		memset(args[i].secretKey, 0, SECRETKEY_BITS);

		if (pthread_create(&t[i], NULL, attack_launchAttack, &args[i])) {
			DEBUG("Unable to create thread #%d\nKilling process.", i);
			exit(1);
		}
	}


	// Thread Joining
	for (int i=0 ; i<PROCESSING_THREADS ; ++i) {
		pthread_join(t[i], NULL);
		if (keyFound == i+1) {
			memcpy(secretKey, args[i].secretKey, SECRETKEY_BITS);
		}
	}
	free(t);


	datetime = time(NULL);
	local = localtime(&datetime);
	DEBUG("Attack Terminated on %s", asctime(local));
	if (keyFound) {
		DEBUG("Secret Key Found:");
		DUMP_CHAR_VECTOR(secretKey, SECRETKEY_BITS, "Secret Key");
		return 0;
	} else {
		DEBUG("Failure");
		return 1;
	}

};




// Documentation in header file
int attack_test() {

	// Sample Message
	byte originalMessage1[SOURCEWORD_LENGTH] = {1,0,0,0,1,0,1,0,0,0,0,1,1,1,1,1,1,0,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,1,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,1,1,0,0,1,0,1,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,1,0,0,1,1,0,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,1,0,1,0,1,1,1,1,0};
	byte originalMessage2[SOURCEWORD_LENGTH] = {1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,1,0,1,1,0,0,0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,1,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,0,1,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,1,0,0,0,1,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,1,0,1,0,1,0,0,0,0,1,1,0,1,0};
	byte originalMessage3[SOURCEWORD_LENGTH] = {0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,0,0,1,1,0,1,0,1,0,1,1,0,1,1,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,1,1,0,0,1,0,1,0,1,1,1,0,0,1,0,0,1,0,1,0,0,0,0,0,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0,1,0,1,0,1,0,0,0,0,1,1,0,1,0,1,0,1,0,1,1};

	// Encryption parameters
	// byte secretKey[SECRETKEY_BITS] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	byte secretKey[SECRETKEY_BITS] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
	byte frameId[FRAMEID_BITS] = {1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	cipherTextArgs ctArgs;

	// Message Coding
	byte G[SOURCEWORD_LENGTH][CODEWORD_LENGTH];
	processFullEncodingGMatrix(G);

	byte originalEncodedMessage1[CODEWORD_LENGTH];
	byte originalEncodedMessage2[CODEWORD_LENGTH];
	byte originalEncodedMessage3[CODEWORD_LENGTH];

	BINPRODUCT_VECTOR_MATRIX(originalMessage1, G, originalEncodedMessage1, SOURCEWORD_LENGTH, CODEWORD_LENGTH);
	BINPRODUCT_VECTOR_MATRIX(originalMessage2, G, originalEncodedMessage2, SOURCEWORD_LENGTH, CODEWORD_LENGTH);
	BINPRODUCT_VECTOR_MATRIX(originalMessage3, G, originalEncodedMessage3, SOURCEWORD_LENGTH, CODEWORD_LENGTH);


	// Dictionary Initialization
	initializeRAM("bin/matrices.bin");


	time_t time1, time2;
	double diffsec;
	double totaltime = 0;
	int totalexecs   = 0;

	srand(time(NULL));

	for (int testcase=0 ; testcase<10 ; ++testcase) {

		// Retrieving Encoded Message
		memcpy(ctArgs.cipherText1, originalEncodedMessage1, CODEWORD_LENGTH);
		memcpy(ctArgs.cipherText2, originalEncodedMessage2, CODEWORD_LENGTH);
		memcpy(ctArgs.cipherText3, originalEncodedMessage3, CODEWORD_LENGTH);

		// Generating random secretKey
		for (int k=0 ; k<SECRETKEY_BITS ; ++k) {
			secretKey[k] = rand() & 1;
		}
		// Generating random frameId
		for (int k=0 ; k<FRAMEID_BITS ; ++k) {
			frameId[k] = rand() & 1;
		}

		DEBUG("Processing Test Case #%d...", testcase);
		DUMP_CHAR_VECTOR(secretKey, SECRETKEY_BITS, "SecretKey");
		DUMP_CHAR_VECTOR(frameId, FRAMEID_BITS, "FrameId");

		memcpy(ctArgs.frameId, frameId, FRAMEID_BITS);

		// Message Encryption
		keysetup(secretKey, frameId);
		byte keystream[CODEWORD_LENGTH*NEEDED_ENCRYPTED_MESSAGES];
		getKeystream(keystream, CODEWORD_LENGTH*NEEDED_ENCRYPTED_MESSAGES);

		XOR_CHARARRAYS(ctArgs.cipherText1, keystream,                   CODEWORD_LENGTH);
		XOR_CHARARRAYS(ctArgs.cipherText2, keystream+  CODEWORD_LENGTH, CODEWORD_LENGTH);
		XOR_CHARARRAYS(ctArgs.cipherText3, keystream+2*CODEWORD_LENGTH, CODEWORD_LENGTH);

		// Decryption Process
		byte decipheredSecretKey[SECRETKEY_BITS];

		time(&time1);
		attack(&ctArgs, decipheredSecretKey);
		time(&time2);
		diffsec = difftime(time2,time1);
		totaltime += diffsec;
		++totalexecs;

		// We check that the secret key we deciphered is really the same as the one used to generate sample data:
		for (int i=0 ; i<SECRETKEY_BITS ; ++i) {
			if (secretKey[i] != decipheredSecretKey[i]) {
				DEBUG("Self-check aborted [%.2lf seconds]: a discrepancy was found comparing the deciphered secret key and the original one:", diffsec);
				DUMP_CHAR_VECTOR(decipheredSecretKey, SECRETKEY_BITS, "Deciphered secretKey");
				DUMP_CHAR_VECTOR(secretKey, SECRETKEY_BITS, "Verification");
				freeRAM();
				return 1;
			}
		}

		DEBUG("Self-check succeeded [%.2lf seconds]: the deciphered secret key matches the original one", diffsec);
		DEBUG_LF;

	}

	// Dictionary Finalization
	freeRAM();

	DEBUG_LF;
	DEBUG("All Attacks Successful");
	DEBUG_LF;
	DEBUG("Total Processsing time:  %.2lf seconds for %d attacks", totaltime, totalexecs);
	DEBUG("Average Processing time: %.2lf seconds", totaltime/(double)totalexecs);

	return 0;

}

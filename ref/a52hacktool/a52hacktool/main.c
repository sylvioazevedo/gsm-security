/*============================================================================*
 *                                                                            *
 *                                    main.c                                  *
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
  * @file main.c
  * @brief Command-line interface for program operation
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #21
  * @date 01/12/2011
  * */

/*! \mainpage A52HackTool Documentation
 * \section sec_intro Introduction
 * This tool is related to the paper <em>Implementation and performance analysis of Barkan, Biham and Keller's attack on A5/2</em> written by Nicolas Paglieri & Olivier Benjamin on the 8th of June 2011. \n
 * You can find this article on the project webpage: <http://www.ni69.info/security-gsm-en.php>
 * \image html "logo.png"
 * \section sec_copyright Copyright and License
 * Copyright © 2011   -   Nicolas Paglieri   &   Olivier Benjamin \n
 * All rights reserved. \n\n
 * These files may be used under the terms of the GNU General Public License version 3 as published by the Free Software Foundation. \n
 * See <http://www.gnu.org/licenses/> or GPL.txt included in the packaging of this file. \n
 * The information detailed here is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * \n\n
 * \section sec_contact Contact Information:
 * nicolas.paglieri [at] ensimag.fr \n
 * olivier.benjamin [at] ensimag.fr \n\n
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


#include "const_code.h"
#include "utils.h"

#include "code.h"
#include "keygen.h"
#include "attack.h"
#include "matrices_generation.h"
#include "keysetup_reverse.h"




/**
 * \enum OperationParam
 * \brief CLI Operation
 *
 * Gathers all possible CLI operation parameters.
 */
typedef enum {
	OP_NONE,       //!< No Operation
	OP_ENCODE,     //!< Perform Encoding
	OP_DECODE,     //!< Perform Decoding
	OP_ENCRYPT,    //!< Perform Encryption
	OP_ATTACK,     //!< Perform Attack
	OP_PRECOMPUTE, //!< Generate Resolution Matrices
	OP_AUTOTEST    //!< Launch Autotest
} OperationParam;




/**
 * \fn void displayHelp()
 * \brief Displays the program banner and help about possible CLI commands
 */
void displayHelp() {

	printf("#===============================================================================\n");
	printf("#                   _____ ___  _    _            _ _______          _          #\n");
	printf("#             /\\   | ____|__ \\| |  | |          | |__   __|        | |         #\n");
	printf("#            /  \\  | |__    ) | |__| | __ _  ___| | _| | ___   ___ | |         #\n");
	printf("#           / /\\ \\ |___ \\  / /|  __  |/ _` |/ __| |/ / |/ _ \\ / _ \\| |         #\n");
	printf("#          / ____ \\ ___) |/ /_| |  | | (_| | (__|   <| | (_) | (_) | |         #\n");
	printf("#         /_/    \\_\\____/|____|_|  |_|\\__,_|\\___|_|\\_\\_|\\___/ \\___/|_|         #\n");
	printf("#         Turnkey & easy to use tool  for cracking the GSM A5/2 cipher         #\n");
	printf("#------------------------------------------------------------------------------#\n");
	printf("#         Copyright © 2011   -   Nicolas Paglieri  &  Olivier Benjamin         #\n");
	printf("#                                                                              #\n");
	printf("#                                  Version 1.3.0                               #\n");
	printf("#                    http://www.ni69.info/security-gsm-en.php                  #\n");
	printf("#==============================================================================#\n");
	printf("\n");
	printf("Here are the available commands:\n");
	printf(" - ask for help      :  --help | -h\n");
	printf(" - encode  a message :  --ENCODE  -s [source] -d [destination]\n");
	printf(" - encrypt a message :  --ENCRYPT -s [source] -d [destination] -k [secretkey] -f [frameId]\n");
	printf(" - decrypt a message :  --DECRYPT -s [source] -d [destination] -k [secretkey] -f [frameId]\n");
	printf(" - decode  a message :  --DECODE  -s [source] -d [destination]\n");
	printf(" - precompute data   :  --PRECOMPUTE\n");
	printf(" - perfom attack     :  --ATTACK  -s [source] -f [frameId]\n");
	printf(" - launch autotest   :  --AUTOTEST\n");
	printf("\n");

}


/**
 * \fn int main(int argc, char* argv[])
 * \brief Program entry point
 *
 * \param[in] argc Arguments count
 * \param[in] argv Arguments values
 * \return 0 if the program terminates with no error, non-zero otherwise
 */
int main(int argc, char* argv[]) {



	char param_sourcefile[255] = "";
	char param_destfile[255]   = "";

	byte param_secretKey[SECRETKEY_BITS];
	memset(param_secretKey, 0, SECRETKEY_BITS);
	byte param_frameId[FRAMEID_BITS];
	memset(param_frameId,   0, FRAMEID_BITS);

	OperationParam param_operation = OP_NONE;

	int argi = 1;

	#define _UNIQUE_OPERATION_TEST(opcode)                                                         \
        if (param_operation != OP_NONE) {                                                          \
            printf("Error: operation field must be unique\n"); return 1;                           \
        }                                                                                          \
		param_operation = (opcode)

	/* Command-line parameters retrieval */
	while (argi<argc) {

		if (strcmp(argv[argi],"--ENCODE")==0) {

			_UNIQUE_OPERATION_TEST(OP_ENCODE);

		} else if (strcmp(argv[argi],"--DECODE")==0) {

			_UNIQUE_OPERATION_TEST(OP_DECODE);

		} else if (strcmp(argv[argi],"--ENCRYPT")==0 || strcmp(argv[argi],"--DECRYPT")==0) {

			// --ENCRYPT is the same as --DECRYPT (since it's bitwise XORing the message)
			_UNIQUE_OPERATION_TEST(OP_ENCRYPT);

		} else if (strcmp(argv[argi],"--ATTACK")==0) {

			_UNIQUE_OPERATION_TEST(OP_ATTACK);

		} else if (strcmp(argv[argi],"--PRECOMPUTE")==0) {

			_UNIQUE_OPERATION_TEST(OP_PRECOMPUTE);

		} else if (strcmp(argv[argi],"--AUTOTEST")==0) {

			_UNIQUE_OPERATION_TEST(OP_AUTOTEST);

		} else if (strcmp(argv[argi],"-s")==0) {

			if ((argi+1) >= argc){
				printf("Invalid '-s' parameter\n"); return 1;
			} else {
				strncpy(param_sourcefile, argv[argi+1], 255);
			}
			++argi;

		} else if (strcmp(argv[argi],"-d")==0) {

			if ((argi+1) >= argc){
				printf("Invalid '-d' parameter\n"); return 1;
			} else {
				strncpy(param_destfile, argv[argi+1], 255);
			}
			++argi;

		} else if (strcmp(argv[argi],"-k")==0) {

			if ((argi+1) >= argc
			|| stringToByteArray(argv[argi+1], param_secretKey, SECRETKEY_BITS)) {
				printf("Invalid '-k' parameter\n"); return 1;
			}
			++argi;

		} else if (strcmp(argv[argi],"-f")==0) {

			if ((argi+1) >= argc
			|| stringToByteArray(argv[argi+1], param_frameId, FRAMEID_BITS)) {
				printf("Invalid '-f' parameter\n"); return 1;
			}
			++argi;

		} else if (strcmp(argv[argi],"-h")==0
		       ||  strcmp(argv[argi],"--help")==0) {

			displayHelp();
			return 0;

		} else {

			printf("Unrecognized parameter '%s'\n", argv[argi]);
			return 1;

		}
		++argi;

	}

	// File Parameters testing
	switch (param_operation) {
		case OP_NONE:
			displayHelp(); return 0;
		case OP_ENCODE:
		case OP_DECODE:
		case OP_ENCRYPT:
			if (strcmp(param_destfile, "")==0) {
				printf("Invalid destination file\n"); return 1;
			}
		case OP_ATTACK:
			if (strcmp(param_sourcefile, "")==0 || !fileExists(param_sourcefile)) {
				printf("Invalid source file\n"); return 1;
			}

		default:;
	}

	// Dictionary validity check
	if (((param_operation==OP_ATTACK) || (param_operation==OP_AUTOTEST)) && (!fileExists("bin/matrices.bin"))) {
		printf("Unable to locate dictionary 'bin/matrices.bin'.\nPlease launch the program with --PRECOMPUTE option before attacking.\n");
		return 1;
	}

	// Secret Key and Frame Id
	switch (param_operation) {
		case OP_ENCRYPT:
			printf("Using Secret Key: ");
			for (int i=0 ; i<SECRETKEY_BITS ; ++i)
				printf("%d", param_secretKey[i]);
			printf("\n");
		case OP_ATTACK:
			printf("Using Frame Id: ");
			for (int i=0 ; i<FRAMEID_BITS ; ++i)
				printf("%d", param_frameId[i]);
			printf("\n");
		default:;
	}



	// ========================================================================================== //
	//                                      PROCESSING PART                                       //
	// ========================================================================================== //


	byte G[SOURCEWORD_LENGTH][CODEWORD_LENGTH];

	FILE* sourcefile;
	FILE* destfile;

	unsigned int filesize = 0;
	unsigned int step     = 0;


	byte sourceword_BitBuffer[SOURCEWORD_LENGTH];
	byte sourceword_ByteBuffer[SOURCEWORD_LENGTH/8];
	byte codeword_BitBuffer[CODEWORD_LENGTH];
	byte codeword_ByteBuffer[CODEWORD_LENGTH/8];
	memset(sourceword_BitBuffer,  0, SOURCEWORD_LENGTH);
	memset(sourceword_ByteBuffer, 0, SOURCEWORD_LENGTH/8);
	memset(codeword_BitBuffer,    0, CODEWORD_LENGTH);
	memset(codeword_ByteBuffer,   0, CODEWORD_LENGTH/8);


	byte encrword_BitBuffer[CODEWORD_LENGTH];
	byte encrword_ByteBuffer[CODEWORD_LENGTH/8];
	memset(encrword_BitBuffer,  0, CODEWORD_LENGTH);
	memset(encrword_ByteBuffer, 0, CODEWORD_LENGTH/8);


	byte keystream_BitBuffer[CODEWORD_LENGTH];
	byte keystream_ByteBuffer[CODEWORD_LENGTH/8];
	memset(keystream_BitBuffer,  0, CODEWORD_LENGTH);
	memset(keystream_ByteBuffer, 0, CODEWORD_LENGTH/8);


	cipherTextArgs ctArgs;


	int dataread = 0;



    #define _LOAD_SOURCE                                                                               \
        sourcefile = fopen(param_sourcefile, "rb");                                                    \
        if (!sourcefile) {                                                                             \
            printf("Unable to open file [rb] '%s'\n", param_sourcefile);                               \
            return 1;                                                                                  \
        }                                                                                              \
        fseek(sourcefile, 0, SEEK_END); filesize = ftell(sourcefile); fseek(sourcefile, 0, SEEK_SET);


    #define _LOAD_DEST                                                                                 \
        destfile = fopen(param_destfile, "wb");                                                        \
        if (!destfile) {                                                                               \
            printf("Unable to open file [wb] '%s'\n", param_destfile);                                 \
            return 1;                                                                                  \
        }


	switch (param_operation) {

		case OP_ENCODE: // -------------------------------------------------------------------------

			printf("Initialization... ");

			_LOAD_SOURCE
			_LOAD_DEST

			processFullEncodingGMatrix(G);

			dataread = 0;
			memset(sourceword_ByteBuffer, 0, SOURCEWORD_LENGTH/8);
			while ((dataread = fread(sourceword_ByteBuffer, sizeof(byte), SOURCEWORD_LENGTH/8, sourcefile))) {

				CLEARLINE; printf("Encoding message... "); PROGRESSBAR_NOLF( (step+=dataread)*100/filesize );

				if (dataread < SOURCEWORD_LENGTH/8) {
					for (int k=(SOURCEWORD_LENGTH/8)-1 ; k>=dataread ; --k) {
						sourceword_ByteBuffer[k] = 0; // Padding of 0
					}
				}

				BYTE_VECTOR_TO_BIT_VECTOR(sourceword_ByteBuffer, sourceword_BitBuffer, SOURCEWORD_LENGTH);
				BINPRODUCT_VECTOR_MATRIX(sourceword_BitBuffer, G, codeword_BitBuffer, SOURCEWORD_LENGTH, CODEWORD_LENGTH);
				BIT_VECTOR_TO_BYTE_VECTOR(codeword_BitBuffer, codeword_ByteBuffer, CODEWORD_LENGTH);

				fwrite(codeword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, destfile);
				memset(sourceword_ByteBuffer, 0, SOURCEWORD_LENGTH/8);
			}

			fclose(destfile);
			fclose(sourcefile);

			CLEARLINE; printf("File successfully encoded                                                       \n");
			return 0;
			break;



		case OP_DECODE: // -------------------------------------------------------------------------

			printf("Initialization... ");

			_LOAD_SOURCE
			_LOAD_DEST

			dataread = 0;
			memset(codeword_ByteBuffer, 0, CODEWORD_LENGTH/8);
			while ((dataread = fread(codeword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, sourcefile))) {

				CLEARLINE; printf("Decoding message... "); PROGRESSBAR_NOLF( (step+=dataread)*100/filesize );

				if (dataread < CODEWORD_LENGTH/8) {
					CLEARLINE; printf("The file size isn't a multiple of 456bits: Unable to decode                     \n");
					return 1;
				}

				BYTE_VECTOR_TO_BIT_VECTOR(codeword_ByteBuffer, codeword_BitBuffer, CODEWORD_LENGTH);
				processFullDecoding(codeword_BitBuffer, sourceword_BitBuffer);
				BIT_VECTOR_TO_BYTE_VECTOR(sourceword_BitBuffer, sourceword_ByteBuffer, SOURCEWORD_LENGTH);

				fwrite(sourceword_ByteBuffer, sizeof(byte), SOURCEWORD_LENGTH/8, destfile);
				memset(codeword_ByteBuffer, 0, CODEWORD_LENGTH/8);
			}

			fclose(destfile);
			fclose(sourcefile);


			CLEARLINE; printf("File successfully decoded                                                       \n");
			return 0;
			break;



		case OP_ENCRYPT: // ------------------------------------------------------------------------

			printf("Initialization... ");

			_LOAD_SOURCE
			_LOAD_DEST

			keysetup(param_secretKey, param_frameId);

			dataread = 0;
			memset(encrword_ByteBuffer, 0, CODEWORD_LENGTH/8);
			while ((dataread = fread(encrword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, sourcefile))) {

				CLEARLINE; printf("Applying cipher... "); PROGRESSBAR_NOLF( (step+=dataread)*100/filesize );

				if (dataread < CODEWORD_LENGTH/8) {
					CLEARLINE; printf("Error: The file size isn't a multiple of 456bits: Unable to process             \n");
					return 1;
				}

				getKeystream(keystream_BitBuffer, CODEWORD_LENGTH);
				BIT_VECTOR_TO_BYTE_VECTOR(keystream_BitBuffer, keystream_ByteBuffer, CODEWORD_LENGTH);
				XOR_CHARARRAYS(encrword_ByteBuffer, keystream_ByteBuffer, CODEWORD_LENGTH/8);

				fwrite(encrword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, destfile);
				memset(encrword_ByteBuffer, 0, CODEWORD_LENGTH/8);
			}

			fclose(destfile);
			fclose(sourcefile);

			CLEARLINE; printf("File successfully encrypted/decrypted                                           \n");
			return 0;
			break;


		case OP_ATTACK: // ------------------------------------------------------------------------

			printf("Initialization... ");

			_LOAD_SOURCE


			dataread = 0;

			dataread += fread(encrword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, sourcefile);
			BYTE_VECTOR_TO_BIT_VECTOR(encrword_ByteBuffer, ctArgs.cipherText1, CODEWORD_LENGTH);

			dataread += fread(encrword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, sourcefile);
			BYTE_VECTOR_TO_BIT_VECTOR(encrword_ByteBuffer, ctArgs.cipherText2, CODEWORD_LENGTH);

			dataread += fread(encrword_ByteBuffer, sizeof(byte), CODEWORD_LENGTH/8, sourcefile);
			BYTE_VECTOR_TO_BIT_VECTOR(encrword_ByteBuffer, ctArgs.cipherText3, CODEWORD_LENGTH);

			fclose(sourcefile);

			if (dataread != 3*CODEWORD_LENGTH/8) {
				printf("Error: Source file too small to attack\n"); return 1;
			}

			memcpy(ctArgs.frameId, param_frameId, FRAMEID_BITS);


			byte decipheredSecretKey[SECRETKEY_BITS];

			initializeRAM("bin/matrices.bin");
			if (attack(&ctArgs, decipheredSecretKey)) {
				printf("Attack Failed.\n");
				freeRAM();
				return 1;
			}
			freeRAM();

			printf("Secret Key Found:\n");
			for (int i=0 ; i<SECRETKEY_BITS ; ++i) {
				printf("%d", decipheredSecretKey[i]);
			}
			printf("\n");

			return 0;
			break;


		case OP_PRECOMPUTE: // --------------------------------------------------------------------

			mkdir("bin", S_IRWXU | S_IRGRP | S_IROTH);
			return exportAllMatrices("bin/matrices.bin");
			break;


		case OP_AUTOTEST: // ----------------------------------------------------------------------

			;
			int total_tests=0, cumulative_res=0;

			printf("======= STARTING AUTOTESTS =======\n");

			printf("\n---- Testing Encoding and Decoding...\n");
			++total_tests;   cumulative_res += code_test();

			printf("\n---- Testing Keygen...\n");
			++total_tests;   cumulative_res += keygen_test();

			printf("\n---- Testing Keysetup Reverse...\n");
			++total_tests;   cumulative_res += keysetup_reverse_test();

			printf("\n---- Testing Matrices Generation...\n");
			++total_tests;   cumulative_res += matrices_generation_test();

			printf("\n---- Testing Attack...\n");
			++total_tests;   cumulative_res += attack_test();


			printf("\n\n\n======= TEST REPORT =======\n");
			printf("\n");
			printf("  %d tests out of %d succeeded\n", total_tests-cumulative_res, total_tests);

			return cumulative_res;
			break;

		default: // -------------------------------------------------------------------------------
			break;
	}

	// ========================================================================================== //

	return 0;

 }

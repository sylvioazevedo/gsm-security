/*============================================================================*
 *                                                                            *
 *                                   attack.h                                 *
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
  * @file attack.h
  * @brief Specification of A52 attack
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #10
  * @date 01/12/2011
  * */


#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "const_A52.h"
#include "const_code.h"


// Attack related constants

//! Number of empty equations remaining after proceeding to Gauss Elimination in (HS × ? = Syndrome)
#define SYNDROME_EMPTY_EQUATIONS (NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH - REGS_TOTAL_VARS)




/**
 * \struct cipherTextArgs
 * \brief Set of arguments to be provided to perform the attack
 *
 * cipherTextArgs contains three chunks of cipher text,
 * along with the Frame Id (which is publicly known)
 */
typedef struct {
	byte cipherText1[CODEWORD_LENGTH]; //!< First chunk of cipher text
	byte cipherText2[CODEWORD_LENGTH]; //!< Second chunk of cipher text
	byte cipherText3[CODEWORD_LENGTH]; //!< Third chunk of cipher text
	byte frameId[FRAMEID_BITS];        //!< Frame Id
} cipherTextArgs;




/**
 * \struct threadArgs
 * \brief Set of arguments related to a thread in a multithreaded attack context
 *
 * threadArgs contains a pointer to the problem to solve,
 * along with the bounds of the subset of solutions to explore.
 * It also points to a volatile flag indicating if the solution has been found by another thread.
 * Each set of parameters also contains a potential sorage for a result (to prevent from getting
 * problems with multiple accesses)
 */
typedef struct {
	cipherTextArgs* ctArgs;         //!< Problem to solve
	int lowindex;                   //!< Index to start the search form (inclusive)
	int highindex;                  //!< Last index to be analyzed (exclusive)
	int *keyFound;                  //!< Solution found flag
	byte secretKey[SECRETKEY_BITS]; //!< Local storage for a potential solution
} threadArgs;




/**
 * \fn void freeRAM()
 * \brief Frees the RAM used to store resolution matrices.
 */
void freeRAM();




/**
 * \fn int initializeRAM(const char* filename)
 * \brief Initializes the RAM storage of resolution matrices from a given binary file
 *
 * \param[in] filename Path of the file containing resolution matrices
 * \return 0 if the initialization is successfull, non-zero otherwise
 */
int initializeRAM(const char* filename);




/**
 * \fn int attack(cipherTextArgs* ctArgs, byte secretKey[SECRETKEY_BITS])
 * \brief Performs the attack on a given problem, then writes back the solution
 *
 * \param[in]  ctArgs Problem to be solved
 * \param[out] secretKey Deciphered secret key if the attack succeeded, all zeros otherwise
 * \return 0 if the attack is successfull, non-zero otherwise
 */
int attack(cipherTextArgs* ctArgs, byte secretKey[SECRETKEY_BITS]);




/**
 * \fn int attack_test()
 * \brief Autotests the attack on a verified set of problem/solution
 *
 * \return 0 if the test is successfull, non-zero otherwise
 */
int attack_test();




#endif

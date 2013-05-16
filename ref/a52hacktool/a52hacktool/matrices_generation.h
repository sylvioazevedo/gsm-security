/*============================================================================*
 *                                                                            *
 *                             matrices_generation.h                          *
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
  * @file matrices_generation.h
  * @brief Specification of first attack phase: matrices dictionary generation
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #13
  * @date 24/06/2011
  * */


#ifndef _MATRICES_GENERATION_H_
#define _MATRICES_GENERATION_H_

#include "const_A52.h"
#include "const_code.h"


// Decryption related constants

//! Number of ciphered messages needed to proceed to decryption
#define NEEDED_ENCRYPTED_MESSAGES 3

//! Number of equations to generate
#define EQN_SYSTEM_SIZE (NEEDED_ENCRYPTED_MESSAGES*CODEWORD_LENGTH)

//! Buffer size corresponding to the generated equations
#define BUFFER_SIZE     (NEEDED_ENCRYPTED_MESSAGES*SYNDROME_LENGTH*REGS_TOTAL_VARS/8)




/**
 * \fn void getR4fromIndex(const int index, byte R4[R4_BITS])
 * \brief Returns the value of LFSR R4 corresponding to a particular index in [0..TOTAL_MATRICES-1]
 *
 * \param[in] index Considered index
 * \param[in] R4 Corresponding value of R4
 */
void getR4fromIndex(const int index, byte R4[R4_BITS]);




/**
 * \fn int exportAllMatrices(const char* filename)
 * \brief Exports all Resolution Matrices into the specified file
 *
 * \param[in] filename Path of the file to export to
 * \return 0 if the export is successfull, non-zero otherwise
 */
int exportAllMatrices(const char* filename);




/**
 * \fn int matrices_generation_test()
 * \brief Autotests the matrices generation on a verified set
 *
 * \return 0 if the test is successfull, non-zero otherwise
 */
int matrices_generation_test();



#endif

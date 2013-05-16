/*============================================================================*
 *                                                                            *
 *                                    code.h                                  *
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
  * @file code.h
  * @brief Specification of full coding & decoding process
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #13
  * @date 01/12/2011
  * */


#ifndef _CODE_H_
#define _CODE_H_

#include "utils.h"

#include "firecode.h"
#include "const_code.h"
#include "convolution.h"
#include "interleaving.h"




/**
 * \fn void processFullEncodingGMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH])
 * \brief Calculates the encoding matrix corresponding to all encoding steps (Fire code, Convolution and Interleaving)
 *
 * \param[out] G Full encoding matrix
 */
void processFullEncodingGMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH]);




/**
 * \fn void processFullEncodingGSystematicMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH], byte L[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH], byte P[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH])
 * \brief Calculates the encoding matrix corresponding to all encoding steps (Fire code, Convolution and Interleaving) in systematic form
 *
 * \param[out] G Full encoding matrix in systematic form
 * \param[out] L Invertible matrix
 * \param[out] P Permutation matrix
 */
void processFullEncodingGSystematicMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH], byte L[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH], byte P[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH]);



/**
 * \fn void processFullEncodingHSystematicMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH], byte H[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH])
 * \brief Calculates the systematic parity-check matrix H associated to the code matrix G in systematic form.
 *
 * \param[in]  G Full encoding matrix in systematic form
 * \param[out] H Parity-check matrix in systematic form
 */
void processFullEncodingHSystematicMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH], byte H[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH]);




/**
 * \fn void processFullEncodingHMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH], byte H[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH])
 * \brief Calculates the real parity-check matrix H associated to the code matrix G. Thus, H×g=0 iff g is a codeword.
 *
 * \param[in]  G Full encoding matrix
 * \param[out] H Parity-check matrix
 */
void processFullEncodingHMatrix(byte G[FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH], byte H[INTERLEAVING_LENGTH - FIRE_SOURCE_LENGTH][INTERLEAVING_LENGTH]);




/**
 * \fn void InvertMatrixL(byte L[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH], byte L_I[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH])
 * \brief Inverts the provided L matrix (peculiar case of Gauss elimination when L is a square matrix of size FIRE_SOURCE_LENGTH)
 *
 * \param[in,out] L Invertible matrix, modified by the function to become Identity Matrix
 * \param[out]    L_I inverse of the initial L matrix
 */
void InvertMatrixL(byte L[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH], byte L_I[FIRE_SOURCE_LENGTH][FIRE_SOURCE_LENGTH]);




/**
 * \fn void processFullDecoding(byte codeword[CODEWORD_LENGTH], byte sourceword[SOURCEWORD_LENGTH])
 * \brief Decodes a previously encoded message
 *
 * \param[in]  codeword Data to decode
 * \param[out] sourceword Decoded data
 */
void processFullDecoding(byte codeword[CODEWORD_LENGTH], byte sourceword[SOURCEWORD_LENGTH]);




/**
 * \fn int code_test()
 * \brief Autotests the encoding and decoding on a random set of problem/solution
 *
 * \return 0 if the test is successfull, non-zero otherwise
 */
int code_test();




#endif

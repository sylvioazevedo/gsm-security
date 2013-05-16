/*============================================================================*
 *                                                                            *
 *                                  firecode.h                                *
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
  * @file firecode.h
  * @brief Specification of first encoding phase: Fire code (CRC)
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #9
  * @date 24/06/2011
  * */


#ifndef _FIRECODE_H_
#define _FIRECODE_H_

#include "const_code.h"




/**
 * \fn void processFireGMatrix(byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH])
 * \brief Calculates the encoding matrix corresponding to the Fire encoding step
 *
 * \param[out] G Fire encoding matrix
 */
void processFireGMatrix(byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH]);




/**
 * \fn void processFireHMatrix(byte H[FIRE_CRC_LENGTH][FIRE_CODE_LENGTH], byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH])
 * \brief Calculates the parity-check matrix H corresponding to the Fire encoding step from the Fire encoding matrix G
 *
 * \param[out] H Fire parity-check matrix
 * \param[in]  G Fire encoding matrix
 */
void processFireHMatrix(byte H[FIRE_CRC_LENGTH][FIRE_CODE_LENGTH], byte G[FIRE_SOURCE_LENGTH][FIRE_CODE_LENGTH]);




/**
 * \fn void reverseFire(byte in[FIRE_CODE_LENGTH], byte out[FIRE_SOURCE_LENGTH])
 * \brief Decodes a previously Fire-encoded message
 *
 * \param[in]  in Fire-encoded data to decode
 * \param[out] out Decoded data
 */
void reverseFire(byte in[FIRE_CODE_LENGTH], byte out[FIRE_SOURCE_LENGTH]);




#endif

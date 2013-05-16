/*============================================================================*
 *                                                                            *
 *                                interleaving.h                              *
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
  * @file interleaving.h
  * @brief Specification of third encoding & decoding phase: Interleaving
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #7
  * @date 24/06/2011
  * */


#ifndef _INTERLEAVING_H_
#define _INTERLEAVING_H_

#include "const_code.h"




/**
 * \fn void processInterleavingGMatrix(byte G[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH])
 * \brief Calculates the encoding matrix corresponding to the Interleaving encoding step
 *
 * \param[out] G Interleaving encoding matrix
 */
void processInterleavingGMatrix(byte G[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH]);




/**
 * \fn void reverseInterleaving(byte in[INTERLEAVING_LENGTH], byte out[INTERLEAVING_LENGTH])
 * \brief Decodes a previously interleaved message
 *
 * \param[in]  in Interleaved data to decode
 * \param[out] out Decoded data
 */
void reverseInterleaving(byte in[INTERLEAVING_LENGTH], byte out[INTERLEAVING_LENGTH]);



#endif

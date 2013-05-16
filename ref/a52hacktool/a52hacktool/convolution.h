/*============================================================================*
 *                                                                            *
 *                                convolution.h                               *
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
  * @file convolution.h
  * @brief Specification of second encoding & decoding phase: convolution
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #8
  * @date 24/06/2011
  * */

#ifndef _CONVOLUTION_H_
#define _CONVOLUTION_H_

#include "const_code.h"




/**
 * \fn void processConvolutionGMatrix(byte G[CONVOLUTION_SOURCE_LENGTH][CONVOLUTION_CODE_LENGTH])
 * \brief Calculates the encoding matrix corresponding to the Convolution encoding step
 *
 * \param[out] G Convolution encoding matrix
 */
void processConvolutionGMatrix(byte G[CONVOLUTION_SOURCE_LENGTH][CONVOLUTION_CODE_LENGTH]);




/**
 * \fn void reverseConvolution(byte in[CONVOLUTION_CODE_LENGTH], byte out[CONVOLUTION_SOURCE_LENGTH])
 * \brief Decodes a previously convoluted message
 *
 * \param[in]  in Convoluted data to decode
 * \param[out] out Decoded data
 */
void reverseConvolution(byte in[CONVOLUTION_CODE_LENGTH], byte out[CONVOLUTION_SOURCE_LENGTH]);




#endif

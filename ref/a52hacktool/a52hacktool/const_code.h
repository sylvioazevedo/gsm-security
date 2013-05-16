/*============================================================================*
 *                                                                            *
 *                                const_code.h                                *
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
  * @file const_code.h
  * @brief Constants definition for encoding process inside GSM. These are not to be modified,
  * as one single change could have desastrous consequences on the global process
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #7
  * @date 24/06/2011
  * */

#ifndef _CONST_CODE_H_
#define _CONST_CODE_H_




// ========================= Data length relative to full code, input size =========================

//! Length of a source word (in bits)
#define SOURCEWORD_LENGTH 184




// ======================= Constants relative to specific encoders/decoders ========================

// Fire Encoding - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// We consider here the following polynomial for GSM Fire code (SACCH):
//                (D^23 + 1)*(D^17 + D^3 + 1)
// which can be developped into:
//             D^40 + D^26 + D^23 + D^17 + D^3 + D^0
// The polynomial is divided into two parts which can be recombined using the following formula:
// (UPPER_POLYNOMIAL << 9) | (LOWER_POLYNOMIAL >> 23)
// Note that the polynomial representation is mirrored compared to 0x80024100 || 0x04800000 !
#define UPPER_POLYNOMIAL  ((unsigned int)0x90004120);        //!< Upper polynomial representation
#define LOWER_POLYNOMIAL  ((unsigned int)0x800000);          //!< Lower polynomial representation

#define FIRE_POLYNOMIAL_LENGTH 41                            //!< Meaningful length (in bits)
#define LOWER_PADDINGBITS   (32*2 - FIRE_POLYNOMIAL_LENGTH)  //!< Number of padding bits

//! Fire code input length (in bits)
#define FIRE_SOURCE_LENGTH  SOURCEWORD_LENGTH                                 // 184
//! Fire code output length (in bits)
#define FIRE_CODE_LENGTH    (FIRE_SOURCE_LENGTH + FIRE_POLYNOMIAL_LENGTH - 1) // 224
//! Fire code CRC length (in bits)
#define FIRE_CRC_LENGTH     (FIRE_CODE_LENGTH - FIRE_SOURCE_LENGTH)           // 40


// Convolution - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Before convolution is applied to a fired message, 4bits are appended to the input (all 0s)

//! Convolution input length (in bits)
#define CONVOLUTION_SOURCE_LENGTH (FIRE_CODE_LENGTH + 4)                      // 228
//! Convolution output length (in bits)
#define CONVOLUTION_CODE_LENGTH   (CONVOLUTION_SOURCE_LENGTH * 2)             // 456


// Interleaving  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//! Convolution input length (in bits)
#define INTERLEAVING_LENGTH CONVOLUTION_CODE_LENGTH                           // 456




// ========================= Data length relative to full code, output size ========================

//! Length of a code word (in bits)
#define CODEWORD_LENGTH INTERLEAVING_LENGTH                                   // 456

//! Syndrome length (in bits), defined by the code parity-check matrix
#define SYNDROME_LENGTH (CODEWORD_LENGTH - SOURCEWORD_LENGTH)                 // 272




#endif

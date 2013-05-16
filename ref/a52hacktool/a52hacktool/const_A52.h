/*============================================================================*
 *                                                                            *
 *                                 const_A52.h                                *
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
  * @file const_A52.h
  * @brief Constants definition for A5/2 cipher. These are not to be modified,
  * as one single change could have desastrous consequences on the global process
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #8
  * @date 24/06/2011
  * */

#ifndef _CONST_A52_H_
#define _CONST_A52_H_

//@{
//! LFSR capacity
#define R1_BITS 19
#define R2_BITS 22
#define R3_BITS 23
#define R4_BITS 17
#define REGS_BITS (R1_BITS+R2_BITS+R3_BITS+R4_BITS)
//@}


//@{
//! Clocking Unit Tap
#define R4_CLOCKTAP_R1 10
#define R4_CLOCKTAP_R2 3
#define R4_CLOCKTAP_R3 7
//@}


//@{
//! Output Tap
#define R1_OUTTAP_1 12
#define R1_OUTTAP_2 14 //!< Output Tap to be xored with 1
#define R1_OUTTAP_3 15
#define R2_OUTTAP_1 9
#define R2_OUTTAP_2 13
#define R2_OUTTAP_3 16 //!< Output Tap to be xored with 1
#define R3_OUTTAP_1 13 //!< Output Tap to be xored with 1
#define R3_OUTTAP_2 16
#define R3_OUTTAP_3 18
//@}


//@{
//! Shift Tap
#define R1_SHIFTTAP_1 13
#define R1_SHIFTTAP_2 16
#define R1_SHIFTTAP_3 17
#define R1_SHIFTTAP_4 18
#define R2_SHIFTTAP_1 20
#define R2_SHIFTTAP_2 21
#define R3_SHIFTTAP_1  7
#define R3_SHIFTTAP_2 20
#define R3_SHIFTTAP_3 21
#define R3_SHIFTTAP_4 22
#define R4_SHIFTTAP_1 11
#define R4_SHIFTTAP_2 16
//@}


//@{
//! Location of Constant Value defined at the end of KeySetup Phase
#define R1_INITIAL_CONST_POS 15
#define R2_INITIAL_CONST_POS 16
#define R3_INITIAL_CONST_POS 18
#define R4_INITIAL_CONST_POS 10
//@}


//! Secret Key length
#define SECRETKEY_BITS 64
//! Frame Id length
#define FRAMEID_BITS   22


//@{
//! Simple Variables (exactly one location of each register is set to constant "1" at the beginning, thus reducing the number of variables to R{1,2,3}_SIMPLE_VARS = (R{1,2,3}_BITS-1))
#define R1_SIMPLE_VARS   (R1_BITS - 1)
#define R2_SIMPLE_VARS   (R2_BITS - 1)
#define R3_SIMPLE_VARS   (R3_BITS - 1)
#define REGS_SIMPLE_VARS (R1_SIMPLE_VARS + R2_SIMPLE_VARS + R3_SIMPLE_VARS)
//@}


//@{
//! Product Variables (products may be done at the register level when outputing keystream)
#define R1_PROD_VARS ((R1_SIMPLE_VARS * (R1_SIMPLE_VARS-1)) / 2)
#define R2_PROD_VARS ((R2_SIMPLE_VARS * (R2_SIMPLE_VARS-1)) / 2)
#define R3_PROD_VARS ((R3_SIMPLE_VARS * (R3_SIMPLE_VARS-1)) / 2)
#define REGS_PROD_VARS (R1_PROD_VARS + R2_PROD_VARS + R3_PROD_VARS)
//@}


//! Total Amount of Variables (including constant "1")
#define REGS_TOTAL_VARS (REGS_SIMPLE_VARS + REGS_PROD_VARS + 1) // +1 for constant "1"






//! Number of possible values of the fourth LFSR (R4)
#define TOTAL_MATRICES     (1<<(R4_BITS-1))
//! Number of threads used for both matrices generation and attack (must be a power of 2)
#define PROCESSING_THREADS 4 //(1<<5)
//! Amount of data that will be processed by each thread
#define THREAD_CHUNKSIZE   (TOTAL_MATRICES/PROCESSING_THREADS)


//! A matrix of integers will be used for faster decryption. The last integer of the line will be half-loaded with two bytes
#define RESOLUTION_MATRIX_INT_WIDTH ((REGS_TOTAL_VARS+31)/32)



#endif

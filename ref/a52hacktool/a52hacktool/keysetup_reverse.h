/*============================================================================*
 *                                                                            *
 *                              keysetup_reverse.h                            *
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
  * @file keysetup_reverse.h
  * @brief Specification of last attack phase on A5/2: keysetup reversal
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #3
  * @date 24/06/2011
  * */


#ifndef _KEYSETUP_REVERSE_H_
#define _KEYSETUP_REVERSE_H_

#include "const_A52.h"




/**
 * \fn int reverseKeysetup(const byte R1[R1_BITS], const byte R2[R2_BITS], const byte R3[R3_BITS], const byte R4[R4_BITS], const byte frameId[FRAMEID_BITS], byte secretKey[SECRETKEY_BITS])
 * \brief Performs the reversal of the keysetup, retrieving the Secret Key from the LFSRs state after keysetup
 *
 * \param[in] R1 Status of register R1 after keysetup
 * \param[in] R2 Status of register R2 after keysetup
 * \param[in] R3 Status of register R3 after keysetup
 * \param[in] R4 Status of register R4 after keysetup
 * \param[in] frameId Publicly known Frame Id
 * \param[out] secretKey Recovered secret key if the attack succeeded, all zeros otherwise
 * \return 0 if the reversal is successful, non-zero otherwise
 */
int reverseKeysetup(const byte R1[R1_BITS], const byte R2[R2_BITS], const byte R3[R3_BITS],        \
                    const byte R4[R4_BITS], const byte frameId[FRAMEID_BITS],                      \
                    byte secretKey[SECRETKEY_BITS]);




/**
 * \fn int keysetup_reverse_test()
 * \brief Autotests the reversal of the keysetup on a verified set of problem/solution
 *
 * \return 0 if the test is successfull, non-zero otherwise
 */
int keysetup_reverse_test();




#endif

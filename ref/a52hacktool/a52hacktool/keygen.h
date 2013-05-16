/*============================================================================*
 *                                                                            *
 *                                   keygen.h                                 *
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
  * @file keygen.h
  * @brief Specification of A5/2 keystream generation
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #6
  * @date 24/06/2011
  * */


#ifndef _KEYGEN_H_
#define _KEYGEN_H_

#include "const_A52.h"




/**
 * \fn void keysetup(const byte Kc[SECRETKEY_BITS], const byte frameId[FRAMEID_BITS])
 * \brief Initializes the whole system according to the keysetup procedure
 *
 * \param[in] Kc Secret Key
 * \param[in] frameId Frame Id
 */
void keysetup(const byte Kc[SECRETKEY_BITS], const byte frameId[FRAMEID_BITS]);




/**
 * \fn void getKeystream(byte keystream[], const int len)
 * \brief Generates the desired amount of keystream
 *
 * \param[out] keystream Keystream to be generated
 * \param[in]  len desired amount of keystream bits
 */
void getKeystream(byte keystream[], const int len);




/**
 * \fn int keygen_test()
 * \brief Autotests the key generation on a verified set
 *
 * \return 0 if the test is successfull, non-zero otherwise
 */
int keygen_test();




#endif

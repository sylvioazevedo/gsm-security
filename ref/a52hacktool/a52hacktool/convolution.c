/*============================================================================*
 *                                                                            *
 *                                convolution.c                               *
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
  * @file convolution.c
  * @brief Implementation of second encoding & decoding phase: convolution
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #10
  * @date 24/06/2011
  * */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

#include "convolution.h"

#include "const_code.h"




// Documentation in header file
void processConvolutionGMatrix(byte G[CONVOLUTION_SOURCE_LENGTH][CONVOLUTION_CODE_LENGTH]) {

	memset(G, 0, CONVOLUTION_SOURCE_LENGTH*CONVOLUTION_CODE_LENGTH*sizeof(byte));

	byte pattern[10] = {1,1,0,1,0,0,1,1,1,1};

	//   +-------- polynomial for even bits
	//   |+-------- polynomial for odd bits
	//   ||
	//   vv
	//
	//   11
	//   1111
	//   001111
	//  _01001111________
	//=>|1101001111.. <= pattern to propagate through G. Everything else is 0 (..)
	//  |..1101001111
	//  |....11010011
	//  |......110100
	//  |........1101
	//  |..........11
	//  |
	for (int i=0 ; i<CONVOLUTION_SOURCE_LENGTH ; ++i) {
		memcpy(G[i]+2*i, pattern, ((i<CONVOLUTION_SOURCE_LENGTH-4)? 10 : CONVOLUTION_CODE_LENGTH - 2*i)*sizeof(byte));
	}

}




// Documentation in header file
void reverseConvolution(byte in[CONVOLUTION_CODE_LENGTH], byte out[CONVOLUTION_SOURCE_LENGTH]) {

	memset(out, 0, CONVOLUTION_SOURCE_LENGTH*sizeof(byte));
	byte bit_0, bit_1;

	for (int i=0 ; i<(CODEWORD_LENGTH/32)+1 ; ++i) {
		for (int b=2 ; b<32 ; ){
			bit_0 = in[32*i + (b++)];
			bit_1 = in[32*i + (b++)];
			// Since c(2*k) = u(k)^u(k-3)^u(k-4) and c(2*k + 1) = u(k)^u(k-1)^u(k-3)^u(k-4)
			// u((k-1)/2) = c(k)^c(k+1) for all even values of k
			out[16*i + (b-2)/2 - 1] = bit_0 ^ bit_1;
		}
		//We must handle the last bit of each half of the new int
		out[16*i + 15] = bit_0 ^ out[16*i + 15 - 3] ^ out[16*i + 15 - 4];
	}

}



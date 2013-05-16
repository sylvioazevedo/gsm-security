/*============================================================================*
 *                                                                            *
 *                                interleaving.c                              *
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
  * @file interleaving.c
  * @brief Implementation of third encoding & decoding phase: Interleaving
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #8
  * @date 24/06/2011
  * */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "const_code.h"
#include "interleaving.h"




// Documentation in header file
void processInterleavingGMatrix(byte G[INTERLEAVING_LENGTH][INTERLEAVING_LENGTH]) {

	memset(G, 0, INTERLEAVING_LENGTH*INTERLEAVING_LENGTH*sizeof(byte));

	unsigned int B[8][114];
	memset(B, 0, 8*114*sizeof(unsigned int));

	for (int k=0 ; k<INTERLEAVING_LENGTH ; ++k) {
		B[k%8][(2*((49*k)%57) + ((k%8)/4))] = k;
	}

	int i=0;
	for(int k=0; k<4; ++k){
		for(int j=0; j<57; ++j){
			G[B[k][2*j]][i] = 1;
			i++;
			G[B[k+4][2*j+1]][i] = 1;
			i++;
		}
	}

}




// Documentation in header file
void reverseInterleaving(byte in[INTERLEAVING_LENGTH], byte out[INTERLEAVING_LENGTH]) {

	memset(out, 0, CONVOLUTION_SOURCE_LENGTH*sizeof(byte));

	unsigned int B[8][114];
	memset(B, 0, 8*114*sizeof(unsigned int));

	for (int k=0 ; k<INTERLEAVING_LENGTH ; ++k) {
		B[k%8][(2*((49*k)%57) + ((k%8)/4))] = k;
	}

	int i=0;
	for(int k=0; k<4; ++k){
		for(int j=0; j<57; ++j){
			out[B[k][2*j]] = in[i++];
			out[B[k+4][2*j+1]] = in[i++];
		}
	}
}

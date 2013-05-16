/*============================================================================*
 *                                                                            *
 *                                   utils.c                                  *
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
  * @file utils.c
  * @brief Utilitary tools
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #6
  * @date 24/06/2011
  * */

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"




// Documentation in header file
int stringToByteArray(char* s, byte a[], unsigned int len) {
	if (strlen(s) != len)
		return 1;
	for (unsigned int i=0 ; i<len ; ++i) {
		switch(s[i]) {
			case '0': a[i] = 0; break;
			case '1': a[i] = 1; break;
			default: return 1;
		}
	}
	return 0;
}




// Documentation in header file
int fileExists(const char * filename) {
	FILE * file;
	if ((file = fopen(filename, "rb"))) {
		fclose(file);
        return 1;
	}
    return 0;
}




// Documentation in header file
long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time) {

	struct timeval temp_diff;

	if (difference == NULL) {
		difference = &temp_diff;
	}

	difference->tv_sec  = end_time->tv_sec  - start_time->tv_sec ;
	difference->tv_usec = end_time->tv_usec - start_time->tv_usec;

	while (difference->tv_usec < 0) {
		difference->tv_usec += 1000000;
		difference->tv_sec  -= 1;
	}

	return 1000000LL * difference->tv_sec + difference->tv_usec;

}

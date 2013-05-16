#============================================================================#
#                                                                            #
#                                   Makefile                                 #
#                                                                            #
#============================================================================#
#                                                                            #
# Part of A52HackTool                                                        #
#                                                                            #
# Copyright © 2011   -   Nicolas Paglieri   &   Olivier Benjamin             #
# All rights reserved.                                                       #
#                                                                            #
# Contact Information:  nicolas.paglieri [at] ensimag.fr                     #
#                       olivier.benjamin [at] ensimag.fr                     #
#                                                                            #
#============================================================================#
#                                                                            #
# This file may be used under the terms of the GNU General Public License    #
# version 3 as published by the Free Software Foundation.                    #
# See <http://www.gnu.org/licenses/> or GPL.txt included in the packaging of #
# this file.                                                                 #
#                                                                            #
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE    #
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  #
#                                                                            #
#============================================================================#

CC        = gcc
CFLAGS    = -Wall -Wextra -std=c99 -O3 -Wno-unused
LIBS      = -lm -lpthread

#----------------------------------------------------------------------------#

OBJS_CODE = code.o firecode.o convolution.o interleaving.o
OBJS_A52  = keygen.o keysetup_reverse.o matrices_generation.o attack.o

OBJS_AUX  = utils.o $(OBJS_CODE) $(OBJS_A52)
OBJS      = main.o  $(OBJS_AUX)

#----------------------------------------------------------------------------#

a52hacktool:	$(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o a52hacktool $(LIBS)

main.o:	main.c
	gcc -c main.c $(CFLAGS)

$(OBJS_AUX):	%.o:	%.c %.h
	$(CC) -c $<  $(CFLAGS)

#----------------------------------------------------------------------------#

clean:
	rm -f *.o a52hacktool *~

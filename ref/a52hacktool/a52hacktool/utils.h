/*============================================================================*
 *                                                                            *
 *                                   utils.h                                  *
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
  * @file utils.h
  * @brief Utilitary tools & Macros
  *
  * @author Nicolas Paglieri  &  Olivier Benjamin
  * @version File Revision #20
  * @date 24/06/2011
  * */


#ifndef _UTILS_H_
#define _UTILS_H_


//! Type byte is defined here to improve portability
typedef unsigned char byte;


/**
 * \fn int stringToByteArray(char* s, byte a[], unsigned int len)
 * \brief Translates a string composed of "0"s or "1"s into a byte array
 *
 * \param[in]  s Input string
 * \param[out] a Array to construct
 * \param[in]  len Length of the string
 * \return 0 if the translation has been successful, non-zero otherwise
 */
int stringToByteArray(char* s, byte a[], unsigned int len);


/**
 * \fn int fileExists(const char * filename)
 * \brief Checks the accessibility of a file for reading
 *
 * \param[in] filename Path of the file to test
 * \return non-zero if the file is accessible, 0 otherwise
 */
int fileExists(const char * filename);



#include <sys/time.h>
/**
 * \fn long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time)
 * \brief Computes the difference between two time values
 *
 * \param[out] difference Processed difference
 * \param[in]  end_time End time
 * \param[in]  start_time Start time

 * \return time difference in microseconds
 */
long long timeval_diff(struct timeval *difference, struct timeval *end_time, struct timeval *start_time);



//! Debug Flag. Set to non-zero to activate debug info display
#define DEBUG_INFO 0


//! Standard min math function, returns the minimum of \a a and \a b
#define MIN(a,b) (((a)<(b)) ? (a) : (b))
//! Standard max math function, returns the maximum of \a a and \a b
#define MAX(a,b) (((a)>(b)) ? (a) : (b))


//! Returns the most significant bit of a given 32bit-Integer \a i
#define MSBIT(i) (((i) & 0x80000000) >> 31)


// Operations on compact bit storage (32bit-integer array, every bit is meaningful)
//! Sets the \a i-th bit of the compact int storage array \a a (32bit per int) to the value \a bit
#define SET_INTARRAY_BIT(a,i,bit)                                                             \
        do {                                                                                  \
            unsigned int __i__ = (i);                                                         \
            byte __tmp__ = (bit)&1;                                                           \
            (a)[__i__/32] &= ~(1 << (31-(__i__%32)));                                         \
            (a)[__i__/32] |= ((__tmp__) << (31-(__i__%32)));                                  \
        } while (0)
//! Gets the \a i-th bit of the compact int storage array \a a (32bit per int)
#define GET_INTARRAY_BIT(a,i)                                                                 \
		(((int)(i)<0) ? 0 : ((a)[(i)/32] >> (31-((i)%32)))&1)


// Operations on compact bit storage (8bit-char array, every bit is meaningful)
//! Sets the \a i-th bit of the compact byte storage array \a a (8bit per byte) to the value \a bit
#define SET_CHARARRAY_BIT(a,i,bit)                                                            \
        do {                                                                                  \
            unsigned int __i__ = (i);                                                         \
            byte __tmp__ = (bit)&1;                                                           \
            (a)[__i__/8] &= ~(1 << (7-(__i__%8)));                                            \
            (a)[__i__/8] |= ((__tmp__) << (7-(__i__%8)));                                     \
        } while (0)
//! Gets the \a i-th bit of the compact byte storage array \a a (8bit per byte)
#define GET_CHARARRAY_BIT(a,i)                                                                \
		(((int)(i)<0) ? 0 : ((a)[(i)/8] >> (7-((i)%8)))&1)


//! XOR operation on two byte vectors \a V1 & \a V2 of length \a len, returning the result in \a V1
#define XOR_CHARARRAYS(V1, V2, len)                                                           \
        do {                                                                                  \
            unsigned int __len__ = (len);                                                     \
            for (unsigned int __i__=0 ; __i__<__len__ ; ++__i__) {                            \
                (V1)[__i__] ^= (V2)[__i__];                                                   \
            }                                                                                 \
        } while (0)


//! Returns \a 1 if and only if at least 2 bits out of 3 are non-zero
#define MAJORITY(bit1, bit2, bit3)                                                            \
        ( ( ((bit1)!=0) + ((bit2)!=0) + ((bit3)!=0) >= 2 ) & 1 )


//! Displays a char matrix \a M of \a lines lines and \a cols columns, labelled \a name
#define DUMP_CHAR_MATRIX(M, lines, cols, name)                                                \
        do {                                                                                  \
            unsigned int __lines__ = (lines);                                                 \
            unsigned int __cols__  = (cols);                                                  \
            printf("[%s: %s, l.%u] Matrix " name "\n", __FILE__,  __func__, __LINE__);        \
            for (unsigned int __i__=0 ; __i__<__lines__ ; ++__i__) {                          \
                for (unsigned int __j__=0 ; __j__<__cols__ ; ++__j__) {                       \
                    printf("%d", (M)[__i__][__j__]);                                          \
                }                                                                             \
                printf("\n");                                                                 \
            }                                                                                 \
        } while (0)


//! Displays a char vector \a V of \a len elements, labelled \a name (single-line display)
#define DUMP_CHAR_VECTOR(V, len, name)                                                        \
        do {                                                                                  \
            unsigned int __len__ = (len);                                                     \
            printf("[%s: %s, l.%u] Vector " name "\n", __FILE__,  __func__, __LINE__);        \
            for (unsigned int __i__=0 ; __i__<__len__ ; ++__i__) {                            \
                printf("%d", (V)[__i__]);                                                     \
            }                                                                                 \
            printf("\n");                                                                     \
        } while (0)


//! Displays a int vector \a V of \a len elements, labelled \a name (multi-line display)
#define DUMP_INT_VECTOR(V, len, name)                                                         \
        do {                                                                                  \
            unsigned int __len__ = (len);                                                     \
            printf("[%s: %s, l.%u] Vector " name "\n", __FILE__,  __func__, __LINE__);        \
            for (unsigned int __i__=0 ; __i__<__len__ ; ++__i__) {                            \
                printf("%08X\n", (V)[__i__]);                                                 \
            }                                                                                 \
        } while (0)


//! Displays some debug (includes file name and line, along with parent function)
#define DEBUG(msg, ...)                                                                       \
    do                                                                                        \
        printf("[%s: %s, l.%u] " msg "\n", __FILE__,                                          \
                                           __func__,                                          \
                                           __LINE__,                                          \
                                           ##__VA_ARGS__);                                    \
    while (0)


//! Simply prints a new line
#define DEBUG_LF                                                                              \
    do                                                                                        \
        printf("\n");                                                                         \
    while (0)


//! Initializes Matrix \a I to Identity matrix of size \a size
#define CHAR_IDENTITY(I, size)                                                                \
    do {                                                                                      \
        unsigned int __size__ = (size);                                                       \
        memset((I), 0, __size__*__size__*sizeof(char));                                       \
        for(unsigned int __i__=0 ; __i__<__size__ ; ++__i__){                                 \
            (I)[__i__][__i__]=1;                                                              \
        }                                                                                     \
     } while(0)


/** \brief Processes the result of binary char matrix multiplication (\a M1 × \a M2) into \a PROD.
 * \a PROD_lines represent the result line size, \a PROD_cols represent the result col size,
 * \a depth represent the common dimension of \a M1 and \a M2. */
#define BINPRODUCT_MATRIX_MATRIX(M1, M2, PROD, PROD_lines, PROD_cols, depth)                 \
        do {                                                                                 \
            unsigned int __PROD_lines__ = (PROD_lines);                                      \
            unsigned int __PROD_cols__ = (PROD_cols);                                        \
            unsigned int __depth__ = (depth);                                                \
            memset((PROD), 0, __PROD_lines__*__PROD_cols__*sizeof(byte));                    \
            for (unsigned int __line__=0 ; __line__<__PROD_lines__ ; ++__line__) {           \
                for (unsigned int __col__=0 ; __col__<__PROD_cols__ ; ++__col__) {           \
                    for (unsigned int __i__=0 ; __i__<__depth__ ; ++__i__) {                 \
                        (PROD)[__line__][__col__] ^= ((M1)[__line__][__i__])                 \
                                                 & ((M2)[__i__][__col__])  & 1;              \
                    }                                                                        \
                }                                                                            \
            }                                                                                \
        } while (0)


/** \brief Processes the result of binary char matrix vector multiplication (\a M × \a V) into \a PROD.
 * \a M_lines represent the matrix line size, \a M_cols represent the matrix col size */
#define BINPRODUCT_MATRIX_VECTOR(M, V, PROD, M_lines, M_cols)                                \
        do {                                                                                 \
            unsigned int __M_lines__ = (M_lines);                                            \
            unsigned int __M_cols__ = (M_cols);                                              \
            memset((PROD), 0, __M_lines__*sizeof(byte));                                     \
            for (unsigned int __line__=0 ; __line__<__M_lines__ ; ++__line__) {              \
                for (unsigned int __i__=0 ; __i__<__M_cols__ ; ++__i__) {                    \
                    (PROD)[__line__] ^= ((M)[__line__][__i__]) & ((V)[__i__]) & 1;           \
                }                                                                            \
            }                                                                                \
        } while (0)


/** \brief Processes the result of binary char vector matrix multiplication (\a V × \a M) into \a PROD.
 * \a M_lines represent the matrix line size, \a M_cols represent the matrix col size */
#define BINPRODUCT_VECTOR_MATRIX(V, M, PROD, M_lines, M_cols)                                \
        do {                                                                                 \
            unsigned int __M_lines__ = (M_lines);                                            \
            unsigned int __M_cols__ = (M_cols);                                              \
            memset((PROD), 0, __M_cols__*sizeof(byte));                                      \
            for (unsigned int __col__=0 ; __col__<__M_cols__ ; ++__col__) {                  \
                for (unsigned int __i__=0 ; __i__<__M_lines__ ; ++__i__) {                   \
                    (PROD)[__col__] ^= ((M)[__i__][__col__]) & ((V)[__i__]) & 1;             \
                }                                                                            \
            }                                                                                \
        } while (0)


//(CHAR_V_size+31)/32 ensures that the size is right even if CHAR_V_size is not a multiple of 32
//! Translates a sparse-byte representation (1bit per byte) into a full-integer one (32bit per int)
#define CHAR_VECTOR_TO_INT_VECTOR(CHAR_V, INT_V, CHAR_V_size)                                \
        do {                                                                                 \
            unsigned int __CHAR_V_size__ = (CHAR_V_size);                                    \
            memset((INT_V), 0, ((__CHAR_V_size__+31)/32)*sizeof(unsigned int));              \
            for (unsigned int __k__=0 ; __k__<__CHAR_V_size__ ; ++__k__) {                   \
                SET_INTARRAY_BIT((INT_V), __k__, ((CHAR_V)[__k__]));                         \
            }                                                                                \
        } while (0)


//! Translates a full-integer representation (32bit per int) into a sparse-byte one (1bit per byte)
#define INT_VECTOR_TO_CHAR_VECTOR(INT_V, CHAR_V, CHAR_V_size)                                \
        do {                                                                                 \
            unsigned int __CHAR_V_size__ = (CHAR_V_size);                                    \
            memset((CHAR_V), 0, __CHAR_V_size__*sizeof(byte));                               \
            for (unsigned int __k__=0 ; __k__<__CHAR_V_size__ ; ++__k__) {                   \
                (CHAR_V)[__k__] = GET_INTARRAY_BIT((INT_V), __k__);                          \
            }                                                                                \
        } while (0)


//! Translates a sparse-byte representation (1bit per byte) into a full-byte one (8bit per byte)
#define BIT_VECTOR_TO_BYTE_VECTOR(BIT_V, BYTE_V, BIT_V_size)                                 \
        do {                                                                                 \
            unsigned int __BIT_V_size__ = (BIT_V_size);                                      \
            memset((BYTE_V), 0, ((__BIT_V_size__+7)/8)*sizeof(byte));                        \
            for (unsigned int __k__=0 ; __k__<__BIT_V_size__ ; ++__k__) {                    \
                SET_CHARARRAY_BIT((BYTE_V), __k__, ((BIT_V)[__k__]));                        \
            }                                                                                \
        } while (0)


//! Translates a full-byte representation (8bit per byte) into a sparse-byte one (1bit per byte)
#define BYTE_VECTOR_TO_BIT_VECTOR(BYTE_V, BIT_V, BIT_V_size)                                 \
        do {                                                                                 \
            unsigned int __BIT_V_size__ = (BIT_V_size);                                      \
            memset((BIT_V), 0, __BIT_V_size__*sizeof(byte));                                 \
            for (unsigned int __k__=0 ; __k__<__BIT_V_size__ ; ++__k__) {                    \
                (BIT_V)[__k__] = GET_CHARARRAY_BIT((BYTE_V), __k__);                         \
            }                                                                                \
        } while (0)


//! Displays a console progress bar (single line, with ending LF)
#define PROGRESSBAR(percent)                                                                \
        do {                                                                                \
            printf("[");                                                                    \
            unsigned int __percent__ = (percent);                                           \
            unsigned int __steps__   = __percent__*20/100;                                  \
            for (unsigned int __i__=0 ; __i__<__steps__ ; ++__i__)                          \
                printf("=");                                                                \
            for (unsigned int __i__=0 ; __i__<20-__steps__ ; ++__i__)                       \
                printf("-");                                                                \
            printf("] %d%%\n", __percent__);                                                \
        } while (0)


//! Displays a console progress bar (single line, without ending LF)
#define PROGRESSBAR_NOLF(percent)                                                           \
        do {                                                                                \
            printf("[");                                                                    \
            unsigned int __percent__ = (percent);                                           \
            unsigned int __steps__   = __percent__*20/100;                                  \
            for (unsigned int __i__=0 ; __i__<__steps__ ; ++__i__)                          \
                printf("=");                                                                \
            for (unsigned int __i__=0 ; __i__<20-__steps__ ; ++__i__)                       \
                printf("-");                                                                \
            printf("] %d%%", __percent__);                                                  \
        } while (0)


//! Erases the last 80 characters on the current console line
#define CLEARLINE                                                                           \
        do {                                                                                \
            for (int __c__=0 ; __c__<80 ; ++__c__) printf("\b");                            \
        } while (0)




#endif

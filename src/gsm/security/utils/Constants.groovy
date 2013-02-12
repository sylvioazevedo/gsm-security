/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class Constants {
    
    // -- some constants
    
    // registers size
    static final int R1_BITS  = 19
    static final int R2_BITS  = 22
    static final int R3_BITS  = 23
    static final int R4_BITS  = 17
    static final int REG_BITS = R1_BITS + R2_BITS + R3_BITS + R4_BITS
    
    // clocking unit taps
    static final int R4_CLOCKTAB_R1 = 10
    static final int R4_CLOCKTAP_R2 =  3
    static final int R4_CLOCKTAP_R3 =  7
    
    // output taps
    static final int R1_OUTTAP_1 = 12
    static final int R1_OUTTAP_2 = 14 // Output Tap to be xored with 1
    static final int R1_OUTTAP_3 = 15
    static final int R2_OUTTAP_1 =  9
    static final int R2_OUTTAP_2 = 13
    static final int R2_OUTTAP_3 = 16 // Output Tap to be xored with 1
    static final int R3_OUTTAP_1 = 13 // Output Tap to be xored with 1
    static final int R3_OUTTAP_2 = 16
    static final int R3_OUTTAP_3 = 18
    
    // shift taps
    static final int R1_SHIFTTAP_1 = 13
    static final int R1_SHIFTTAP_2 = 16
    static final int R1_SHIFTTAP_3 = 17
    static final int R1_SHIFTTAP_4 = 18
    static final int R2_SHIFTTAP_1 = 20
    static final int R2_SHIFTTAP_2 = 21
    static final int R3_SHIFTTAP_1 =  7
    static final int R3_SHIFTTAP_2 = 20
    static final int R3_SHIFTTAP_3 = 21
    static final int R3_SHIFTTAP_4 = 22
    static final int R4_SHIFTTAP_1 = 11
    static final int R4_SHIFTTAP_2 = 16
    
    // position of one set bits after keysetup
    static final int R1_INITIAL_CONST_POS = 15
    static final int R2_INITIAL_CONST_POS = 16
    static final int R3_INITIAL_CONST_POS = 18
    static final int R4_INITIAL_CONST_POS = 10
    
    // secret key lenght
    static final int SECRETKEY_BITS = 64
    
    // gsm frame id length
    static final int FRAMEID_BITS = 22
    
    /**
     * Simple Variables (exactly one location of each register is set to
     * constant "1" at the beginning, thus reducing the number of variables to 
     * R{1,2,3}_SIMPLE_VARS = (R{1,2,3}_BITS-1))
     */
    static final int R1_SIMPLE_VARS = (R1_BITS - 1)
    static final int R2_SIMPLE_VARS = (R2_BITS - 1)
    static final int R3_SIMPLE_VARS = (R3_BITS - 1)
    static final int REGS_SIMPLE_VARS = (R1_SIMPLE_VARS + R2_SIMPLE_VARS + R3_SIMPLE_VARS)
    
    /**
     * Product Variables (products may be done at the register level when
     * outputing keystream).
     */
    static final int R1_PROD_VARS = ((R1_SIMPLE_VARS * (R1_SIMPLE_VARS-1)) / 2)
    static final int R2_PROD_VARS = ((R2_SIMPLE_VARS * (R2_SIMPLE_VARS-1)) / 2)
    static final int R3_PROD_VARS = ((R3_SIMPLE_VARS * (R3_SIMPLE_VARS-1)) / 2)
    static final int REGS_PROD_VARS = (R1_PROD_VARS + R2_PROD_VARS + R3_PROD_VARS)

    // Total Amount of Variables (including constant "1")
    static final int REGS_TOTAL_VARS = (REGS_SIMPLE_VARS + REGS_PROD_VARS + 1) // +1 for constant "1"
    
    // Number of possible values of the fourth LFSR (R4)
    static final int TOTAL_MATRICES = (1<<(R4_BITS-1))
    
    /**
     * Number of threads used for both matrices generation and attack (must be
     * a power of 2).
     */
    static final int PROCESSING_THREADS = 2 //(1<<5)
    
    // Amount of data that will be processed by each thread
    static final int THREAD_CHUNKSIZE = (TOTAL_MATRICES/PROCESSING_THREADS)
    
    /**
     * A matrix of integers will be used for faster decryption. The last integer
     * of the line will be half-loaded with two bytes.
     */
    static final int RESOLUTION_MATRIX_INT_WIDTH = ((REGS_TOTAL_VARS+31)/32)
    
    
    /** 
     * Data length relative to full code, input size
     */

    /**
     * Length of a source word (in bits)
     */
    static final int SOURCEWORD_LENGTH = 184




    /**
     * Constants relative to specific encoders/decoders
     */

    /**
     * Fire Encoding 
     *
     * We consider here the following polynomial for GSM Fire code (SACCH):
     *                (D^23 + 1)*(D^17 + D^3 + 1)
     * which can be developped into:
     *             D^40 + D^26 + D^23 + D^17 + D^3 + D^0
     * The polynomial is divided into two parts which can be recombined using the following formula:
     * (UPPER_POLYNOMIAL << 9) | (LOWER_POLYNOMIAL >> 23)
     * Note that the polynomial representation is mirrored compared to 0x80024100 || 0x04800000 !
     */
    static final int UPPER_POLYNOMIAL = ((int)0x90004120)                      // Upper polynomial representation
    static final int LOWER_POLYNOMIAL = ((int)0x800000)                        // Lower polynomial representation

    static final int FIRE_POLYNOMIAL_LENGTH = 41                                // Meaningful length (in bits)
    static final int LOWER_PADDINGBITS = (32*2 - FIRE_POLYNOMIAL_LENGTH)        // Number of padding bits

    /**
     * Fire code input length (in bits)
     */
    static final int FIRE_SOURCE_LENGTH = SOURCEWORD_LENGTH                                 // 184
    
    /** 
     * Fire code output length (in bits)
     */
    static final int FIRE_CODE_LENGTH = (FIRE_SOURCE_LENGTH + FIRE_POLYNOMIAL_LENGTH - 1)   // 224
    
    /**
     * Fire code CRC length (in bits)
     */
    static final int FIRE_CRC_LENGTH = (FIRE_CODE_LENGTH - FIRE_SOURCE_LENGTH)              // 40


    /**
     * Convolution
     * Before convolution is applied to a fired message, 4bits are appended to the input (all 0s)
     */

    /**
     * Convolution input length (in bits)
     */
    static final int CONVOLUTION_SOURCE_LENGTH = (FIRE_CODE_LENGTH + 4)                     // 228
    
    /**
     * Convolution output length (in bits)
     */
    static final int CONVOLUTION_CODE_LENGTH = (CONVOLUTION_SOURCE_LENGTH * 2)              // 456


    /** 
     * Interleaving
     */

    /**
     * Convolution input length (in bits)
     */
    static final int INTERLEAVING_LENGTH = CONVOLUTION_CODE_LENGTH                          // 456

    /**
     * Data length relative to full code, output size
     */

    /**
     * Length of a code word (in bits)
     */
    static final int CODEWORD_LENGTH = INTERLEAVING_LENGTH                                   // 456

    /**
     * Syndrome length (in bits), defined by the code parity-check matrix
     */
    static final int SYNDROME_LENGTH = (CODEWORD_LENGTH - SOURCEWORD_LENGTH)                 // 272
}


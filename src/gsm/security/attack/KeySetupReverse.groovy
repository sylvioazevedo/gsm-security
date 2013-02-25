package gsm.security.attack

import gsm.security.utils.ArrayUtil
import gsm.security.utils.Constants

/**
 *
 * @author sazevedo
 */
class KeySetupReverse {

    class ReverseLFSRSet {
        def r1 = new byte[Constants.R1_BITS][Constants.SECRETKEY_BITS+1]         // R1 expressed from keystream unknowns
        def r2 = new byte[Constants.R2_BITS][Constants.SECRETKEY_BITS+1]         // R2 expressed from keystream unknowns
        def r3 = new byte[Constants.R3_BITS][Constants.SECRETKEY_BITS+1]         // R3 expressed from keystream unknowns
        def r4 = new byte[Constants.R4_BITS][Constants.SECRETKEY_BITS+1]         // R4 expressed from keystream unknowns
    } 
    
    def keysetupReverseClockRegs(ReverseLFSRSet LFSRs) {

        byte carry[SECRETKEY_BITS+1];
        
        def carry

        // R1
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r1[13])
        ArrayUtil.xor(carry, LFSRs.r1[16])
        ArrayUtil.xor(carry, LFSRs.r1[17])
        ArrayUtil.xor(carry, LFSRs.r1[18])
        
//        ArrayUtil.xor(LFSRs.r1[1], LFSRs.r1[0])
//        memmove(LFSRs->R1[1], LFSRs->R1[0], (R1_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
//        memcpy(LFSRs->R1[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

        // R2
        carry = new byte[SECRETKEY_BITS+1]
        ArrayUtil.xor(carry, LFSRs.r2[20])
        ArrayUtil.xor(carry, LFSRs.r2[21])
        
        memmove(LFSRs->R2[1], LFSRs->R2[0], (R2_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
        memcpy(LFSRs->R2[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

        // R3
        carry = new byte[SECRETKEY_BITS+1]
        XOR_CHARARRAYS(carry, LFSRs->R3[ 7], SECRETKEY_BITS+1);
        XOR_CHARARRAYS(carry, LFSRs->R3[20], SECRETKEY_BITS+1);
        XOR_CHARARRAYS(carry, LFSRs->R3[21], SECRETKEY_BITS+1);
        XOR_CHARARRAYS(carry, LFSRs->R3[22], SECRETKEY_BITS+1);
        memmove(LFSRs->R3[1], LFSRs->R3[0], (R3_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
        memcpy(LFSRs->R3[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));

        // R4
        carry = new byte[SECRETKEY_BITS+1]
        XOR_CHARARRAYS(carry, LFSRs->R4[11], SECRETKEY_BITS+1);
        XOR_CHARARRAYS(carry, LFSRs->R4[16], SECRETKEY_BITS+1);
        memmove(LFSRs->R4[1], LFSRs->R4[0], (R4_BITS-1)*(SECRETKEY_BITS+1)*sizeof(byte));
        memcpy(LFSRs->R4[0], carry, (SECRETKEY_BITS+1)*sizeof(byte));
    }
}

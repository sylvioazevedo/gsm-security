package gsm.security.attack

import gsm.security.utils.ArrayUtil
import gsm.security.utils.Constants

/**
 *
 * @author sazevedo
 */
class KeySetupReverse {

    class ReverseLFSRSet {
        def r1 = new byte[Constants.R1_BITS][Constants.SECRETKEY_BITS+1]        // R1 expressed from keystream unknowns
        def r2 = new byte[Constants.R2_BITS][Constants.SECRETKEY_BITS+1]        // R2 expressed from keystream unknowns
        def r3 = new byte[Constants.R3_BITS][Constants.SECRETKEY_BITS+1]        // R3 expressed from keystream unknowns
        def r4 = new byte[Constants.R4_BITS][Constants.SECRETKEY_BITS+1]        // R4 expressed from keystream unknowns
    } 
    
    def reverseClockRegs(ReverseLFSRSet LFSRs) {
        
        def carry

        // R1
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r1[13])
        ArrayUtil.xor(carry, LFSRs.r1[16])
        ArrayUtil.xor(carry, LFSRs.r1[17])
        ArrayUtil.xor(carry, LFSRs.r1[18])
        
        ArrayUtil.shiftLeft(LFSRs.r1, carry)

        // R2
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r2[20])
        ArrayUtil.xor(carry, LFSRs.r2[21])
        
        ArrayUtil.shiftLeft(LFSRs.r2, carry)
        
        // R3
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r3[7])
        ArrayUtil.xor(carry, LFSRs.r3[20])
        ArrayUtil.xor(carry, LFSRs.r3[21])
        ArrayUtil.xor(carry, LFSRs.r3[22])
        
        ArrayUtil.shiftLeft(LFSRs.r3, carry)
        
        // R4
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r4[11])
        ArrayUtil.xor(carry, LFSRs.r4[16])
        
        ArrayUtil.shiftLeft(LFSRs.r4, carry)
    }
    
    def reverseKeysetup(def r1, def r2, def r3, def r4, def frameId, def key) {
        
    }
}

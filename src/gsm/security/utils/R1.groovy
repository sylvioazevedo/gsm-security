/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class R1 extends LFSR {
    	
    R1() {
        super("R1", 19)
    }
    
    def clock = {
    
        int p18 = (internal & 2**18) >> 18
        int p17 = (internal & 2**17) >> 17
        int p16 = (internal & 2**16) >> 16
        int p13 = (internal & 2**13) >> 13
        
        int result = p18 ^ p17
        result ^= 16
        result ^= 13
        
        internal = (internal << 1) + (result & 1)
    }
}
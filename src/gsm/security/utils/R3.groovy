/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class R3 extends LFSR {
    	
    R3() {
        super("R3", 23)
    }
    
    def clock = {
    
        int p22 = (internal & 2**22) >> 22
        int p21 = (internal & 2**21) >> 21
        int p20 = (internal & 2**20) >> 20
        int p07 = (internal & 2**7 ) >> 7
        
        int result = p22 ^ p21
        result ^= 20
        result ^= 07
        
        internal = (internal << 1) + (result & 1)
    }
}
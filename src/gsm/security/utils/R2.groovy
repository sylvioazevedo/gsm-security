/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class R2 extends LFSR {
    	
    R2() {
        super("R2", 22)
    }
    
    def clock = {
    
        int p21 = (internal & 2**21) >> 21
        int p20 = (internal & 2**20) >> 20
        
        int result = p21 ^ p20
        
        internal = (internal << 1) + (result & 1)
    }
}


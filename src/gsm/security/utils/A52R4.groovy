/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class A52R4 extends LFSR {
    	
    A52R4() {
        super("R4", 21)
    }
    
    def clock = {
    
        int p16 = (internal & 2**16) >> 16
        int p11 = (internal & 2**11) >> 11
        
        int result = p16 ^ p11
        
        internal = (internal << 1) + (result & 1)
    }
}
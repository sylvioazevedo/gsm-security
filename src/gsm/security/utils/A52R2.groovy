/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class A52R2 extends LFSR {
    
    // define mask and tapped bits.
    static final long mask = 0x3FFFFF
    static final long tap = 0x300000
    	
    A52R2() {
        super("R2", 22, mask, tap)
    }
}


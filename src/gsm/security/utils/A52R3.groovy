/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class A52R3 extends LFSR {
    
    // define mask and tapped bits.
    static final long mask = 0x7FFFFF
    static final long tap = 0x700080
    	
    A52R3() {
        super("R3", 23, mask, tap)
    }
}
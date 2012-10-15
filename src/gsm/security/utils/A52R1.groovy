/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class A52R1 extends LFSR {
    
    // define mask and tapped bits.
    static final long mask = 0x0007FFFF
    static final long tap = 0x00072000
    	
    A52R1() {
        super("R1", 19, mask, tap)
    }
}
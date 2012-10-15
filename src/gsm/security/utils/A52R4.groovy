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
    
    // define mask and tapped bits.
    static final long mask = 0x01FFFF
    static final long tap = 0x010800
    
    // bits tapped on R4 to control the other register clocking.
    static final long tapR1 = 0x000400 
    static final long tapR2 = 0x000008 
    static final long tapR3 = 0x000080
    	
    A52R4() {
        super("R4", 17, mask, tap)
    }
}
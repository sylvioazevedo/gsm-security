/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.cipher

import gsm.security.utils.A52R1
import gsm.security.utils.A52R2
import gsm.security.utils.A52R3
import gsm.security.utils.A52R4
/**
 *
 * @author sazevedo
 */
class A52 {
    
    // internal linear-feedback shift registers (LFSRR).
    def r1 = new A52R1()
    def r2 = new A52R2()
    def r3 = new A52R3()
    def r4 = new A52R4()
    
    def keySetup(int key, int iv) {
        
        // initialize with the key
        println "Setting the key"
        (0..63).each {
            
            clock(true, true, true)
            
            r1.bit = key >> it
            r2.bit = r2.bit ^ 0x01 & (key >> it)
            r3.bit = r3.bit ^ 0x01 & (key >> it)
            r4.bit = r4.bit ^ 0x01 & (key >> it)
            
            println "${r1.showInternal()}"
        }
        
        
        // initialize with the initial vector
        println "\nSetting the initial vector"
        (0..21).each {
            r1.bit = r1.bit ^ 0x01 & (iv >> it)
            r2.bit = r2.bit ^ 0x01 & (iv >> it)
            r3.bit = r3.bit ^ 0x01 & (iv >> it)
            r4.bit = r4.bit ^ 0x01 & (iv >> it)
        }
    }
    
    def clock(boolean c1, boolean c2, boolean c3) {
        
        r1.clock()
        r2.clock()
        r3.clock()
        r4.clock()
    }
    
    def getBit = {
        return 0
    }
    
    def showInternals = {
        
    }
}


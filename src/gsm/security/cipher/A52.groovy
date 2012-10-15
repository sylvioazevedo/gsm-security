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
    
    // internal linear-feedback shift registers (LFSR).
    def r1 = new A52R1()
    def r2 = new A52R2()
    def r3 = new A52R3()
    def r4 = new A52R4()
    
    def keySetup(long key, int iv) {
        
        // initialize with the key
        println "Setting the key"
        
        (0..63).each {
            
            // clock all registers once
            clockAll();
            
            def keyBit = 0x01 & (key >> it)
            
            r1.bit = keyBit
            r2.bit = keyBit
            r3.bit = keyBit
            r4.bit = keyBit
            
            //println "${r4.toString()}"
        }
        
        
        // initialize with the initial vector
        println "\nSetting the initial vector"
        (0..21).each {
            
            // clock all registers once
            clockAll();
            
            def ivBit = 0x01 & (iv >> it)
            
            r1.bit = ivBit
            r2.bit = ivBit
            r3.bit = ivBit
            r4.bit = ivBit
            
            println "${it}:${r4.toString()}"
        }
        
        // force some bits to one.
        r1.internal |= (1 << 15)
        r2.internal |= (1 << 16)
        r3.internal |= (1 << 18)
        
        // set r4, main weak in this algorithm
        r4.internal |= (1 << 10)
        
        println "${r4.toString()}"
    }
    
    def clock() {
        
        def majority = Majority.calc(r4 & A52R4.tapR1, r4 & A52R4.tapR2, r4 & A52R4.tapR3)
        
        if (((r4 & A52R4.tapR1) != 0) == majority) {
            r1.clock();
        }

        if (((r4 & A52R4.tapR2) != 0) == majority) {
            r2.clock();
        }

        if ((((R4 & A52R4.tapR3) != 0) == majority)) {
            r3.clock();
        }

        r4.clock();
    }
    
    /**
     * Clock all registers, it's used only in the key setup.
     */
    def clockAll() {
        
        // clock all registers.
        r1.clock()
        r2.clock()
        r3.clock()
        r4.clock()
    }
    
    def getBit = {
        
    }
    
    def showInternals = {
        
    }
}
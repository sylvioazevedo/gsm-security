/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.cipher

import gsm.security.utils.A52R1
import gsm.security.utils.A52R2
import gsm.security.utils.A52R3
import gsm.security.utils.A52R4
import gsm.security.utils.Majority


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
            
            //println "${it}:${r3.toString()}"
        }
        
        // force some bits to one.
        r1.internal |= (1 << 15)
        r2.internal |= (1 << 16)
        r3.internal |= (1 << 18)
        
        // set r4, main weak in this algorithm
        r4.internal |= (1 << 10)
        
        // show R1, R2, R3 and R4
//        println "R1: ${r1.toString()}"
//        println "R2: ${r2.toString()}"
//        println "R3: ${r3.toString()}"
//        println "R4: ${r4.toString()}"
        
        clock();
        
        println "R1: ${r1.toString()}"
        println "R2: ${r2.toString()}"
        println "R3: ${r3.toString()}"
        println "R4: ${r4.toString()}"
        
        (0..100).each { 
            clock()
//            println "R1: ${r1.toString()}"
//            println "R2: ${r1.toString()}"
//            println "R3: ${r1.toString()}"
//            
//            println "${it}: ${getBit()}"
        }
    }
    
    def clock() {
        
        def majority = Majority.calc(r4.internal & A52R4.tapR1, r4.internal & A52R4.tapR2, r4.internal & A52R4.tapR3)
        
        if (((r4.internal & A52R4.tapR1) != 0) == majority) {
            r1.clock();
        }

        if (((r4.internal & A52R4.tapR2) != 0) == majority) {
            r2.clock();
        }

        if ((((r4.internal & A52R4.tapR3) != 0) == majority)) {
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
        
        int topBits = r1.bit ^ r2.bit ^r3.bit
        
        return topBits
    }
    
    def showInternals = {
        
    }
}

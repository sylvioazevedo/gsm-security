/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.attack

import MatlabUtil.MatlabUtil
import gsm.security.code.Keygen
import gsm.security.utils.Constants

/**
 *
 * @author sazevedo
 */
class A52GWG {
    
    static def r4InvClock = [ 
        [ 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
        [ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0],
    ]
    
    static def reverseR4(def r4) {
        
        //MatlabUtil mu = new MatlabUtil()
        //println mu.multGF2(1, [r4, r4InvClock])                
        
        def result = new int[r4.size()]
        
        (0..r4.size()-1).each { row ->            
            (0..r4.size()-1).each { col ->            
                result[row] = (result[row] + (r4[col] * r4InvClock[row][col])) % 2                
            }        
        }
        
        return result
    }
    
    static def calcDelta(def r4, def f) {
        
        def regs = new int[3];        
        def a52 = new Keygen()        
                 
        a52.r1 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]                 
        a52.r2 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]                 
        a52.r3 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        a52.r4 = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ]                 
        
        (0..Constants.FRAMEID_BITS-1).each { i->            
            a52.clockingUnit(1);
            a52.r1[0] ^= f[i];
            a52.r2[0] ^= f[i];
            a52.r3[0] ^= f[i];
            a52.r4[0] ^= f[i];
        }        
        
        println "- frame inserted"
        println a52.r1
        println a52.r2
        println a52.r3
        println a52.r4
        
        a52.r4 = r4        
        
        a52.r1[Constants.R1_INITIAL_CONST_POS] = 1;
        a52.r2[Constants.R2_INITIAL_CONST_POS] = 1;
        a52.r3[Constants.R3_INITIAL_CONST_POS] = 1;
        a52.r4[Constants.R4_INITIAL_CONST_POS] = 1;
        
//        // -- init
//        println a52.r1
//        println a52.r2
//        println a52.r3
//        println a52.r4
        
        // First 99 cycles of pre-processing (output discarded)
        for(int i=0 ; i<100 ; ++i) {
//            a52.clockingUnit(0);
//            println "-- [${i}]"
//            println a52.r1
//            println a52.r2
//            println a52.r3
//            println a52.r4
        }
        
        //def keystream1 = a52.getKeystreamDump(114)
    }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.code

import gsm.security.utils.Constants
import gsm.security.utils.Majority

/**
 *
 * @author sazevedo
 */
class Keygen {

    // LFSR
    def r1, r2, r3, r4
    
    
    def clockReg(int index) {
        
        byte carry = 0;
        
        switch(index) {
        
            case 1:
                carry ^= r1[Constants.R1_SHIFTTAP_1] ^ r1[Constants.R1_SHIFTTAP_2] ^ r1[Constants.R1_SHIFTTAP_3] ^ r1[Constants.R1_SHIFTTAP_4]
                r1.remove(r1.size()-1)
                r1.add(0, carry)
                break
                
            case 2:
                carry ^= r2[Constants.R2_SHIFTTAP_1] ^ r2[Constants.R2_SHIFTTAP_2]
                r2.remove(r2.size()-1)
                r2.add(0, carry)
                break;
                
            case 3:
                carry ^= r3[Constants.R3_SHIFTTAP_1] ^ r3[Constants.R3_SHIFTTAP_2] ^ r3[Constants.R3_SHIFTTAP_3] ^ r3[Constants.R3_SHIFTTAP_4]
                r3.remove(r3.size()-1)
                r3.add(0, carry)
                break;
            case 4:
                carry ^= r4[Constants.R4_SHIFTTAP_1] ^ r4[Constants.R4_SHIFTTAP_2];
                r4.remove(r4.size()-1)
                r4.add(0, carry)
                break;
                
            default:
                println "Erro! LFRS not found."
                break;
        }
    }

    
    def clockingUnit(int clockAll) {
        
        byte maj = Majority.calc(r4[Constants.R4_CLOCKTAP_R1], r4[Constants.R4_CLOCKTAP_R2], r4[Constants.R4_CLOCKTAP_R3]);
        
        if (clockAll || maj==r4[Constants.R4_CLOCKTAP_R1]) {
            clockReg(1)
        }
        
        if (clockAll || maj==r4[Constants.R4_CLOCKTAP_R2]) {
            clockReg(2)
        }
        
        if (clockAll || maj==r4[Constants.R4_CLOCKTAP_R3]) {
            clockReg(3);
        }
        
        clockReg(4);
    }


    def getOutBit() {
        
        return r1[Constants.R1_BITS-1] ^ r2[Constants.R2_BITS-1] ^ r3[Constants.R3_BITS-1] \
             ^ Majority.calc(r1[Constants.R1_OUTTAP_1], 1 ^ r1[Constants.R1_OUTTAP_2], r1[Constants.R1_OUTTAP_3]) \
             ^ Majority.calc(r2[Constants.R2_OUTTAP_1], r2[Constants.R2_OUTTAP_2], 1 ^ r2[Constants.R2_OUTTAP_3]) \
             ^ Majority.calc(1 ^ r3[Constants.R3_OUTTAP_1], r3[Constants.R3_OUTTAP_2], r3[Constants.R3_OUTTAP_3])
    }
    
    def keysetup(def Kc, def frameId) {

        r1 = []
        r2 = []
        r3 = []
        r4 = []
        
        (1..Constants.R1_BITS).each {r1.add(0)}
        (1..Constants.R2_BITS).each {r2.add(0)}
        (1..Constants.R3_BITS).each {r3.add(0)}
        (1..Constants.R4_BITS).each {r4.add(0)}

        (0..Constants.SECRETKEY_BITS-1).each  { i->
            clockingUnit(1);
            r1[0] ^= Kc[i];
            r2[0] ^= Kc[i];
            r3[0] ^= Kc[i];
            r4[0] ^= Kc[i];
        }
        
        println "- key inserted"
        println r1
        println r2
        println r3
        println r4
        
        (0..Constants.FRAMEID_BITS-1).each { i->            
            clockingUnit(1);
            r1[0] ^= frameId[i];
            r2[0] ^= frameId[i];
            r3[0] ^= frameId[i];
            r4[0] ^= frameId[i];
        }        
        
        println "- frame inserted"
        println r1
        println r2
        println r3
        println r4

        r1[Constants.R1_INITIAL_CONST_POS] = 1;
        r2[Constants.R2_INITIAL_CONST_POS] = 1;
        r3[Constants.R3_INITIAL_CONST_POS] = 1;
        r4[Constants.R4_INITIAL_CONST_POS] = 1;
        
        // -- init
//        println "-- init"
//        println r1
//        println r2
//        println r3
//        println r4
        
        // First 99 cycles of pre-processing (output discarded)
        for(int i=0 ; i<100 ; ++i) {
            clockingUnit(0);   
//            println "-- [${i}]"
//            println r1
//            println r2
//            println r3
//            println r4
        }
    }

    def getKeystream(int len) {

        def keystream = new byte[len]

        (0..len-1).each { i->
            keystream[i] = getOutBit();
            clockingUnit(0);
        }
        
        return keystream
    }
    
    def getKeystreamDump(int len) {

        def keystream = new byte[len]

        (0..len-1).each { i->
            keystream[i] = getOutBit();
            clockingUnit(0);
            
            println "${i}-> R1: ${r1}"
            println "${i}-> R2: ${r2}"
            println "${i}-> R3: ${r3}"
            println "${i}-> R4: ${r4}"
            println "--"
        }
        
        return keystream
    }

    // Documentation in header file
    def keygen_test() {

        def Kc = [0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
        def frameId = [1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

        // This keystream is the standard keystream that should be obtained considering the values above
        def verifiedKeystream = [1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,1,0,0,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,1,0,1,1,0,0,1,0,0,1,1,0,1,1,1,0,1,1,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,1,1,0,0,1,0,0,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,0,1,0,1,0,0,1,1,0,1,1,1,0,0,1,1,0,1,0,1,1,1,1,0,1,1,1,0,0,1,0,1,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0]

        def keystream = new byte[228]

        keysetup(Kc, frameId);
        keystream = getKeystream(228);

        (0..227).each { i-> 

            if (keystream[i] != verifiedKeystream[i]) {
                println "Self-check aborted: a discrepancy was found comparing the produced keystream and the verified one"
                return 1;
            }
        }

        println "Self-check succeeded: the produced keystream matches the verified one"
        return 0;
    }
}
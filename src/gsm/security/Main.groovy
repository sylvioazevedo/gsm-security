/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security

import gsm.security.utils.Constants
import gsm.security.code.Convolution
import gsm.security.code.Keygen

import gsm.security.utils.ArrayUtil
import gsm.security.attack.A52GWG


//def start = Calendar.instance
//
//def covFile = new File("convolution-matrix.txt")
//
//if(covFile.exists()) {
//    covFile.delete()
//}
//
//// process convolution matrix.
//def g = Convolution.processMatrix()
//
//(0..Constants.CONVOLUTION_SOURCE_LENGTH-1).each { r-> 
//    (0..Constants.CONVOLUTION_CODE_LENGTH-1).each { c-> 
//        covFile << g[r][c]
//    }
//    
//    covFile << "\r\n"
//}
//
//
//def end = Calendar.instance
//println "Processing time: ${end.timeInMillis - start.timeInMillis}"

//new Keygen().keygen_test()

def Kc = [0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]

def Kc1 = [0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,1]

def f1 = [1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
def f2 = [1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0]

def a521 = new Keygen()

//a521.keysetup(Kc, f1)

//def keystream1 = a521.getKeystreamDump(114)

def a522 = new Keygen()

//a522.keysetup(Kc, f2)

//def keystream2 = a522.getKeystreamDump(114)

//(0..113).each { i->
//    
//    a521.clockingUnit(0);
//    a522.clockingUnit(0);
//
//    println "${i}-> R11: ${a521.r1}"
//    println "${i}-> R12: ${a522.r1}"
//    println "-"
//    println ArrayUtil.xorNoAcum(a521.r1, a522.r1)
//    println "--"
//    
//    println "${i}-> R21: ${a521.r2}"
//    println "${i}-> R22: ${a522.r2}"
//    println "-"
//    println ArrayUtil.xorNoAcum(a521.r2, a522.r2)
//    println "--"
//    
//    println "${i}-> R31: ${a521.r3}"
//    println "${i}-> R32: ${a522.r3}"
//    println "-"
//    println ArrayUtil.xorNoAcum(a521.r3, a522.r3)
//    println "--"
//    
//    println "${i}-> R41: ${a521.r4}"
//    println "${i}-> R42: ${a522.r4}"
//    println "-"
//    println ArrayUtil.xorNoAcum(a521.r4, a522.r4)
//    println "--"
//    println "--------------"
//}

//println "---//---"
//def result = a521.r4
//
//def result = [1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1]
//
//(0..100).each {
//    result = A52GWG.reverseR4( result )
//    println result
//}



//println keystream1
//
//println keystream2
//
//def keystreamXOR = ArrayUtil.xor(keystream1, keystream2)
//println keystreamXOR
                 
//A52GWG.calcDelta([0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0])

A52GWG.calcDelta([1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0], f1)
A52GWG.calcDelta([1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0], f2)


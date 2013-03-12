/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security

import gsm.security.utils.Constants
import gsm.security.code.Convolution
import gsm.security.code.Keygen


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

def f1= [1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
def f2 = [1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0]



def a52 = new Keygen()

a52.keysetup(Kc, f1)

def keystream1 = a52.getKeystream(114)

a52 = new Keygen()

a52.keysetup(Kc, f2)

def keystream2 = a52.getKeystream(114)

println keystream1
println keystream2



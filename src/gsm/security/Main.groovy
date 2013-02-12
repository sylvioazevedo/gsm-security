/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security

import gsm.security.utils.Constants
import gsm.security.code.Convolution


def start = Calendar.instance

def covFile = new File("convolution-matrix.txt")

if(covFile.exists()) {
    covFile.delete()
}

// process convolution matrix.
def g = Convolution.encode()

(0..Constants.CONVOLUTION_SOURCE_LENGTH-1).each { r-> 
    (0..Constants.CONVOLUTION_CODE_LENGTH-1).each { c-> 
        covFile << g[r][c]
    }
    
    covFile << "\r\n"
}


def end = Calendar.instance

println "Processing time: ${end.timeInMillis - start.timeInMillis}"
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.code

import gsm.security.utils.Constants

/**
 *
 * @author sazevedo
 */
class Convolution {
    
    /**
     * Convolution encode.
     */
    static def processMatrix() {
        
        // allocate convultion matrix memory space.
        def g = new byte[Constants.CONVOLUTION_SOURCE_LENGTH][Constants.CONVOLUTION_CODE_LENGTH]
        
        // convultion pattern
        byte[] pattern = [ 1, 1, 0, 1, 0, 0, 1, 1, 1, 1 ]
        
        //   +-------- polynomial for even bits
	//   |+-------- polynomial for odd bits
	//   ||
	//   vv
	//
	//   11
	//   1111
	//   001111
	//  _01001111________
	//=>|1101001111.. <= pattern to propagate through G. Everything else is 0 (..)
	//  |..1101001111
	//  |....11010011
	//  |......110100
	//  |........1101
	//  |..........11
	//  |

        // write convolution matrix
        (0..Constants.CONVOLUTION_SOURCE_LENGTH-1).each { i ->
            System.arraycopy(pattern, 0, g[i], 2*i, ((i<Constants.CONVOLUTION_SOURCE_LENGTH-4)? 10 : Constants.CONVOLUTION_CODE_LENGTH - 2*i))
        }
        
        return g
    }
    
    /**
     * Reverse convolution process.
     */
    static def reverse(def input) {

        def output = new byte[Constants.CONVOLUTION_SOURCE_LENGTH]
        
        byte bit0, bit1;
        
        
        (0..(Constants.CODEWORD_LENGTH/32)+1).each { i->
            
            for(int b=2; b<32;) {
                
                bit_0 = input[32*i + (b++)];
                bit_1 = input[32*i + (b++)];
                // Since c(2*k) = u(k)^u(k-3)^u(k-4) and c(2*k + 1) = u(k)^u(k-1)^u(k-3)^u(k-4)
                // u((k-1)/2) = c(k)^c(k+1) for all even values of k
                output[16*i + (b-2)/2 - 1] = bit_0 ^ bit_1;
            }
            
            //We must handle the last bit of each half of the new int
            output[16*i + 15] = bit_0 ^ output[16*i + 15 - 3] ^ output[16*i + 15 - 4];
        }
    }
}
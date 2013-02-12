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
class FireCode {
    
    /**
     * Bit representation of the Fire polynomial
     */
    def polynomial = [1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1]
    
    def polynomialMirrored = [1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1]
    
    static def processGMatrix() {
        
        def g = new byte[Constants.FIRE_SOURCE_LENGTH][Constants.FIRE_CODE_LENGTH]

        (0..Constants.FIRE_CODE_LENGTH -1).each { i->
            
            System.arraycopy(polynomialMirrored, 0, g[i], i, 41);
        }

	// We assume the standard form of the matrix (1 diag first):
	// 1xxxxxxx...
	// 01xxxxxx...
	// 001xxxxx...
	// 0001xxxx...
	// ...........
	// We now proceed a Gauss elimination
        (0..Constants.FIRE_CODE_LENGTH -1).each { c -> 
            
            for(int r = c-1; r>=0; r--) {
                
                if(g[r][c] == 1) {
                    
                    for (int cod=c ; cod < Constants.FIRE_CODE_LENGTH; ++cod) {
                        g[l][cod] ^= g[c][cod];
                    }
                }
            }
        }
        
        return g
    }
    
    static def processHMatrix(def g) {
        
        def h = new byte[Constants.FIRE_CRC_LENGTH][Constants.FIRE_CODE_LENGTH]
        
        (0..Constants.FIRE_CRC_LENGTH-1).each { r-> 
            
            (0..Constants.FIRE_SOURCE_LENGTH-1).each { c->
                
                H[r][c] = G[c][r+Constants.FIRE_SOURCE_LENGTH];
            }
            
            H[r][Constants.FIRE_SOURCE_LENGTH+r] = 1;
        }
        
        return h
    }
    
    static def reverse(def input) {
        
        def output = new byte[Constants.FIRE_SOURCE_LENGTH]
        def decoded = new byte[Constants.FIRE_SOURCE_LENGTH]
        
        /**
         * @todo Check why it isn't executed just once.
         */
        def g = processGMatrix()
        def h = processHMatrix(g)
        
        (0..Constants.FIRE_CRC_LENGTH-1).each { i ->
        
            (0..Constants.FIRE_CODE_LENGTH-1).each { j->
                
                decoded[i] ^= input[j] & h[i][j];
                
                // This prevented from having a non null syndrome, since non codewords make it return all 0s
		if (decoded[i] != 0) {
			println("Error checking CRC\n");
		}
        
            }
        }
	
	return output
    }
}


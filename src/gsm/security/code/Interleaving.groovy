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
class Interleaving {
    
    static def processMatrix() {
        
        def g = new byte[Constants.INTERLEAVING_LENGTH][Constants.INTERLEAVING_LENGTH]
        
        def b = new int[8][114]
        
        (0..Constants.INTERLEAVING_LENGTH-1).each { k->
            b[k%8][(2*((49*k)%57) + ((k%8)/4))] = k
        }
        
        int i=0
	for(int k=0; k<4; ++k){
            for(int j=0; j<57; ++j){
                g[b[k][2*j]][i] = 1;
                i++;
                g[b[k+4][2*j+1]][i] = 1;
                i++;
            }
	}
    }
    
    static def reverse(def input) {
        
        def output = new byte[Constants.INTERLEAVING_LENGTH]

	def b = new int[8][114]

	for (int k=0 ; k < Constants.INTERLEAVING_LENGTH ; ++k) {
            b[k%8][(2*((49*k)%57) + ((k%8)/4))] = k;
	}

	int i=0;
	for(int k=0; k<4; ++k){
            for(int j=0; j<57; ++j){
                output[b[k][2*j]] = input[i++];
                output[b[k+4][2*j+1]] = input[i++];
            }
	}
    }
}


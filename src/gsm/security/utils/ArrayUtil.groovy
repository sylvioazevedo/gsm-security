/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class ArrayUtil {
	
    static def xor(def v1, def v2) {
        
        (0..v1.size()-1).each { i->
            v1[i] ^= v2[i]
        }
        
        return v1
    }
    
    
    static def shiftLeft(def target, def carry) {
    
        target.remove(target.size()-1)
        target.add(0, carry)
        
        return target
    }
}
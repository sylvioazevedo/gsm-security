/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class Majority {
    
    static def calc(def a, def b, def c) {
        
        a = a!=0? 1 : 0
        b = b!=0? 1 : 0
        c = c!=0? 1 : 0
        
        return a+b+c >=2 ? 1 : 0
    }
}


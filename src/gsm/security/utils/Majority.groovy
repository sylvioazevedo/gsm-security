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
        
        return (a*b + b*c + a*c) > 0 ? 1 : 0
    }
}


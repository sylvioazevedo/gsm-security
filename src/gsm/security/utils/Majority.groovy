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
        
        int sum = (a != 0) + (b != 0) + (c != 0);
        
        if (sum >= 2)
            return 1;
        else
            return 0;
    }
}


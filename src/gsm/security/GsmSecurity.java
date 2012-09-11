/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package gsm.security;

import gsm.security.cipher.A52;

/**
 *
 * @author sazevedo
 */
public class GsmSecurity {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        A52 a52 = new A52();
        
        a52.keySetup(7, 0);
        
    }
}

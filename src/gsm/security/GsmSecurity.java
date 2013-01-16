/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package gsm.security;

import gsm.security.cipher.A52;
import gsm.security.cryptanalysis.A52HackTool;

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
        
        //long key = (long) 0x00fcffffffffffffL;
        
        long key = (long) 0xfffffffffffffc00L;
        
        a52.keySetup(key, 0x21);
        
        for(int i = 0; i < 114; i++) {
            a52.clock();
            System.out.print(a52.getBit());
        }
        System.out.println();
        
        new A52HackTool().run(args);
        
    }
}

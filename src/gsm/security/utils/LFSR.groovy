/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gsm.security.utils

/**
 *
 * @author sazevedo
 */
class LFSR {
    
    def name
    
    int size
    int count = 1
    int internal = 0
    
    LFSR(def name, int size) {
    
        this.name = name
        this.size = size
    }
    
    def setKey(def key) {
        
    }
    
    def show = {
    
        "${name}: Clk:${count++}: ${showInternal()}"
    }
    
    def showInternal = {
        
        def exit = ""
        
        (0..size-1).each { it
            exit = "${(internal & 2**(it)) >> it}${exit}"
        }
        
        return exit
    }
    
    def getBit() {
        (internal & 2**(size-1)) >> (size-1)
    }
    
    def setBit(int bit) {
        
        internal = internal ^ (0x01 & bit)
    }
    
    def getInt = {
        internal
    }	
}

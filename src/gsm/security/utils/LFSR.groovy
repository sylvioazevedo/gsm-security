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
    
    int  size
    int  count = 1
    long internal = 0
    
    long mask
    long tap
    
    LFSR(def name, int size, long mask, long tap) {
    
        this.name = name
        this.size = size
        this.mask = mask
        this.tap = tap
    }
    
    def setKey(def key) {
        
    }
    
    def show = {
    
        "${name}: Clk:${count++}: ${showInternal()}"
    }
    
    
    long parity(long x) {

       x ^= (x >> 16)
       x ^= (x >> 8)
       x ^= (x >> 4)
       x ^= (x >> 2)
       x ^= (x >> 1)

       return (x & 1)
    }
    
    def clock() {

        long t = internal & tap
        internal = (internal << 1) & mask
        internal |= parity(t)
    }
    
    String toString() {
        
        def exit = ""
        
        (0..size-1).each { d ->
            exit = "${(internal & 2**(d)) >> d}${exit}"
        }
        
        return exit
    }
    
    def getBit() {
        //(internal & 2**(size-1)) >> (size-1)
        
        (internal >> size - 1) & 0x01
    }
    
    def setBit(int bit) {
        
        internal ^= (0x01 & bit)
    }
    
    def getInt = {
        internal
    }	
}

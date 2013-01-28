/*
 * Reimplementation in groovy code of Nicolas Paglieri & Olivier Benjamin 
 * A52HackTool.
 */
package gsm.security.cryptanalysis

/**
 *
 * @author sazevedo
 */
class A52HackTool {
    
    // possible operations codes
    public static enum Operation {
        OP_NONE, 
        OP_ENCODE,
        OP_DECODE,
        OP_ENCRYPT,
        OP_ATTACK,
        OP_PRECOMPUTE,
        OP_AUTOTEST
    }
    
    // properties
    String sourceFile
    String destFile
    byte[] key
    byte[] frame
    
    /**
     * Show correct command syntax.
     */
    private void showSintax() {
        
        println("#===============================================================================");
	println("#                   _____ ___  _    _            _ _______          _          #");
	println("#             /\\   | ____|__ \\| |  | |          | |__   __|        | |         #");
	println("#            /  \\  | |__    ) | |__| | __ _  ___| | _| | ___   ___ | |         #");
	println("#           / /\\ \\ |___ \\  / /|  __  |/ _` |/ __| |/ / |/ _ \\ / _ \\| |         #");
	println("#          / ____ \\ ___) |/ /_| |  | | (_| | (__|   <| | (_) | (_) | |         #");
	println("#         /_/    \\_\\____/|____|_|  |_|\\__,_|\\___|_|\\_\\_|\\___/ \\___/|_|         #");
	println("#         Turnkey & easy to use tool  for cracking the GSM A5/2 cipher         #");
	println("#------------------------------------------------------------------------------#");
	println("#         Copyright © 2011   -   Nicolas Paglieri  &  Olivier Benjamin         #");
	println("#                                                                              #");
	println("#                                  Version 1.3.0                               #");
	println("#                    http://www.ni69.info/security-gsm-en.php                  #");
	println("#==============================================================================#");
	println("");
	println("Here are the available commands:");
	println(" - ask for help      :  --help | -h");
	println(" - encode  a message :  --ENCODE  -s [source] -d [destination]");
	println(" - encrypt a message :  --ENCRYPT -s [source] -d [destination] -k [secretkey] -f [frameId]");
	println(" - decrypt a message :  --DECRYPT -s [source] -d [destination] -k [secretkey] -f [frameId]");
	println(" - decode  a message :  --DECODE  -s [source] -d [destination]");
	println(" - precompute data   :  --PRECOMPUTE");
	println(" - perfom attack     :  --ATTACK  -s [source] -f [frameId]");
	println(" - launch autotest   :  --AUTOTEST");
	println("");
    }
    
    
    public void run(String[] args) {
        
        // check if args is empty.
        if(args.size() == 0) {
            
            println showSintax();
            
            return;
        }
        
        println "Arguments: ${args}"
        
        def operation = Operation.OP_NONE
        
        (0..args.size()-1).each { i ->
            
            switch(args[i]) {
                
                // encoding operation
                case "--ENCODE":
                    println "Encoding operation."
                    break;
                    
                // decoding operation
                case "--DECODE":
                    println "Decoding operation."
                    break;
                    
                // encrypting operation
                case "--ENCRYPT":
                    println "Encrypting operation."
                    break;
                    
                // attack operation
                case "--ATTACK":
                    println "Attacking operation."
                    break;
                    
                // precomputing operation
                case "--PRECOMPUTE":
                    println "Precomputing operation."
                    break;
                    
                // do autotest
                case "--AUTOTEST":
                    println "Autotest operation."
                    break;
                    
                // source file definition
                case "-s":
                    
                    if((i+1) >= args.size()) {
                        println "Invalid \'-s\' parameter."
                        return -1;
                    }
                    
                    sourceFile = args[i+1]
                    println "Source file accepted: ${sourceFile}"
                    break;
                    
                // target file definition
                case "-d":
                    if((i+1) >= args.size()) {
                        println "Invalid \'-s\' parameter."
                        return -1;
                    }
                    
                    destFile = args[i+1]
                    println "Target file accepted: ${destFile}"
                    break;
                    
                // A52 key definition
                case "-k":
                    if((i+1) >= args.size()) {
                        println "Invalid \'-s\' parameter."
                        return -1;
                    }
                    
                    key = args[i+1]
                    println "Key accepted: ${key}"
                    break;
                    
                // gsm packet frame number definition
                case "-f":
                    if((i+1) >= args.size()) {
                        println "Invalid \'-s\' parameter."
                        return -1;
                    }
                    
                    frame = args[i+1]
                    println "Frame accepted: ${frame}"
                    break;
                    
                // help
                case "-h":
                case "--help":
                    if((i+1) >= args.size()) {
                        println "Invalid \'-s\' parameter."
                        return -1;
                    }
                    
                    destFile = args[i+1]
                    println "Target file accepted: ${destFile}"
                    break;
            }
        }
    }
}


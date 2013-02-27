package gsm.security.attack

import gsm.security.utils.ArrayUtil
import gsm.security.utils.Constants

/**
 *
 * @author sazevedo
 */
class KeySetupReverse {

    class ReverseLFSRSet {
        def r1 = new byte[Constants.R1_BITS][Constants.SECRETKEY_BITS+1]        // R1 expressed from keystream unknowns
        def r2 = new byte[Constants.R2_BITS][Constants.SECRETKEY_BITS+1]        // R2 expressed from keystream unknowns
        def r3 = new byte[Constants.R3_BITS][Constants.SECRETKEY_BITS+1]        // R3 expressed from keystream unknowns
        def r4 = new byte[Constants.R4_BITS][Constants.SECRETKEY_BITS+1]        // R4 expressed from keystream unknowns
    } 
    
    def reverseClockRegs(ReverseLFSRSet LFSRs) {
        
        def carry

        // R1
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r1[13])
        ArrayUtil.xor(carry, LFSRs.r1[16])
        ArrayUtil.xor(carry, LFSRs.r1[17])
        ArrayUtil.xor(carry, LFSRs.r1[18])
        
        ArrayUtil.shiftLeft(LFSRs.r1, carry)

        // R2
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r2[20])
        ArrayUtil.xor(carry, LFSRs.r2[21])
        
        ArrayUtil.shiftLeft(LFSRs.r2, carry)
        
        // R3
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r3[7])
        ArrayUtil.xor(carry, LFSRs.r3[20])
        ArrayUtil.xor(carry, LFSRs.r3[21])
        ArrayUtil.xor(carry, LFSRs.r3[22])
        
        ArrayUtil.shiftLeft(LFSRs.r3, carry)
        
        // R4
        carry = new byte[SECRETKEY_BITS+1]
        
        ArrayUtil.xor(carry, LFSRs.r4[11])
        ArrayUtil.xor(carry, LFSRs.r4[16])
        
        ArrayUtil.shiftLeft(LFSRs.r4, carry)
    }
    
    def reverseKeysetup(def r1, def r2, def r3, def r4, def frameId) {
        
        def secretKey = new byte[SECRETKEY_BITS]
        
        def LFSRs = new ReverseLFSRSet()
        
        // -------------------------------------------------------
	// Keysetup steps mirroring using secret key variables...
	// -------------------------------------------------------        
        (0..SECRETKEY_BITS).each { i ->            
            reverseClockRegs(LFSRs)            
            def Kc = new byte[SECRETKEY_BITS+1]
            Kc[i] = 1
            ArrayUtil.xor(LFSRs.r1[0], Kc)
            ArrayUtil.xor(LFSRs.r2[0], Kc)
            ArrayUtil.xor(LFSRs.r3[0], Kc)
            ArrayUtil.xor(LFSRs.r4[0], Kc)            
        }
		
        (0..FRAMEID_BITS).each { i ->            
            reverseClockRegs(LFSRs)
            if(frameId[i]==1) {
                LFSRs.r1[0][SECRETKEY_BITS] ^= 1
                LFSRs.r2[0][SECRETKEY_BITS] ^= 1
                LFSRs.r3[0][SECRETKEY_BITS] ^= 1
                LFSRs.r4[0][SECRETKEY_BITS] ^= 1                
            }
        }

	
	memset(LFSRs->R1[15], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R1[15][SECRETKEY_BITS] = 1;
	memset(LFSRs->R2[16], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R2[16][SECRETKEY_BITS] = 1;
	memset(LFSRs->R3[18], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R3[18][SECRETKEY_BITS] = 1;
	memset(LFSRs->R4[10], 0, (SECRETKEY_BITS+1)*sizeof(char));  LFSRs->R4[10][SECRETKEY_BITS] = 1;


	// -------------------------------------------------------
	// Linear System Generation...
	// -------------------------------------------------------

	// Initial state of R1->R4 LFSRs is determined by the previous deciphering step (given as param)
	byte regStatus[REGS_BITS];
	memset(regStatus, 0, REGS_BITS*sizeof(byte));
	memcpy(regStatus,                         R1, R1_BITS*sizeof(byte));
	memcpy(regStatus+R1_BITS,                 R2, R2_BITS*sizeof(byte));
	memcpy(regStatus+R1_BITS+R2_BITS,         R3, R3_BITS*sizeof(byte));
	memcpy(regStatus+R1_BITS+R2_BITS+R3_BITS, R4, R4_BITS*sizeof(byte));

	// Concatenate in a line the 4 matrices of equations into a single linear system,
	// thus changing data representation [line/col]=>[col/line]
	byte linearSystem[REGS_BITS][SECRETKEY_BITS];
	memset(linearSystem, 0, (REGS_BITS)*SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R1_BITS ; ++i)
		memcpy(linearSystem[i], LFSRs->R1[i], SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R2_BITS ; ++i)
		memcpy(linearSystem[i+R1_BITS], LFSRs->R2[i], SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R3_BITS ; ++i)
		memcpy(linearSystem[i+R1_BITS+R2_BITS], LFSRs->R3[i], SECRETKEY_BITS*sizeof(char));
	for (int i=0 ; i<R4_BITS ; ++i)
		memcpy(linearSystem[i+R1_BITS+R2_BITS+R3_BITS], LFSRs->R4[i], SECRETKEY_BITS*sizeof(char));

	// -------------------------------------------------------
	// Construction step done. Now entering resolution...
	// -------------------------------------------------------

	// Detach last line (line representing ones), xor with provided result from previous decrypting
	// step, and simply solve the system of 64 unknown quantities and 19+22+23+17 = 81 equations
	// (4 of them are just 0=0, some others are dependant...)
	for (int i=0 ; i<R1_BITS ; ++i)
		regStatus[i] ^= LFSRs->R1[i][SECRETKEY_BITS];
	for (int i=0 ; i<R2_BITS ; ++i)
		regStatus[i+R1_BITS] ^= LFSRs->R2[i][SECRETKEY_BITS];
	for (int i=0 ; i<R3_BITS ; ++i)
		regStatus[i+R1_BITS+R2_BITS] ^= LFSRs->R3[i][SECRETKEY_BITS];
	for (int i=0 ; i<R4_BITS ; ++i)
		regStatus[i+R1_BITS+R2_BITS+R3_BITS] ^= LFSRs->R4[i][SECRETKEY_BITS];

	// Gauss Elimination
	byte headFactor = 0;
	for (int col=0 ; col<SECRETKEY_BITS ; ++col) {
		for (int line=col ; line<REGS_BITS ; ++line) {
			// Pivot finding
			headFactor = linearSystem[line][col];
			if (headFactor) {
				// Line Swap if necessary
				if (line!=col) {
					byte temp;
					for (int c=0 ; c<SECRETKEY_BITS ; ++c) {
						temp = linearSystem[col][c];
						linearSystem[col][c]  = linearSystem[line][c];
						linearSystem[line][c] = temp;
					}
					temp = regStatus[col];
					regStatus[col] = regStatus[line];
					regStatus[line] = temp;
				}
				// Elimination
				for (int l=col+1 ; l<REGS_BITS ; ++l) {
					if (linearSystem[l][col]) {
						for (int c=col; c<SECRETKEY_BITS; ++c) {
							linearSystem[l][c] ^= linearSystem[col][c];
						}
						regStatus[l] ^= regStatus[col];
					}
				}
				break;
			}
		}
		if (!headFactor) {
			DEBUG("Error: Not a Cramer System, returning all 0");
			free(LFSRs);
			return 1;
		}
	}

	// Solution finding (starting from line #SECRETKEY_BITS, assuming we have a Cramer System)
	for (int line=SECRETKEY_BITS-1 ; line>=0 ; --line) {
		secretKey[line] = regStatus[line];
		// propagate new result amongst above equations
		for (int l=line-1 ; l>=0 ; --l) {
			regStatus[l] ^= linearSystem[l][line] & secretKey[line];
			linearSystem[l][line] = 0;
		}
	}

	// -------------------------------------------------------
	// Solution Found
	// -------------------------------------------------------

	free(LFSRs);
	return 0;
    }
    
    // Documentation in header file
    int keysetup_reverse_test() {

	byte secretKey[SECRETKEY_BITS];

	// The frameId is totally public
	def frameId = [1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

	// Initial state of R1->R4 LFSRs is determined after keysetup (before the discarded 99 cycles)
	byte R1[R1_BITS] = {1,0,1,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1,1};
	byte R2[R2_BITS] = {0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1};
	byte R3[R3_BITS] = {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,1,1,1};
	byte R4[R4_BITS] = {1,1,1,1,0,1,1,1,0,1,1,0,0,0,0,0,0};
	reverseKeysetup(R1, R2, R3, R4, frameId, secretKey);

	// This secret key is the standard secret key that generates the values above
	byte verifiedSecretKey[SECRETKEY_BITS] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

	// We check that the secret key we processed is really the same as the one used to generate sample data:
	for (int i=0 ; i<SECRETKEY_BITS ; ++i) {
		if (secretKey[i] != verifiedSecretKey[i]) {
			DEBUG("Self-check aborted: a discrepancy was found comparing the produced secret key and the verified one:");
			DUMP_CHAR_VECTOR(secretKey, SECRETKEY_BITS, "Computed secretKey");
			DUMP_CHAR_VECTOR(verifiedSecretKey, SECRETKEY_BITS, "Verification");
			return 1;
		}
	}
	DEBUG("Self-check succeeded: the produced secret key matches the verified one");
	return 0;

    }
}

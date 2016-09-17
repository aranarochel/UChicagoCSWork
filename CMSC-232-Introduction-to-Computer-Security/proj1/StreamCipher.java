
public class StreamCipher {
    // This class encrypts or decrypts a stream of bytes, using a stream cipher.

    public static final int KeySizeBytes = 32;  // IMPLEMENT THIS
    public static final int KeySizeBits = KeySizeBytes*8;
    
    public static final int NonceSizeBytes = 8;
    public static final int NonceSizeBits = NonceSizeBytes*8;

    public byte[] key_k;
    public PRGen prg;
    public byte[] nonce;
    
    public StreamCipher(byte[] key) {
	// <key> is the key, which must be KeySizeBytes bytes in length.
	
	assert key.length == KeySizeBytes;
	
	// IMPLEMENT THIS
	this.key_k = key;
    }
    
    public void setNonce(byte[] arr, int offset){
	// Reset to initial state, and set a new nonce.
	// The nonce is in arr[offset] thru arr[offset+NonceSizeBytes-1].
	// It is an error to call setNonce with the same nonce
	//    more than once on a single StreamCipher object.
	// StreamCipher does not check for nonce uniqueness;
	//    that is the responsibility of the caller.
	
	// Set new nonce
	byte[] nonce = new byte[8];
	for(int i = 0; i < NonceSizeBytes; i++) {
	    nonce[i] = arr[offset+i];
	}

	this.nonce = nonce;

	// Reset PRG using the key and new nonce
	PRF prf = new PRF(this.key_k);
	byte[] prg_k = prf.eval(nonce);

	this.prg = new PRGen(prg_k);
    }
    
    public void setNonce(byte[] nonce) {
	// Reset to initial state, and set a new nonce
	// It is an error to call setNonce with the same nonce
	//    more than once on a single StreamCipher object.
	// StreamCipher does not check for nonce uniqueness;
	//    that is the responsibility of the caller.
	
	assert nonce.length == NonceSizeBytes;
	setNonce(nonce, 0);
    }
    
    public byte cryptByte(byte in) {
	// Encrypt/decrypt the next byte in the stream
	byte pad_byte = (byte)this.prg.nextInt();
	byte encr_byte = (byte)(pad_byte ^ in);
	return encr_byte;
    }
    
    public void cryptBytes(byte[] inBuf, int inOffset, 
			   byte[] outBuf, int outOffset, 
			   int numBytes) {
	// Encrypt/decrypt the next <numBytes> bytes in the stream
	// Take input bytes from inBuf[inOffset] thru inBuf[inOffset+numBytes-1]
	// Put output bytes at outBuf[outOffset] thru outBuf[outOffset+numBytes-1];
	
	try {
	    for(int i = 0; i < numBytes; i++) 
		outBuf[outOffset+i] = cryptByte(inBuf[inOffset+i]);
	}catch(NullPointerException x){
	    System.err.println("\n" + x +": Set a nonce before encrypting!\n");
	    System.exit(0);
	}
    }

    public static void main(String []argv) {
	//tests
	byte[] k = new byte[KeySizeBytes];
	for(int i = 0; i < KeySizeBytes; i++)  k[i] = (byte)i;

	byte[] n = new byte[8];
	for(int m = 0; m < 8; m++)  n[m] = (byte)(m*m);

	// Test encryption and decryption
	StreamCipher sc = new StreamCipher(k);
	sc.setNonce(n);
	
	byte[] message = new byte[32];
	for(int x = 0; x < 32 ; x++)
	    message[x] = (byte)(x+x);

	System.out.print("Original Message\n");
	for(int x1 = 0; x1 < 32; x1++) {
	    System.out.print(message[x1]);
	    System.out.print(" ");
	}
	
	byte[] ciph = new byte[32];

	sc.cryptBytes(message,0,ciph,0,32);
	
	System.out.print("\nEncrypted Message\n");
	for(int z = 0; z < KeySizeBytes; z++) {
	    System.out.print(ciph[z]);
	    System.out.print(" ");
	}


	byte[] decrypted = new byte[32];
	StreamCipher sc2 = new StreamCipher(k);
	sc2.setNonce(n);
	sc2.cryptBytes(ciph,0,decrypted,0,32);
	System.out.print("\nDecrypted Cipher\n");

	for(int z2 = 0; z2 < KeySizeBytes; z2++) {
	    System.out.print(decrypted[z2]);
	    System.out.print(" ");
	}
	
    }
}

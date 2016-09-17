
public class AuthEncryptor {
    // This class is used to compute the authenticated encryption of values.  
    //     Authenticated encryption protects the confidentiality of a value, so that the only 
    //     way to recover the initial value is to do authenticated decryption of the value using the 
    //     same key and nonce that were used to encrypt it.   At the same time, authenticated encryption
    //     protects the integrity of a value, so that a party decrypting the value using
    //     the same key and nonce (that were used to decrypt it) can verify that nobody has tampered with the
    //     value since it was encrypted.
    
    public static final int KeySizeBytes = 32;   // IMPLEMENT THIS
    public static final int KeySizeBits = KeySizeBytes*8;
    
    public static final int NonceSizeBytes = StreamCipher.NonceSizeBytes;

    private StreamCipher sc;
    private PRF mac;
    private byte[] in_key;    // original key
    private byte[] k_1;       // key used for encryption. Generated from PRGen
    private byte[] k_2;       // key used for the MAC. Generated from PRGen
    
    public AuthEncryptor(byte[] key) {
	assert key.length == KeySizeBytes;

	// Set keys for encryption and MAC
	this.in_key = key;

	PRGen prg = new PRGen(key);

	this.k_1 = new byte[KeySizeBytes];
	prg.nextBytes(this.k_1);

	this.k_2 = new byte[KeySizeBytes];
	prg.nextBytes(this.k_2);

	// Create StreamCipher for encryption
	// Create MAC for authentication
	this.sc = new StreamCipher(this.k_1);
	this.mac = new PRF(this.k_2);
    }
    
    public byte[] encrypt(byte[] in, byte[] nonce, boolean includeNonce) {
	// Encrypts the contents of <in> so that its confidentiality and 
	//    integrity are protected against would-be attackers who do 
	//    not know the key that was used to initialize this AuthEncryptor.
	// Callers are forbidden to pass in the same nonce more than once;
	//    but this code will not check for violations of this rule.
	// The nonce will be included as part of the output iff <includeNonce>
	//    is true.  The nonce should be in plaintext if it is included.
	//
	// This returns a newly allocated byte[] containing the authenticated
	//    encryption of the input.
	
	// IMPLEMENT THIS

	// Encrypt message "byte[] in"
	int m_length = in.length;

	this.sc.setNonce(nonce);  // Set the nonce for encryption

	byte[] c = new byte[m_length];  // set outBuf
	this.sc.cryptBytes(in,0,c,0,m_length);

	// Create the MAC tag
	byte[] tag;  // set tag
	tag = this.mac.eval(c);

	if(includeNonce) {  // include nonce at end of encrypted message
	    byte[] auth_enc = new byte[m_length+KeySizeBytes+NonceSizeBytes];

	    for(int i = 0; i < m_length; i++)
		auth_enc[i] = c[i];
	    for(int k = 0; k < KeySizeBytes; k++)
		auth_enc[m_length+k] = tag[k];
	    for(int l = 0; l < NonceSizeBytes; l++)
		auth_enc[m_length+KeySizeBytes+l] = nonce[l];

	    return auth_enc;

	}else {  // don't include the nonce
	    byte[] auth_enc = new byte[m_length+KeySizeBytes];

	    for(int i = 0; i < m_length; i++)
		auth_enc[i] = c[i];
	    for(int k = 0; k < KeySizeBytes; k++)
		auth_enc[m_length+k] = tag[k];

	    return auth_enc;
	}
    }

    public static void main(String []argv) {
	//tests
	//key
	byte[] k = new byte[KeySizeBytes];
	for(int i = 0; i < KeySizeBytes; i++)  k[i] = (byte)i;

	//nonce
	byte[] n = new byte[8];
	for(int m = 0; m < 8; m++)  n[m] = (byte)(m*m);

	//message
	byte[] message = new byte[32];
	for(int x = 0; x < 32 ; x++)
	    message[x] = (byte)(x+x);

	System.out.print("Original Message\n");
	for(int x1 = 0; x1 < 32; x1++) {
	    System.out.print(message[x1]);
	    System.out.print(" ");
	}
	
	AuthEncryptor ae = new AuthEncryptor(k);
	
	byte[] c = ae.encrypt(message,n,true);

	System.out.print("\nEncrypted Message\n");
	for(int z = 0; z < c.length; z++) {
	    System.out.print(c[z]);
	    System.out.print(" ");
	}
	
    }
}

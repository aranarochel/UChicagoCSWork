import java.util.Arrays;

public class AuthDecryptor {
    // This class is used to decrypt and authenticate a sequence of values that were encrypted 
    //     by an AuthEncryptor.
    
    public static final int KeySizeBytes = AuthEncryptor.KeySizeBytes;
    public static final int KeySizeBits = AuthEncryptor.KeySizeBits;
    
    public static final int NonceSizeBytes = StreamCipher.NonceSizeBytes;
    
    private StreamCipher sc;
    private PRF mac;
    private byte[] in_key;
    private byte[] k_1;
    private byte[] k_2;
    
    public AuthDecryptor(byte[] key) {
	assert key.length == KeySizeBytes;
	
	// Set keys for decryption and MAC
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
    
    public byte[] decrypt(byte[] in, byte[] nonce, boolean nonceIncluded) {
	// Decrypt and authenticate the contents of <in>.  The value passed in will normally
	//    have been created by calling encrypt() with the same nonce in an AuthEncryptor 
	//    that was initialized with the same key as this AuthDecryptor.
	// If <nonceIncluded> is true, then the nonce has been included in <in>, and
	//    the value passed in as <nonce> will be disregarded.
	// If <nonceIncluded> is false, then the value of <nonce> will be used.
	// If the integrity of <in> cannot be verified, then this method returns null.   Otherwise it returns 
	//    a newly allocated byte-array containing the plaintext value that was originally 
	//    passed to encrypt().
	
	byte[] mac_tag, non;
	byte[] message;
	int m_len1 = in.length-KeySizeBytes-NonceSizeBytes;
	int m_len2 = in.length-KeySizeBytes;
	
	if(nonceIncluded) {
	    
	    mac_tag = new byte[KeySizeBytes];
	    for(int m = 0; m < KeySizeBytes; m++)
		mac_tag[m] = in[m + m_len1];
	    
	    non = new byte[NonceSizeBytes];
	    for(int n = 0; n < NonceSizeBytes; n++)
		non[n] = in[n + m_len1 + KeySizeBytes];
	    
	    //check for integrity
	    byte[] check;
	    check = this.mac.eval(in,0,m_len1);  //calculate MAC from cipher txt

	    if(!(Arrays.equals(check,mac_tag))) {
		return null;
	    }else{
		this.sc.setNonce(non);
		message = new byte[m_len1];
		this.sc.cryptBytes(in,0,message,0,m_len1);
	    }
	    
	} else {
	    mac_tag = new byte[KeySizeBytes];
	    for(int m = 0; m < KeySizeBytes; m++)
		mac_tag[m] = in[m + m_len2];

	    //check for integrity
	    byte[] check = this.mac.eval(in,0,m_len2);
	    
	    if(!(Arrays.equals(check,mac_tag))) {
	    	message = null;
	    }else{
		this.sc.setNonce(nonce);
		message = new byte[m_len2];
		this.sc.cryptBytes(in,0,message,0,m_len2);
	    }   
	}

	return message;
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
	byte[] c = ae.encrypt(message,n,false);

	System.out.print("\nEncrypted Message\n");
	for(int z = 0; z < c.length; z++) {
	    System.out.print(c[z]);
	    System.out.print(" ");
	}

	System.out.print('\n');
	
	AuthDecryptor ad = new AuthDecryptor(k);
	byte[] txt = ad.decrypt(c,n,false);

	if(txt == null)
	    System.out.print("it's null");
	
	System.out.print("\nDecrypted Message\n");
	for(int z1 = 0; z1 < txt.length; z1++) {
	    System.out.print(txt[z1]);
	      System.out.print(" ");
	    }
	
	
    }
    
}

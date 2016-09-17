
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;

public class SecureChannel extends InsecureChannel {
	// This is just like an InsecureChannel, except that it provides 
	//    authenticated encryption for the messages that pass
	//    over the channel.   It also guarantees that messages are delivered 
	//    on the receiving end in the same order they were sent (returning
	//    null otherwise).  Also, when the channel is first set up,
	//    the client authenticates the server's identity, and the necessary
	//    steps are taken to detect any man-in-the-middle (and to close the
	//    connection if a MITM is detected).
	//
	// The code provided here is not secure --- all it does is pass through
	//    calls to the underlying InsecureChannel.
    
    byte[] masterKey;
    PRGen noncer;
    AuthEncryptor enc;
    AuthDecryptor dec;
	public SecureChannel(InputStream inStr, OutputStream outStr, 
			PRGen rand, boolean iAmServer,
			RSAKey serverKey) throws IOException {
		// if iAmServer==false, then serverKey is the server's *public* key
		// if iAmServer==true, then serverKey is the server's *private* key
	    
		super(inStr, outStr);

		// create KeyExchange instance for DH; prepare outmessage
		KeyExchange dh = new KeyExchange(rand);
		byte[] out = dh.prepareOutMessage();

		if (iAmServer) {
		    // send the prepared outmessage and the signature
		    byte[] sig = serverKey.sign(out,rand);
		    super.sendMessage(out);
		    super.sendMessage(sig);
		    // decrypt client message and derive Master Key
		    byte[] in = super.receiveMessage();
		    byte[] dec = serverKey.decrypt(in);
		  
		    this.masterKey = dh.processInMessage(dec);
		    this.noncer = new PRGen(this.masterKey);
		    this.enc = new AuthEncryptor(this.masterKey);
		    this.dec = new AuthDecryptor(this.masterKey);
		} else {
		    // encrypt out message and send it
		    byte[] outmessage = serverKey.encrypt(out,rand);
		    super.sendMessage(outmessage);
		    
		    // process in message. If can't verify signature, close connection
 		    byte[] in = super.receiveMessage();
		    byte[] sig = super.receiveMessage();

		    boolean verified = serverKey.verifySignature(in,sig);
		    if (verified == false)
			super.close();

		    this.masterKey = dh.processInMessage(in);
		    this.noncer = new PRGen(this.masterKey);
		    this.enc = new AuthEncryptor(this.masterKey);
		    this.dec = new AuthDecryptor(this.masterKey);
		}
	}

	public void sendMessage(byte[] message) throws IOException {
	    // encrypt message using AuthEncryptor & send
	    byte[] nonce = new byte[AuthEncryptor.NonceSizeBytes];
	    noncer.nextBytes(nonce);
	    byte[] outmessage = enc.encrypt(message,nonce,true);
	    super.sendMessage(outmessage);    // IMPLEMENT THIS
	}

	public byte[] receiveMessage() throws IOException {
	    byte[] in = super.receiveMessage();   // IMPLEMENT THIS
	    byte[] nonce = new byte[AuthEncryptor.NonceSizeBytes];
	    noncer.nextBytes(nonce);
	    // get nonce from message and compare
	    // non equal nonces tell us message is out of order
	    byte[] nonceM = new byte[AuthEncryptor.NonceSizeBytes];
	    for(int i = 0; i < AuthEncryptor.NonceSizeBytes; i++)
		nonceM[i] = in[in.length-AuthEncryptor.NonceSizeBytes+i];

	    if(!(Arrays.equals(nonce,nonceM)))
		return null;
	   
	    byte[] inmessage = dec.decrypt(in,nonceM,true);
	    return inmessage;
	}
}

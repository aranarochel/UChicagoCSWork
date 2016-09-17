
import java.security.Key;
import java.security.SecureRandom;
import javax.crypto.KeyGenerator;
import javax.crypto.Mac;

import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import javax.crypto.ShortBufferException;


public class PRF {
	// This class implements a pseudorandom function.
	// It takes a key of size <KeySizeBits> bits == <KeySizeBytes> bytes.
	// It takes an input value of arbitrary size.  The input can be provided
	//    in multiple steps: zero or more calls to update will provide input,
	//    then one call to eval will provide the last of the input and will
	//    return the result of the PRF.
	// It produces a result of size <OutputSizeBits> bits == <OutputSizeBytes> bytes.
	// After the PRF has produced a result (i.e. after a call to eval), the
	//    PRF will be reset. This result of resetting is as if the PRF had
	//    just been created and initialized (with the same key).  Therefore a
	//    PRF can be reused as many times as desired.  But a PRF's key can't be
	//    changed, so using a different key requires creating a different PRF.
	//
	// NOTE TO STUDENTS: This is the ONLY crypto code you are allowed to use,
	//   other than code you have written yourself.   You may not use any
	//   third-party crypto code of any kind, not even the crypto code in the
	//   standard Java libraries.   All of your symmetric (i.e., non public 
	//   key) crypto should be built on this.   Your public key crypto should
	//   be built on the java.math.BigInteger class.

	public static final int KeySizeBits = 256;
	public static final int KeySizeBytes = KeySizeBits/8;
	public static final int OutputSizeBits = 256;
	public static final int OutputSizeBytes = OutputSizeBits/8;

	private static final String AlgorithmName = "HmacSHA256";

	private Mac mac;

	public PRF(byte[] prfKey) {
		// Make a new PRF, with key <prfKey>.   
		// The key must be <KeySizeBytes> bytes in length.
		assert prfKey.length == KeySizeBytes;
		try {
			mac = Mac.getInstance(AlgorithmName);
			KeyGenerator keygen = KeyGenerator.getInstance(AlgorithmName);
			SecureRandom secRand = SecureRandom.getInstance("SHA1PRNG");
			secRand.setSeed(prfKey);
			keygen.init(KeySizeBits, secRand);
			Key key = keygen.generateKey();
			mac.init(key);
		}catch(NoSuchAlgorithmException x){
			x.printStackTrace(System.err);
		}catch(InvalidKeyException x){
			x.printStackTrace(System.err);
		}
	}

	public synchronized void update(byte[] inBuf, int inOffset, int numBytes) {
		// Provide input to the PRF, from inBuf[inOffset] thru inBuf[inOffset+nbytes-1].  
		// More input will be provided later, in subsequent calls to update or eval.
		mac.update(inBuf, inOffset, numBytes);
	}

	public synchronized void update(byte[] inBuf) {
		// Provide input to the PRF.  More input will be provided later, 
		//    in subsequent calls to update or eval.
		update(inBuf, 0, inBuf.length);
	}

	public synchronized void eval(byte[] inBuf, int inOffset, int numBytes,
			byte[] outBuf, int outOffset) throws ShortBufferException {
		// Provide final input to the PRF, and get its output.  The input value
		//   is inBuf[inOffset] through inBuf[inOffset+numBytes-1].
		// The result is put in outBuf[outOffset] 
		//   through outBuf[outOffset+OutputSizeBytes-1].
		// Throws ShortBufferException iff the result won't fit in outBuf.
		mac.update(inBuf, inOffset, numBytes);
		mac.doFinal(outBuf, outOffset);
	}

	public synchronized byte[] eval(byte[] inBuf, int inOffset, int numBytes) {
		// Provide final input to the PRF, and get its output.  The input value
		//   is inBuf[inOffset] through inBuf[inOffset+numBytes-1].
		// This returns a byte-array of length <OutputSizeBytes>.
		try {
			byte[] ret = new byte[OutputSizeBytes];
			eval(inBuf, inOffset, numBytes, ret, 0);
			return ret;
		}catch(ShortBufferException x){
			// should never happen -- we just allocated a big enough buffer
			x.printStackTrace(System.err);
			return null;
		}
	}

	public byte[] eval(byte[] val) {
		// Evaluate the PRF with the contents of val as input.
		return eval(val, 0, val.length);
	}

	public static void main(String[] argv) {
		// This is for testing purposes.   
		// It just sanity-checks the implementation.
		byte[] k = new byte[KeySizeBytes];
		for(int i=0; i<KeySizeBytes; ++i)    k[i] = (byte)i;
		byte[] v = new byte[57];
		for(int i=0; i<v.length; ++i)    v[i] = (byte)i;
		byte[] v2 = new byte[61];
		for(int i=0; i<v2.length; ++i)    v2[i] = (byte)(i+73);

		PRF prf = new PRF(k);
		byte[] x = prf.eval(v);
		assert x.length == OutputSizeBytes;
		byte[] x2 = prf.eval(v2);
		assert x2.length == OutputSizeBytes;
		assert !(x.equals(x2));

		PRF prf2 = new PRF(k);
		byte[] xAgain = prf2.eval(v);
		assert xAgain.length == OutputSizeBytes;
		for(int i=0; i<x.length; ++i){
			assert x[i] == xAgain[i];
		}

		System.out.println("OK");
	}
}

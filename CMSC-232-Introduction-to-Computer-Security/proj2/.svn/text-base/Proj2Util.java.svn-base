
import java.math.BigInteger;

public class Proj2Util {
	
	public static final int HashSizeBits = PRF.OutputSizeBits;
	public static final int HashSizeBytes = HashSizeBits/8;
	
	private static final int PRIME_GEN_CERTAINTY = 128;
	private static final byte[] HASH_PRF_KEY = new byte[PRF.KeySizeBytes]; // defaults to an array of zeroes
	
	// Generates a prime using of length numBits using pseudorandomness from the supplied PRGen
	public static BigInteger generatePrime(PRGen rand, int numBits) {
		 return new BigInteger(numBits, PRIME_GEN_CERTAINTY, rand);
	}
	
	// A hash function. Note that it's just a PRF with a constant key.
	public static byte[] hash(byte[] val) {
		return hash(val, 0, val.length);
	}
	
	// A hash function. Note that it's just a PRF with a constant key.
	public static byte[] hash(byte[] inBuf, int inOffset, int numBytes) {
		PRF hashPrf = new PRF(HASH_PRF_KEY);
		return hashPrf.eval(inBuf, inOffset, numBytes);
	}
	
	// A hash function that returns a "stretched" output, extended to stretchToLengthBytes bytes
	// Hint: Useful for digital signatures
	public static byte[] stretchedHash(byte[] val, int stretchToLengthBytes) {
		assert stretchToLengthBytes >= HashSizeBytes;

		byte[] x = hash(val);
		byte[] ret = new byte[stretchToLengthBytes];
		for(int i=0; i<x.length; ++i){
			ret[i] = x[i];
		}
		return ret;
	}
	
	public static BigInteger bytesToBigInteger(byte[] buf) {
		// convert a byte-array to a BigInteger
		return new BigInteger(1, buf);
	}

	public static byte[] bigIntegerToBytes(BigInteger x, int outputSize) {
		// Convert the BigInteger x to a byte-array
		// x must be non-negative
		// outputSize is the expected size of the output array
		//     (you need to supply this because the code here can't tell how
		//      big an array the caller wants)
		//
		// This operation is the inverse of <bytesToBigInteger>.

		assert x.compareTo(BigInteger.ZERO) >= 0;

		byte[] rawByteArray = x.toByteArray();
		if(rawByteArray.length==outputSize)    return rawByteArray;

		byte[] ret = new byte[outputSize];
		if (rawByteArray.length > outputSize){
			assert (rawByteArray.length == outputSize+1);
			// a sign-extension byte got added; remove it
			for(int i=0; i<outputSize; ++i){
				ret[i] = rawByteArray[i+1];
			}
		} else {  // rawByteArray.length < outputSize
			int diff = outputSize-rawByteArray.length;
			for(int i=0; i<outputSize; ++i){
				ret[i] = (i<diff) ? 0 : rawByteArray[i-diff];
			}
		}
		return ret;
	}

	public static void main(String[] argv) {  // do some basic tests
		BigInteger[] vals = { BigInteger.ZERO, BigInteger.ONE, BigInteger.TEN };

		for(int i=0; i<vals.length; ++i){
			BigInteger result = bytesToBigInteger(bigIntegerToBytes(vals[i], 40));
			if( ! vals[i].equals(result) ){
				System.out.println(vals[i].toString() + " " + result.toString());
			}
		}

		System.out.println("OK");
	}
}
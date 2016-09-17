import java.math.BigInteger;

public class RSAKeyPair {
    private RSAKey publicKey;
    private RSAKey privateKey;
    
    private BigInteger p;
    private BigInteger q;

    public RSAKeyPair(PRGen rand, int numBits) {
	// Create an RSA key pair.  rand is a PRGen that this code can use to get pseudorandom
	//     bits.  numBits is the size in bits of each of the primes that will be used.
	
	// IMPLEMENT THIS
	
	// Generate Primes
	p = Proj2Util.generatePrime(rand,numBits);
	q = Proj2Util.generatePrime(rand,numBits);

	// Calculate N (modulus)
	BigInteger modulus = p.multiply(q);

	// Pick e (public key exponent)
	BigInteger e = new BigInteger("65537");

	// Calculate d (private key exponent)
	BigInteger i = new BigInteger("-1");
	BigInteger d = e.modInverse((p.add(i)).multiply(q.add(i)));

	publicKey = new RSAKey(e,modulus);
	privateKey = new RSAKey(d,modulus);
    }
    
    public RSAKey getPublicKey() {
	return publicKey;
    }
    
    public RSAKey getPrivateKey() {
	return privateKey;
    }
    
    public BigInteger[] getPrimes() {
	// Returns an array containing the two primes that were used in key generation.
	//   In real life we don't always keep the primes around.
	//   But including this helps us grade the assignment.
	BigInteger[] ret = new BigInteger[2];
	ret[0] = p; // IMPLEMENT THIS
	ret[1] = q;
	return ret;
    }

    public static void main(String[] argv){
	
	byte[] k = new byte[32];
	for(int i = 0; i < 32; i++)
	    k[i] = (byte)i;
	
	PRGen prg = new PRGen(k);

	// Test to make sure d was calculated correctly
	// i.e ed mod (p-1)(q-1) == 1
	RSAKeyPair kp = new RSAKeyPair(prg,1024);
	RSAKey pu = kp.getPublicKey();
	RSAKey pr = kp.getPrivateKey();
	BigInteger[] primes = kp.getPrimes();
	BigInteger p = primes[0];
	BigInteger q = primes[1];

	BigInteger expub = pu.getExponent();
	BigInteger exprv = pr.getExponent();
	
	BigInteger i = new BigInteger("-1");
	BigInteger mul = expub.multiply(exprv);
	BigInteger uno = mul.mod((p.add(i)).multiply(q.add(i)));

	System.out.println(uno);
    }
}

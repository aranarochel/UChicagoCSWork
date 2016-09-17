
import java.util.Random;



public class PRGen extends Random {
    // This implements a pseudorandom generator.  It extends java.util.Random, which provides
    //     a useful set of utility methods that all build on next(.).  See the documentation for
    //     java.util.Random for an explanation of what next(.) is supposed to do.
    // If you're calling a PRGen, you probably want to call methods of the Random superclass.
    //
    // There are two requirements on a pseudorandom generator.  First, it must be pseudorandom,
    //     meaning that there is no (known) way to distinguish its output from that of a
    //     truly random generator, unless you know the key.  Second, it must be deterministic, 
    //     which means that if two programs create generators with the same seed, and then
    //     the two programs make the same sequence of calls to their generators, they should
    //     receive the same return values from all of those calls.
    // Your generator must have an additional property: backtracking resistance.  This means that if an
    //     adversary is able to observe the full state of the generator at some point in time, that
    //     adversary cannot reconstruct any of the output that was produced by previous calls to the
    //     generator.
    
    private static final long serialVersionUID = 4210047820764873211L;
    
    public static final int KeySizeBytes = 32;   // IMPLEMENT THIS
    public static final int KeySizeBits = KeySizeBytes*8;
    
    public byte[] state;
	
    public PRGen(byte[] key) {
	super();
	assert key.length == KeySizeBytes;
	
	// IMPLEMENT THIS

	// Set S_0
	this.state = key;
    }
    
    protected int next(int bits) {
	// For description of what this is supposed to do, see the documentation for 
	//      java.util.Random, which we are subclassing.
	
	// IMPLEMENT THIS

	//Advance(S_i), aka update the PRG state
	byte[] m = new byte[1]; m[0] = 0;
	
	PRF prf = new PRF(this.state);
	byte[] new_state = prf.eval(m);
	this.state = new_state;

	//Output(S_i), aka return the next random int
	byte[] n = new byte[1]; n[0] = 1;

	byte[] r = prf.eval(n);

	int output = (r[0] << 24) | (r[1] & 0xFF) << 16 | (r[2] & 0xFF) << 8 | (r[3] & 0xFF);
	output = output >>> (32 - bits);

	return output;
    }

    public static void main(String []argv) {
	//tests
	
	byte[] k = new byte[KeySizeBytes];
	for(int i = 0; i < KeySizeBytes; ++i)   k[i] = (byte)i;

	PRGen prg = new PRGen(k);
	PRGen prg2 = new PRGen(k);
	
	for(int v = 0; v < 10; v++) {
	    System.out.print(prg.next(4));
	    System.out.print(" ");
	}

	System.out.print('\n');
	for(int u = 0; u < 10; u++) {
	    System.out.print(prg2.next(4));
	    System.out.print(" ");
	}
	
    }
}

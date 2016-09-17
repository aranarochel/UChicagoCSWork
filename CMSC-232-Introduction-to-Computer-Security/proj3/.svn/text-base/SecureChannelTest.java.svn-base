import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;
import java.util.Random;


public class SecureChannelTest implements Runnable {
	// This program tests the implementation of channels.   It's not a 
	//    complete test -- if this works, you can't be certain that your
	//    channel implementation is good -- but it does provide a good 
	//    sanity check, and it might smoke out synchronization / deadlock
	//    problems.

	private InputStream     inStream;
	private OutputStream    outStream;
	private boolean         iAmServer;
	private PRGen       prgen;
	private RSAKey      rsaKey;

	private static int messagesSent = 0;
	private static int messagesReceived = 0;

	public SecureChannelTest(InputStream inStr, OutputStream outStr, 
			boolean iAmServer, RSAKey rsaKey) {
		inStream = inStr;
		outStream = outStr;
		this.iAmServer = iAmServer;
		this.rsaKey = rsaKey;

		byte[] prgenKey = new byte[PRGen.KeySizeBytes];
		Random random = new Random();
		random.nextBytes(prgenKey);

		prgen = new PRGen(prgenKey);
	} 

	private static RSAKeyPair generateRSAKeyPair(int numbits)
	{
		byte[] prgenKey = new byte[PRGen.KeySizeBytes];
		Random random = new Random();
		random.nextBytes(prgenKey);

		PRGen prgen = new PRGen(prgenKey);

		RSAKeyPair rsakp = new RSAKeyPair(prgen, numbits);
		return rsakp;
	}

	private static boolean sendMessageSafe(SecureChannel chan, String threadName, String channelName, byte[] message)
	{
		try {
			chan.sendMessage(message);
		}
		catch (IOException e)
		{
			String exceptionMessage = e.getMessage();
			if (e.toString().equals("java.io.EOFException"))
				System.out.printf("%s thread ending. %s channel indicates there will be no more data (EOF).\n", threadName, channelName);
			else
				if (exceptionMessage.equals("Pipe closed"))
					System.out.printf("%s thread ending. %s channel is closed.\n", threadName, channelName);
				else
				{
					System.out.printf("%s thread ending. Exception thrown that should not have occurred (unless you specifically threw it).\n If you can't fix, post on Piazza.", threadName);
					e.printStackTrace();
				}
			return false;
		}

		messagesSent++;
		return true;
	}


	private static byte[] receiveMessageSafe(SecureChannel chan, String threadName, String channelName)
	{
		byte[] returnMsg;
		try {
			returnMsg = chan.receiveMessage();
		}
		catch (IOException e)
		{
			String exceptionMessage = e.getMessage();
			if (e.toString().equals("java.io.EOFException"))
				System.out.printf("%s thread ending. %s channel indicates there will be no more data (EOF).\n", threadName, channelName);
			else
				if (exceptionMessage.equals("Pipe closed"))
					System.out.printf("%s thread ending. %s channel is closed.\n", threadName, channelName);
				else
				{
					System.out.printf("%s thread ending. Exception thrown that should not have occurred (unless you specifically threw it).\n If you can't fix, post on Piazza.", threadName);
					e.printStackTrace();
				}
			return null;
		}

		messagesReceived++;
		return returnMsg;
	}

	public void run() {

		try {

			SecureChannel chan = new SecureChannel(inStream, outStream, prgen,
					iAmServer, rsaKey);

			// server echoes back messages it received
			if(iAmServer) {
				while(true) 
				{

					byte[] msg = receiveMessageSafe(chan, "Server", "Client -> Server");
					if (msg == null)
						return;


					//Example debug statement
					//String debugTag = String.format("server received: ");
					//String endDebugTag  = "\n";
					//SecureChannelUtils.printTaggedByteArray(debugTag, msg, endDebugTag);

					boolean succeeded = sendMessageSafe(chan, "Server", "Server -> Client", msg);
					if (!succeeded)
						return;

				}
			}

			// client sends message, receives it back, verifies equal
			else {

				//10 messages hard coded to be 73 bytes long
				byte[] buf = new byte[73];        
				for(int i=0; i < 10; ++i){

					//jth byte in message #i is i+j
					for(int j=0; j < buf.length; ++j){
						buf[j] = (byte) (i+j);
					}


					boolean succeeded = sendMessageSafe(chan, "Client", "Client -> Server", buf);
					if (!succeeded)
						return;

					byte[] echo = receiveMessageSafe(chan, "Client", "Server -> Client");
					if (echo == null)
						return;


					assert echo.length == buf.length;
					for(int j=0; j<buf.length; ++j){
						assert buf[j] == echo[j];
					}
				}
				chan.close();
			}
		} catch(IOException x){
			x.printStackTrace(System.err);
		}
	}


	public static void main(String[] argv) throws IOException {
		//should be chosen so that maxPlaintextLength is > 72
		int RSA_KEY_PAIR_BITS = 1024; 
		RSAKeyPair rsakp = generateRSAKeyPair(RSA_KEY_PAIR_BITS);
		//if this fails, you need to adjust KEY_PAIR_BITS!
		assert(rsakp.getPublicKey().maxPlaintextLength() > 72); 

		PipedOutputStream out1 = new PipedOutputStream();
		PipedInputStream in1 = new PipedInputStream(out1);
		PipedOutputStream out2 = new PipedOutputStream();
		PipedInputStream in2 = new PipedInputStream(out2);

		SecureChannelTest cct = new SecureChannelTest(in1, out2, 
				false, rsakp.getPublicKey());
		SecureChannelTest sct = new SecureChannelTest(in2, out1, 
				true, rsakp.getPrivateKey());
		Thread clntThread = new Thread(cct);
		Thread servThread = new Thread(sct);
		servThread.setDaemon(true);

		servThread.start();
		clntThread.start();

		try {
			clntThread.join();
			System.out.println("OK");
		}catch(InterruptedException x){
			x.printStackTrace(System.err);
		}

		System.out.printf("Total messages sent: %d, Total messages received: %d\n", messagesSent, messagesReceived);
	}
}
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PipedInputStream;
import java.io.PipedOutputStream;


public class ChannelTest implements Runnable {
	// This program tests the implementation of channels.   It's not a 
	//    complete test -- if this works, you can't be certain that your
	//    channel implementation is good -- but it does provide a good 
	//    sanity check, and it might smoke out synchronization / deadlock
	//    problems.

	private InputStream     inStream;
	private OutputStream    outStream;
	private boolean         iAmServer;


	public ChannelTest(InputStream inStr, OutputStream outStr, 
			boolean iAmServer) {
		inStream = inStr;
		outStream = outStr;
		this.iAmServer = iAmServer;
	} 


	public void run() {
		try {

			InsecureChannel chan = new InsecureChannel(inStream, outStream);
			
			// Uncomment to put the channel in debug mode so that it prints the messages sent and received
			//chan.setDebugMode(true);
			
			// Uncomment to change the number of bytes of each message printed in debug mode (default: all the bytes)
			// chan.setNumBytesToEcho(12);

			// server echoes back messages it received
			if(iAmServer) {
				while(true) 
				{
					byte[] msg = chan.receiveMessage();

					//Example debug statement
					//String debugTag = String.format("server received: ");
					//String endDebugTag  = "\n";
					//SecureChannelUtils.printTaggedByteArray(debugTag, msg, endDebugTag);

					chan.sendMessage(msg);
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


					chan.sendMessage(buf);

					byte[] echo = chan.receiveMessage();

					assert echo.length == buf.length;
					for(int j=0; j<buf.length; ++j){
						assert buf[j] == echo[j];
					}
				}
			}
		} catch(IOException x){
			x.printStackTrace(System.err);
		}
	}


	public static void main(String[] argv) throws IOException {

		PipedOutputStream out1 = new PipedOutputStream();
		PipedInputStream in1 = new PipedInputStream(out1);
		PipedOutputStream out2 = new PipedOutputStream();
		PipedInputStream in2 = new PipedInputStream(out2);

		ChannelTest cct = new ChannelTest(in1, out2, false);
		ChannelTest sct = new ChannelTest(in2, out1, true);
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
	}
}
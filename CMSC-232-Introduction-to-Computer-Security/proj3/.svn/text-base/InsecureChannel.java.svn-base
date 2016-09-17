import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;


public class InsecureChannel {
	// This implements a workable communication channel abstraction.   
	// But it offers no particular security guarantees.    You will extend this in a subclass
	// to provide security.
	
	private static final int ECHO_NOTHING = 0;
	private static final int ECHO_EVERYTHING = Integer.MAX_VALUE;
	
	
	
	private DataInputStream inStream;
	private DataOutputStream outStream;

	private static int channelCount = 0;    
	private int channelID; //set at creation
	private static int sendCount = 0;
	private static int receiveCount = 0;
	
	private boolean debugMode = false;
	private int numBytesToEcho = ECHO_EVERYTHING;

	public InsecureChannel(InputStream inStr, OutputStream outStr)
			throws IOException {
		// Take a pair of streams, one incoming and one outgoing, and use them to 
		// build a bi-directional channel.   By assumption, there is another 
		// program somewhere that is at the other end of each one of these 
		// streams, so that our inStream is hooked up to their outStream, 
		// and vice versa.


		channelID = channelCount;
		channelCount++;
		inStream = new DataInputStream(inStr);
		outStream = new DataOutputStream(outStr);
	}

	public void sendMessage(byte[] message) throws IOException {
		// Send a discrete message (datagram) to the party on the other end of the 
		// channel. We assume that the party on the other end will make a matching 
		// call to receiveMessage.

		outStream.writeInt(message.length);
		for (int i = 0; i < message.length; i++)	
			outStream.write(message[i]);		

		echoMessage(true, message);

		outStream.flush();
	}

	public byte[] receiveMessage() throws IOException {
		// Receive a discrete message (datagram) from the party on the other end of 
		// the channel. We assume that the party on the other end will make a 
		// matching call to sendMessage.
		// This will throw an EOFException if the stream has been closed.

		int len = inStream.readInt();
		if(len < 0){
			// channel was closed, return null
			return null;
		}
		
		byte[] message = new byte[len];
		for(int i=0; i < message.length; i++)
			message[i] = inStream.readByte();
		
		echoMessage(false, message);

		return message;
	}

	public void close() throws IOException {
		// Close the channel
		outStream.close();
		inStream.close();
	}
	
	public void setDebugMode(boolean debugMode) {
		// Enable or disable debug mode, in which the channel prints the contents of messages sent and received
		
		this.debugMode = debugMode;
	}
	
	public void setNumBytesToEcho(int numBytesToEcho) {
		// Change the number of bytes of each message printed in debug mode (default: all the bytes)
		
		assert numBytesToEcho >= 0;
		this.numBytesToEcho = numBytesToEcho;
	}
	
	
	
	private void echoMessage(boolean send, byte[] message) {
		if (!debugMode) {
			return;
		}
		
		int bytesToEcho = Math.min(numBytesToEcho, message.length);
		String ellipsis = (bytesToEcho < message.length) ? "..." : "";
		byte[] echoBytes = Arrays.copyOf(message, bytesToEcho);
		
		
		String prefix;
		int count;
		if (send) {
			prefix = "Written to";
			count = sendCount++;
		} else {
			prefix = "Removed from";
			count = receiveCount++;
		}
		
		SecureChannelUtils.printTaggedByteArray(String.format("%s channel %d: msg #%d [", prefix, channelID, count), 
											  	echoBytes, 
											  	String.format("%s]\n", ellipsis));	
	}
}
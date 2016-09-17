public class SecureChannelUtils {

	// This method prints a byte array as 2 digit hex numbers
	
	public static void printByteArray(byte[] byteArray)
	{
		StringBuilder sb = new StringBuilder();

		for (int i = 0; i < byteArray.length; i++)    	  	
			sb.append(String.format("%02X ", byteArray[i]));
		
		System.out.print(sb.toString());
	}

	// This method prints a byte array as 2 digit hex numbers, prepended
	// by the given tag

	public static void printTaggedByteArray(String tag, byte[] byteArray)
	{
		StringBuilder sb = new StringBuilder();
		sb.append(tag);

		for (int i = 0; i < byteArray.length; i++)    	  	
			sb.append(String.format("%02X ", byteArray[i]));
		
		System.out.print(sb.toString());
	}

	// This method prints a byte array as 2 digit hex numbers, prepended
	// by the given beginTag, and postpended by the given endTag

	public static void printTaggedByteArray(String beginTag, byte[] byteArray, String endTag)
	{
		StringBuilder sb = new StringBuilder();
		sb.append(beginTag);

		for (int i = 0; i < byteArray.length; i++)    	  	
			sb.append(String.format("%02X ", byteArray[i]));
		
		sb.append(endTag);
		System.out.print(sb.toString());		
	}
}
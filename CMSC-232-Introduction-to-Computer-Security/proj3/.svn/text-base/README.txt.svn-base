I. Files & Compiling

Files:

ChannelTest.java	: Sends messages between a client and server over an insecure channel
InsecureChannel.java    : Implements the methods for sending and receiving messages in an insecure channel
README.txt		: README file
SecureChannel.java	: Implements methods for establishing a secure channel for sending and receiving messages
SecureChannelTest.java	: Sends messages between a client and server over a secure channel
SecureChannel.Utils.java: Utility functions useful for debugging the SecureChannel implementation
project1.jar		: JAR file containing code needed for authenticated encryption
project2.jar     	: JAR file containing code needed for Diffie-Hellman key exchange & RSA


Compiling:
"javac -cp project1.jar:project2.jar:. [.java files]"
Running:
"java -ea -cp project1.jar:project2.jar:. [main class]"


II. Design

The purpose of the SecureChannel class is to implement a channel that is secure enough for a client and server to send messages between eachother. This implies that once established, the secure channel will protect against any eavesdroppers or MITM, effectively ensuring Authenticty between the client & server, and Confidentiality and Integrity of the messages they send.

    A. Authenticity:
       
       SecureChannel first starts by ensuring that the server the client is trying to connect to can actually be authenticated. This is all done in the constructor to the SecureChannel class.

       The client (or server) first starts by instantiating a SecureChannel class using an input and output stream, a strongly secure pseudorandom generator, a boolean (true if instantiator is the server; false if client), and an RSA key (the server uses its private key, while the client uses the server's public key). If instantiated correctly, the constructor uses Diffie-Hellman (DH) as the key exchange protocol to generate a secret master key.

       On the server side, the server creates an instance of a KeyExchange class which is used to generate a starting message ("byte[] out"). The server then signs this message and sends both to the client as two separate messages. The client also prepares a message, but instead of signing it, the client encrypts it using the server's public key.
       Once the server receives the client's encrypted message, the server decrypts and processes it to generate the secret master key. When the client receives the server's message and signature, it verifies that the signature actually belongs to the server. If all checks out, the client also processes the message and generates the secret master key. If the signature couldn't have belonged to the server, it closes the connection.

       The benefit of having the client verify the server's signature is that it authenticates the server. A MITM would not be able to produce a valid signature since the signature could only have been produced using the server's private key. That, compounded with the fact that the client encrypts its message, ensures that a MITM would not be able to produce the same secret master key.

       The constructor finishes by also creating a pseudorandom generator from the master key ("PRGen noncer") to cretae nonces, and authenticated encryptors/decryptors ("AuthEncryptor enc, AuthDecryptor dec") using the same master key. Having the client and server create the same encryptor and decryptor using their shared secret key allows for symmetric encryption using a stream cipher.


       B. Confidentiality & Integrity

       	  After finishing the handshake that generates a shared secret master key, the provided methods "sendMessage()" & "receiveMessage()" ensure that the messages sent and received are protected from any eavesdroppers looking or any MITM who might try to reorder the messages.

	  sendMessage(): 
	  		 The client/server uses the authenticated encryptor to encrypt their message and includes the nonce at the end. Symmetric encryption using a stream cipher ensures that messages are confidential. The encryptor also includes a MAC tag in the message for integrity purposes.

	  receiveMessage(): 
	  		    The client/server uses the authenticated decryptor to generate the original message sent. If the decryptor returns no errors, that means that the MAC tag in the encrypted message was verified and that the original message wan't tampered with by a possible MITM. This ensures the integrity of messages.

	  This method does the additional work of verifying if a message was sent out-of-order. It can do this becauses the nonces that are used in encrypting the messages are generated using a PRGen created using the master key. Therefore both the client and server should generate the same nonces in the same order. So when the client receives a message, it (1) checks the nonce in the message, (2) generates its next nonce, (3) compares its nonce with the nonce in the received message, (4) if the same, the message is in the right order; if not the message is out-of-order. The diagram below shows this:


	       	      

[creates nonce 1]		Client ----------------- Message(nonce1) --------------------> Server	 [creates nonce 1; check passes]
[creates nonce2; check passes]  Client <---------------- Message(nonce2) --------------------- Server	 [creates nonce 2]
[creates nonce 3]      		Client ----------------- Message(nonce3) --------------------> Server	 [creates nonce3; check passes]
	       			       			            |
								    |
								    |
								   etc.
								    |
								    |
								    |
[creates nonce 10]		Client -------- Message(nonce 10)\	/ Message(nonce 11)--> Server	[creates nonce 10; check fails, thus out-of-order]
								   MITM 
[creates nonce 11]	       	Client -------- Message(nonce 11)/ 	\ Message(nonce 10)--> Server	[creates nonce 11; check fails, thus out-of-order]



III. Conclusion
     
     If used correctly, the SecureChannel class provides (1) Authenticity between client and server, (2) Confidentiality of messages, and (3) Integrity of messages.
     (1) Authenticity is provided in the Diffie-Hellman key exchange process where the client verifies the server's signature.
     (2) Confidentiality is provided in the stream cipher encryption of messages
     (3) Integrity is provided in the creation and verification of MAC tags in the messages (this is done at the same time a decryption)

     This leaves little room for security breaches, as any threat will no be able to generate the same secret master key, won't be able to eavesdrop on messages, won't be able to tamper with messages, and won't be able to reorder messages without the client or server noticing.


								 

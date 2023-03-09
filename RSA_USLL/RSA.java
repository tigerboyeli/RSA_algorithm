import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.PrivateKey;
import java.security.PublicKey;

import javax.crypto.Cipher;
import java.util.Arrays;

// compiler : javac ras.java
// Executer : java rsa

public class rsa {
    public static void main(String[] args) throws Exception {
    	System.out.println("Hello");
    	
    	for (String arg : args){
    		System.out.println("arg: " + arg);
    	}
    
    	String message = "Hello Server!";
    
        // Generate public and private keys
        KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
        keyGen.initialize(1024);
        KeyPair keyPair = keyGen.generateKeyPair();
        PublicKey publicKeyClient = keyPair.getPublic();
        PrivateKey privateKeyClient = keyPair.getPrivate();

        // Get instance of the Cipher object
        Cipher cipher = Cipher.getInstance("RSA");

        // Encrypt the message
        cipher.init(Cipher.ENCRYPT_MODE, publicKeyClient);
        byte[] encryptedMessage = cipher.doFinal(message.getBytes());

        // Send encrypted message to server
        //Server.receiveMessage(encryptedMessage);

        System.out.println("public key client: " + publicKeyClient);
        System.out.println("private key client: " + privateKeyClient);
        System.out.println("message: " + message);
        System.out.println("encrypted message: " +  Arrays.toString(encryptedMessage));


        //--------------

        cipher.init(Cipher.DECRYPT_MODE, privateKeyClient );
        String decryptedMessage = new String(cipher.doFinal(encryptedMessage));

        System.out.println("Decrypted message: " + decryptedMessage);

    }
}

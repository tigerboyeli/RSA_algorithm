import java.util.Random;

public class RSA {

    // Greatest Common Divisor
    public static int gcd(int a, int b) {
        int temp;
        while (b != 0) {
            temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    // Function to calculate (a ^ b) % c
    public static long modulo(long a, long b, long c) {
        long x = 1, y = a;
        while (b > 0) {
            if (b % 2 == 1) {
                x = (x * y) % c;
            }
            y = (y * y) % c;
            b /= 2;
        }
        return x % c;
    }

    // Returns true if n is prime
    public static boolean isPrime(long n) {
        if (n <= 1 || n == 4)
            return false;
        if (n <= 3)
            return true;
        for (int i = 2; i <= Math.sqrt(n); i++) {
            if (n % i == 0)
                return false;
        }
        return true;
    }

    // Generates public and private keys
    public static void generateKeys(long[] keys) {
        Random rand = new Random();
        long p = rand.nextLong() % (Long.MAX_VALUE - 100) + 100;
        while (!isPrime(p)) {
            p = rand.nextLong() % (Long.MAX_VALUE - 100) + 100;
        }

        long q = rand.nextLong() % (Long.MAX_VALUE - 100) + 100;
        while (!isPrime(q)) {
            q = rand.nextLong() % (Long.MAX_VALUE - 100) + 100;
        }

        long n = p * q;
        long phi = (p - 1) * (q - 1);

        long e = 2;
        while (e < phi) {
            if (gcd((int) e, (int) phi) == 1)
                break;
            else
                e++;
        }

        long d = modulo(e, 1, phi);

        keys[0] = p;
        keys[1] = q;
        keys[2] = e;
        keys[3] = d;
        keys[4] = n;
    }

    // Encrypts message
    public static long encryptMessage(long message, long e, long n) {
        return modulo(message, e, n);
    }

    // Decrypts message
    public static long decryptMessage(long ciphertext, long d, long n) {
        return modulo(ciphertext, d, n);
    }

    public static void main(String[] args) {
        long[] keys = new long[5];
        generateKeys(keys);
        long p = keys[0];
        long q = keys[1];
        long e = keys[2];
        long d = keys[3];
        long n = keys[4];

        System.out.println("p " + p);
        System.out.println("q " + q);
        System.out.println("d " + d);
        System.out.println("n " + n);
        System.out.println("e " + e);

        long message = 1234;
        System.out.println("Message: " + message);

        long ciphertext = encryptMessage(message, e, n);
        System.out.println("Ciphertext: " + ciphertext);

        long decryptedMessage = decryptMessage(ciphertext, d, n);
        System.out.println("Decrypted Message" + decryptedMessage);

    } 

}

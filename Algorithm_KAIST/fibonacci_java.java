package elice;

import java.io.*;
import java.util.Scanner;
import elice.EliceUtils;

public class Main {
	public static void main(String args[]) {
        long start = System.nanoTime();
        
        // Evaluate implemented functions measuring its CPU time on various inputs.
        // Measurement is intended to be understood as per trial on a single input not as 'for n = 1...10^10 do f(n)'
        
		//System.out.println(FibRec(15));
		// System.out.println(FibIter(15));
		 System.out.println(FibRepSq(1000000000));
        
        long time = System.nanoTime() - start;
        System.out.print("CPU time used (s): ");
		System.out.println((float) time / 1000000000);

	}


    public static int PRIME = 30011;
    


	public static int FibRec(int n) {
		if(n==0) {
	    	return 0;
	    }
	    else if(n==1)
	    {
	    	return 1;
	    }
	    else return ( FibRec(n-1) + FibRec(n-2) ) % PRIME;
	}
	public static int FibIter(int n) {
		if(n == 0) return 0;
		int fib = 1; int fibL = 0;
		while(n > 1) {
			int tmp = fibL;
			fibL = fib;
			fib = (fibL + tmp) % PRIME;
			n = n - 1;
		}
			        
		return fib % PRIME;
	}
	
	public static long[][] mul1(long[][] M) {
		long[][] A = {{0, 0}, {0, 0}};
		A[0][0] = (M[0][0] + M[0][1]) % (long)PRIME; 
		A[0][1] = M[0][0] % (long)PRIME;
		A[1][0] = (M[1][0] + M[1][1]) % (long)PRIME;
		A[1][1] = M[1][0] % (long)PRIME;
		return A;
	}

	public static long[][] mul2(long[][] M1,long[][]  M2){
		long[][] A = {{0, 0}, {0, 0}};
		A[0][0] = ((M1[0][0] * M2[0][0]) % (long)PRIME) + ((M1[0][1] * M2[1][0]) % (long)PRIME);
        A[0][1] = ((M1[0][0] * M2[0][1]) % (long)PRIME) + ((M1[0][1] * M2[1][1]) % (long)PRIME);
        A[1][0] = ((M1[1][0] * M2[0][0]) % (long)PRIME) + ((M1[1][1] * M2[1][0]) % (long)PRIME);
        A[1][1] = ((M1[1][0] * M2[0][1]) % (long)PRIME) + ((M1[1][1] * M2[1][1]) % (long)PRIME);
        return A;        
	}
        
	public static long[][] RepSq(long n){
		long[][] iden = {{1, 0}, {0, 1}};
		if(n > 1) {
			iden = RepSq(n >> 1);
		    iden = mul2(iden, iden);
		}
		if(n % 2 == 1) iden = mul1(iden);            
		        
		return iden;
	}
        
	public static int FibRepSq(int n) {
            long N = n;
			long[][] A = RepSq(n);
        	return (int)(A[0][1] % (long)PRIME);
	}

    /**
     * 30pts (10pts each) Experiment
     * Try evaluating your implemented functions on different sizes of n
     * Determine the largest n which makes each function runs within 30 seconds
     * The number ranges among (1: [0, 100], 2: [101, 10000], 
     * 3: [10001, 1000000], 4: [1000001, 1000000000], 5: [1000000001, 2147483647])
     * store the index of the range that you could reach for each function.
     */
    public static int MAX_OF_INPUT_REC = 1;    
    public static int MAX_OF_INPUT_ITER = 4;
    public static int MAX_OF_INPUT_REPSQ = 4;


}

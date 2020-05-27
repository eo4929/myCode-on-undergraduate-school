PRIME = 30011


def main():
    import time
    start = time.clock()
    
    # Evaluate implemented functions measuring its CPU time on various inputs.
    # Measurement is intended to be understood as per trial on a single input not as 'for n = 1...10^10 do f(n)'
    
    print(FibRec(34))  # exceed
    print(FibIter(34)) # PRIME
    print(FibRepSq(34)) # 0.0001
    
    print("CPU time used (s): " + str(time.clock() - start))
    
    
def FibRec(n):
    """
    20pts. Implement Fibonacci number with Recursive algorithm
    @param n: 
    int
    @return:
    int. return fib(n) modulo PRIME. 
    See the side section its definition
    """    
    #implement here
    if n==0: 
        return 0
    elif n==1: 
        return 1
    else: 
        return ( FibRec(n-1) + FibRec(n-2) ) % PRIME
'''
def FibIter(n):
    if n < 2:
        return n

    a, b = 0, 1
    for i in range(n-1):
        a, b = b, a + b

    return b % PRIME

'''

def FibIter(n):
    """
    20pts. Implement Fibonacci number with Iterative algorithm
    @param n:
    int
    @return:
    int. return fib(n) modulo PRIME.
    See the side section its definition
    """
    # implement here
    
    if n == 0: return 0
    fib, fibL = 1, 0
    while n > 1:
        tmp = fibL
        fibL = fib
        fib = (fibL + tmp) % PRIME # not overflow..
        n = n - 1
    return fib % PRIME

def FibRepSq(n):
    def RepSq(n):
        def mul1(M):
            A = [[0, 0], [0, 0]]
            A[0][0] = (M[0][0] + M[0][1]) % PRIME 
            A[0][1] = M[0][0] % PRIME
            A[1][0] = (M[1][0] + M[1][1]) % PRIME
            A[1][1] = M[1][0] % PRIME
            return A

        def mul2(M1, M2):
            A = [[0, 0], [0, 0]]
            A[0][0] = ((M1[0][0] * M2[0][0]) % PRIME) + ((M1[0][1] * M2[1][0]) % PRIME)
            A[0][1] = ((M1[0][0] * M2[0][1]) % PRIME) + ((M1[0][1] * M2[1][1]) % PRIME)
            A[1][0] = ((M1[1][0] * M2[0][0]) % PRIME) + ((M1[1][1] * M2[1][0]) % PRIME)
            A[1][1] = ((M1[1][0] * M2[0][1]) % PRIME) + ((M1[1][1] * M2[1][1]) % PRIME)
            return A

        iden = [[1, 0], [0, 1]]
        if n > 1:
            iden = RepSq(n >> 1)
            iden = mul2(iden, iden)
        if n % 2 == 1:
            iden = mul1(iden)
        return iden

    return RepSq(n)[0][1] % PRIME

    

''' 
30pts (10pts each) Experiment
Try evaluating your implemented functions on different sizes of n
Determine the largest n which makes each function terminates within 30 seconds
The number ranges among (1: [0, 100], 2: [101, 10000], 
3: [10001, 1000000], 4: [1000001, 1000000000], 5: [1000000001, 2147483647])
Store the index of the range that you could reach for each function.
'''
MAX_OF_INPUT_REC = 1
MAX_OF_INPUT_ITER = 4
MAX_OF_INPUT_REPSQ = 5


if __name__ == "__main__":
    main()

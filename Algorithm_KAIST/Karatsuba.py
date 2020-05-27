'''
This skeleton code shows you how to deal with input/output.
You may or may not utilize this skeleton code.
You can also start from scratch.
'''

def main():
    N = int(input())
    A = list(map(float, input().split()))
    B = list(map(float, input().split()))
    
    C = multiply(A, B)
    for c in C:
        print('%.10f' % c, end=' ')

def naive(A, B):
    res = [0] * (len(A) + len(B) - 1)
    
    for n in range(len(A)):
        for m in range(len(B)):
            res[n + m] = res[n + m] + B[m] * A[n] 

    return res

    
def multiply(A, B):
    def helper(A, B):
        flip = 1
        res = [0] * max(len(A), len(B))
        for i in range(len(res)):
            if i < len(A):
                res[i] += A[i]
            if i < len(B):
                res[i] += B[i] * flip
        return res
    def helper2(A, B):
        flip = -1
        res = [0] * max(len(A), len(B))
        for i in range(len(res)):
            if i < len(A):
                res[i] += A[i]
            if i < len(B):
                res[i] += B[i] * flip
        return res

    if len(A) <= 20:
        return naive(A, B)
    else:
        n = len(A)
        half = int(n / 2)
        A_lo, A_hi = A[:half], A[half:]
        B_lo, B_hi = B[:half], B[half:]
        C_lo = multiply(A_lo, B_lo)
        C_hi = multiply(A_hi, B_hi)

        C_mid = multiply(helper(A_lo, A_hi), helper(B_lo, B_hi))
        C_mid = helper2(C_mid, C_lo)
        C_mid = helper2(C_mid, C_hi)

        C_mid = [0] * half + C_mid
        C_hi = [0] * (half * 2) + C_hi

    return helper(helper(C_lo, C_mid), C_hi)

if __name__ == '__main__':
    main()
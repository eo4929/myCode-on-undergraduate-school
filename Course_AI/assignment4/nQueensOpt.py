'''
2016059207 박대영
과제4(성능 100배 이상으로 최적화하기) 완료
'''

from z3 import *
import time

# Number of Queens
print("N: ")
N = int(input())

start = time.time()
# Variables
newX = [ Int("x_%s" % (i + 1)) for i in range(N) ] # 명세서에 주어진 대로 1차원 List로 변수 정의 (1~N)
#print(newX)

# Constraints
newDomain = [And(1<=newX[col], newX[col]<=N) for col in range(N) ] # 도메인도 0~1에서 1~N을 사용하도록 만듦 => 각 변수마다 And operation 으로 도메인 표현
#print(newDomain)

newRowColConst = [ simplify( Distinct( [ newX[i] for i in range(N) ] ), blast_distinct=True) ] # row,col별 1개의 queen만 갖도록 제약조건 추가
newRowColConst = [Distinct(newX)] # 위의 행과 열에 대한 제약조건을 이 방식으로 수정하였습니다 => 이렇게 formula를 정의하니 위 라인으로 돌린 것 보다 성능이 0.1sec 향상(N=20일때)
# Distinct(x, y) 는 x != y 와 동일함 => 파라미터로 List가 들어가도 가능
# simplify(Distinct(x, y, z), blast_distinct=True) 는 And(Not(x == y), Not(x == z), Not(y == z)) 와 동일한 기능을 제공함
#print(newRowConst)

newDigConst = [ If(i==j , True, And( newX[i] - newX[j] != (i-j) , newX[i] - newX[j] != (j-i) ) ) for i in range(N) for j in range(i) ] # 성능 저하의 주요 원인인 대각선 체크 부분 수정
# i==j이면 제약조건 검사 의미없으니 True로 보내고 다를 경우 대각선에 queen 있는지 검사 => newX는 Z3 integer 이니 abs 함수를 사용할 수 없음 그래서 i와 j 순서 다를 경우 고려(And 연산자를 이용)
#print(newDigConst)

'''
기존 알고리즘에서 사용한 Symbol과 Constraint들
X = [[Int("x_%s_%s" % (row, col)) for row in range(N)] for col in range(N) ]
domain = [Or(X[row][col] == 0, X[row][col] == 1) for row in range(N) for col in range(N) ]
rowConst = [Sum(X[row]) == 1 for row in range(N)]
colConst = [Sum([X[row][col] for row in range(N)]) == 1 for col in range(N) ]
digConst = [Implies(And(X[i][j] == 1, X[k][h] == 1,
            i != k, j != h), abs(k - i) != abs(j - h))
            for i in range(N) for j in range(N)
            for k in range(N) for h in range(N)]
eight_queens_c = domain + rowConst + colConst + digConst
'''

new_eight_queens_c = newDomain + newRowColConst + newDigConst # 서치 스페이스를 줄인 새로운 KB 완성

s = Solver()
#s.add(eight_queens_c)
s.add(new_eight_queens_c)

if s.check() == sat:
    m = s.model()
    #r = [[m.evaluate(X[i][j]) for j in range(N)] for i in range(N)]
    r = [ m.evaluate(newX[i]) for i in range(N) ] # 1차원 배열(리스트)이니 거기에 맞게 파라미터 수정
    print_matrix(r)

print("elapsed time: ", time.time() - start, " sec")


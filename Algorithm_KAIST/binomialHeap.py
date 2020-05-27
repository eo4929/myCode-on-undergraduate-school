
INCLUDE_TEST = [1, 2, 3, 4, 5, 6, 7]
#INCLUDE_TEST = [8]

import random
import time

class Pointer: 
    def __init__(self,BinomialTree):
        self.BinomialTree = BinomialTree
    def get_key(self):
        return self.BinomialTree.key
    def decrease_key(self,new_key):
        pointer_me = Pointer(self.BinomialTree)
        pointer_me.BinomialTree.key = new_key

        while pointer_me.BinomialTree.parent != None:
            pointer_parent = pointer_me.BinomialTree.parent.pointer
            if pointer_me.BinomialTree.key < pointer_parent.BinomialTree.key:
                pointer_me.BinomialTree.binomialHeap = pointer_parent.BinomialTree.binomialHeap
                pointer_me.BinomialTree.key, pointer_parent.BinomialTree.key = pointer_parent.BinomialTree.key, pointer_me.BinomialTree.key
                pointer_me.BinomialTree.pointer, pointer_parent.BinomialTree.pointer = pointer_parent.BinomialTree.pointer, pointer_me.BinomialTree.pointer

                pointer_me.BinomialTree, pointer_parent.BinomialTree = pointer_parent.BinomialTree, pointer_me.BinomialTree
            else: break

        self.BinomialTree = pointer_me.BinomialTree
        self.BinomialTree.pointer = self
    def delete_key(self):
        pointer_me = Pointer(self.BinomialTree)
        pointer_me.BinomialTree.key = self.BinomialTree.key

        while pointer_me.BinomialTree.parent != None:
            pointer_parent = pointer_me.BinomialTree.parent.pointer

            pointer_me.BinomialTree.binomialHeap = pointer_parent.BinomialTree.binomialHeap
            pointer_me.BinomialTree.key, pointer_parent.BinomialTree.key = pointer_parent.BinomialTree.key, pointer_me.BinomialTree.key
            pointer_me.BinomialTree.pointer, pointer_parent.BinomialTree.pointer = pointer_parent.BinomialTree.pointer, pointer_me.BinomialTree.pointer

            pointer_me.BinomialTree, pointer_parent.BinomialTree = pointer_parent.BinomialTree, pointer_me.BinomialTree

        self.BinomialTree = pointer_me.BinomialTree

        #self.BinomialTree.pointer = self
        pointer_me.BinomialTree.pointer = self # 필요없으면 지우기
        pointer_me.BinomialTree.pointer.BinomialTree.key = -float('inf')
        pointer_me.BinomialTree.binomialHeap.extract_min()
        pointer_me.BinomialTree.pointer.BinomialTree.key = None # 필요없으면 지우기

class BinomialTree:
    def __init__(self,key):
        self.key = key
        self.binomialHeap = None

        self.parent = None
        self.childs = []

        self.pointer = None




def attach_tree(tA,tB):
    tB.parent = tA
    tA.childs.append( tB )
    return tA

def combine_tree(tA,tB):
    if tA == None and tB == None:
        return None
    if tA == None:
        return tB
    if tB == None:
        return tA

    if tA.key >= tB.key:
        return attach_tree(tB,tA)
    else:
        return attach_tree(tA,tB)

class BinomialHeap:

    def __init__(self):
        self.BinomialTrees = [None for _ in range(72)] # 필요하면 길이 늘리기

    def insert(self,key):
        singleBinomialHeap = BinomialHeap()

        singleTree = BinomialTree(key)
        #print(key)
        newpointer = Pointer(singleTree)
        singleTree.pointer = newpointer
        #singleTree.pointer = Pointer(singleTree)

        singleBinomialHeap.BinomialTrees[0] = singleTree
        #print(self)
        self.merge(singleBinomialHeap)

        return newpointer
        #return singleTree.pointer

    def peek_min(self):
        minV = None # 틀리면 바꾸기
        headList = self.BinomialTrees
        size = len(headList)

        for i in range(size):
            if self.BinomialTrees[i] != None:
                if minV == None:
                    minV = self.BinomialTrees[i].key

                elif self.BinomialTrees[i].key < minV:
                    minV = self.BinomialTrees[i].key

        return minV

    def extract_min(self):
        #minV = self.peek_min()  # 틀리면 바꾸기
        #print(minV)
        headList = self.BinomialTrees
        size = len(headList)
        val_for_finding_idx = None
        idx_minTree = None

        for i in range(size):
            if self.BinomialTrees[i] != None:
                if val_for_finding_idx == None:
                    val_for_finding_idx = self.BinomialTrees[i].key
                    idx_minTree = i
                elif self.BinomialTrees[i].key < val_for_finding_idx:
                    val_for_finding_idx = self.BinomialTrees[i].key
                    idx_minTree = i

        if idx_minTree != None:
            decomposedTree = self.BinomialTrees[idx_minTree]
            #nums_childs = len(decomposedTree.childs)
            newBinomialHeap = BinomialHeap()

            self.BinomialTrees[idx_minTree].key = None
            self.BinomialTrees[idx_minTree] = None

            # key도 None 넣어야 하면 코드 추가하기
            #for i in range(nums_childs):
            for i in range(len(decomposedTree.childs)):
                eachChild = decomposedTree.childs[i]
                eachChild.parent = None
                newBinomialHeap.BinomialTrees[len(eachChild.childs)] = eachChild 
                # 실수 조심 이항 트리 유지하려면 위에서 사이즈별 순서로 넣어야 함

            self.merge(newBinomialHeap)

        return val_for_finding_idx


    def merge(self,bh):
        binomialTrees =  [None for _ in range(72)]  # 필요하면 길이 늘리기
        size = len(binomialTrees)
        upper = None
        for i in range(size):
            combinedTree = combine_tree(self.BinomialTrees[i],bh.BinomialTrees[i])
            #if combinedTree != None:
            #    num_childs = len(combinedTree.childs)
            if combinedTree == None:
                binomialTrees[i] = upper
                upper = None
            elif i == len(combinedTree.childs):
                if upper == None:
                    binomialTrees[i] = combinedTree
                    upper = None
                else:
                    binomialTrees[i] = None
                    upper = combine_tree(upper,combinedTree)

            elif i+1 == len(combinedTree.childs):
                binomialTrees[i] = upper
                upper = combinedTree


        self.BinomialTrees = binomialTrees

        for i in range(size):
            if self.BinomialTrees[i] != None:
                self.BinomialTrees[i].parent = None
                self.BinomialTrees[i].binomialHeap = self

        # return self


def test1():
    # Create an empty binomial heap.
    bh = BinomialHeap()
    # {} 이걸 리턴해야 되나보네??
    #print('debuging')
    # The binomial heap must return `None` when empty.
    #print(bh.extract_min())
    assert bh.extract_min() == None

    # {}
    # Insert `10` and `20` to the binomial heap.
    # Each insert operation returns a pointer to the inserted key.
    # `ptr1` corresponds to `10`; `ptr2` corresponds to `20`.
    ptr1 = bh.insert(10)
    ptr2 = bh.insert(20)
    # {10, 20}
    assert type(ptr1) == Pointer
    assert type(ptr2) == Pointer
    assert ptr1.get_key() == 10
    assert ptr2.get_key() == 20

    # The minimum key in the binomial heap is `10`.
    # Each pointer has access to its key.
    #print(bh.peek_min())
    assert bh.peek_min() == 10  # peek_min 후 변함이 없는지 보실려는듯
    # {10, 20}
    assert ptr1.get_key() == 10
    assert ptr2.get_key() == 20

    # Extract the minimum key `10` from the binomial heap.
    # `10` is no more contained in the heap.
    # `ptr1` is invalidated; an invalidated pointer loses access to the key.
    assert bh.extract_min() == 10
    # {20}
    print(ptr1.get_key())
    assert ptr1.get_key() == None
    assert ptr2.get_key() == 20

    # Extract the minimum key `20` from the binomial heap.
    # `ptr2` is invalidated.
    assert bh.extract_min() == 20
    # {}
    assert ptr1.get_key() == None
    assert ptr2.get_key() == None

    # The binomial heap must return `None` when empty.
    assert bh.extract_min() == None

def test8():
    ptrs = []
    bh = BinomialHeap()
    for _ in range(10):
        ptrs.append(bh.insert(12345))
    for i in range(10):
        print( ptrs[i].get_key() )

    for i in range(10):
        print(i)
        print( ptrs[i].get_key() )
        print( bh.extract_min() )

def test2():
    # Create an empty binomial heap.
    bh = BinomialHeap()

    # Insert ten keys into the binomial heap and collect the pointers
    # 똑같은 값 10개 넣네.
    ptrs = []
    for _ in range(10):
        ptrs.append(bh.insert(12345))

    #for i in range(10):
    #    print(bh.BinomialTrees[i].key)

    # After deleting n keys, exactly 10-n pointers will be valid.
    i=0
    for n in reversed(range(10)):
        #print(n)
        print(bh.peek_min())
        assert bh.extract_min() == 12345
        i+=1

        # Count the number of valid pointers
        cnt = 0
        for p in ptrs:  # 아 10, 9, 8...씩 유효한 포인터의 개수가 있느닞 확인하네
            #print(p.get_key())
            cnt += p.get_key() != None
        assert cnt == n


def test3():
    # Test the implimentation of `BinomialHeap` against a Python list.
    # 아 리스트에서 뺸 값이랑 힙에서 뺸 값이랑 계쏙 비교하네
    Q = 1000

    bh = BinomialHeap()
    ar = []

    for _ in range(Q):
        if random.random() < 0.7:
            # Insert with 70% of probability
            x = random.randint(10, 100)
            ar.append(x)
            bh.insert(x)
        else:
            # Extract the minimum with 30% of probabiilty
            ar.sort()
            ar.reverse()
            a = None if ar == [] else ar.pop()
            b = bh.extract_min()

            # Compare the extracted keys
            assert a == b


def test4():
    bh = BinomialHeap()
    # {}

    p1 = bh.insert(100)
    p2 = bh.insert(100)
    p3 = bh.insert(100)
    # {100, 100, 100}
    assert bh.peek_min() == 100

    p1.decrease_key(90)  # 아 포인터 당 키 감소 메서드가 있는거구나
    # {90, 100, 100}
    assert bh.peek_min() == 90
    assert p1.get_key() == 90
    assert p2.get_key() == 100
    assert p3.get_key() == 100

    p2.decrease_key(80)
    # {90, 80, 100}
    assert bh.peek_min() == 80
    assert p1.get_key() == 90
    assert p2.get_key() == 80
    assert p3.get_key() == 100

    p2.decrease_key(50)
    # {90, 50, 100}
    assert bh.peek_min() == 50
    assert p1.get_key() == 90
    assert p2.get_key() == 50
    assert p3.get_key() == 100

    assert bh.extract_min() == 50
    # {90, 100}
    assert p1.get_key() == 90
    assert p2.get_key() == None
    assert p3.get_key() == 100


def test5():
    N = 10

    # Create two binomial heaps.
    bh1 = BinomialHeap()
    bh2 = BinomialHeap()

    # Insert `N` random keys to each binomial heap. Collect the pointers.
    ptr1 = [bh1.insert(10 * N + random.random()) for _ in range(N)]
    ptr2 = [bh2.insert(10 * N + random.random()) for _ in range(N)]

    # Merge.
    bh1.merge(bh2)
    ptr1 += ptr2

    # Decrease keys to 0, 1, 2, ...... 2N-1. => 2N개 모두 키 감소 시키네
    random.shuffle(ptr1)
    for i, p in enumerate(ptr1):
        p.decrease_key(i)

    # delete 0, extract_min 1, delete 2, extract_min 3, ......
    # 짝수번째 놈 포인터로 delete, 홀수번째 놈 힙에서 추출한 뒤, 포인터로        # 비교
    for n in range(N):
        ptr1[2 * n].delete_key()
        assert bh1.extract_min() == 2 * n + 1
        assert ptr1[2 * n + 1].get_key() == None


def test6():
    N = 100  # The number of binomial heaps
    Q = 10000  # The number of test queries

    BH = [BinomialHeap() for _ in range(N)]  # 힙 오브젝 100개 만들기
    PTR = [[] for _ in range(N)]  # 거기에 맞는 포인터들
    AR = [[] for _ in range(N)]  # 거기에 맞는 배열들

    # With 79% of probability, inserts a random element to a binomial heap.
    # With  1% of probability, merges two binomial heaps.
    # With  5% of probability, extracts minimum from a binomial heap.
    # With  5% of probability, peeks minimum from a binomial heap
    # With  5% of probability, deletes a key using a pointer
    # With  5% of probability, decreases a key using a pointer

    for t in range(Q):
        assert len(BH) == len(AR)
        r = random.random()
        idx = random.randint(0, len(BH) - 1)

        if r < 0.79:
            # With 79% of probability, inserts a random element to a binomial heap.
            x = random.randint(10, 100)
            AR[idx].append(x)

            ptr = BH[idx].insert(x)
            PTR[idx].append(ptr)

        elif r < 0.80:
            # With  1% of probability, merges two binomial heaps.
            if len(BH) < 2: continue
            ar = AR.pop()
            AR[-1] += ar

            bh = BH.pop()
            BH[-1].merge(bh)  # 맨 뒤에 있는 놈들 2개 멀지하네
            p = PTR.pop()
            PTR[-1] += p

        elif r < 0.85:
            # With  5% of probability, extracts minimum from a binomial heap.
            AR[idx].sort()
            AR[idx].reverse()
            x = AR[idx].pop() if AR[idx] != [] else None
            y = BH[idx].extract_min()
            assert x == y

        elif r < 0.90:
            # With  5% of probability, peeks minimum from a binomial heap
            AR[idx].sort()
            x = AR[idx][0] if AR[idx] != [] else None
            y = BH[idx].peek_min()

            assert x == y

        elif r < 0.95:
            # With  5% of probability, deletes a key using a pointer
            while PTR[idx] != [] and PTR[idx][-1].get_key() == None:
                PTR[idx].pop()
            if PTR[idx] == []: continue

            key = PTR[idx][-1].get_key()
            assert key != None

            AR[idx].remove(key)

            PTR[idx][-1].delete_key()
            assert PTR[idx][-1].get_key() == None
            PTR[idx].pop()


        else:
            # With 5% of probability, decreases a key using a pointer
            while PTR[idx] != [] and PTR[idx][-1].get_key() == None:
                PTR[idx].pop()
            if PTR[idx] == []: continue

            key = PTR[idx][-1].get_key()
            assert key != None

            newKey = key - random.randint(10, 100)

            AR[idx].remove(key)
            AR[idx].append(newKey)

            PTR[idx][-1].decrease_key(newKey)
            assert PTR[idx][-1].get_key() == newKey


def test7():  # 이게 시간복잡도 체ㅋ ㅡ 하네
    N = 23456
    begin = time.clock()

    bh = BinomialHeap()
    ptr = []
    for _ in range(N):
        p = bh.insert(random.random())
        ptr.append(p)

    for _ in range(N):
        bh.extract_min()
        bh.insert(random.random())

    for p in ptr:
        if p.get_key() != None:
            p.delete_key()

    elapsed_time = time.clock() - begin
    assert elapsed_time < 25
    return elapsed_time


def main():
    tests = [test1, test2, test3, test4, test5, test6,test8]
    print('main')
    for i, test in enumerate(tests):
        if not (i + 1) in INCLUDE_TEST: continue
        test()
        print('Test %d passed ^_^' % (i + 1))

    test8()

    if 7 in INCLUDE_TEST:
        elapsed_time = test7()
        print('Test 7 passed ^_^ in %f seconds' % elapsed_time)


if __name__ == "__main__":
    main()

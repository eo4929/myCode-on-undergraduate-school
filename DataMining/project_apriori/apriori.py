import sys
import itertools

def getInputFile():
    #print('from getInputFile()')
    #print()

    with open(inputfile, 'r') as f:
        strings_all_transactions_list = f.read().split('\n')
        #print('strings_all_transactions_list: ',end='')
        #print(strings_all_transactions_list)

        for transaction in strings_all_transactions_list:
            transaction = transaction.split('\t')
            #print('each transaction: ',end='')
            #print(transaction)
            transactions_DB.append(transaction)

        #print('transactions_DB: ',end='')
        #print(transactions_DB)
        #print()

def makeOutputFile(associationRule_with_sup_con):
    #print('from makeOutputFile()')
    #print()

    with open(outputfile, 'a') as f:
        f.write(associationRule_with_sup_con)

    #print()

def remove_by_applying_minSup(candidateSet):
    #print('from remove_by_applying_minSup()')
    #print()
    theNum_T_forminSup = minSup * len(transactions_DB) # 25개임

    frequent_itemset = dict()
    for item in candidateSet.keys():
        if candidateSet[item] >= theNum_T_forminSup:
            frequent_itemset[item] = candidateSet[item]

    #print('frequent_itemset: ',end='')
    #print(frequent_itemset)
    

    if len(frequent_itemset) == 0:
        #print('there is no frequent_itemset')
        #print()
        exit()
    else:
        #print()
        return frequent_itemset

def first_frequent_itemset():
    #print('from first_frequent_itemset()')
    #print()

    first_items_with_Count = dict()
    for transaction in transactions_DB:
        for item in transaction:
            if item in first_items_with_Count.keys():
                first_items_with_Count[item] += 1
            else:
                first_items_with_Count[item] = 1

    #print('first_itemset(before applying minSup): ',end='')
    #print(first_items_with_Count)
    #print()

    return remove_by_applying_minSup(first_items_with_Count)

def convert_tupleEle_To_setEle(listOftuple): # combination하면 원소가 tuple임 따라서 set형태로 바꾸어 줄 필요가 있음 
    #print('from convert_tupleEle_To_setEle()')
    #print()

    #print('listOftuple :',end='')
    #print(listOftuple)

    listOfset = list()
    for element in listOftuple:
        listOfset.append( set(element) )
    
    #print('listOfset :',end='')
    #print(listOfset)
    #print()

    return listOfset

def selfJoining(size, prev_frequent_itemset):
    #print('from selfJoining()')
    #print()

    #print('size: ',end='')
    #print(size)
    #print('prev_frequent_itemset: ',end='')
    #print(prev_frequent_itemset)

    items_for_k_plus_one_Itemset = list()

    if size == 2: # 1-itemset 의미
        return convert_tupleEle_To_setEle( list( itertools.combinations(prev_frequent_itemset,size) ) )
    else:
        for itemset in prev_frequent_itemset:
            for eachitem in itemset:
                if eachitem in items_for_k_plus_one_Itemset:
                    continue
                else:
                    items_for_k_plus_one_Itemset.append(eachitem)

    #print('items_for_k_plus_one_Itemset: ',end='')
    #print(items_for_k_plus_one_Itemset)

    k_plus_one_itemset = list( itertools.combinations( items_for_k_plus_one_Itemset,size ) )
    k_plus_one_itemset = convert_tupleEle_To_setEle(k_plus_one_itemset)

    #print('k_plus_one_itemset: ',end='')
   # print(k_plus_one_itemset)
    #print()

    return k_plus_one_itemset

def pruning(size, prev_frequent_itemset, candidateSet):
    #print('from pruning()')
    #print()

    #print('size: ',end='')
    #print(size)
    #print('prev_frequent_itemset: ',end='')
    #print(prev_frequent_itemset)
    #print('candidateSet: ',end='')
    #print(candidateSet)

    potential_frequent_itemset = dict()

    if size == 2:
        new_prev_frequent_itemset = list()
        for item in prev_frequent_itemset:
            #new_prev_frequent_itemset.append( list(item) ) 디버깅 성공! 이렇게 하면  prev_frequent_itemset: [['7'], ['1', '8'], ['2'], ['4'], ['1'], ['1', '1'], ['1', '5'], ['1', '6'], ['1', '3']]
            new_prev_frequent_itemset.append( list([item]) ) # 이렇게 해야 prev_frequent_itemset: [['7'], ['18'], ['2'], ['4'], ['1'], ['11'], ['15'], ['16'], ['13']]
        prev_frequent_itemset = new_prev_frequent_itemset
        
        #print('if size == 2, prev_frequent_itemset: ',end='')
        #print(prev_frequent_itemset) # list of list 형태
    else:
        prev_frequent_itemset = convert_tupleEle_To_setEle( prev_frequent_itemset )
        #print('if size > 2, prev_frequent_itemset: ',end='')
        #print(prev_frequent_itemset)

    for itemSet in candidateSet:
        val_for_downwardClosureProperty = 0

        #print('itemSet: ',end='')
        #print(itemSet)

        subItemset = list( itertools.combinations( itemSet , size-1 ) )

        #print('subItemset: ',end='')
        #print(subItemset) # ex) subItemset: [('2',), ('1',)]

        for item in subItemset: # item은 tuple이므로
            
            if size > 2: item = set(item) # 바꿔주기
            if size == 2: item = list(item) # # 바꿔주기

            #print('item: ',end='')
            #print(item)
            if item not in prev_frequent_itemset:
                #print(item, end='')
                #print('is not in prev_frequent_itemset')
                break
            #else:
                #print(item, end='')
                #print('is in prev_frequent_itemset')
            
            val_for_downwardClosureProperty += 1

        if size == val_for_downwardClosureProperty: # 모든 subitemset이 prev_frequent_itemset에 들어있다면 그건 사용
            potential_frequent_itemset[ tuple(itemSet) ] = 0 # DB scan 전이니 일단 count 0으로 넣어둠
    
    for itemset in potential_frequent_itemset.keys():
        for transaction in transactions_DB:
            if set(itemset) <= set(transaction):
                potential_frequent_itemset[itemset] += 1
    
    #print('potential_frequent_itemset: ',end='')
    #print(potential_frequent_itemset)
    #print()

    return remove_by_applying_minSup(potential_frequent_itemset)


def associationRule(size, freItemset):
    #print('from associationRule()')
    #print()

    #print('size: ',end='')
    #print(size)
    #print('freItemset(entire dict): ',end='')
    #print(freItemset)

    for freItemset, num in freItemset.items(): # freItemset은 tuple형태
        #print('freItemset(each key): ',end='')
        #print(freItemset)
        size_of_freItemset = size

        while size_of_freItemset >= 2:
            sub_freItemset = list( itertools.combinations(freItemset, size_of_freItemset-1) )
           # print('sub_freItemset: ',end='')
            #print(sub_freItemset)

            for item in sub_freItemset:
                item = set(item)
                theRest_itemset = set(freItemset) - item # freItemset이 tuple이니 set으로 바꾸기

                #print('item: ',end='')
                #print(item)
                #print('theRest_itemset: ',end='')
                #print(theRest_itemset)

                num_item = 0
                for transaction in transactions_DB:
                    if item <= set(transaction):
                        num_item += 1
                sup = num / len(transactions_DB) * 100
                con = num / num_item * 100

                item = set( map(int, item) ) # ('1','2')에서 각 ele를 일단 int로 바꾸고 ( ) -> { } 한다음에 { } 전체를 string으로 바꿀 것임
                theRest_itemset = set( map(int, theRest_itemset) )
                associationRule_with_sup_con = str(item) + '\t' + str(theRest_itemset) + '\t' + str('%.2f' % round(sup,2)) + '\t' + str('%.2f' % round(con,2)) + '\n'

               # with open(outputfile,'a') as f:            
                f.write(associationRule_with_sup_con)
            size_of_freItemset -= 1
    #print()

def apriori():
    #print('from apriori()')
    #print()
    size = 2
    all_frequent_itemset.append( first_frequent_itemset() )

    while True:
        prev_frequent_itemset = list( all_frequent_itemset[size-1].keys() )
        
        candidateSet = selfJoining(size,prev_frequent_itemset)
        new_frequent_itemSet = pruning(size, prev_frequent_itemset, candidateSet)
        associationRule(size,new_frequent_itemSet)

        #print(len(transactions_DB))
        if len(new_frequent_itemSet) == 0:
            #print('new_frequent_itemSet is empty')
            #print()
            exit()
        else:
            all_frequent_itemset.append(new_frequent_itemSet)
            size += 1

if __name__ == '__main__':
    transactions_DB = list()

    argv = sys.argv
    minSup = argv[1]
    minSup = float(minSup) / 100
    inputfile = argv[2]
    outputfile = argv[3]
    
    getInputFile()
    all_frequent_itemset = [''] # at first, empty set
    
    candidateSet = list()
    f = open(outputfile,'w')
    apriori()
    f.close()


    #selfJoining(2,[ '1' , '2' , '3'])
    #selfJoining(3,[ {'1','2'}, {'2','3'},{'1','4'} ])

    #a = pruning(2, ['1','2','3'], selfJoining(2,['1','2','3'])  )
    #a= pruning(3, [{'1','2'},{'2','3'}], selfJoining(3,[{'1','2'},{'2','3'}]))
    
    #associationRule(2,a)

    #frequent_itemset = pruning(2, ['1','2','3'], selfJoining(2,['1','2','3'])  )
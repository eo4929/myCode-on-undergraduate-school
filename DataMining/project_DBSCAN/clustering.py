import sys
import numpy as np

def euclidian_dist(p,q):
    result = 0.0
    result += ( float(p[1]) - float(q[1]) ) ** 2
    result += ( float(p[2]) - float(q[2]) ) ** 2

    return result ** 0.5  # root 씌우는 것임

def find_list_of_neighbor(dataSet,q,eps):
    neighbors = list() # 이웃들을 모아두는 queue로 사용

    for p in range(0,len(dataSet)):
        if euclidian_dist(dataSet[p],dataSet[q]) < float(eps):
            neighbors.append(p)
    #print("q = " + str(q))
    #print(neighbors)
    return neighbors # 실수! 실제 data가 아니라 index의 리스트임

def createCluster(q,list_of_neighbor,cluster_id):
    visitInfo_byLabels[q] = cluster_id

    i=0
    while i < len(list_of_neighbor):
        p = list_of_neighbor[i]

        if visitInfo_byLabels[p] == -1:
            i+=1
            continue #일단 outlier는 버려보자 vs 현재 cluster_id에 넣기
            #visitInfo_byLabels[p] = cluster_id
        elif visitInfo_byLabels[p] == 0:
            visitInfo_byLabels[p] = cluster_id
            
            neighbors_of_p = find_list_of_neighbor(dataSet,p,eps)

            if len(neighbors_of_p) >= int(minpts):
                for r in neighbors_of_p:
                    if ( r!= q and r != p and not( r in list_of_neighbor ) and visitInfo_byLabels[r] == 0):
                        list_of_neighbor.append(r)
                        #print(len(list_of_neighbor))
                        #print("i= ",end='')
                        #print(i)
        #else:
        #    print("visitInfo_byLabels[" + str(p) + "] = ",end='')
        #    print(visitInfo_byLabels[p])
        
        i+=1

def DBSCAN(dataSet,eps,minpts):
    cluster_id = 0

    for q in range(0,len(dataSet)):
        
        if not (visitInfo_byLabels[q] == 0):
            continue
        
        list_of_neighbor = find_list_of_neighbor(dataSet,q,eps)

        if len(list_of_neighbor) < int(minpts):
            visitInfo_byLabels[q] = -1 # -1은 outlier를 의미함
        else:
            cluster_id += 1 # 새로운 cluster 만들 준비
            createCluster(q,list_of_neighbor,cluster_id)

def getInputFile():

    with open(input_file, 'r') as f:
        obj_list = f.read().split('\n')

        for obj in obj_list:
            obj = obj.split('\t')
            #print('each obj: ',end='')
            #print(obj)
            dataSet.append(obj)

        dataSet.pop()
        print()

def make_list_of_Clusters():
    i=0
    i_max = max(visitInfo_byLabels)
    one_of_cluster = list()
    
    while i<i_max:
        for q in range(0,len(dataSet)):
            if visitInfo_byLabels[q] == (i+1):
                one_of_cluster.append( dataSet[q][0] )
        
        list_of_clusters.append(one_of_cluster)
        one_of_cluster = list()
        i+=1

def sorting_cluster():
    n = len(list_of_clusters)
    for i in range(0, n - 1):
        min_idx = i
        for j in range(i + 1, n):
            if len(list_of_clusters[j]) < len(list_of_clusters[min_idx]):
                min_idx = j
        list_of_clusters[i], list_of_clusters[min_idx] = list_of_clusters[min_idx], list_of_clusters[i]

def makeOutputFile():
    frontName = input_file.split('.')
    frontName = frontName[0]

    if len(list_of_clusters) < int(n):
        j=0
        for each_cluster in list_of_clusters:
            outputfile = frontName + '_cluster_' + str(j) + '.txt'
            with open(outputfile, 'a') as f:
                for obj in each_cluster:
                    f.write( obj + '\n' )
            j+=1
        return

    i=0
    while i<int(n):
        outputfile = frontName + '_cluster_' + str(i) + '.txt'
        each_cluster = list_of_clusters.pop()
        with open(outputfile, 'a') as f:
            for obj in each_cluster:
                f.write( obj + '\n' )
        i+=1
    print()

if __name__ == '__main__':
    argv = sys.argv

    input_file = argv[1]
    if input_file == 'input1.txt':
        print('if inputed data is input1.txt, it takes a while time ')
    n = argv[2] #n은 cluster의 개수
    eps = argv[3]
    minpts = argv[4]

    dataSet = list()
    getInputFile()

    visitInfo_byLabels = [0] * len(dataSet) # 0은 아직 unvisited된 point

    DBSCAN(dataSet,eps,minpts)

    #print(visitInfo_byLabels)

    list_of_clusters = list()
    make_list_of_Clusters()
    sorting_cluster()

    makeOutputFile()
import numpy as np
import math

def norm(vector):
    if isinstance(vector,float)==1:
        return vector
    sum = 0
    for i in range(len(vector)):
        sum = sum + math.pow(vector[i],2)
    sum = math.sqrt(sum)
    return sum

def K_means(PureData,K):
    #输入是不带标签的纯数据 输出是带有类别标签的数据
    #k_means算法适用于每个聚类的先验概率相同，各维变量独立，且各维变量服从同样的正态分布
    #K表示分成K类
    if len(PureData)==1:
        return
    # SumVector = PureData[0]
    # for i in range(1,len(PureData)):
    #     SumVector = SumVector + PureData[i]

    mindata = []
    maxdata = []
    for i in range(len(PureData[0])):
        mindata.append(PureData[0][i])
        maxdata.append(PureData[0][i])
    for i in range(1,len(PureData)):
        for j in range(len(PureData[i])):
            if PureData[i][j]<mindata[j]:
                mindata[j] = PureData[i][j]
            if PureData[i][j]>maxdata[j]:
                maxdata[j] = PureData[i][j]

    randommatrix = np.random.rand(K,len(PureData[0]))

    #随机生成K个聚类中心点
    CentersOfmeans = []
    for i in range(K):
        Centerthis = []
        for j in range(len(PureData[0])):
            Centerthis.append(mindata[j]+randommatrix[i][j]*(maxdata[j]-mindata[j]))
        CentersOfmeans.append(Centerthis)

    dataContainDy = [] #带有分类标签的数据
    for i in range(len(PureData)):
        dist = []
        for j in range(len(CentersOfmeans)):
            dist.append(norm(CentersOfmeans[j]-PureData[i])) #这里可能出错，需要验证一下 就是涉及到list相减的问题
        Dythis = dist.index(min(dist))
        dataContainDy.append([PureData[i],Dythis])

    CountVisit = [] #记录对应种类的访问次数
    for i in range(K):
        CountVisit.append(0)
    for i in range(len(dataContainDy)):
        variety = dataContainDy[i][1]
        if CountVisit[variety]==0:
            CentersOfmeans[variety] = dataContainDy[i][0]
        else:
            CentersOfmeans[variety] = CentersOfmeans[variety] + dataContainDy[i][0]
        CountVisit[variety] = CountVisit[variety] + 1

    for i in range(len(CentersOfmeans)):
        if CountVisit[i]!=0: #如果一次都没有访问到的话 聚类中心不变
            CentersOfmeans[i] = CentersOfmeans[i]/CountVisit[i]





    return
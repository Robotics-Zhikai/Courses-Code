import numpy as np
import matplotlib.pyplot as plt
import abnormalDisp
from mpl_toolkits.mplot3d import Axes3D
import os
import math

def GenerateGauseData(mean,cov,num,dim):
    if dim>=2:
        # print(cov)
        data = np.random.multivariate_normal(mean,cov,num)
    elif dim==1:
        # print(cov)
        # print(num)
        data = np.random.normal(mean,cov[0],num)
    return data

def ShowData(data):
    if len(data)==0:
        return

    if isinstance(data[0][0],float):
        dim = 1
    elif len(data[0][0])>1:
        dim = len(data[0][0])

    # colors = ['b','g','r','k','c','m','y','#e24fff','#524C90','#845868']
    colors = ['b','g','r','k','c','m','y','#524C90','#845868']
    nums = ['o','v','^','8','s','P','*','+','X','1']

    variety = [] #储存类别，一共可以可视化len(colors)*len(nums)多种类别 还可以继续扩展
    for i in range(len(colors)*len(nums)):
        variety.append(0)
    k = 0
    for i in range(len(nums)):
        for j in range(len(colors)):
            variety[k] = [colors[j],nums[i]]
            k = k + 1

    fig = plt.figure()
    if dim == 1:
        minele = data[0][0]
        maxele = data[0][0]
        for i in range(len(data)):
            plt.text(data[i][0],1.5,variety[data[i][1]][1],color=variety[data[i][1]][0],fontdict={'weight': 'bold', 'size': 9})
            if minele > data[i][0]:
                minele = data[i][0]
            if maxele < data[i][0]:
                maxele = data[i][0]
        plt.axis([minele-5,maxele+5,0,3])
    elif dim == 2:
        minelex = data[0][0][0]
        maxelex = data[0][0][0]
        mineley = data[0][0][1]
        maxeley = data[0][0][1]
        for i in range(len(data)):
            # print(variety[data[i][1]][1])
            # print(variety[data[i][1]][0])
            plt.text(data[i][0][0],data[i][0][1],variety[data[i][1]][1],color=variety[data[i][1]][0],fontdict={'weight': 'bold', 'size': 9})
            if minelex > data[i][0][0]:
                minelex = data[i][0][0]
            if maxelex < data[i][0][0]:
                maxelex = data[i][0][0]
            if mineley > data[i][0][1]:
                mineley = data[i][0][1]
            if maxeley < data[i][0][1]:
                maxeley = data[i][0][1]
        plt.axis([minelex,maxelex,mineley,maxeley])
    elif dim>=3:
        maxtype = 0
        for i in range(len(data)):
            if data[i][1] > maxtype:
                maxtype = data[i][1]

        datatype = []
        for i in range(maxtype+1):
            datatype.append([])
        for i in range(len(data)):
            for j in range(maxtype+1):
                if data[i][1] == j:
                    datatype[j].append(data[i][0])
                    break

        ax = fig.add_subplot(111, projection='3d')
        for i in range(len(datatype)):
            datathis = datatype[i]
            xs = []
            ys = []
            zs = []
            for j in range(len(datathis)):
                xs.append(datathis[j][0])
                ys.append(datathis[j][1])
                zs.append(datathis[j][2])
            ax.scatter(xs,ys,zs,c=variety[i][0],marker=variety[i][1])
            ax.set_xlabel('X Label')
            ax.set_ylabel('Y Label')
            ax.set_zlabel('Z Label')
        if dim>3:
            print("维度大于3了,图片不能完整的反应实貌，因此只取前三个维度")
    else:
        abnormalDisp.error("不支持0维的情况")

    return

def RandGenerateGMMalpha(numM):
    fenge = np.random.rand(1,numM-1)
    fenge.sort()
    fenge = fenge[0]
    alpharesult = []

    last = 0
    for i in range(numM-1):
        alpharesult.append(fenge[i]-last)
        last = fenge[i]
    alpharesult.append(1-last)
    # RangeMax = 1
    # SUM = 0
    # for i in range(numM-1):
    #     randnum = RangeMax * np.random.rand(1,1)
    #     SUM = SUM + randnum
    #     alpharesult.append(randnum)
    #     RangeMax = 1 - SUM
    # alpharesult.append(1-SUM)
    return alpharesult

def AverageGenerateGMMalpha(numM):
    alpharesult = []
    for i in range(numM):
        alpharesult.append(1/numM)
    return alpharesult

def RandGenerateGMMtheta(numM,dim,meanRange,CovRange):
    #随机生成GMM的模型参数 numM代表高斯分量的数量 dim代表数据维度 meanRange代表平均值向量元素的取值范围 CovRange代表协方差矩阵元素的取值范围
    out = []
    for i in range(numM):
        meanthis = meanRange[0] + (meanRange[1]-meanRange[0]) * np.random.rand(1,dim)
        covthis = CovRange[0] * np.identity(dim) + (CovRange[1]-CovRange[0]) * np.random.rand(dim,dim)
        out.append([meanthis,covthis])
    return out

def RandGenerateGMMthetaWithIndividualVariable(numM,dim,meanRange,CovRange):
    #部分随机生成GMM的模型参数 numM代表高斯分量的数量 dim代表数据维度 meanRange代表平均值向量元素的取值范围 Covrange代表各变量方差的取值范围
    #需要注意的是，各维变量统计无关，是相互独立的
    out = []
    for i in range(numM):
        meanthis = meanRange[0] + (meanRange[1]-meanRange[0]) * np.random.rand(1,dim)
        covthis = CovRange[0] * np.identity(dim) + (CovRange[1]-CovRange[0]) * np.random.rand(dim,dim)
        for j in range(dim):
            for k in range(dim):
                if j!=k:
                    covthis[j][k]=0
        out.append([meanthis,covthis])
    return out

def RandMeanGenerateGMMthetaWithIndividualVariable(numM,dim,meanRange,Cov):
    #随机生成GMM的模型参数 numM代表高斯分量的数量 dim代表数据维度 meanRange代表平均值向量元素的取值范围 Cov代表各变量的方差值
    #需要注意的是，各维变量统计无关，是相互独立的，仅平均值随机分布
    out = []
    for i in range(numM):
        meanthis = meanRange[0] + (meanRange[1]-meanRange[0]) * np.random.rand(1,dim)
        covthis = Cov * np.identity(dim)
        out.append([meanthis,covthis])
    return out

def GenrateGMMdata(alpha,theta,numdata,dim):
    #模型参数为alpha和theta的GMM模型，生成numdata个数据，dim为每个数据的维度
    #返回值为numdata个List，最后一列带有是由哪个子高斯模型生成的
    sumalpha = 0
    accumalpha = []
    accumalpha.append(0)
    for i in range(len(alpha)):
        sumalpha = sumalpha + alpha[i]
        accumalpha.append(sumalpha)
    if math.fabs(sumalpha-1)>0.001:
        abnormalDisp.error("alpha的求和必须为1")

    data = []
    for i in range(numdata):
        randnum = np.random.rand(1,1)
        category = -1
        for j in range(len(accumalpha)-1):
            if randnum>=accumalpha[j] and randnum<accumalpha[j+1]:
                category = j
                break
        if category == -1:
            abnormalDisp.error("程序出错")
        # print(theta[category][0][0])
        # print(theta[category][1])
        datatmp1 = GenerateGauseData(theta[category][0][0],theta[category][1],1,dim)
        datatmp1 = datatmp1[0]
        datatmp1 = [datatmp1,category]
        data.append(datatmp1)
    return data

def ExtractPureData(DataContainDy):
    #从带有标签类别的数据中提取出纯数据，即把标签去掉
    PureData = []
    for i in range(len(DataContainDy)):
        PureData.append(DataContainDy[i][0])
    return PureData

# GenrateGMMdata([0.1,0.3,0.4,0.2],[[[1],[3]],[[1],[3]],[[1],[3]],[[1],[3]]],10,1)
# data1 = GenerateGauseData([0],[1],280,1)
# data1 = GenerateGauseData([0,2],[[1,0],[0,1]],280,2)
# data2 = GenerateGauseData([3,2],[[1,0],[0,1]],208,2)
#
# data = np.append(data1,data2,0)
#
# ShowData(data)
import numpy as np
import classData
import matplotlib.pyplot as plt
import ParEstAlgorithm
import math
import coverage

dim = 2
numM = 10
meanRange = [0,25]
CovRange = [0,1]
Cov = 0.5
numdata = 450
np.random.seed(1) #使得每次都随机都一样
# alpha = classData.RandGenerateGMMalpha(numM) #随机决定alpha的值，alpha的各元素之和应为1
alpha = classData.AverageGenerateGMMalpha(numM) #平均决定alpha的值


theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)
# theta = classData.RandGenerateGMMtheta(numM, dim, meanRange, CovRange)
# theta = classData.RandGenerateGMMthetaWithIndividualVariable(numM,dim,meanRange,CovRange)
OriginData = classData.GenrateGMMdata(alpha, theta, numdata, dim)

PureData = classData.ExtractPureData(OriginData)

classData.ShowData(OriginData)

classData.ShowData(ParEstAlgorithm.K_means(PureData,numM))

plt.show()

# print(OriginData[0][0][1])
print(alpha)


# a = [[0,2],[[2,3],[3,4]]]
# print(a[1][0][0])
# print(a[1,0][0]) #这样是不可以的 注意与MATLAB进行区分
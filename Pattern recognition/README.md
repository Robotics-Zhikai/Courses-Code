# HIT计算机科学与技术学院学位核心课——模式识别
abnormalDisp.py 出现异常时的显示函数 error warning

## 1 数据的生成和数据可视化
classData.py 包含数据的生成和可视化函数
### 
考虑到Gauss-Mixture-Model(GMM)可以看作是一种通用的概率密度函数，当子高斯密度函数的数量足够大时，GMM可以任意精度逼近任意分布的密度函数，包括GMM样本产生的正过程和已知GMM样本推测模型的逆过程。

dim表示采样样本的维度；numM表示GMM模型的子高斯模型个数；meanRange表示GMM模型中各子高斯模型的均值向量元素的分布范围；CovRange表示GMM模型中各子高斯模型的协方差矩阵；Cov表示各维变量相互独立时的方差；numdata表示GMM生成的数据个数；alpha是dim维变量表示各子高斯模型占比；theta是numM维变量，每个元素包括dim维均值和dim*dim维协方差矩阵

###
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig1.png)

dim = 1;numM = 4;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.RandGenerateGMMalpha(numM)

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig2.png)

dim = 2;numM = 4;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.RandGenerateGMMalpha(numM)

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig3.png)

dim = 2;numM = 4;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.AverageGenerateGMMalpha(numM)

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig4.png)

dim = 2;numM = 4;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.AverageGenerateGMMalpha(numM)

theta = classData.RandGenerateGMMtheta(numM, dim, meanRange, CovRange)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig5.png)

dim = 2;numM = 4;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.AverageGenerateGMMalpha(numM)

theta = classData.RandGenerateGMMthetaWithIndividualVariable(numM,dim,meanRange,CovRange)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig6.png)

dim = 3;numM = 4;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.RandGenerateGMMalpha(numM)

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig7.png)

dim = 2;numM = 20;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.RandGenerateGMMalpha(numM)

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig8.png)

dim = 5;numM = 20;meanRange = [0,20];CovRange = [0,1];Cov = 0.5;numdata = 250

alpha = classData.RandGenerateGMMalpha(numM)

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

在这个问题中，dim=5，超出了三维绘制的维度，因此只取前三个维度进行绘制

## 2 概率密度函数的参数估计（Parameter estimation algorithm）
ParEstAlgorithm.py 存放参数估计算法 如k-means EM等
### 2.1 K-means算法
#### 2.1.1 自定义随机数据测试

demo.py 为如何运行classData.py和ParEstAlgorithm.py的demo文件

数据生成方法：

dim = 2；numM = 5；meanRange = [0,40]；CovRange = [0,1]；Cov = 0.5；numdata = 450

alpha = classData.AverageGenerateGMMalpha(numM) #平均决定alpha的值

theta = classData.RandMeanGenerateGMMthetaWithIndividualVariable(numM, dim, meanRange, Cov)

下述各图中，左子图为原始聚类数据，右子图为估计的聚类数据

一、聚类的初始中心值是随机在数据集中选取了numM个点

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig10.png)

经过聚类得这样的结果，可以发现聚类效果非常好

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig11.png)

同样的参数不同的聚类初始点，发现聚类效果不行，陷入了局部最优而不是全局最优

二、聚类的初始中心值不在数据集中选取，而是在整个取值空间中随机选取

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig12.png)

可以看到，此种情形时，有可能出现聚类不足的情况，即本来应该聚5类，但是却聚成了3类，这是因为某些初始聚类中心离样本点太远，导致没办法归类为该类

修改meanRange = [0,10]

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig13.png)

当把数据集中的样本点数增加到1450时，发现聚类效果非常好，可以发现聚类效果会随样本点数的增加而变好

三、同样数据集不同的初始聚类取法（一或二）的效果对比

数据集中样本点数为1450

一的取法，得到的结果图为：

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig15.png)

二的取法，得到的结果图为：

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig14.png)

可以看到，二的取法中估计的聚类数量小于实际的聚类数量

四、用“一”中所示方法进行初始聚类中心的选取，改变样本数量的效果对比

numdata = 150

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig16.png)

numdata = 250

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig17.png)

numdata = 450

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig18.png)

numdata = 1450

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig19.png)

numdata = 2450

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig20.png)

可以发现，数据越多，分类效果越好

numM = 10 meanRange = [0,25]

numdata = 2450

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig21.png)

numdata = 3450

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig22.png)

numdata = 6450

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig23.png)

#### 2.1.2 MNIST数据集测试
数据集下载：
链接：https://pan.baidu.com/s/1HSRvGdMrhbMz4IfKjEvOIg 
提取码：o92l 

MNISTClassify.py为数据集分类的测试函数

MNIST数据集测试：ClusterSamples中的10000个784维特征手写数字样本聚类为10个类别，根据SampleLabels中的标签统计每个聚类中不同样本的数量。测试不同初始值对聚类结果的影响。

各种类具有的样本个数：

[960, 1126, 994, 990, 959, 941, 979, 1058, 979, 1014]

五次不同初始聚类中心下的实验，聚类中心是随机在数据集中选取的10个784维值：

估计的各类别正确的数目：

[2, 0, 1, 28, 490, 308, 50, 53, 549, 3]

[114, 0, 10, 42, 260, 23, 21, 51, 33, 13]

[335, 3, 40, 32, 504, 17, 79, 5, 17, 7]

[2, 417, 89, 33, 4, 5, 0, 104, 16, 19]

[2, 0, 17, 27, 4, 255, 67, 0, 69, 1]

估计的各类别正确率（单位/%）：

[0.208, 0.0, 0.101, 2.828, 51.095, 32.731, 5.107, 5.009, 56.078, 0.296]

[11.875, 0.0, 1.006, 4.242, 27.112, 2.444, 2.145, 4.82, 3.371, 1.282]

[34.896, 0.266, 4.024, 3.232, 52.555, 1.807, 8.069, 0.473, 1.736, 0.69]

[0.208, 37.034, 8.954, 3.333, 0.417, 0.531, 0.0, 9.83, 1.634, 1.874]

[0.208, 0.0, 1.71, 2.727, 0.417, 27.099, 6.844, 0.0, 7.048, 0.099]

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Pattern%20recognition/image/fig9.png)

上图绘制了5次实验的正确率，正确率理想的最高值为百分之百，对应于图中是100；发现k-means算法对于该数据的聚类的正确率不高

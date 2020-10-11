# HIT计算机科学与技术学院学位核心课——模式识别
## 数据的生成和数据可视化

### 
考虑到Gauss-Mixture-Model(GMM)可以看作是一种通用的概率密度函数，当子高斯密度函数的数量足够大时，GMM可以任意精度逼近任意分布的密度函数，包括GMM样本产生的正过程和已知GMM样本推测模型的逆过程。

classData.py 包含数据的生成和可视化函数

classDataDEMO.py 为如何运行classData.py的demo文件

abnormalDisp.py 出现异常时的显示函数 error warning

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

## 概率密度函数的参数估计（Parameter estimation algorithm）


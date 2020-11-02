# Numerical Analysis
# 工大研究生课程数值分析代码
## 非线性方程求解
涉及到二分法、牛顿法（单点迭代）、割线法（多点迭代），拟牛顿法（秩为1的Broydon方法），牛顿下山法

MATLAB eval函数的使用，以及批量变量的生成方法，迭代函数确定后可以进行单点迭代和任意多点迭代，数值求解f(x)=0 F(X)=0 的解

批量变量的生成方法：
https://blog.csdn.net/qq_40728599/article/details/109142354

## 线性方程组的求解
利用高斯消元法和列主元高斯消去法，后者适用于系数矩阵某些元素为极小量的时候，主要原因是当除以一个极小量时返回一个极大量可能会有截断误差

## 龙格现象的产生和克服
涉及到拉格朗日插值、分段线性插值、为了克服龙格现象的三次样条插值（第一边界条件、第二边界条件、周期边界条件）。在三次样条插值的求解过程中，涉及到第二部分线性方程组的求解，进行了bug修复。编写了基于递归的任意阶差商的计算函数。以下是实验结果：

采样点N = 15

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/7.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/10.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/8.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/11.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/9.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/12.png)

采样点N = 50

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/1.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/4.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/2.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/5.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/3.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/6.png)

可以发现，在三次样条插值中均匀的采样数据点效果好于随机的采样数据点；对于均匀的采样数据来说，增加采样点的个数三次样条插值的效果没有明显改善，但是对随机采样数据来说，增加采样点的个数三次样条插值的效果有明显改善；对于拉格朗日插值来说，均匀采样的数据点效果好于随机采样的数据点，均有龙格现象；对于均匀采样的数据来说，随着采样点数的增加，龙格现象愈发明显。
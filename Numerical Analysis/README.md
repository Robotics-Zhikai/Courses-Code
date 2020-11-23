# Numerical Analysis
检索词：哈工大数值分析 哈工大 数值分析 数值分析哈工大 哈尔滨工业大学

哈尔滨工业大学硕士研究生学位课——数值分析。包含实验代码和期末复习总结
# 目录
* 1. [实验](#实验)
  * 1.1 [非线性方程求解](#非线性方程求解)
  * 1.2 [线性方程组的求解](#线性方程组的求解)
  * 1.3 [龙格现象的产生和克服](#龙格现象的产生和克服)
  * 1.4 [曲线拟合的最小二乘法](#曲线拟合的最小二乘法)
  * 1.5 [龙贝格积分法](#龙贝格积分法)
  * 1.6 [常微分方程初值问题的数值解法](#常微分方程初值问题的数值解法)

* 2. [期末复习总结](#期末复习总结)
  * 2.0 [数学基础](#数学基础) 
  * 2.1 [非线性方程(组)的数值解法](#非线性方程(组)的数值解法) 
  * 2.2 [线性方程(组)的数值解法](#线性方程(组)的数值解法)
  * 2.3 [插值法与数值逼近](#插值法与数值逼近)
  * 2.4 [数值积分](#数值积分)
  * 2.5 [矩阵特征值与特征向量的计算](#矩阵特征值与特征向量的计算)
  * 2.6 [常微分方程数值解法](#常微分方程数值解法)

# 实验
## 非线性方程求解
涉及到二分法、牛顿法（单点迭代）、割线法（多点迭代），拟牛顿法（秩为1的Broydon方法），牛顿下山法

MATLAB eval函数的使用，以及批量变量的生成方法，迭代函数确定后可以进行单点迭代和任意多点迭代，数值求解f(x)=0 F(X)=0 的解

批量变量的生成方法：
https://blog.csdn.net/qq_40728599/article/details/109142354

## 线性方程组的求解
利用高斯消元法和列主元高斯消去法，后者适用于系数矩阵某些元素为极小量的时候，主要原因是当除以一个极小量时返回一个极大量可能会有截断误差

## 龙格现象的产生和克服
涉及到拉格朗日插值、分段线性插值、为了克服龙格现象的三次样条插值（第一边界条件、第二边界条件、周期边界条件）。在三次样条插值的求解过程中，涉及到第二部分线性方程组的求解，进行了bug修复。编写了基于递归的任意阶差商的计算函数。以下是实验结果：

结果中，蓝色曲线为原函数，1/(1+25*x^2),红色曲线为对应的插值函数，圆圈为采样节点。

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

## 曲线拟合的最小二乘法
实现的是工大版数值分析原理P166页的递推式

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/13.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/14.png)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/15.png)

这是对一些点的拟合，发现效果不是特别好，只在数据量少的时候有用。当数据非线性明显且分布范围广泛时效果非常差。

## 龙贝格积分法
暂时无法处理被积函数无界的情况。用到了try catch 以对被积函数的某一点无意义作相应处理

## 常微分方程初值问题的数值解法
四阶龙格库塔法和预测矫正法

# 期末复习总结
第一次上传这些图片时由于手机拍照时姿势不对，图片有自动旋转，造成上传GitHub之后图片方向不对，重新按照相同的姿势拍照使得图片都朝一个方向再重新上传就解决了方向不对这个问题。
## 数学基础
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image1.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image2.jpeg)

## 非线性方程(组)的数值解法
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image3.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image4.jpeg)

## 线性方程(组)的数值解法
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image5.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image6.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image7.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image7.5.jpg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image8.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image9.jpeg)

## 插值法与数值逼近
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image10.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image11.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image12.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image13.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image14.jpeg)

## 数值积分
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image15.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image16.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image17.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image18.jpeg)

## 矩阵特征值与特征向量的计算
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image19.jpeg)

## 常微分方程数值解法
![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image20.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image21.jpeg)

![image](https://github.com/Robotics-Zhikai/Courses-Code/blob/master/Numerical%20Analysis/images/image22.jpeg)
clc
clear
close all 
%%
VarNum = 10;%生成变量的个数
vars = []; 
for i = 1:VarNum
    vars{1,i} = ['x',num2str(i-1)]; %批量生成变量，变量名为x0,x1,x2,......
    vars{2,i} = i;
    eval([vars{1,i},'=','vars{2,i};']);
end


% Seq = IteratedFunc(Func_str,InitValue,InterationNum,ERROR)
Seq = IteratedFunc('(exp(-x))/2',0.5,15);
Seq = IteratedFunc('(cos(x)+sin(x))/4',10,15);
Seq = IteratedFunc('x-(4-2^x-x)/(-log(2)*2^x-1)',10,152);
Seq = IteratedFunc('x1-(x1*exp(x1)-1)/(x1*exp(x1)-x0*exp(x0))*(x1-x0)',[0.5,0.6],10)

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第一部分实验 非线性方程求解
BinarySolve('sin(x)-x^2/2',[1,2],0.5e-5)
BinarySolve('sin(x)-x^2/2',[1,2],0.5e-5)

NewtonSolve('x*exp(x)-1',110.5,10e-5)
NewtonSolve('x^3-x-1',1,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.45,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.65,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.85,10e-8) %说明不同的初值会收敛到不同的解

SecantNewtonSolve('(x-1)^2*(2*x-1)',0.4,0.6,0.001) %割线法求方程的解

BroydonRank_1_Solve([{'x0*x1-x2^2-1'},{'x0*x1*x2+x1^2-x0^2-2'},{'exp(x0)+x2-exp(x1)-3'}],[1,1,1],10e-5)
BroydonRank_1_Solve([{'4*x0^2+x1^2+2*x0*x1-x1-2'},{'2*x0^2+3*x0*x1+x1^2-3'}],[1,2],0.5*10e-5)

NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.55,1.2,10e-5) %通过修改下山因子可以改变收敛性
NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.7,2,10e-5) %通过修改下山因子可以改变收敛性
% NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.55,2,10e-5)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第二部分实验 高斯列主元消去法
A1 = [1e-8 2 3;-1 3.712 4.623;-2 1.072 5.643];
b1 = [1;2;3];
A2 = [4,-2,4;-2,17,10;-4,10,9];
b2 = [10;3;7];
x1 = GaussianElimination(A1,b1)
x2 = GaussianElimination(A2,b2)

x1Column = GaussianColumnElimination(A1,b1)
x2Column = GaussianColumnElimination(A2,b2)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第三部分实验 龙格现象的产生和克服
fucStr = '1/(1+25*x^2)';
qujian = [-1,1];

N = 15;
PointsUniform = UniformSamplefunction(fucStr,qujian,N);
PointsRandom = RandomSamplefunction(fucStr,qujian,N);

Points = PointsRandom;
figure
% plot(Points(1,:),Points(2,:),'-');
% hold on 
PlotFuc(fucStr,qujian,'b-');
hold on
[Outn_x,ValueList] = LagrangeInterpolation(Points,N-1);
plot(qujian(1):0.01:qujian(2),subs(Outn_x,qujian(1):0.01:qujian(2)),'.');
hold on 
plot(ValueList(1,:),ValueList(2,:),'o');
axis([-1,1,-4,4])
title('随机采样数据;拉格朗日插值;.为拉格朗日插值函数;。为用到的插值点')

figure
LinearInterpolationresult = PieceLinearInterpolation(Points);
stepplot = 0.001;
PlotMultiSubFuc(LinearInterpolationresult,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
hold on 
plot(Points(1,:),Points(2,:),'o');
hold on 
title('随机采样数据;分段线性插值')

figure
subplot(131)
result1 = ThreeMomentInterpolation(Points,'first',[0.0740 -0.0740]);
PlotMultiSubFuc(result1,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('随机采样数据;第一边界条件的三弯矩插值')

subplot(132)
result2 = ThreeMomentInterpolation(Points,'second',[1 1]);
PlotMultiSubFuc(result2,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('随机采样数据;第二边界条件的三弯矩插值')

subplot(133)
result3 = ThreeMomentInterpolation(Points,'third');
PlotMultiSubFuc(result3,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('随机采样数据;周期边界条件的三弯矩插值')



Points = PointsUniform;
figure
% plot(Points(1,:),Points(2,:),'-');
% hold on 
PlotFuc(fucStr,qujian,'b-');
hold on
[Outn_x,ValueList] = LagrangeInterpolation(Points,N-1);
plot(qujian(1):0.01:qujian(2),subs(Outn_x,qujian(1):0.01:qujian(2)),'.');
hold on 
plot(ValueList(1,:),ValueList(2,:),'o');
axis([-1,1,-4,4])
title('均匀采样数据;拉格朗日插值;.为拉格朗日插值函数;。为用到的插值点')

figure
LinearInterpolationresult = PieceLinearInterpolation(Points);
stepplot = 0.001;
PlotMultiSubFuc(LinearInterpolationresult,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('均匀采样数据;分段线性插值')

figure
subplot(131)
result1 = ThreeMomentInterpolation(Points,'first',[0.0740 -0.0740]);
PlotMultiSubFuc(result1,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('均匀采样数据;第一边界条件的三弯矩插值')

subplot(132)
result2 = ThreeMomentInterpolation(Points,'second',[1 1]);
PlotMultiSubFuc(result2,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('均匀采样数据;第二边界条件的三弯矩插值')

subplot(133)
result3 = ThreeMomentInterpolation(Points,'third');
PlotMultiSubFuc(result3,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('均匀采样数据;周期边界条件的三弯矩插值')

syms x;
y = 1/(1+25*x^2);
ytaylor = taylor(y,'order',151);
range = [-0.2,0.2];
k = 1;
ynum = [];
for i = range(1):0.01:range(2)
    x = i;
    ynum(1,k) = eval(ytaylor);
    k = k+1;
end
 figure,plot(range(1):0.01:range(2),ynum,'-')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第四部分 曲线拟合的最小二乘法
x = [3 4 5 6 7 8 9];
fx = [2.01 2.98 3.50 5.02 5.47 6.02 7.05];
Data = [x;fx];
[fitFuc,Error] = CurveFitLeastSquareApproximation(Data);
figure
plot(x,fx,'.');
hold on 
X = min(Data(1,:)):0.1:max(Data(1,:));
plot(X,subs(fitFuc,X),'-');


Data = [];
Data(1,:) = 0:0.1:1; %似乎只有小范围内的 数据量小的 用实现的最小二乘结果才像那么回事
Data(2,:) = 3*Data(1,:).^3+4*Data(1,:).^4;
[fitFuc,Error] = CurveFitLeastSquareApproximation(Data);
figure
plot(Data(1,:),Data(2,:),'.');
hold on 
X = min(Data(1,:)):0.1:max(Data(1,:));
plot(X,subs(fitFuc,X),'-');

%效果差
% Data = [];
% Data(1,:) = 0:0.1:2;
% Data(2,:) = exp(Data(1,:));
% [fitFuc,Error] = CurveFitLeastSquareApproximation(Data)
% figure
% plot(Data(1,:),Data(2,:),'.');
% hold on 
% X = min(Data(1,:)):0.1:max(Data(1,:));
% plot(X,subs(fitFuc,X),'-');


Data = [0 0.25 0.5 0.75 1;0.1 0.35 0.81 1.09 1.96];
[fitFuc,Error] = CurveFitLeastSquareApproximation(Data);
figure
plot(Data(1,:),Data(2,:),'.');
hold on 
X = min(Data(1,:)):0.1:max(Data(1,:));
plot(X,subs(fitFuc,X),'-');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第五部分 龙贝格积分法
fucstr = 'x^3';
qujian = [6,100];
n = 10;
Trapezoid_Integer = double(Multi_Trapezoid_Integer(fucstr,n,qujian))
Simpson_Integer = double(Multi_Simpson_Integer(fucstr,n,qujian)) 
Cotes_Integer = double(Multi_Cotes_Integer(fucstr,n,qujian))
[Romberg_Integervalue,T] = Romberg_Integer(fucstr,qujian,1e-15)

fucstr = '(sin(x))/x';
qujian = [0,1];
n = 10;
Trapezoid_Integer = double(Multi_Trapezoid_Integer(fucstr,n,qujian))
Simpson_Integer = double(Multi_Simpson_Integer(fucstr,n,qujian)) 
Cotes_Integer = double(Multi_Cotes_Integer(fucstr,n,qujian))
[Romberg_Integervalue,T] = Romberg_Integer(fucstr,qujian,1e-15)

fucstr = 'x^2';
qujian = [0,1];
n = 10;
Trapezoid_Integer = double(Multi_Trapezoid_Integer(fucstr,n,qujian))
Simpson_Integer = double(Multi_Simpson_Integer(fucstr,n,qujian)) 
Cotes_Integer = double(Multi_Cotes_Integer(fucstr,n,qujian))
[Romberg_Integervalue,T] = Romberg_Integer(fucstr,qujian,1e-15)

% fucstr = '1/x';
% qujian = [-4,5];%这种目前暂时无法处理 中间有间断点处函数无界
% n = 10;
% Trapezoid_Integer = double(Multi_Trapezoid_Integer(fucstr,n,qujian))
% Simpson_Integer = double(Multi_Simpson_Integer(fucstr,n,qujian)) 
% Cotes_Integer = double(Multi_Cotes_Integer(fucstr,n,qujian))
% [Romberg_Integervalue,T] = Romberg_Integer(fucstr,qujian,1e-15)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第六部分 常微分方程初值问题的数值解法
%基本问题是dy/dx = f(x,y) y0给定
fucstr = 'x+y';
init = [0,-1];
qujian = [0,1];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = '-x-1';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20')

fucstr = '-y^2';
init = [0,1];
qujian = [0,1];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = '1/(x+1)';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20')


fucstr = '2/x*y+x^2*exp(x)';
init = [1,0];
qujian = [1,3];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = 'x^2*(exp(x)-exp(1))';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20')


fucstr = '1/x*(y^2+y)';
init = [1,-2];
qujian = [1,3];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = '2*x/(1-2*x)';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20')



fucstr = '-20*(y-x^2)+2*x';
init = [0,1/3];
qujian = [0,1];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = 'x^2+1/3*exp(-20*x)';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20')


fucstr = '-20*y+20*sin(x)+cos(x)';
init = [0,1];
qujian = [0,1];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = 'exp(-20*x+sin(x))';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20')


fucstr = '-20*(y-exp(x)*sin(x))+exp(x)*(sin(x)+cos(x))';
init = [0,0];
qujian = [0,1];
stepnum = 100;
[seqRunge_kutta5,seqPredictCorrection_PMECME_Mode5] = TestEx6(fucstr,init,qujian,5,stepnum);
[seqRunge_kutta10,seqPredictCorrection_PMECME_Mode10] = TestEx6(fucstr,init,qujian,10,stepnum);
[seqRunge_kutta20,seqPredictCorrection_PMECME_Mode20] = TestEx6(fucstr,init,qujian,20,stepnum);
figure
analyticStr = 'exp(x)*sin(x)';
subplot(231)
PlotError2Reality(analyticStr,seqRunge_kutta5) 
title('Rungekutta5')
subplot(234)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode5) 
title('PMECMEMode5')
subplot(232)
PlotError2Reality(analyticStr,seqRunge_kutta10) 
title('Rungekutta10')
subplot(235)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode10) 
title('PMECMEMode10')
subplot(233)
PlotError2Reality(analyticStr,seqRunge_kutta20)
title('Rungekutta20')
subplot(236)
PlotError2Reality(analyticStr,seqPredictCorrection_PMECME_Mode20) 
title('PMECMEMode20') 
%这应该封装个函数的 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function PlotError2Reality(analyticStr,seq) 
%输入解析式(f(x))，和通过求常微分方程数值解的序列 绘制误差图

%     plot(seq(:,1),seq(:,2),'o');
%     hold on
%     x = seq(:,1);
    y = [];
    for i = 1:size(seq,1)
        x = seq(i,1);
        y(i,1) = eval(analyticStr);
    end
    errory = y-seq(:,2);
    plot(seq(:,1),abs(errory),'-');
end

function [seqRunge_kutta,seqPredictCorrection_PMECME_Mode] = TestEx6(fucstr,init,qujian,N,stepnum)
    h = (max(qujian)-min(qujian))/N;
    seqRunge_kutta = Runge_kutta(fucstr,init,h,qujian,stepnum);
    seqPredictCorrection_PMECME_Mode = PredictCorrection_PMECME_Mode(fucstr,init,h,qujian,stepnum);
end

function PlotMultiSubFuc(result,stepplot,linetype)
%     stepplot = 0.001;
    for i = 1:size(result,2)
        X = result{1,i}(1):stepplot:result{1,i}(2);
        plot(X,subs(result{2,i},X),linetype);
        hold on 
    end
end

function PlotFuc(fucStr,qujian,linetype) %输入为[x;f(x)]序列
    fucStr = str2sym(fucStr);
    Step = (qujian(2)-qujian(1))/1000;
    X = qujian(1):Step:qujian(2);
    plot(X,subs(fucStr,X),linetype);
end




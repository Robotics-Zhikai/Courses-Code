clc
clear
close all 

Seq = IteratedFunc('(exp(-x))/2',0.5,15);
Seq = IteratedFunc('(cos(x)+sin(x))/4',10,15);
Seq = IteratedFunc('x-(4-2^x-x)/(-log(2)*2^x-1)',10,152);

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%第一部分实验 非线性方程求解
BinarySolve('sin(x)-x^2/2',[1,2],0.5e-5)
BinarySolve('sin(x)-x^2/2',[1,2],0.5e-5)

NewtonSolve('x*exp(x)-1',110.5,10e-5)
NewtonSolve('x^3-x-1',1,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.45,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.85,10e-8) %说明不同的初值会收敛到不同的解

SecantNewtonSolve('(x-1)^2*(2*x-1)',0.4,0.6,0.001) %割线法求方程的解

BroydonRank_1_Solve([{'x0*x1-x2^2-1'},{'x0*x1*x2+x1^2-x0^2-2'},{'exp(x0)+x2-exp(x1)-3'}],[1,1,1],10e-5)

NewtonDownhillSolve('(x-1)^2*(2*x-1)',0.55,1.2,10e-5)
NewtonDownhillSolve('(x-1)^2*(2*x-1)',0.55,2,10e-5)













%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
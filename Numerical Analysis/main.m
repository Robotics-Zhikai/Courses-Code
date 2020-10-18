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
NewtonSolve('(x-1)^2*(2*x-1)',0.65,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.85,10e-8) %说明不同的初值会收敛到不同的解

SecantNewtonSolve('(x-1)^2*(2*x-1)',0.4,0.6,0.001) %割线法求方程的解

BroydonRank_1_Solve([{'x0*x1-x2^2-1'},{'x0*x1*x2+x1^2-x0^2-2'},{'exp(x0)+x2-exp(x1)-3'}],[1,1,1],10e-5)

NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.55,1.2,10e-5) %通过修改下山因子可以改变收敛性
NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.7,2,10e-5) %通过修改下山因子可以改变收敛性
NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.55,2,10e-5)













%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
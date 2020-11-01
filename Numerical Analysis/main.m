clc
clear
close all 
%%
VarNum = 10;%���ɱ����ĸ���
vars = []; 
for i = 1:VarNum
    vars{1,i} = ['x',num2str(i-1)]; %�������ɱ�����������Ϊx0,x1,x2,......
    vars{2,i} = i;
    eval([vars{1,i},'=','vars{2,i};']);
end



Seq = IteratedFunc('(exp(-x))/2',0.5,15);
Seq = IteratedFunc('(cos(x)+sin(x))/4',10,15);
Seq = IteratedFunc('x-(4-2^x-x)/(-log(2)*2^x-1)',10,152);

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%��һ����ʵ�� �����Է������
BinarySolve('sin(x)-x^2/2',[1,2],0.5e-5)
BinarySolve('sin(x)-x^2/2',[1,2],0.5e-5)

NewtonSolve('x*exp(x)-1',110.5,10e-5)
NewtonSolve('x^3-x-1',1,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.45,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.65,10e-5)
NewtonSolve('(x-1)^2*(2*x-1)',0.85,10e-8) %˵����ͬ�ĳ�ֵ����������ͬ�Ľ�

SecantNewtonSolve('(x-1)^2*(2*x-1)',0.4,0.6,0.001) %���߷��󷽳̵Ľ�

BroydonRank_1_Solve([{'x0*x1-x2^2-1'},{'x0*x1*x2+x1^2-x0^2-2'},{'exp(x0)+x2-exp(x1)-3'}],[1,1,1],10e-5)

NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.55,1.2,10e-5) %ͨ���޸���ɽ���ӿ��Ըı�������
NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.7,2,10e-5) %ͨ���޸���ɽ���ӿ��Ըı�������
% NewtonDownhillSolveLamda('(x-1)^2*(2*x-1)',0.55,2,10e-5)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%�ڶ�����ʵ�� ��˹����Ԫ��ȥ��
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
%��������ʵ�� ��������Ĳ����Ϳ˷�
fucStr = '1/(1+25*x^2)';
qujian = [-1,1];
PointsUniform = UniformSamplefunction(fucStr,qujian,7);
PointsRandom = RandomSamplefunction(fucStr,qujian,20);

Points = PointsRandom;

figure
plot(Points(1,:),Points(2,:),'.');
hold on 
[Outn_x,ValueList] = LagrangeInterpolation(Points,13);
plot(qujian(1):0.01:qujian(2),subs(Outn_x,qujian(1):0.01:qujian(2)),'.');
hold on 
plot(ValueList(1,:),ValueList(2,:),'o');
axis([-1,1,-4,4])

figure
LinearInterpolationresult = PieceLinearInterpolation(Points);
stepplot = 0.001;
for i = 1:size(LinearInterpolationresult,2)
    X = LinearInterpolationresult{1,i}(1):stepplot:LinearInterpolationresult{1,i}(2);
    plot(X,subs(LinearInterpolationresult{2,i},X),'.');
    hold on 
end

ThreeMomentInterpolation(Points,'first',[0.0740 -0.0740])












%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



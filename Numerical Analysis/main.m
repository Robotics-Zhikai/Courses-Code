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


% Seq = IteratedFunc(Func_str,InitValue,InterationNum,ERROR)
Seq = IteratedFunc('(exp(-x))/2',0.5,15);
Seq = IteratedFunc('(cos(x)+sin(x))/4',10,15);
Seq = IteratedFunc('x-(4-2^x-x)/(-log(2)*2^x-1)',10,152);
Seq = IteratedFunc('x1-(x1*exp(x1)-1)/(x1*exp(x1)-x0*exp(x0))*(x1-x0)',[0.5,0.6],10)

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
BroydonRank_1_Solve([{'4*x0^2+x1^2+2*x0*x1-x1-2'},{'2*x0^2+3*x0*x1+x1^2-3'}],[1,2],0.5*10e-5)

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
title('�����������;�������ղ�ֵ;.Ϊ�������ղ�ֵ����;��Ϊ�õ��Ĳ�ֵ��')

figure
LinearInterpolationresult = PieceLinearInterpolation(Points);
stepplot = 0.001;
PlotMultiSubFuc(LinearInterpolationresult,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
hold on 
plot(Points(1,:),Points(2,:),'o');
hold on 
title('�����������;�ֶ����Բ�ֵ')

figure
subplot(131)
result1 = ThreeMomentInterpolation(Points,'first',[0.0740 -0.0740]);
PlotMultiSubFuc(result1,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('�����������;��һ�߽�����������ز�ֵ')

subplot(132)
result2 = ThreeMomentInterpolation(Points,'second',[1 1]);
PlotMultiSubFuc(result2,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('�����������;�ڶ��߽�����������ز�ֵ')

subplot(133)
result3 = ThreeMomentInterpolation(Points,'third');
PlotMultiSubFuc(result3,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('�����������;���ڱ߽�����������ز�ֵ')



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
title('���Ȳ�������;�������ղ�ֵ;.Ϊ�������ղ�ֵ����;��Ϊ�õ��Ĳ�ֵ��')

figure
LinearInterpolationresult = PieceLinearInterpolation(Points);
stepplot = 0.001;
PlotMultiSubFuc(LinearInterpolationresult,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('���Ȳ�������;�ֶ����Բ�ֵ')

figure
subplot(131)
result1 = ThreeMomentInterpolation(Points,'first',[0.0740 -0.0740]);
PlotMultiSubFuc(result1,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('���Ȳ�������;��һ�߽�����������ز�ֵ')

subplot(132)
result2 = ThreeMomentInterpolation(Points,'second',[1 1]);
PlotMultiSubFuc(result2,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('���Ȳ�������;�ڶ��߽�����������ز�ֵ')

subplot(133)
result3 = ThreeMomentInterpolation(Points,'third');
PlotMultiSubFuc(result3,stepplot,'r-')
hold on
PlotFuc(fucStr,qujian,'b-');
plot(Points(1,:),Points(2,:),'o');
hold on 
title('���Ȳ�������;���ڱ߽�����������ز�ֵ')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%���Ĳ��� ������ϵ���С���˷�
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
Data(1,:) = 0:0.1:1; %�ƺ�ֻ��С��Χ�ڵ� ������С�� ��ʵ�ֵ���С���˽��������ô����
Data(2,:) = 3*Data(1,:).^3+4*Data(1,:).^4;
[fitFuc,Error] = CurveFitLeastSquareApproximation(Data);
figure
plot(Data(1,:),Data(2,:),'.');
hold on 
X = min(Data(1,:)):0.1:max(Data(1,:));
plot(X,subs(fitFuc,X),'-');

%Ч����
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





function PlotMultiSubFuc(result,stepplot,linetype)
%     stepplot = 0.001;
    for i = 1:size(result,2)
        X = result{1,i}(1):stepplot:result{1,i}(2);
        plot(X,subs(result{2,i},X),linetype);
        hold on 
    end
end

function PlotFuc(fucStr,qujian,linetype) %����Ϊ[x;f(x)]����
    fucStr = str2sym(fucStr);
    Step = (qujian(2)-qujian(1))/1000;
    X = qujian(1):Step:qujian(2);
    plot(X,subs(fucStr,X),linetype);
end




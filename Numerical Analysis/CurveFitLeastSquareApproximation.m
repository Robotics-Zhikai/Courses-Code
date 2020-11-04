function [fitFuc,Error] = CurveFitLeastSquareApproximation(Data)
%曲线拟合的最小二乘法
%拟合的最终结果为线性组合的多项式，由泰勒定理可知任意函数都可分解为任意线性组合的多项式
%似乎只适用于小数据量，低阶的,X范围小于1的 具体是为什么就进入知识盲区了
    X = Data(1,:);
    F = Data(2,:);
    syms x;
    thou = ones(1,size(Data,2)); 
    phi = [];
    
    phi_0 = 1;
    alpha0 = Get_alpha_k(thou,X,1);
    phi_1 = (x-alpha0)*phi_0;
    a0_star = Get_ak_star(thou,X,F,phi_0);
    phi = phi_1;
    a_star = Get_ak_star(thou,X,F,phi(1));
    
    N = 10000; %最多迭代10000次
    phi_n = a0_star*phi_0;
    phi_n_store = [];
    
%     Error = a0_star*GetInnerProduct(X,F,thou,phi_0);
    for i = 2:N
        alpha_k = Get_alpha_k(thou,X,phi(i-1));
        if i-2==0
            beita_k = Get_beita_k(thou,X,phi_0,phi(1));
            phithis = (x-alpha_k)*phi(1)-beita_k*phi_0;
        else
            beita_k = Get_beita_k(thou,X,phi(i-2),phi(i-1));
            phithis = (x-alpha_k)*phi(i-1)-beita_k*phi(i-2);
        end
        a_star = [a_star Get_ak_star(thou,X,F,phi(i-1))];
        phi = [phi phithis];
        
        phi_n = phi_n + a_star(end-1)*phi(end-1);
        
%         Error = Error + a_star(end-1)+ GetInnerProduct(X,F,thou,phi(end-1));
%         sum(F.^2) - Error %平方误差不知道怎么实现
        Error = sum((subs(phi_n,X)-F).^2);
        double(Error)
        if isnan(Error)
            break;
        end
        phi_n_store = [phi_n_store;[phi_n Error]];
    end
    [minerror,minindex] = min(phi_n_store(:,2));
    
    Error = double(minerror);
    fitFuc = phi_n_store(minindex,1);
end

function InnerProduct = GetInnerProduct(X,F,thou,phi_k)
    phi_k = subs(phi_k,X);
    InnerProduct = 0;
    for i = 1:size(X,2)
        InnerProduct = InnerProduct + thou(i) * phi_k(i) * F(i);
    end
end

function ak_star = Get_ak_star(thou,X,F,phi_k)
    Ak = GetAk(thou,X,phi_k);
    phi_k = subs(phi_k,X);
    ak_star = 0;
    for i = 1:size(X,2)
        ak_star = ak_star + thou(i)*F(i)*phi_k(i);
    end
    ak_star = ak_star/Ak;
end

function Ak = GetAk(thou,X,phi_k)
    phi_k = subs(phi_k,X);
    Ak = 0;
    for i = 1:size(X,2)
        Ak = Ak + thou(i)*phi_k(i)*phi_k(i);
    end
end

function beita_k = Get_beita_k(thou,X,phi_k_minus_1,phi_k)
    phi_k_minus_1 = subs(phi_k_minus_1,X);
    phi_k = subs(phi_k,X);
    
    fenzi = 0;
    fenmu = 0;
    for i = 1:size(X,2)
        fenzi = fenzi + thou(i)*phi_k(i)^2;
        fenmu = fenmu + thou(i)*phi_k_minus_1(i)^2;
    end
    beita_k = fenzi/fenmu;
end

function Alpha_k = Get_alpha_k(thou,X,Phi_k)
    Phi_k = subs(Phi_k,X);
    
    fenzi = 0;
    fenmu = 0;
    for i = 1:size(X,2)
        fenzi = fenzi + thou(i)*X(i)*Phi_k(i)^2;
        fenmu = fenmu + thou(i)*Phi_k(i)^2;
    end
    Alpha_k = fenzi/fenmu;
end
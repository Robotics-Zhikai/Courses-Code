function [result,ValueList] = ThreeMomentInterpolation(ValueList,Condition,ConditionValue)
%����ز�ֵ����������ֵ�� ��ֵ������ÿ��С�����������ζ���ʽ��s(x)����ֱ��2�׵�����������ֵ�ڵ㴦���
%condition='first' ��һ�߽����������˵�ĵ������ֵ�����ĵ������ ConditionValueΪ�������ɢ��ı߽��һ�׵���ֵ
%condition='second' �ڶ��߽����������˵�Ķ��׵������ֵ�����Ķ��׵������ ConditionValueΪ�������ɢ��ı߽�Ķ��׵���ֵ
%condition='third' �����߽���������ֵ����Ϊ���ں���������0��1��2�׵�������������Ҷ˵��ں�һ���������� ConditionValue����ν
%result Ϊ���յĲ�ֵ����
%ValueList Ϊ���յĲ�ֵ�ڵ� 
    ValueList = MakeItUniqueRow(ValueList,1);
    if size(ValueList,2)<3
        error('С�������㣬�޷�����������ֵ');
    end
    h = [];
    for i = 1:size(ValueList,2)-1
        h = [h ValueList(1,i+1)-ValueList(1,i)];
    end
    miu = [];
    lameda = [];
    for i = 1:size(h,2)-1
        miu = [miu h(i)/(h(i)+h(i+1))];
        lameda = [lameda 1-h(i)/(h(i)+h(i+1))];
    end
    
    if strcmp(Condition,'first')
        h1 = h(1);
        hn = ValueList(1,end)-ValueList(1,end-1);
        f1 = ValueList(2,2);
        f0 = ValueList(2,1);
        d_f0 = ConditionValue(1);
        d_fn = ConditionValue(2);
        fn = ValueList(2,end);
        fn_minus_1 = ValueList(2,end-1);
        
        d0 = 6/h1*((f1-f0)/h1-d_f0);
        dn = 6/hn*(d_fn-(fn-fn_minus_1)/hn);
        d = [];
        for i = 1:size(ValueList,2)-2
            d = [d;6*GetDifferenceQuotient(ValueList(:,i:i+2))];
        end
        d = [d0;d;dn];
        
        dim = size(ValueList,2);
        A = zeros(dim,dim);
        for i = 2:dim-1
            base = i-1;
            A(i,base:base+2) = [miu(i-1) 2 lameda(i-1)];
        end
        A(1,1:2) = [2 1];
        A(end,end-1:end) = [1 2];
        M = ChasingMethod(A,d);
%         M1 = GaussianColumnElimination(A,d); %��������ʽ��Ҳ�ǿ��Ե�
%         M2 = GaussianElimination(A,d);
    elseif strcmp(Condition,'second')
        M0 = ConditionValue(1);
        Mn = ConditionValue(2);
        n = size(ValueList,2)-1;
        A = zeros(n-1,n-1);
        A(1,1:2) = [2,lameda(1)];
        A(end,end-1:end) = [miu(n-1),2];
        
        for i = 2:size(A,1)-1
            base = i-1;
            A(i,base:base+2) = [miu(i) 2 lameda(i)];
        end
        
        d = 6*GetDifferenceQuotient(ValueList(:,1:3)) - miu(1)*ConditionValue(1);
        for i = 2:n-2
            d = [d; 6*GetDifferenceQuotient(ValueList(:,i:i+2))];
        end
        d = [d;6*GetDifferenceQuotient(ValueList(:,end-2:end))-lameda(n-1)*ConditionValue(2)];
        
        M = ChasingMethod(A,d);
%         M1 = GaussianColumnElimination(A,d); %��������ʽ��Ҳ�ǿ��Ե�
%         M2 = GaussianElimination(A,d);
        M = [M0;M;Mn];
    elseif strcmp(Condition,'third')
        n = size(ValueList,2)-1;
        ValueList = [ValueList [ValueList(1,end)+(ValueList(1,end)-ValueList(1,end-1));ValueList(2,2)]];
        
        lameda = [lameda h(1)/(h(n)+h(1))];
        miu = [miu h(n)/(h(n)+h(1))];
        A = zeros(n,n);
        A(1,1:2) = [2,lameda(1)];
        A(1,end) = miu(1);
        A(end,1) = lameda(n);
        A(end,end-1:end)=[miu(n) 2];
        for i = 2:size(A,1)-1
            base = i-1;
            A(i,base:base+2) = [miu(i) 2 lameda(i)];
        end
        
        d = [];
        for i =2:n+1
            d=[d;GetDifferenceQuotient(ValueList(:,i-1:i+1))];
        end
        ValueList(:,end) = []; %��������һ��ȥ������
%         M = GaussianColumnElimination(A,d);
        M = GaussianElimination(A,d);
        M = [M(end);M];
%         M = ChasingMethod(A,d); %������ʹ��׷�Ϸ������� �����ǿ����ø�˹��Ԫ
    else
        error('������ƥ���Condition����ֵ')
    end
    
    syms x;
    result = [];
    for i = 1:size(ValueList,2)-1
        x_jp1 = ValueList(1,i+1);
        x_j = ValueList(1,i);
        h_jp1 = ValueList(1,i+1)-ValueList(1,i);
        f_jp1 = ValueList(2,i+1);
        f_j = ValueList(2,i);
        M_j = M(i);
        M_jp1 = M(i+1);
        B_j = f_j-h_jp1^2/6*M_j;
        A_j = (f_jp1-f_j)/h_jp1-h_jp1/6*(M_jp1-M_j);
        s = (x_jp1-x)^3/(6*h_jp1)*M_j+(x-x_j)^3/(6*h_jp1)*M_jp1+A_j*(x-x_j)+B_j;
        result = [result [{[x_j,x_jp1]};{s}]];
    end
    
end














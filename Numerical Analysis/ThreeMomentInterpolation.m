function ThreeMomentInterpolation(ValueList,Condition,ConditionValue)
%����ز�ֵ����������ֵ�� ��ֵ������ÿ��С�����������ζ���ʽ��s(x)����ֱ��2�׵�����������ֵ�ڵ㴦���
%condition='first' ��һ�߽����������˵�ĵ������ֵ�����ĵ������ ConditionValueΪ�������ɢ��ı߽��һ�׵���ֵ
%condition='second' �ڶ��߽����������˵�Ķ��׵������ֵ�����Ķ��׵������ ConditionValueΪ�������ɢ��ı߽�Ķ��׵���ֵ
%condition='third' �����߽���������ֵ����Ϊ���ں���������0��1��2�׵�������������Ҷ˵��ں�һ���������� ConditionValue����ν
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
    elseif strcmp(Condition,'second')
    elseif strcmp(Condition,'third')
    else
        error('������ƥ���Condition����ֵ')
    end
end














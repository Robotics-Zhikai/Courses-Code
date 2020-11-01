function ThreeMomentInterpolation(ValueList,Condition,ConditionValue)
%三弯矩插值法（样条插值） 插值函数在每个小区间上是三次多项式，s(x)及其直到2阶导数连续，插值节点处相等
%condition='first' 第一边界条件，两端点的导数与插值函数的导数相等 ConditionValue为输入的离散点的边界的一阶导数值
%condition='second' 第二边界条件，两端点的二阶导数与插值函数的二阶导数相等 ConditionValue为输入的离散点的边界的二阶导数值
%condition='third' 第三边界条件，插值函数为周期函数，且其0、1、2阶导数的子区间的右端等于后一子区间的左端 ConditionValue无所谓
    ValueList = MakeItUniqueRow(ValueList,1);
    if size(ValueList,2)<3
        error('小于三个点，无法进行样条插值');
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
        error('不存在匹配的Condition参数值')
    end
end














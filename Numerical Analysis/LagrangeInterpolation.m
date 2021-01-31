function [Outn_x,ValueList] = LagrangeInterpolation(ValueList,n_input) 
%n_input次拉格朗日插值  
%输入是一个离散的函数值表，包括xi和f(xi)
%ValueList是两行的向量，第一行是插值节点，第二行是对应的数值
%输出ValueList表示用到的插值节点序列 
    if isempty(ValueList)
        error('无法插值');
    end
    ValueList = MakeItUniqueRow(ValueList,1);
    a = min(ValueList(1,:));
    b = max(ValueList(1,:));
    if (size(ValueList,2)-1>n_input)
        if n_input~=0
            ValueList = UniformDivision(ValueList,n_input+1);%输入数据维度如果超出了n_input+1 则尽量均匀的保留n_input+1个点
            
            %如果是随机去掉点的话，精度会差一点
%             IndexNota_b = intersect(find(ValueList(1,:)~=a),find(ValueList(1,:)~=b));
%             ranpermindex = randperm(size(IndexNota_b,2));
%             DeleteIndex = ranpermindex(1:size(ValueList,2)-1-n_input);
%             ValueList(:,IndexNota_b(DeleteIndex)) = []; %输入数据维度如果超出了n_input+1 则随机去掉一些点
        else
            ValueList = ValueList(:,1);
        end
        n = n_input;
    elseif size(ValueList,2)-1<n_input
        n = size(ValueList,2)-1;
    else
        n = n_input;
    end
    
    syms x;
    L = [];
    for i = 1:n+1
        A_i = 1;
        tmp = 1;
        for j=1:n+1
            if i~=j
                tmp = tmp * (x-ValueList(1,j));
                A_i = A_i*(ValueList(1,i)-ValueList(1,j));
            end
        end
        A_i = 1/A_i;
        l_i = tmp*A_i;
        L = [L l_i];
    end
    
    Outn_x = 0;
    for i = 1:n+1
        Outn_x = Outn_x + L(i)*ValueList(2,i);
    end
end







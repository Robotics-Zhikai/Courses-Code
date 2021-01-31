function [Outn_x,ValueList] = LagrangeInterpolation(ValueList,n_input) 
%n_input���������ղ�ֵ  
%������һ����ɢ�ĺ���ֵ������xi��f(xi)
%ValueList�����е���������һ���ǲ�ֵ�ڵ㣬�ڶ����Ƕ�Ӧ����ֵ
%���ValueList��ʾ�õ��Ĳ�ֵ�ڵ����� 
    if isempty(ValueList)
        error('�޷���ֵ');
    end
    ValueList = MakeItUniqueRow(ValueList,1);
    a = min(ValueList(1,:));
    b = max(ValueList(1,:));
    if (size(ValueList,2)-1>n_input)
        if n_input~=0
            ValueList = UniformDivision(ValueList,n_input+1);%��������ά�����������n_input+1 �������ȵı���n_input+1����
            
            %��������ȥ����Ļ������Ȼ��һ��
%             IndexNota_b = intersect(find(ValueList(1,:)~=a),find(ValueList(1,:)~=b));
%             ranpermindex = randperm(size(IndexNota_b,2));
%             DeleteIndex = ranpermindex(1:size(ValueList,2)-1-n_input);
%             ValueList(:,IndexNota_b(DeleteIndex)) = []; %��������ά�����������n_input+1 �����ȥ��һЩ��
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







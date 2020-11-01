function ValueList = UniformDivision(ValueList,n)
%��ValueList�е���ֵ�����ܾ��ȵı���n���� ����ValueList���������ڵ���n
%ValueList��һ���ǲ�ֵ�ڵ㣬�ڶ����ǲ�ֵ�ڵ��Ӧ�ĺ���ֵ
    if (isempty(ValueList))
        return;
    end
    if (size(ValueList,2)==n)
        return;
    elseif size(ValueList,2)>n
        if n==0
            ValueList = [];
        elseif n == 1
            ValueList = ValueList(:,ceil(size(ValueList,2)/2));
        else
            a = min(ValueList(1,:));
            b = max(ValueList(1,:));
            step = (b-a)/n;
%             avgList = a:step:b; %�����������ǰ�պ��ҵ�һ��ֵ ���һ��ǰ�պ��
            
            ValueList = SortWithRowMain(ValueList,1);
            
%             [~,index] = sort(ValueList(1,:));
%             sortedNode = ValueList(1,index);
%             sortedf = ValueList(2,index);
%             ValueList = [sortedNode;sortedf];
            
            Type = [];
            for i=1:size(ValueList,2)
                Type = [Type floor((ValueList(1,i)-a)/step)];
            end
            
            TypeAll = [];
            result = [];
            Typethis = Type(1);
            TypeAll = [TypeAll Typethis];
            result = [result ValueList(:,1)];
            for i = 2:size(Type,2)
                if Type(i)~=Typethis
                    Typethis = Type(i);
                    TypeAll = [TypeAll Typethis];
                    result = [result ValueList(:,i)];
                end
            end
            
            if (size(result,2)>n)
                result = result(:,1:n);
            elseif (size(result,2)<n) %˵�����Ǿ��ȷֲ��ģ���ĳһ�����оۼ�
                Store = [];
                for i = 1:size(TypeAll,2)
                    tmp = TypeAll(i);
                    indextmp = find(Type==tmp);
                    if size(indextmp,2)==1
                        continue;
                    else
                        indextmp(1) = [];
                        Store = [Store indextmp];
                        if (size(Store,2)>=n-size(result,2))
                            break;
                        end
                    end
                end
                Store = Store(1:n-size(result,2));
                result = [result ValueList(:,Store)];
            end
            
            ValueList = result;
        end
    else
        error('���ܴ����������');
    end
end

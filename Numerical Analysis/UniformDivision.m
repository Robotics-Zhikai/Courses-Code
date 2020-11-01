function ValueList = UniformDivision(ValueList,n)
%将ValueList中的数值尽可能均匀的保留n个点 其中ValueList的列数大于等于n
%ValueList第一行是插值节点，第二行是插值节点对应的函数值
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
%             avgList = a:step:b; %在这个区间里前闭后开找第一个值 最后一段前闭后闭
            
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
            elseif (size(result,2)<n) %说明不是均匀分布的，在某一区间有聚集
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
        error('不能处理这种情况');
    end
end

function result = GetDifferenceQuotient(ValueList)
    if CheckUniqueRow(ValueList,1)==1
        result = GetDifferenceQuotientRecur(ValueList);
    else
        error('第一行有重复值，暂时无法处理此时的差商')
    end
%     ValueList = MakeItUniqueRow(ValueList,1);
%     
end

function result = GetDifferenceQuotientRecur(ValueList)
%得到ValueList的差商，ValueList有多少列就是多少阶差商 通过递归得到
%ValueList的第一行必须保证不重复
    if size(ValueList,2) <= 1
        error('不存在差商');
    end
    if size(ValueList,2)==2
        result = (ValueList(2,1)-ValueList(2,2))/(ValueList(1,1)-ValueList(1,2));
    else
        for i = 1:size(ValueList,2)-1
            result = (GetDifferenceQuotientRecur(ValueList(:,2:end))-GetDifferenceQuotientRecur(ValueList(:,1:end-1)))/(ValueList(1,end)-ValueList(1,1));
        end
    end
    
end
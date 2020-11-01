function result = GetDifferenceQuotient(ValueList)
    if CheckUniqueRow(ValueList,1)==1
        result = GetDifferenceQuotientRecur(ValueList);
    else
        error('��һ�����ظ�ֵ����ʱ�޷������ʱ�Ĳ���')
    end
%     ValueList = MakeItUniqueRow(ValueList,1);
%     
end

function result = GetDifferenceQuotientRecur(ValueList)
%�õ�ValueList�Ĳ��̣�ValueList�ж����о��Ƕ��ٽײ��� ͨ���ݹ�õ�
%ValueList�ĵ�һ�б��뱣֤���ظ�
    if size(ValueList,2) <= 1
        error('�����ڲ���');
    end
    if size(ValueList,2)==2
        result = (ValueList(2,1)-ValueList(2,2))/(ValueList(1,1)-ValueList(1,2));
    else
        for i = 1:size(ValueList,2)-1
            result = (GetDifferenceQuotientRecur(ValueList(:,2:end))-GetDifferenceQuotientRecur(ValueList(:,1:end-1)))/(ValueList(1,end)-ValueList(1,1));
        end
    end
    
end
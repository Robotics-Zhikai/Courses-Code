function mat = SortWithRowMain(mat,row) %�Ե�row����Ϊ������������ֵ�������� ��С���� ͬʱ��Ҫ���ֶ�Ӧ�������ݵ���Թ�ϵ����
    if isempty(mat)
        return;
    end
    [~,index] = sort(mat(row,:));
    sortedNode = mat(row,index);
    
    sortedf1 = mat(1:row-1,index);
    sortedf2 = mat(row+1:size(mat,1),index);
    mat = [sortedf1;sortedNode;sortedf2];
end
function mat = SortWithRowMain(mat,row) %以第row行作为主导的排序数值进行排序 从小到大 同时需要保持对应的列数据的相对关系不变
    if isempty(mat)
        return;
    end
    [~,index] = sort(mat(row,:));
    sortedNode = mat(row,index);
    
    sortedf1 = mat(1:row-1,index);
    sortedf2 = mat(row+1:size(mat,1),index);
    mat = [sortedf1;sortedNode;sortedf2];
end
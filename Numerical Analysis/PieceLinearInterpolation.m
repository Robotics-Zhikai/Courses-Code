function result = PieceLinearInterpolation(ValueList) 
%分段线性插值
    if size(ValueList,2)<=1
        error('无法插值');
    end
    ValueList = MakeItUniqueRow(ValueList,1);
    ValueList = SortWithRowMain(ValueList,1);
    result = [];
    syms x;
    for i = 1:size(ValueList,2)-1
        x_i = ValueList(1,i);
        x_i_1 = ValueList(1,i+1);
        f_x_i = ValueList(2,i);
        f_x_i_1 = ValueList(2,i+1);
        
        thiscol = [{[x_i,x_i_1]};{(x-x_i_1)/(x_i-x_i_1)*f_x_i+(x-x_i)/(x_i_1-x_i)*f_x_i_1}];
        result = [result thiscol];
    end
end
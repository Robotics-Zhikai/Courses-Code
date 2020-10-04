function Seq = IteratedFunc(Func_str,InitValue,InterationNum)
    x = InitValue;
    
    Seq = [];
    Seq = [Seq x];
    for i = 1:InterationNum
        x = eval(Func_str);
        Seq = [Seq x];
    end
end
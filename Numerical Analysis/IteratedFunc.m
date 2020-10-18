function Seq = IteratedFunc(Func_str,InitValue,InterationNum,ERROR)
%可以进行多点迭代，也可以进行单点迭代
%单点迭代时输入的函数式必须是以x为自变量
%多点迭代时输入的函数式必须是以x0,x1,x2,.......为自变量
%initValue是行向量
    if size(InitValue,2)==1
        x = InitValue;

        Seq = [];
        Seq = [Seq x];
        for i = 1:InterationNum
            x = eval(Func_str);
            if isnan(x) 
                error('迭代不收敛,需要重新设置初值');
            end
            
            Seq = [Seq x];
            if nargin==4
                if abs(Seq(end)-Seq(end-1))<ERROR
                    return
                end
            end
            
            if (size(Seq,2)>2 && abs(Seq(end)-Seq(end-1))>10e-4 && abs(Seq(end)-Seq(end-2))<10e-4) %当上下跳动时迭代不收敛
                error('上下跳动,迭代不收敛')
            end
        end
    elseif size(InitValue,2)>1
        for i = 1:size(InitValue,2)
            vars{1,i} = (['x',num2str(i-1)]); %批量生成变量
            vars{2,i} = InitValue(i);
            eval([vars{1,i},'=','vars{2,i};']);
        end
        
        Seq = [];
        Seq = [Seq,InitValue];
        for i = 1:InterationNum
            x = eval(Func_str);
            if isnan(x) 
                if i == 1
                    error('迭代不收敛,需要重新设置初值');
                else
                    warning('出现了NAN,迭代可能是收敛的，但是计算的分母为0，比如割线法时迭代在收敛，但是分母会趋于0.迭代序列Seq：')
                    Seq
                    return
                end
            end
            
            for j = 1:size(vars,2)-1
                vars{2,j}=vars{2,j+1};
            end
            vars{2,end} = x;
            for j = 1:size(vars,2)
                eval([vars{1,j},'=','vars{2,j};']);
            end
            
            Seq = [Seq x];
            
            if nargin==4
                if abs(Seq(end)-Seq(end-1))<ERROR
                    return
                end
            end
            
            if (size(Seq,2)>2 && abs(Seq(end)-Seq(end-1))>10e-4 && abs(Seq(end)-Seq(end-2))<10e-4) %当上下跳动时迭代不收敛
                error('上下跳动,迭代不收敛')
            end
        end
    end
    
end
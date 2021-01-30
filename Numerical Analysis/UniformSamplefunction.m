function Points = UniformSamplefunction(equ,qujian,N)
%连续函数equ均匀在qujian内采样N个互异点
    Points = [];
    if (size(qujian,2)>2)
        error('qujian不能超过两个数');
    end
    if N==0 || N<0
        return;
    end
    if (size(qujian,2)==1)
        qujian = [qujian qujian ];
        Points = qujian;
        if N==1
            return;
        else
            error('无法产生互异的点')
        end
    else
        minqujian = min(qujian);
        maxqujian = max(qujian);
        qujian = [minqujian maxqujian ];
    end
    if N == 1
        step = (qujian(1)+qujian(2))/2;
    else
        step = (qujian(2)-qujian(1))/(N-1);
    end
    
    Points = [];
    for i = 0:N-1
        Points(1,i+1) = qujian(1)+i*step;
        x = Points(1,i+1);
        Points(2,i+1) = eval(equ);
    end 
    
    Points = MakeItUniqueRow(Points,1);
end





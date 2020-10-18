function Seq = IteratedFunc(Func_str,InitValue,InterationNum,ERROR)
%���Խ��ж�������Ҳ���Խ��е������
%�������ʱ����ĺ���ʽ��������xΪ�Ա���
%������ʱ����ĺ���ʽ��������x0,x1,x2,.......Ϊ�Ա���
%initValue��������
    if size(InitValue,2)==1
        x = InitValue;

        Seq = [];
        Seq = [Seq x];
        for i = 1:InterationNum
            x = eval(Func_str);
            if isnan(x) 
                error('����������,��Ҫ�������ó�ֵ');
            end
            
            Seq = [Seq x];
            if nargin==4
                if abs(Seq(end)-Seq(end-1))<ERROR
                    return
                end
            end
            
            if (size(Seq,2)>2 && abs(Seq(end)-Seq(end-1))>10e-4 && abs(Seq(end)-Seq(end-2))<10e-4) %����������ʱ����������
                error('��������,����������')
            end
        end
    elseif size(InitValue,2)>1
        for i = 1:size(InitValue,2)
            vars{1,i} = (['x',num2str(i-1)]); %�������ɱ���
            vars{2,i} = InitValue(i);
            eval([vars{1,i},'=','vars{2,i};']);
        end
        
        Seq = [];
        Seq = [Seq,InitValue];
        for i = 1:InterationNum
            x = eval(Func_str);
            if isnan(x) 
                if i == 1
                    error('����������,��Ҫ�������ó�ֵ');
                else
                    warning('������NAN,���������������ģ����Ǽ���ķ�ĸΪ0��������߷�ʱ���������������Ƿ�ĸ������0.��������Seq��')
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
            
            if (size(Seq,2)>2 && abs(Seq(end)-Seq(end-1))>10e-4 && abs(Seq(end)-Seq(end-2))<10e-4) %����������ʱ����������
                error('��������,����������')
            end
        end
    end
    
end
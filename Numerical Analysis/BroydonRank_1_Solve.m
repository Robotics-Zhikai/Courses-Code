function xout = BroydonRank_1_Solve(Funcs_Str,initValue,error)
    %��ţ�ٷ��������Է�����Ľ� ÿ��H�ı仯��Ϊ1
    %Funcs_Str�Ƿ�������ߵķ��̼��ϣ��������ұߵ���������
    %�������δ֪����ҲӦ����x0,x1,x2,......
    for i = 1:size(Funcs_Str,2)
        equ = Funcs_Str{i};
        Funcs_StrNew(i) = str2sym(equ);
    end
    Funcs_Str = Funcs_StrNew;
    Funcs_Str = Funcs_Str';
    
    H = [];
    for i = 1:size(Funcs_Str,1)
        for j = 1:size(Funcs_Str,1)
            H{i,j} = diff(Funcs_Str(i),['x',num2str(j-1)]);
        end
    end
    
    vars = [];
    x_k = initValue';
    for i = 1:size(initValue,2)
        vars{1,i} = (['x',num2str(i-1)]); %�������ɱ���
        vars{2,i} = x_k(i);
        eval([vars{1,i},'=','vars{2,i};']);
    end
    
%     H = inv(H);
    H_k = subs(subs(H));
    H_k = double(H_k);
    H_k = inv(H_k);
    
    while 1
        for i = 1:size(initValue,2)
            vars{2,i} = x_k(i);
            eval([vars{1,i},'=','vars{2,i};']);
        end
        F_k = subs(Funcs_Str);
        F_k = double(F_k);
        if norm(F_k)<error
            break;
        end
        
        x_k_1 = x_k - H_k*F_k;
        for i = 1:size(initValue,2)
            vars{2,i} = x_k_1(i);
            eval([vars{1,i},'=','double(vars{2,i});']);
        end
        F_k_1 = subs(Funcs_Str);
        r_k = x_k_1-x_k;
        y_k = F_k_1-F_k;
        
        F_k_1 = double(F_k_1);
%         r_k = double(r_k);
        y_k = double(y_k);
        H_k_1 = H_k + (r_k-H_k*y_k)*(r_k'*H_k)/(r_k'*H_k*y_k); %��������H��ֵ�Ļ��Ͳ���ÿ�ζ�������
        H_k_1 = double(H_k_1);
        
        x_k = x_k_1;
        H_k = H_k_1;
    end
    
    xout = double(x_k);
end
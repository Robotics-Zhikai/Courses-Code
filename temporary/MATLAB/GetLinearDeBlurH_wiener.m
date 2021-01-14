
function Filter = GetLinearDeBlurH_wiener(width,height,a,b,T,K)%得到线性去模糊的H
    Hblur = GetLinearBlurH(width,height,a,b,T);
    for v=1:size(Hblur,1)
        for u=1:size(Hblur,2)
            Hblur(v,u) = 1/Hblur(v,u)*(abs(Hblur(v,u)))^2/((abs(Hblur(v,u)))^2+K);
        end
    end
    Filter = Hblur;
end

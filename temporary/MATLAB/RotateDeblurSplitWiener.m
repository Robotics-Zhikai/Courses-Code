function DeblurImage = RotateDeblurSplitWiener(originblurimage,subwidth,subheight,omiga,pixelsPerMeter_x,pixelsPerMeter_y)
    if mod(subwidth,2)==0 || mod(subheight,2)==0
        error('最好弄成奇数的，奇数的容易计算');
    end
    resizeblurimage = ResizeImageTimesOf(originblurimage,subwidth,subheight);
    subimages = SplitImage2subImages(resizeblurimage,subwidth,subheight);
    CentersLoc = GetSubimagesCentersLoc(subimages,pixelsPerMeter_x,pixelsPerMeter_y);
    CentersVelocity = GetVelocityFromCentersLoc(CentersLoc,omiga);
    CentersVector = GetVectorFromCentersLoc(CentersLoc,omiga);
    CentersMotion = GetCentersMotion(CentersVelocity,CentersVector);
    
    for i =1:size(subimages,1)
        for j = 1:size(subimages,2)
            Extendedimage = ExtendImage(subimages{i,j},4);
            xa = CentersMotion{i,j}(1);
            yb = -CentersMotion{i,j}(2); %坐标系不一样 退化函数的左边原点在左上角 而这个在左下角 y轴相对 值相反
            T = 1;
            K = 0.015;
            imagedeblur = WienerLinearDeblur(Extendedimage,xa,yb,T,K);
            subimages{i,j} = imagedeblur(1:subheight,1:subwidth);
%             imshow(uint8(imagedeblur)) %线性去模糊的图像
        end
    end
    
    DeblurImage = zeros(size(resizeblurimage,1),size(resizeblurimage,2));
    for i =1:size(subimages,1)
        for j = 1:size(subimages,2)
            DeblurImage((i-1)*subheight+1:i*subheight,(j-1)*subwidth+1:j*subwidth) = subimages{i,j};
        end
    end
end

function CentersLoc = GetSubimagesCentersLoc(subimages,pixelsPerMeter_x,pixelsPerMeter_y) %得到以图像左下角为直角坐标系原点，向右为x正方向 向左为y正方向的直角坐标
    CentersLoc = [];
    if isempty(subimages)
        return ;
    else
        subwidth = size(subimages{1,1},2);
        subheight = size(subimages{1,1},1);
    end 
    
    pixelIntervalx = 1/pixelsPerMeter_x;
    pixelIntervaly = 1/pixelsPerMeter_y;
    for y = size(subimages,1):-1:1
        for x = 1:size(subimages,2)
            CentersLoc{y,x} = [pixelIntervalx*(x*subwidth-(subwidth-1)/2) pixelIntervaly*((size(subimages,1)-y+1)*subheight-(subheight-1)/2)];%必须需要，因为需要得到每一块的运动方向
%             CentersLoc{y,x} = [1*(x*subwidth-(subwidth-1)/2) 1*((size(subimages,1)-y+1)*subheight-(subheight-1)/2)];%似乎不需要pixelIntervalx也可以
        end
    end
end

function CentersVelocity = GetVelocityFromCentersLoc(CentersLoc,omiga) 
    CentersVelocity = zeros(size(CentersLoc,1),size(CentersLoc,2));
    for i =1:size(CentersLoc,1)
        for j = 1:size(CentersLoc,2)
            CentersVelocity(i,j) = (CentersLoc{i,j}(1)^2+CentersLoc{i,j}(2)^2)^0.5;
        end
    end
    CentersVelocity = CentersVelocity./CentersVelocity(1,end);
    CentersVelocity = CentersVelocity.*omiga;
end

function CentersVector = GetVectorFromCentersLoc(CentersLoc,omiga) %omiga为正时逆时针旋转，否则顺时针
    CentersVector = [];
    for i =1:size(CentersLoc,1)
        for j = 1:size(CentersLoc,2)
            if omiga>0
                CentersLoc{i,j} = CentersLoc{i,j}/norm(CentersLoc{i,j});
                CentersVector{i,j} = [-CentersLoc{i,j}(2) CentersLoc{i,j}(1)];
            elseif omiga==0
                CentersVector{i,j} = [0,0];
            else
                CentersLoc{i,j} = CentersLoc{i,j}/norm(CentersLoc{i,j});
                CentersVector{i,j} = [CentersLoc{i,j}(2) -CentersLoc{i,j}(1)];
            end
            
        end
    end
end

function CentersMotion = GetCentersMotion(CentersVelocity,CentersVector)
    CentersMotion = [];
    for i =1:size(CentersVelocity,1)
        for j = 1:size(CentersVelocity,2)
            CentersMotion{i,j} = CentersVelocity(i,j)*CentersVector{i,j};
        end
    end
end






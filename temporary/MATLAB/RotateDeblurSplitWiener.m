function DeblurImage = RotateDeblurSplitWiener(originblurimage,subwidth,subheight,omiga,pixelsPerMeter_x,pixelsPerMeter_y)
    if mod(subwidth,2)==0 || mod(subheight,2)==0
        error('���Ū�������ģ����������׼���');
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
            yb = -CentersMotion{i,j}(2); %����ϵ��һ�� �˻����������ԭ�������Ͻ� ����������½� y����� ֵ�෴
            T = 1;
            K = 0.015;
            imagedeblur = WienerLinearDeblur(Extendedimage,xa,yb,T,K);
            subimages{i,j} = imagedeblur(1:subheight,1:subwidth);
%             imshow(uint8(imagedeblur)) %����ȥģ����ͼ��
        end
    end
    
    DeblurImage = zeros(size(resizeblurimage,1),size(resizeblurimage,2));
    for i =1:size(subimages,1)
        for j = 1:size(subimages,2)
            DeblurImage((i-1)*subheight+1:i*subheight,(j-1)*subwidth+1:j*subwidth) = subimages{i,j};
        end
    end
end

function CentersLoc = GetSubimagesCentersLoc(subimages,pixelsPerMeter_x,pixelsPerMeter_y) %�õ���ͼ�����½�Ϊֱ������ϵԭ�㣬����Ϊx������ ����Ϊy�������ֱ������
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
            CentersLoc{y,x} = [pixelIntervalx*(x*subwidth-(subwidth-1)/2) pixelIntervaly*((size(subimages,1)-y+1)*subheight-(subheight-1)/2)];%������Ҫ����Ϊ��Ҫ�õ�ÿһ����˶�����
%             CentersLoc{y,x} = [1*(x*subwidth-(subwidth-1)/2) 1*((size(subimages,1)-y+1)*subheight-(subheight-1)/2)];%�ƺ�����ҪpixelIntervalxҲ����
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

function CentersVector = GetVectorFromCentersLoc(CentersLoc,omiga) %omigaΪ��ʱ��ʱ����ת������˳ʱ��
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






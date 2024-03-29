#include "IPcode1.h"


bool IPcode1::ReadBmp(char * name)
{
	FILE *fp = fopen(name, "rb");

	if (fp == 0)
		throw exception("读取失败");
	//BITMAPFILEHEADER filehead; // 14字节的文件头
	fread(&filehead, sizeof(BITMAPFILEHEADER), 1, fp);

	if (filehead.bfType != 0x4D42)
		throw exception("目前只能读取tmp格式的图片，其他的无法读取");
	fseek(fp, sizeof(BITMAPFILEHEADER), 0); //跳过BITMAPFILEHEADER 14字节的文件头

	//BITMAPINFOHEADER infohead; //40字节的信息头
	//cout << sizeof(BITMAPINFOHEADER) << endl;
	fread(&infohead, sizeof(BITMAPINFOHEADER), 1, fp);

	auto imageWidth = infohead.biWidth; //图像的宽度，单位为像素
	auto imageHeight = infohead.biHeight;//图像的高度
	auto imageBitCount = infohead.biBitCount;//指定颜色时用到的位数

	if (filehead.bfOffBits > 54) // 说明有调色板数据
	{
		fseek(fp, 54, 0);
		pcolortable = new RGBQUAD[(filehead.bfOffBits - 54)/sizeof(RGBQUAD)];
		fread(pcolortable, filehead.bfOffBits - 54, 1, fp);
	}
		
	fseek(fp, filehead.bfOffBits, 0);//跳过头文件，到达实际的位图数据
	pbmpBuf = new unsigned char[infohead.biSizeImage];//读入图像数据
	fread(pbmpBuf, infohead.biSizeImage, 1, fp);

	fclose(fp);//关闭文件
	return TRUE;
}

bool IPcode1::CheckInImage(int x, int y)const
{
	if ((x >= 0 && x < infohead.biWidth) && (y >= 0 && y < infohead.biHeight))
		return TRUE;
	else
		return FALSE;
}

vector<unsigned long> IPcode1::GetPixel_Index(int x, int y, int & bitlocation)const
//只要返回，肯定返回值不空
{
	vector<unsigned long> result;
	bitlocation = -1;
	if (CheckInImage(x, y) == FALSE)
	{
		cout << __FILE__ << __LINE__;
		throw exception("不能读取在图像范围之外的像素点数据");
		//return result;
	}

	auto bmpWidth = infohead.biWidth;
	auto biBitCount = infohead.biBitCount;
	int lineBytes = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;
	int single = biBitCount / 8;
	unsigned long index;
	if (single == 0)
	{
		if (biBitCount == 4)
		{
			index = x / 2 + y*lineBytes;
			bitlocation = x % 2;
		}
		else if (biBitCount == 1)
		{
			index = x / 8 + y*lineBytes;
			bitlocation = x % 8;
		}
		else
		{
			throw exception("未定义bitBitCount,出错了");
		}
	}
	else
	{
		index = x*single + y*lineBytes;
	}

	if (single == 0 || single == 1)
		result.push_back(index);
	else if (single == 3)
	{
		result.push_back(index);
		result.push_back(index + 1);
		result.push_back(index + 2);
	}
	else
		throw("未定义处理此bitBitCount的程序");
	return result;
}

vector<unsigned char *> IPcode1::GetPixel(int x, int y, int & bitlocation)const
//只要返回，肯定返回值不空
{
	vector<unsigned char *> result;
	bitlocation = -1;
	vector<unsigned long> resultIndex = GetPixel_Index(x, y, bitlocation);
	for (int i = 0; i < resultIndex.size(); i++)
		result.push_back(pbmpBuf + resultIndex[i]);
	return result;
}

void IPcode1::CropBmp(int locx, int locy, int width, int height)//截取BMP的某块区域
//width向右为正 height向上为正
{
	LONG leftdown_x = locx;
	LONG leftdown_y = locy;
	LONG rightdown_x = locx + width;
	LONG rightdown_y = locy;
	LONG leftup_x = locx;
	LONG leftup_y = locy + height;
	LONG rightup_x = locx + width;
	LONG rightup_y = locy + height;

	if (leftdown_x < 0)
	{
		leftdown_x = 0;
		leftup_x = 0;
	}
	else if (leftdown_x >= infohead.biWidth)
	{
		leftdown_x = infohead.biWidth -1;
		leftup_x = infohead.biWidth -1;
	}

	if (rightdown_x < 0)
	{
		rightdown_x = 0;
		rightup_x = 0;
	}
	else if (rightdown_x >= infohead.biWidth)
	{
		rightdown_x = infohead.biWidth - 1;
		rightup_x = infohead.biWidth - 1;
	}

	if (leftdown_y < 0)
	{
		leftdown_y = 0;
		rightdown_y = 0;
	}
	else if (leftdown_y >= infohead.biHeight)
	{
		leftdown_y = infohead.biHeight - 1;
		rightdown_y = infohead.biHeight - 1;
	}

	if (leftup_y < 0)
	{
		leftup_y = 0;
		rightup_y = 0;
	}
	else if (leftup_y >= infohead.biHeight)
	{
		leftup_y = infohead.biHeight - 1;
		rightup_y = infohead.biHeight - 1;
	}

	bool flag = CheckInImage(leftdown_x, leftdown_y) && CheckInImage(rightdown_x, rightdown_y);
	flag = flag &&CheckInImage(leftup_x, leftup_y);
	flag = flag && CheckInImage(rightup_x, rightup_y);
	
	if (flag == FALSE)
		throw exception("程序逻辑出错");

	width = abs(leftdown_x - rightdown_x)+1;
	height = abs(leftdown_y - leftup_y)+1;
	int lineByte = (width * infohead.biBitCount / 8 + 3) / 4 * 4;
	auto sizeimage = height*lineByte;

	unsigned char * newpbmpBuf = new unsigned char[sizeimage];
	LONG indexbmpbuf = 0;
	for (LONG y = min(leftdown_y, leftup_y); y <= max(leftdown_y, leftup_y); y++)
	{
		LONG ByteCount = 0;
		for (LONG x = min(leftdown_x, rightdown_x); x <= max(leftdown_x, rightdown_x); x++)
		{
			int bitlocation;
			auto data = GetPixel(x, y, bitlocation);
			if (data.size() == 1)
			{
				if (bitlocation == -1) //说明是8位的
				{
					newpbmpBuf[indexbmpbuf] = *data[0];
					indexbmpbuf++;
					ByteCount++;
				}
				else
				{
					cout << __FILE__ << "/" << __LINE__;
					throw exception("暂时不写这种情况的处理，之后有空可以补上");
				}
			}
			else if (data.size() == 3)
			{
				for (int i = 0; i < 3; i++)
				{
					newpbmpBuf[indexbmpbuf] = *data[i];
					indexbmpbuf++;
					ByteCount++;
				}
			}
			else if (data.size()==0)
			{
				//GetPixel(x, y, bitlocation);
				cout << __FILE__ << "/" << __LINE__;
				throw exception("程序逻辑出错,至此不应该出现data为空的情况");
			}
		}
		if (ByteCount % 4 != 0)
		{
			for (int i = 0; i < 4 - (ByteCount % 4); i++)
			{
				newpbmpBuf[indexbmpbuf] = 0x00; //不足4字节的倍数就补全0x00
				indexbmpbuf++;
			}
		}
	}
	delete[] pbmpBuf;
	pbmpBuf = newpbmpBuf;
	infohead.biHeight = height;
	infohead.biWidth = width;
	infohead.biSizeImage = sizeimage;

	if (infohead.biSizeImage != indexbmpbuf)
	{
		cout << __FILE__ << "/" << __LINE__;
		throw exception("程序逻辑出错！");
	}
}

IPcode1::IPcode1(const IPcode1 & input)
	:pbmpBuf(NULL), pcolortable(NULL),infohead(input.infohead), filehead(input.filehead)
{
	int linebytes = (input.infohead.biWidth * input.infohead.biBitCount / 8 + 3) / 4 * 4;
	auto sizebytes = input.infohead.biHeight*linebytes;
	pbmpBuf = new unsigned char [sizebytes];
	
	//if (input.infohead.biHeight*linebytes != input.infohead.biSizeImage)
	//	cout << "warning：input.infohead.biHeight*linebytes != input.infohead.biSizeImage" << input.infohead.biHeight*linebytes << " " << input.infohead.biSizeImage;
	////这样有时是合法的，比如input的pbmpbuf大于biSizeImage
	
	for (auto i = 0; i < sizebytes; i++)
	{
		pbmpBuf[i] = input.pbmpBuf[i];
	}
	pcolortable = new RGBQUAD[(input.filehead.bfOffBits - 54) / sizeof(RGBQUAD)];
	for (int i = 0; i < (input.filehead.bfOffBits - 54) / sizeof(RGBQUAD); i++)
	{
		pcolortable[i] = input.pcolortable[i];
	}
}

//IPcode1 & IPcode1::operator=(const IPcode1 & input)
//{
//	pbmpBuf = new unsigned char[input.infohead.biSizeImage];
//	for (int i = 0; i < input.infohead.biSizeImage; i++)
//	{
//		pbmpBuf[i] = input.pbmpBuf[i];
//	}
//	pcolortable = new RGBQUAD[(input.filehead.bfOffBits - 54) / sizeof(RGBQUAD)];
//	for (int i = 0; i < (input.filehead.bfOffBits - 54) / sizeof(RGBQUAD); i++)
//	{
//		pcolortable[i] = input.pcolortable[i];
//	}
//}

void IPcode1::SaveBmp(char * bmpname) 
{
	
	SaveBmpTool(bmpname, pbmpBuf, infohead.biWidth, infohead.biHeight, infohead.biBitCount);
	

	//if (pbmpBuf == NULL)
	//	return;
	////颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
	//int colorTablesize = filehead.bfOffBits-54;

	//if (infohead.biBitCount == 8)
	//{
	//	if (colorTablesize != 1024)
	//	{
	//		cout << __FILE__ << __LINE__ << endl;
	//		throw exception("灰度图像的颜色表不为1024字节");//可能真的有灰度图像的颜色表不为1024字节
	//	}
	//	else
	//		cout << "灰度图像的颜色表确实为1024字节" << endl;
	//}
	//	
	//int lineBytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4; //这个应该是用不到 

	//FILE *fp = fopen(bmpname, "wb");
	//if (fp == NULL)
	//	return;
	////if (infohead.biHeight*lineBytes != infohead.biSizeImage)
	//	//throw exception("其他地方肯定出现问题了");

	//filehead.bfType = 0x4d42;//bmp类型 如果换成其他的如0x4b42 那么就显示不支持此类型
	//filehead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + infohead.biSizeImage;
	//filehead.bfReserved1 = 0;
	//filehead.bfReserved2 = 0;
	//filehead.bfOffBits = 54 + colorTablesize;
	//fwrite(&filehead, sizeof(BITMAPFILEHEADER), 1, fp);

	////infohead在修改图像数据内容时就应该被修改过一遍了,同时不被修改的应该保持原属性不变
	////infohead.biBitCount = biBitCount;
	////infohead.biClrImportant = 0;
	////infohead.biClrUsed = 0;
	////infohead.biCompression = 0;
	////infohead.biHeight = height;
	////infohead.biPlanes = 1;
	////infohead.biSize = 40;
	////infohead.biSizeImage = lineByte*height; 
	////infohead.biWidth = width;
	////infoheadhead.biXPelsPerMeter = 0;
	////infoheadhead.biYPelsPerMeter = 0;
	//fwrite(&infohead, sizeof(BITMAPINFOHEADER), 1, fp);

	//if (colorTablesize!=0)
	//	fwrite(pcolortable, 1, colorTablesize, fp);

	//fwrite(pbmpBuf, infohead.biSizeImage, 1, fp);

	//fclose(fp);
}

void IPcode1::Transfer(void (*pf)(unsigned char*, unsigned char*, unsigned char*))
{
	if (infohead.biBitCount == 24)
	{
		for (auto x = 0; x < infohead.biWidth; x++)
		{
			for (auto y = 0; y < infohead.biHeight; y++)
			{
				int bitlocation;
				auto pixelvalue = GetPixel(x, y, bitlocation);
				if (pixelvalue.size() != 3)
				{
					cout << __FILE__ << __LINE__;
					throw exception("按理说是不应该出现不是3的情况的");
				}
				else
					pf(pixelvalue[0], pixelvalue[1], pixelvalue[2]); //按照入口函数指针进行相应转化
			}
		}
	}
	else
		throw exception("暂时无法处理这种形式的图片");
}

void IPcode1::RGB2YIQ(unsigned char *a, unsigned char* b, unsigned char* c)
{
	unsigned char d = 0.299 * (*a) + 0.587* (*b) + 0.114* (*c);
	unsigned char e = 0.596 * (*a) - 0.274* (*b) - 0.322* (*c);
	unsigned char f = 0.211 * (*a) - 0.523* (*b) + 0.312* (*c);
	if (!(CheckRange0_255(d) && CheckRange0_255(e) && CheckRange0_255(f)))
		throw exception("转化算法有问题，得到的值超限了 0-255");
	*a = d;
	*b = e;
	*c = f;
}

void IPcode1::RGB2YCrCb(unsigned char *a, unsigned char* b, unsigned char* c)
{
	unsigned char d = 0.299 * (*a) + 0.587* (*b) + 0.114* (*c);
	unsigned char e = 0.500 * (*a) - 0.4187* (*b) - 0.0813* (*c);
	unsigned char f = -0.1687 * (*a) - 0.3313* (*b) + 0.500* (*c);
	if (!(CheckRange0_255(d) && CheckRange0_255(e) && CheckRange0_255(f)))
		throw exception("转化算法有问题，得到的值超限了 0-255");
	*a = d;
	*b = e;
	*c = f;
}

void IPcode1::RGB2XYZ(unsigned char *a, unsigned char* b, unsigned char* c)
{
	unsigned char d = 0.490 * (*a) + 0.310* (*b) + 0.200* (*c);
	unsigned char e = 0.177 * (*a) + 0.813* (*b) + 0.011* (*c);
	unsigned char f = 0.000 * (*a) + 0.010* (*b) + 0.990* (*c);
	if (!(CheckRange0_255(d) && CheckRange0_255(e) && CheckRange0_255(f)))
		throw exception("转化算法有问题，得到的值超限了 0-255");
	*a = d;
	*b = e;
	*c = f;
}

void IPcode1::RGB2GrayValue(unsigned char *a, unsigned char* b, unsigned char* c)
{
	unsigned char d = 0.3 * (*a) + 0.59* (*b) + 0.11* (*c);
	unsigned char e = 0.3 * (*a) + 0.59* (*b) + 0.11* (*c);
	unsigned char f = 0.3 * (*a) + 0.59* (*b) + 0.11* (*c);
	if (!(CheckRange0_255(d) && CheckRange0_255(e) && CheckRange0_255(f)))
		throw exception("转化算法有问题，得到的值超限了 0-255");
	*a = d;
	*b = e;
	*c = f;
}

void IPcode1::YIQ2RGB(unsigned char *a, unsigned char* b, unsigned char* c)
{
	unsigned char d = 1.0 * (*a) + 0.956* (*b) + 0.621* (*c);
	unsigned char e = 1.0 * (*a) - 0.272* (*b) - 0.647* (*c);
	unsigned char f = 1.0 * (*a) - 1.106* (*b) - 1.703* (*c);
	if (!(CheckRange0_255(d) && CheckRange0_255(e) && CheckRange0_255(f)))
		throw exception("转化算法有问题，得到的值超限了 0-255");
	*a = d;
	*b = e;
	*c = f;
}

void IPcode1::RGB2HSI(unsigned char *a, unsigned char* b, unsigned char* c)
{
	double R = (*a) / 255.0;
	double G = (*b) / 255.0;
	double B = (*c) / 255.0;
	double H = acos((((R - G) + (R - B)) / 2)/(pow(pow(R-G,2)+(R-B)*(G-B),0.5)+ dt));
	if (G < B)
		H = 2 * pi - H;
	H = H / (2 * pi);
	double S = 1 - 3 * (min(min(R, G), B)) / (R + G + B+ dt);
	double I = 1.0 / 3.0*(R + G + B);

	H = H*255.0;
	S = S*255.0;
	I = I*255.0;
	if (!(CheckRange0_255(H)&& CheckRange0_255(S)&& CheckRange0_255(I)))
		throw exception("转化算法有问题，得到的HSI值超限了");
	*a = H;
	*b = S;
	*c = I;
}

bool IPcode1::CheckRange0_255(double a)
{
	if (a < 0 || a >= 256)
		return FALSE;
	else
		return TRUE;
}

unsigned char * IPcode1::extractDataChannel(int channel, DWORD &sizeimage)
{
	if (infohead.biBitCount == 8)
	{
		if (channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (channel != 0 && channel != 1 && channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	if (channel == 0 || channel == 1 || channel == 2)
	{
		int linebytes = (infohead.biWidth * 8 / 8 + 3) / 4 * 4;
		sizeimage = linebytes*infohead.biHeight;
		unsigned char * result = new unsigned char[sizeimage];
		DWORD ByteCount = 0;
		for (auto y = 0; y < infohead.biHeight; y++)
		{
			for (auto x = 0; x < infohead.biWidth; x++)
			{
				int bitlocation;
				auto pixel = GetPixel(x, y, bitlocation);
				result[ByteCount] = *pixel[channel];
				ByteCount++;
			}
			if (infohead.biWidth % 4 != 0)
			{
				for (int i = 0; i < 4 - (infohead.biWidth % 4); i++)
				{
					result[ByteCount] = 0x00;
					ByteCount++;
				}
			}
		}
		return result;
	}
	else
		throw exception("程序逻辑写的有问题");
}

void IPcode1::SaveChannel(char * path, int channel)
{
	DWORD sizeimage;
	unsigned char * Channeldata = extractDataChannel(channel, sizeimage);
	SaveBmpTool(path, Channeldata, infohead.biWidth, infohead.biHeight, 8);//储存灰度图
	delete[]Channeldata;
}

void IPcode1::SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount)const
//必须保证imgBuf的值等于lineByte*height
{
	if (imgBuf == NULL)
		return;
	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
	int colorTablesize;
	if (biBitCount == 24)
		colorTablesize = 0;
	else if (biBitCount == 8)
		colorTablesize = 1024;
	else
		throw exception("暂时无法处理其他位数的储存");

	////待存储图像数据每行字节数为4的倍数
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//以二进制写的方式打开文件
	FILE *fp = fopen(path, "wb");

	if (fp == 0)
		throw exception("无法打开对应命名的文件");

	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp类型
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = 54 + colorTablesize;
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head;
	head.biBitCount = biBitCount;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lineByte*height;
	head.biWidth = width;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	////如果灰度图像，有颜色表，写入文件 
	if (biBitCount == 8)
	{
		RGBQUAD rgbquad[256];
		for (int i = 0; i<256; i++)
		{
			rgbquad[i].rgbBlue = i;
			rgbquad[i].rgbGreen = i;
			rgbquad[i].rgbRed = i;
			rgbquad[i].rgbReserved = 0;
		}
		fwrite(rgbquad, sizeof(RGBQUAD), 256, fp);
	}
		

	//写位图数据进文件
	fwrite(imgBuf, lineByte*height, 1, fp);

	//关闭文件
	fclose(fp);

	cout << string(path) << " 被保存了" << endl;
}

vector<unsigned char> IPcode1::ReadPixel(int x, int y, int & bitlocation)
{
	vector<unsigned char *> vecptr = GetPixel(x, y, bitlocation);
	vector<unsigned char> result;
	for (int i = 0; i < vecptr.size(); i++)
		result.push_back(*vecptr[i]);
	return result;
}

void IPcode1::makeBmpTimesof(unsigned int widthtimes, unsigned int heighttimes)
{
	if (widthtimes <= 0 || heighttimes <= 0 )//|| widthtimes > infohead.biWidth || heighttimes > infohead.biHeight) //这个应该可以变大
		throw exception("widthtimes、heighttimes设置不对");
	if (infohead.biBitCount == 8 || infohead.biBitCount == 24)
	{
		if (infohead.biWidth % widthtimes == 0 && infohead.biHeight % heighttimes == 0) //如果本来就是widthtimes*heighttimes整数倍的，那么什么都不需要做
			return;
		else
		{
			WORD linebytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;
			if (infohead.biSizeImage != linebytes*infohead.biHeight)
				cout << "warning:图片数据的每行字节数不是4的倍数" << infohead.biSizeImage << " " << linebytes*infohead.biHeight << endl;
				/*throw exception("需要先保证图片数据的每行字节数是4的倍数");*/
			LONG newbiWidth = (infohead.biWidth + widthtimes-1) / widthtimes * widthtimes;
			WORD newLinebytes = (newbiWidth * infohead.biBitCount / 8 + 3) / 4 * 4; //保证字节数是4的倍数
			LONG newbiHeight = (infohead.biHeight + heighttimes-1) / heighttimes * heighttimes;
			if (newLinebytes < linebytes)
				throw exception("需要重新思考下边的逻辑，按理说不应该出现这种情况");

			unsigned char * result = new unsigned char[newbiHeight*newLinebytes];
			DWORD offsetOrigin = 0;
			DWORD offsetNew = 0;
			for (auto i = 0; i < infohead.biHeight; i++)
			{
				offsetOrigin = i*linebytes;
				offsetNew = i * newLinebytes;
				int j;
				for (j = 0; j < linebytes; j++)
				{
					result[offsetNew + j] = pbmpBuf[offsetOrigin + j];
				}
				for (auto k = 0; k < newLinebytes - linebytes; k++)
				{
					result[offsetNew + j + k] = 0x00;
				}
			}

			for (auto i = infohead.biHeight; i < newbiHeight; i++)
			{
				offsetNew = i*newLinebytes;
				for (auto j = 0; j < newLinebytes; j++)
					result[offsetNew + j] = 0x00;
			}
			delete[]pbmpBuf;//先把原来的释放了
			pbmpBuf = result;
			infohead.biHeight = newbiHeight;
			infohead.biSizeImage = newbiHeight*newLinebytes;
			infohead.biWidth = newbiWidth;
		}
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}
}

double IPcode1::KernalTemplate_Multiple_subimage_x(int channel, LONG Globalx, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const
//这个函数就不进行异常检测了，需要上层调用的函数保证入口参数是对的！
{
	double result = 0;
	int halfwidth = (Kernelwidth - 1) / 2;
	int halfheight = (Kernelheight - 1) / 2;
	
	for (int y = CenterY - halfheight; y <= CenterY + halfheight; y++)
	{
		unsigned int pixelnum;
		if (Globalx < 0 || Globalx >= infohead.biWidth || y < 0 || y >= infohead.biHeight)
		{
			pixelnum = 0;//补全值为0
		}
		else
		{
			int bitloc;
			auto vecpix = GetPixel(Globalx, y, bitloc);
			pixelnum = *vecpix[channel];
		}
		result += pixelnum * KernalTemplate[(y - CenterY + halfheight)*Kernelwidth + Globalx - CenterX + halfwidth];
	}
	return result;
}

double IPcode1::KernalTemplate_Multiple_subimage_y(int channel,LONG Globaly, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const
//这个函数就不进行异常检测了，需要上层调用的函数保证入口参数是对的！
{
	double result = 0;
	int halfwidth = (Kernelwidth - 1) / 2;
	int halfheight = (Kernelheight - 1) / 2;
	int count = 0;
	for (int x = CenterX - halfwidth; x <= CenterX + halfwidth; x++)
	{
		unsigned int pixelnum;
		if (x < 0 || x >= infohead.biWidth || Globaly < 0 || Globaly >= infohead.biHeight)
		{
			pixelnum = 0;//补全值为0
		}
		else
		{
			int bitloc;
			auto vecpix = GetPixel(x, Globaly, bitloc);
			pixelnum = *vecpix[channel];
		}
		result += pixelnum * KernalTemplate[(Globaly - CenterY + halfheight)*Kernelwidth + (count++)];
	}
	return result;
}

double IPcode1::KernalTemplate_Multiple_subimage(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const
//不推荐连续重复调用 不高效 
{
	if (infohead.biBitCount == 8)
	{
		if (channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (channel != 0 && channel != 1 && channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}
	if (KernalTemplate.size() != Kernelwidth*Kernelheight)
		throw exception("模板核的大小不对");

	if (Kernelwidth % 2 == 0 || Kernelheight % 2 == 0)
		throw exception("暂时无法处理是偶数的情况");


	
	int halfheight = (Kernelheight - 1) / 2;
	
	double result = 0;
	for (int y = CenterY - halfheight; y <= CenterY + halfheight; y++)
	{
		result += KernalTemplate_Multiple_subimage_y(channel, y, Kernelwidth, Kernelheight, KernalTemplate, CenterX, CenterY);
	}
	return result;
}

vector<double> IPcode1::AVGKernelTemplate(unsigned int M, unsigned int N)
{
	if (M*N == 0)
		throw exception("输入参数不对");
	vector<double> result(M*N);
	for (int i = 0; i < M*N; i++)
	{
		result[i] = 1.0 / (M*N);
	}

	return result;
}

void IPcode1::Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, bool ChangeDimension, string Mode, int stepx, int stepy)
{
	if (infohead.biBitCount == 8)
	{
		if (channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (channel != 0 && channel != 1 && channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	if (Kernelwidth % 2 == 0 || Kernelheight % 2 == 0)
		throw exception("暂时无法处理是偶数的情况");

	if (stepy > Kernelheight || stepx > Kernelwidth)
		throw exception("不能处理stepy>Kernelheight||stepx>Kernelwidth");

	double * midresult = new double[infohead.biWidth*infohead.biHeight];

	vector<double> Storesubimage(Kernelwidth*Kernelheight);
	int halfheight = (Kernelheight - 1) / 2;
	int halfwidth = (Kernelwidth - 1) / 2;
	for (LONG ycenter = 0; ycenter < infohead.biHeight;  ycenter = ycenter + stepy)
	{
		for (LONG xcenter = 0; xcenter < infohead.biWidth; xcenter = xcenter + stepx)
		{
			if (Mode == "median"||Mode == "max") //中值最大值什么的用这个
			{
				size_t count = 0;
				for (LONG y = ycenter - halfheight; y <= ycenter + halfheight; y++)
				{
					for (LONG x = xcenter - halfwidth; x <= xcenter + halfwidth; x++)
					{
						if (x < 0 || x >= infohead.biWidth || y < 0 || y >= infohead.biHeight)
						{
							Storesubimage[count++] = 0;
						}
						else
						{
							int bitloc;
							auto vecpix = GetPixel(x, y, bitloc);
							Storesubimage[count++] = *vecpix[channel];
						}
					}
				}
				if (Mode == "median")
				{
					sort(Storesubimage.begin(), Storesubimage.end());
					midresult[xcenter + ycenter*infohead.biWidth] = Storesubimage[(Kernelwidth*Kernelheight) / 2];
				}
				else if (Mode == "max")
				{
					midresult[xcenter + ycenter*infohead.biWidth] = *max_element(Storesubimage.begin(), Storesubimage.end());
				}
				else
					throw exception("漏定义了一个模式");
			}
			else if (Mode == "sobel") 
			{
				//不加static用时117.126秒
				//加static用时114.88秒 还是有点作用的
				double tmpsobel = 0;
				static const vector<double> Sobel1 = { 1,2,1,0,0,0,-1,-2,-1 };
				tmpsobel += abs(KernalTemplate_Multiple_subimage(channel, Kernelwidth, Kernelheight, Sobel1, xcenter, ycenter));
				static const vector<double> Sobel2 = { -1,0,1,-2,0,2,-1,0,1 };
				tmpsobel += abs(KernalTemplate_Multiple_subimage(channel, Kernelwidth, Kernelheight, Sobel2, xcenter, ycenter));
				midresult[xcenter + ycenter*infohead.biWidth] = tmpsobel;
			}
			else
				throw exception("未定义的Mode");
		}
	}

	//需要用户自己判断到底有没有改变量纲 一般统计排序都是没有改变量纲的 sobel算子应该是改变了量纲的，因为做的是一个微分
	if (ChangeDimension)
	{
		auto vecnorm = normalization_zk(midresult, midresult + infohead.biWidth*infohead.biHeight - 1);
		Denormalization_zk<double>(midresult, midresult + infohead.biWidth*infohead.biHeight - 1, 0, 255, vecnorm);
	}

	for (size_t i = 0; i < infohead.biWidth*infohead.biHeight; i++)
	{
		if (midresult[i] >= 256 || midresult < 0)
			throw exception("按理说到这个时候midresult只能是在0-255,可能是本来变了量纲的但是按没有变量纲处理了");
	}

	for (LONG y = 0; y < infohead.biHeight; y = y + stepy)
	{
		for (LONG x = 0; x < infohead.biWidth; x = x + stepx)
		{
			int bitloc;
			auto vecpix = GetPixel(x, y, bitloc);
			*vecpix[channel] = midresult[x + y*infohead.biWidth];
		}
	}
	delete[]midresult;
}

void IPcode1::Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight,bool ChangeDimension, const vector<double>& KernalTemplate, int stepx, int stepy)
{
	if (stepy > Kernelheight || stepx > Kernelwidth)
		throw exception("不能处理stepy>Kernelheight||stepx>Kernelwidth");

	double * midresult = new double[infohead.biWidth*infohead.biHeight];
	for (size_t i = 0; i < infohead.biWidth*infohead.biHeight; i++)
		midresult[i] = 0;

	int halfheight = (Kernelheight - 1) / 2;
	int halfwidth = (Kernelwidth - 1) / 2;

	double Minus = 0;
	double Plus = 0;
	midresult[0] = KernalTemplate_Multiple_subimage(channel, Kernelwidth, Kernelheight, KernalTemplate, 0, 0);
	for (LONG x = 0; x < infohead.biWidth; )
	{
		for (LONG y = stepy; y < infohead.biHeight; y=y+stepy)
		{
			//当stepy比较小时这样做是高效的，但是比较大时，重叠度不高时就不高效了。
			Minus = 0;
			for (int i = 0; i < stepy; i++)
				Minus += KernalTemplate_Multiple_subimage_y(channel, y - stepy - halfheight + i, Kernelwidth, Kernelheight, KernalTemplate, x, y - stepy);
			Plus = 0;
			for (int i = 0; i < stepy; i++)
				Plus += KernalTemplate_Multiple_subimage_y(channel, y + halfheight - i, Kernelwidth, Kernelheight, KernalTemplate, x, y);
			midresult[x + y*infohead.biWidth] = midresult[x + (y - stepy)*infohead.biWidth] + Plus - Minus;
		}
		x = x + stepx;

		Minus = 0;
		for (int i = 0; i < stepx; i++)
			Minus += KernalTemplate_Multiple_subimage_x(channel, x - stepx - halfwidth + i, Kernelwidth, Kernelheight, KernalTemplate, x - stepx, 0);
		Plus = 0;
		for (int i = 0; i < stepx; i++)
			Plus += KernalTemplate_Multiple_subimage_x(channel, x + halfwidth - i, Kernelwidth, Kernelheight, KernalTemplate, x, 0);
		midresult[x + 0 * infohead.biWidth] = midresult[x - stepx] + Plus - Minus;
	}

	//这个归一化对于量纲不变的变换并不需要  比如均值滤波和中值滤波量纲就没有改变
	if (ChangeDimension)
	{
		auto vecnorm = normalization_zk(midresult, midresult + infohead.biWidth*infohead.biHeight - 1);
		Denormalization_zk<double>(midresult, midresult + infohead.biWidth*infohead.biHeight - 1, 0, 255, vecnorm);
	}

	for (size_t i = 0; i < infohead.biWidth*infohead.biHeight; i++)
	{
		if (midresult[i] >= 256 || midresult < 0)
			throw exception("按理说到这个时候midresult只能是在0-255,检查一下上边的逻辑是否出错");
	}

	for (LONG y = 0; y < infohead.biHeight; y = y + stepy)
	{
		for (LONG x = 0; x < infohead.biWidth; x = x + stepx)
		{
			int bitloc;
			auto vecpix = GetPixel(x, y, bitloc);
			*vecpix[channel] = midresult[x + y*infohead.biWidth];
		}
	}
	delete[]midresult;
}

void IPcode1::Kernel_image(int channel,unsigned int Kernelwidth,unsigned int Kernelheight, string Operation, string inmode,string outmode,double Numin ) //mode表明是保留幅值还是保留相位 0表示保留幅值1表示保留相位
{
	if (infohead.biBitCount == 8)
	{
		if (channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (channel != 0 && channel != 1 && channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	if (channel == 0 || channel == 1 || channel == 2)
	{
		makeBmpTimesof(Kernelwidth, Kernelheight); //先保证图片的长和宽都是所输入参数的倍数
		double * amplitudeStore = NULL;
		double * phaseStore = NULL;
		if (outmode == "ampl")
		{
			amplitudeStore = new double[infohead.biWidth*infohead.biHeight];
		}
		else if (outmode == "phase")
		{
			phaseStore = new double[infohead.biWidth*infohead.biHeight];
		}
		else
			throw exception("未定义outmode");
		
		vector<complex<double>> DFTpart(Kernelwidth*Kernelheight); // 大小应该是width*height
		for (LONG j = 0; j < infohead.biHeight / Kernelheight; j++)
		{
			for (LONG i = 0; i < infohead.biWidth / Kernelwidth; i++)
			{
				auto xoffset = i*Kernelwidth;
				auto yoffset = j*Kernelheight;
				int DFTCount = 0;
				
				for (unsigned int y = 0; y < Kernelheight; y++)
				{
					for (unsigned int x = 0; x < Kernelwidth; x++)
					{
						int bitlocation;
						auto pixel = GetPixel(xoffset + x, yoffset + y, bitlocation);
						DFTpart[DFTCount++] = *pixel[channel];
					}
				}
				if (DFTpart.size() != Kernelwidth*Kernelheight )
					throw exception("程序逻辑出错");

				if (Operation == "DFT")
				{
					for (unsigned int y = 0; y < Kernelheight; y++)
						for (unsigned int x = 0; x < Kernelwidth; x++)
							DFTpart[x + y*Kernelwidth] = DFTpart[x + y*Kernelwidth] * pow(-1, x + y);
					//需要事先平移到Kernelwidth Kernelheight的中心 P155
					DFTpart = Operate::FFT_2D(DFTpart, Kernelwidth, Kernelheight);
				}
				else if (Operation == "DFT_IDFT")
				{
					for (unsigned int y = 0; y < Kernelheight; y++)
						for (unsigned int x = 0; x < Kernelwidth; x++)
							DFTpart[x + y*Kernelwidth] = DFTpart[x + y*Kernelwidth] * pow(-1, x + y);
					//需要事先平移到Kernelwidth Kernelheight的中心 P155

					DFTpart = Operate::FFT_2D(DFTpart, Kernelwidth, Kernelheight);

					if (inmode == "phase") //相角重建
					{
						for (unsigned int i = 0; i < Kernelheight*Kernelwidth; i++)
						{
							ComplexEXP tmp(DFTpart[i]);
							DFTpart[i] = ComplexEXP(1, tmp.read_phase()).readcomp();
						}
					}
					else if (inmode == "ampl") //谱重建
					{
						for (unsigned int i = 0; i < Kernelheight*Kernelwidth; i++)
						{
							ComplexEXP tmp(DFTpart[i]);
							DFTpart[i] = ComplexEXP(tmp.read_amplitude(), 0).readcomp();
						}
					}
					else
						throw exception("输入了未定义的inmode");
					DFTpart = Operate::IFFT_2D(DFTpart, Kernelwidth, Kernelheight);

					for (unsigned int y = 0; y < Kernelheight; y++)
						for (unsigned int x = 0; x < Kernelwidth; x++)
							DFTpart[x + y*Kernelwidth] = DFTpart[x + y*Kernelwidth] / pow(-1, x + y);
					//结束后在变回来
				}
				else if (Operation == "IDFT") //也与IDCT有同样的问题，输入的数据不是原始数据，是经过0-255变换或者对数变换的数据,要想研究谱重建和相角重建，就应该写一个类似DCT_NUMIN_IDCT的功能,除非输入数据是原始的DFT变换后的数据
				{	
					if (inmode == "phase")//只有IDFT关注inmode 
					{
						vector<double> phasevec(DFTpart.size());
						for (int thisi = 0; thisi < phasevec.size(); thisi++)
							phasevec[thisi] = DFTpart[thisi].real();
						double maxelement = *max_element(phasevec.begin(), phasevec.end());
						double minelement = *min_element(phasevec.begin(), phasevec.end());
						for (int thisi = 0; thisi < phasevec.size(); thisi++)
							phasevec[thisi] = abs(double(phasevec[thisi] - minelement)) / abs(double(maxelement - minelement));

						double lower = -pi;
						double upper = pi;
						size_t count = 0;
						for (auto iterator = DFTpart.begin(); iterator != DFTpart.end(); iterator++)
						{
							*iterator = ComplexEXP(1, lower + double(upper - lower)*phasevec[count++]).readcomp();
						}
					}
					else if (inmode == "ampl") {}
					else
						throw exception("输入了未定义的inmode");
					DFTpart = Operate::IFFT_2D(DFTpart, Kernelwidth, Kernelheight);
				}
				else if (Operation == "Black")
				{
					for (int i = 0; i < DFTpart.size(); i++)
						DFTpart[i] = 0;
					DFTpart[0] = 255;
					//DFTpart[DFTpart.size() - 1] = 255;
				}
				else if (Operation == "normal")
				{
					DFTpart = Operate::FFT_2D(DFTpart, Kernelwidth, Kernelheight);
					DFTpart = Operate::IFFT_2D(DFTpart, Kernelwidth, Kernelheight);//测试FFT有没有写对 经过一正一逆如果没有任何改变的话说明就对的
					//cout << "测试FFT有没有写对 经过一正一逆如果没有任何改变的话说明就对的";
				}
				else if (Operation == "DCT") 
				{
					if (Kernelwidth != Kernelheight)
						throw exception("无法处理width != height的情况");
					if (Numin > Kernelwidth*Kernelwidth)
						throw exception("保留的DCT系数个数不对");
					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::DCT_2D(tmpDCT, Kernelwidth);

					for (int i = 0; i < tmpDCT.size(); i++)
						tmpDCT[i] = abs(tmpDCT[i]);
					//都搞到正数上去,显示的DCT越量，说明能量越高

					Operate::zigzagRetainDCT(tmpDCT, Kernelwidth, Numin);
					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);
				}
				else if (Operation == "IDCT") //这其实是有问题的，因为输入是DCT经过绝对值变换后的图像，只有正数，但这并不代表IDCT这个算法不对，算法是对的
				{
					throw exception("IDCT的显示在当前储存图像的像素数据unsigned char下是有问题的，因为输入是DCT经过绝对值变换后的图像，只有正数");
					if (Kernelwidth != Kernelheight)
						throw exception("无法处理width != height的情况");
					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::IDCT_2D(tmpDCT, Kernelwidth);

					auto minelement = *min_element(tmpDCT.begin(), tmpDCT.end());
					if (minelement < 0)
					{
						minelement = abs(minelement);
						for (int i = 0; i < tmpDCT.size(); i++)
							tmpDCT[i] += minelement;
					}//都搞到正数上去

					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);
				}
				else if (Operation == "DCT_Numin_IDCT")
				{
					if (Kernelwidth != Kernelheight)
						throw exception("无法处理width != height的情况");
					if (Numin > Kernelwidth*Kernelwidth)
						throw exception("保留的DCT系数个数不对");

					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::DCT_2D(tmpDCT, Kernelwidth);
					Operate::zigzagRetainDCT(tmpDCT, Kernelwidth, Numin);
					tmpDCT = Operate::IDCT_2D(tmpDCT, Kernelwidth);
					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);
				}
				else if (Operation == "normalDCT")
				{
					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::DCT_2D(tmpDCT, Kernelwidth);
					tmpDCT = Operate::IDCT_2D(tmpDCT, Kernelwidth);
					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);  //现在才是没有问题的
				}
				else
					throw exception("输入的Operation不对，只能在特定范围里选");

				DFTCount = 0;
				for (unsigned int y = 0; y < Kernelheight; y++)
				{
					for (unsigned int x = 0; x < Kernelwidth; x++)
					{
						auto locx = xoffset + x;
						auto locy = yoffset + y;
						ComplexEXP tmp(DFTpart[DFTCount++]);

						if (outmode == "ampl")
						{
							amplitudeStore[locx + locy*infohead.biWidth] = tmp.read_amplitude();
						}
						else if (outmode == "phase")
						{
							phaseStore[locx + locy*infohead.biWidth] = tmp.read_phase();
						}
					}
				}
			}
		}

		if (outmode == "ampl")
		{
			if (Operation == "DFT")
			{
				for (unsigned int i = 0; i < infohead.biWidth*infohead.biHeight; i++)
				{
					if (amplitudeStore[i] < 1)
						amplitudeStore[i] = 0;
					else
						amplitudeStore[i] = 1 + log(abs(amplitudeStore[i]));
				}
				//DFT的动态范围非常大，需要取对数
			}
			double * begin = amplitudeStore;
			double * end = amplitudeStore + (infohead.biWidth*infohead.biHeight - 1);
			vector<double> normal_list = normalization_zk<double>(begin, end);
			Denormalization_zk<double>(begin, end, 0, 255, normal_list);
		}
		else if (outmode == "phase")
		{
			double * begin = phaseStore;
			double * end = phaseStore + (infohead.biWidth*infohead.biHeight - 1);
			vector<double> normal_list = normalization_zk<double>(begin, end);
			Denormalization_zk<double>(begin, end, 0, 255, normal_list);
		}

		int count = 0;
		for (LONG y = 0; y < infohead.biHeight; y++)
		{
			for (LONG x = 0; x < infohead.biWidth; x++)
			{
				unsigned char value;
				if (outmode == "ampl")
				{
					value = amplitudeStore[count++];
				}
				else if (outmode == "phase")
				{
					value = phaseStore[count++];
				}
				int bitlocation;
				auto ptr = GetPixel(x, y, bitlocation);
				*ptr[channel] = value;
			}
		}
		if (amplitudeStore != NULL)
			delete[]amplitudeStore;
		if (phaseStore != NULL)
			delete[]phaseStore;
	}
}

void IPcode1::MakeGray8_TO_BinaryImage_HalfTone()
{
	if (infohead.biBitCount != 8)
		throw exception("暂时无法处理infohead.biBitCount!=8的情况");

	vector<LONG> ld = { 0,0 };
	vector<LONG> lu = { 0,infohead.biHeight - 1 };
	vector<LONG> rd = { infohead.biWidth - 1,0 };
	vector<LONG> ru = { infohead.biWidth - 1,infohead.biHeight - 1 };
	Normalization_Image(0, ld, lu, rd, ru);

	vector<double> ChangeValueRecord(infohead.biWidth*infohead.biHeight, 0); //需要修改的值的大小

	int bitlocation;
	vector<int> xoffset = { 1,-1,0,1 };
	vector<int> yoffset = { 0,-1,-1,-1 };
	vector<double> coef = { 7.0 / 16.0,3.0 / 16.0,5.0 / 16.0,1.0 / 16.0 }; //这三个vector size必须一样
	if (!(xoffset.size() == yoffset.size() && yoffset.size() == coef.size()))
		throw exception("三个vector size必须一样");
	vector<unsigned char*> pixCurrent;

	for (LONG y = 1; y < infohead.biHeight; y=y+3)
	{
		for (LONG x = 1; x < infohead.biWidth-1; x=x+4)
		{
			pixCurrent = GetPixel(x, y, bitlocation);

			//auto pixRight = GetPixel(x+1, y, bitlocation);
			//auto pixLeftDown = GetPixel(x-1, y-1, bitlocation);
			//auto pixDown = GetPixel(x, y-1, bitlocation);
			//auto pixRightDown = GetPixel(x+1, y-1, bitlocation);
			//vector<vector<unsigned char*>> pixstore = { pixRight ,pixLeftDown ,pixDown ,pixRightDown };
						
			int error = 0;
			if (*pixCurrent[0] >= 255 / 2)
			{
				error = 255 - (*pixCurrent[0]);
			}
			else
			{
				error = 0 - (*pixCurrent[0]);
			}

			LONG ytmp;
			LONG xtmp;
			for (size_t i = 0; i < xoffset.size(); i++)
			{
				ytmp = y + yoffset[i];
				xtmp = x + xoffset[i];
				ChangeValueRecord[ytmp*infohead.biWidth + xtmp] += coef[i] * (-error);
			}
		}
	}

	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			pixCurrent = GetPixel(x, y, bitlocation);
			double tmpvalue = *pixCurrent[0] + ChangeValueRecord[y*infohead.biWidth + x];
			if (tmpvalue < 255 / 2)
				tmpvalue = 0;
			else
				tmpvalue = 255;
				
			*pixCurrent[0] = tmpvalue;
		}
	}
}

void IPcode1::MakeGray8_TO_BinaryImage_DirectThreshold()
{
	if (infohead.biBitCount != 8)
		throw exception("暂时无法处理infohead.biBitCount!=8的情况");
	vector<LONG> ld = { 0,0 };
	vector<LONG> lu = { 0,infohead.biHeight - 1 };
	vector<LONG> rd = { infohead.biWidth - 1,0 };
	vector<LONG> ru = { infohead.biWidth - 1,infohead.biHeight - 1 };

	Normalization_Image(0, ld, lu, rd, ru);
	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			if (*pix[0] >= 255 / 2)
				*pix[0] = 255;
			else
				*pix[0] = 0;
		}
	}
}

void IPcode1::MakeRGB24_TO_Gray8()
{
	Transfer(RGB2GrayValue);
	DWORD sizeimage;
	unsigned char * singelchannel = extractDataChannel(0, sizeimage);
	delete[]pbmpBuf;
	pbmpBuf = singelchannel;
	infohead.biBitCount = 8;
	int linebytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;
	infohead.biSizeImage = infohead.biHeight*linebytes;
	if (sizeimage != infohead.biSizeImage)
		throw exception("逻辑有错");
}

void IPcode1::ChangeSingleChannelInputNEWptrData(unsigned char * newbmpdataptr, LONG width, LONG height, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, unsigned char value)
//以输入的单通道数据newbmpdataptr（新申请的地址，与成员数据无关）为起点，ld lu rd ru为待修改的像素方框,value为需要修改的值 
//width height为newbmpdataptr代表的图像的长和宽
{
	CheckRectangleInBigRect(ld, lu, rd, ru, width, height);

	WORD linebytes = (width * 8 / 8 + 3) / 4 * 4; //只能为单通道数据
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			newbmpdataptr[x + y*linebytes] = value;
		}
	}
}
void IPcode1::plot_DCTkernel_Image(int N) //也可以此函数为模板，绘制所有种类的变换核基图像 时间问题，不是技术问题 时间有限
{
	makeBmpTimesof(N, N);
	DWORD sizeimage;
	unsigned char * singelchannel = extractDataChannel(0, sizeimage);
	double* Value = new double[N*N]; //从左到右 从下到上储存每个分块图块的值

	for (int v = 0; v < N; v++)
	{
		for (int u = 0; u < N; u++)
		{
			for (int y = 0; y < N; y++)
			{
				for (int x = 0; x < N; x++)
				{
					if (u != 0 || v != 0)
						Value[x + y*N] = 1.0 / (2 * pow(N, 3))*cos((2 * x + 1)*u*pi)*cos((2 * y + 1)*v*pi);
					else
						Value[x + y*N] = 1.0 / N;
				}
			}
			double * begin = Value;
			double * end = Value + N*N - 1;
			Denormalization_zk<double>(begin, end, 0, 255, normalization_zk<double>(begin, end)); //归一化并化到0-255
			
			for (int valuey = 0; valuey < N; valuey++)
			{
				for (int valuex = 0; valuex < N; valuex++)
				{
					auto pixelwidthblock = infohead.biWidth / N;
					auto pixelLengthblock = infohead.biHeight / N;
					vector<LONG> ld = { valuex *pixelwidthblock,valuey*pixelLengthblock };
					vector<LONG> lu = { valuex *pixelwidthblock,(valuey+1)*pixelLengthblock -1 };
					vector<LONG> rd = { (valuex+1) *pixelwidthblock -1,valuey*pixelLengthblock };
					vector<LONG> ru = { (valuex + 1) *pixelwidthblock - 1,(valuey + 1)*pixelLengthblock - 1 };
					ChangeSingleChannelInputNEWptrData(singelchannel, infohead.biWidth, infohead.biHeight, ld, lu, rd, ru, Value[valuex+ valuey*N]);
				}
			}
			string filename = string("DCTkernel_Image_") + to_string(u)+string("_")+to_string(v)+string(".bmp");
			SaveBmpTool((char*)filename.data(), singelchannel, infohead.biWidth, infohead.biHeight, 8);//保存为灰度图,不修改本来的图片
		}
	}
	delete[]singelchannel;
	delete[]Value;
}

IPcode1 IPcode1::operator-(const IPcode1& input)const
{
	if (input.infohead.biBitCount != infohead.biBitCount)
		throw exception("参与相减的两张图片位数不一致");
	if (input.infohead.biWidth != infohead.biWidth || input.infohead.biHeight != infohead.biHeight)
		throw exception("参与相减的两张图片尺寸不一样");

	IPcode1 result(input);
	WORD linebytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;
	for (LONG i = 0; i < infohead.biHeight*linebytes; i++)
	{
		result.pbmpBuf[i] = abs(int(input.pbmpBuf[i]) - int(pbmpBuf[i]));
	}
	return result;
}

double IPcode1::PSNR(const IPcode1& input)const
{
	if (input.infohead.biBitCount != infohead.biBitCount)
		throw exception("PSNR的两张图片位数不一致");
	if (input.infohead.biWidth != infohead.biWidth || input.infohead.biHeight != infohead.biHeight)
		throw exception("PSNR的两张图片尺寸不一样");

	WORD linebytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;

	if (infohead.biBitCount == 8)
	{
		double MSE = 0;
		for (LONG i = 0; i < infohead.biHeight*linebytes; i++)
		{
			MSE += std::pow(double(pbmpBuf[i]) - double(input.pbmpBuf[i]), 2);
		}
		MSE /= infohead.biWidth*infohead.biHeight;
		double PSNR;
		PSNR = 20 * log10(255.0 / sqrt(MSE));
		return PSNR;
	}
	else
		throw exception("暂时还无法处理其他位数的PSNR");
	
}

double IPcode1::SSIM(const IPcode1& Origin,int Channel)const
{
	if (Origin.infohead.biBitCount != infohead.biBitCount)
		throw exception("SSIM的两张图片位数不一致");
	if (Origin.infohead.biWidth != infohead.biWidth || Origin.infohead.biHeight != infohead.biHeight)
		throw exception("SSIM的两张图片尺寸不一样");

	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	double meanx = Readmean(Channel);
	double meany = Origin.Readmean(Channel);
	double variancex = Readvariance(Channel);
	double variancey = Origin.Readvariance(Channel);
	double covariancexy = Readcovariance(Channel,Origin);

	double result = 0;
	double c1 = pow((0.01*255),2);
	double c2 = pow((0.03*255),2); //https://en.wikipedia.org/wiki/Structural_similarity
	result = (2 * meanx*meany + c1)*(2 * covariancexy + c2) / ((meanx*meanx + meany*meany + c1)*(variancex + variancey + c2));
	return result;
}

void IPcode1::CheckRectangleInBigRect( vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, LONG width, LONG height)const
{
	if (ld[0] != lu[0] || rd[0] != ru[0] || lu[1] != ru[1] || ld[1] != rd[1])
		throw exception("不是方框");
	if (ld[0] > rd[0] || ld[1] > lu[1])
		throw exception("设置的方框值不对");
	if (ld[0] < 0 || rd[0] >= width || ld[1] < 0 || lu[1] >= height)
		throw exception("设置的方框值超出width height了 ");
}

vector<double> IPcode1::Histogram(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru)
{
	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	CheckRectangleInBigRect(ld, lu, rd, ru, infohead.biWidth, infohead.biHeight);

	vector <double> result(256,0);
	LONG CountAll = 0;
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			result[*pix[Channel]]++; //统计个数
			CountAll++;
		}
	}
	for (size_t i = 0; i < result.size(); i++)
	{
		result[i] /= CountAll;
	}
	return result;
}

void IPcode1::Normalization_Image(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru)
//转化到0-255 定死了 否则还得加一些限制条件 时间有限 就不写了
{
	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	vector<unsigned char *>addrStore;
	int maxele = -9999;
	int minele = 9999;
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			addrStore.push_back(pix[Channel]);
			if (*pix[Channel] > maxele)
				maxele = *pix[Channel];
			if (*pix[Channel] < minele)
				minele = *pix[Channel];
		}
	}

	if (maxele == minele)
		return;
	else
	{
		int c = minele;
		int d = maxele;
		int a = 0;
		int b = 255;

		for (size_t i = 0; i < addrStore.size(); i++)
		{
			double tmp = *addrStore[i];
			*addrStore[i] = round((tmp - minele) / double(maxele - minele)*(b - a));
		}
	}
}

void IPcode1::Equalization_Image(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru)
{
	auto histo = Histogram(Channel, ld, lu, rd, ru);
	vector<double> Equalization(256, 0);
	for (int i = 0; i < Equalization.size(); i++)
	{
		double si = 0;
		for (int j = 0; j <= i; j++)
		{
			si += histo[j];
		}
		si *= 255;
		Equalization[i] = round(si);
	}
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			*pix[Channel] = Equalization[*pix[Channel]];
		}
	}
}

void IPcode1::AddNoise(int Channel,vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, string NoiseMode,double par0,double par1,double par2,double par3)
{

	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}
	
	CheckRectangleInBigRect( ld, lu, rd, ru, infohead.biWidth, infohead.biHeight);
	
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			double Noise = 0;
			if (NoiseMode == "Gaussian")
			{
				Noise = Operate::GaussianNoiseGenerator(par0, par1);
			}
			else if (NoiseMode == "Impulse")
			{
				Noise = Operate::ImpulseNoiseGenerator(par0, par1, par2, par3);
			}
			else
				throw exception("未定义此种噪声模式");
			
			double value = double(*pix[Channel]) + Noise;
			if (value > 255)
				value = 255;
			else if (value < 0)
				value = 0;
			*pix[Channel] = value;
		}
	}
}

double IPcode1::Readmean(int Channel)const
{
	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	double result = 0;
	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			result += *pix[Channel];
		}
	}
	result /= infohead.biHeight*infohead.biWidth;
}

double IPcode1::Readvariance(int Channel)const
{
	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	double Meanvalue = Readmean(Channel);

	double result = 0;
	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			result += pow((*pix[Channel] - Meanvalue), 2);
		}
	}
	result /= infohead.biHeight*infohead.biWidth - 1;
}

double IPcode1::Readcovariance(int Channel, const IPcode1& image)const
{
	if (image.infohead.biBitCount != infohead.biBitCount)
		throw exception("待处理的两图像位数不一样");
	if (image.infohead.biWidth != infohead.biWidth || image.infohead.biHeight != infohead.biHeight)
		throw exception("待处理的两图像尺寸不一样");

	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	double meanx = Readmean(Channel);
	double meany = image.Readmean(Channel);
	double result = 0;
	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			double valuex = *pix[Channel];
			auto piy = image.GetPixel(x, y, bitlocation);
			double valuey = *piy[Channel];
			result += (valuex - meanx)*(valuey - meany);
		}
	}
	result /= infohead.biHeight*infohead.biWidth - 1;
}

void IPcode1::MotionLinearBlur(int Channel,double xa,double yb,double T)
{
	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("灰度图只能由channel 0索引");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("三通道，不能超出三通道 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("暂时无法处理这时的情况");
	}

	makeBmpTimesof(2 * pow(2, ceil(log2(infohead.biWidth))), 2 * pow(2, ceil(log2(infohead.biHeight)))); 
	// 为了加快FFT的速度把图像扩成2的幂次 同时为了减少滤波时的混叠，把图像扩展两倍

	vector<complex<double>> image(infohead.biWidth*infohead.biHeight);
	size_t countimage = 0;
	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			int bitloc;
			auto pix = GetPixel(x, y, bitloc);
			image[countimage++] = pow(-1,x+y)*(*pix[Channel]); //变换图像的傅里叶变换到中心
		}
	}

	auto imageFFT = FFT_2D(image, infohead.biWidth, infohead.biHeight);
	vector<complex<double>> Filter(infohead.biWidth*infohead.biHeight);
	size_t FilterCount = 0;
	//double T = 1;
	double a = xa;
	double b = yb;
	for (LONG v = 0; v < infohead.biHeight; v++)
	{
		for (LONG u = 0; u < infohead.biWidth; u++)
		{
			auto uv = pi*(a*(u - infohead.biWidth / 2.0) + b*(v - infohead.biHeight / 2.0));
			Filter[FilterCount++] = ComplexEXP(T*sinxDividex(uv), -uv).readcomp();
		}
	}

	if (imageFFT.size() != Filter.size())
		throw exception("imageFFT.size()!=Filter.size()");

	for (size_t i = 0; i < imageFFT.size(); i++)
		imageFFT[i] = imageFFT[i] * Filter[i];

	image = IFFT_2D(imageFFT, infohead.biWidth, infohead.biHeight);

	for (LONG y = 0; y < infohead.biHeight; y++)
	{
		for (LONG x = 0; x < infohead.biWidth; x++)
		{
			int bitloc;
			auto pix = GetPixel(x, y, bitloc);
			*pix[Channel] = image[x + y*infohead.biHeight].real()*pow(-1, x + y);//变换回去
			//if (abs(image[x + y*infohead.biHeight].imag()) > 0.01)
				//cout << image[x + y*infohead.biHeight].imag() << endl; //测试代码
		}
	}
}
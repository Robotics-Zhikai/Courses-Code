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

bool IPcode1::CheckInImage(int x, int y)
{
	if ((x >= 0 && x < infohead.biWidth) && (y >= 0 && y < infohead.biHeight))
		return TRUE;
	else
		return FALSE;
}

vector<unsigned char *> IPcode1::GetPixel(int x, int y, int & bitlocation)
//只要返回，肯定返回值不空
{
	vector<unsigned char *> result;
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
		result.push_back(pbmpBuf + index);
	else if (single == 3)
	{
		result.push_back(pbmpBuf + index);
		result.push_back(pbmpBuf + index + 1);
		result.push_back(pbmpBuf + index + 2);
	}
	else
		throw("未定义处理此bitBitCount的程序");
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
	pbmpBuf = new unsigned char [input.infohead.biSizeImage];
	for (auto i = 0; i < input.infohead.biSizeImage; i++)
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
	if (pbmpBuf == NULL)
		return;
	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
	int colorTablesize = filehead.bfOffBits-54;

	if (infohead.biBitCount == 8)
	{
		if (colorTablesize != 1024)
		{
			cout << __FILE__ << __LINE__ << endl;
			throw exception("灰度图像的颜色表不为1024字节");//可能真的有灰度图像的颜色表不为1024字节
		}
		else
			cout << "灰度图像的颜色表确实为1024字节" << endl;
	}
		
	int lineBytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;

	FILE *fp = fopen(bmpname, "wb");
	if (fp == NULL)
		return;

	filehead.bfType = 0x4d42;//bmp类型 如果换成其他的如0x4b42 那么就显示不支持此类型
	filehead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineBytes*infohead.biHeight;
	filehead.bfReserved1 = 0;
	filehead.bfReserved2 = 0;
	filehead.bfOffBits = 54 + colorTablesize;
	fwrite(&filehead, sizeof(BITMAPFILEHEADER), 1, fp);

	//infohead在修改图像数据内容时就应该被修改过一遍了,同时不被修改的应该保持原属性不变
	//infohead.biBitCount = biBitCount;
	//infohead.biClrImportant = 0;
	//infohead.biClrUsed = 0;
	//infohead.biCompression = 0;
	//infohead.biHeight = height;
	//infohead.biPlanes = 1;
	//infohead.biSize = 40;
	//infohead.biSizeImage = lineByte*height; 
	//infohead.biWidth = width;
	//infoheadhead.biXPelsPerMeter = 0;
	//infoheadhead.biYPelsPerMeter = 0;
	fwrite(&infohead, sizeof(BITMAPINFOHEADER), 1, fp);

	if (colorTablesize!=0)
		fwrite(pcolortable, 1, colorTablesize, fp);

	fwrite(pbmpBuf, lineBytes*infohead.biHeight, 1, fp);

	fclose(fp);
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

void IPcode1::SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount)
//必须保证imgBuf的值等于lineByte*height
{
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
}

vector<unsigned char> IPcode1::ReadPixel(int x, int y, int & bitlocation)
{
	vector<unsigned char *> vecptr = GetPixel(x, y, bitlocation);
	vector<unsigned char> result;
	for (int i = 0; i < vecptr.size(); i++)
		result.push_back(*vecptr[i]);
	return result;
}

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
{
	vector<unsigned char *> result;
	bitlocation = -1;
	if (CheckInImage(x, y) == FALSE)
		return result;
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
	else if (leftdown_x >= width)
	{
		leftdown_x = width-1;
		leftup_x = width-1;
	}

	if (rightdown_x < 0)
	{
		rightdown_x = 0;
		rightup_x = 0;
	}
	else if (rightdown_x >= width)
	{
		rightdown_x = width - 1;
		rightup_x = width - 1;
	}

	if (leftdown_y < 0)
	{
		leftdown_y = 0;
		rightdown_y = 0;
	}
	else if (leftdown_y >= height)
	{
		leftdown_y = height - 1;
		rightdown_y = height - 1;
	}

	if (leftup_y < 0)
	{
		leftup_y = 0;
		rightup_y = 0;
	}
	else if (leftup_y >= height)
	{
		leftup_y = height - 1;
		rightup_y = height - 1;
	}

	bool flag = CheckInImage(leftdown_x, leftdown_y) && CheckInImage(rightdown_x, rightdown_y);
	flag = flag &&CheckInImage(leftup_x, leftup_y);
	flag = flag && CheckInImage(rightup_x, rightup_y);
	
	if (flag == FALSE)
		throw exception("程序逻辑出错");

	unsigned char * newpbmpBuf = new unsigned char[abs(height)*abs(width)];
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
				cout << __FILE__ << "/" << __LINE__;
				throw exception("程序逻辑出错,至此不应该出现data为空的情况");
			}
		}
		if (ByteCount % 4 != 0)
		{
			for (int i = 0; i < 4 - ByteCount % 4; i++)
			{
				newpbmpBuf[indexbmpbuf] = 0x00; //不足4字节的倍数就补全0x00
				indexbmpbuf++;
			}
		}
	}
	delete[]pbmpBuf;
	pbmpBuf = newpbmpBuf;
	infohead.biHeight = abs(height);
	infohead.biWidth = abs(width);
	int lineByte = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;
	infohead.biSizeImage = infohead.biHeight*lineByte;

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
	for (int i = 0; i < input.infohead.biSizeImage; i++)
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
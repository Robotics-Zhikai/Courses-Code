#include "IPcode1.h"


bool IPcode1::ReadBmp(char * name)
{
	FILE *fp = fopen(name, "rb");

	if (fp == 0)
		throw exception("��ȡʧ��");
	//BITMAPFILEHEADER filehead; // 14�ֽڵ��ļ�ͷ
	fread(&filehead, sizeof(BITMAPFILEHEADER), 1, fp);

	if (filehead.bfType != 0x4D42)
		throw exception("Ŀǰֻ�ܶ�ȡtmp��ʽ��ͼƬ���������޷���ȡ");
	fseek(fp, sizeof(BITMAPFILEHEADER), 0); //����BITMAPFILEHEADER 14�ֽڵ��ļ�ͷ

	//BITMAPINFOHEADER infohead; //40�ֽڵ���Ϣͷ
	//cout << sizeof(BITMAPINFOHEADER) << endl;
	fread(&infohead, sizeof(BITMAPINFOHEADER), 1, fp);

	auto imageWidth = infohead.biWidth; //ͼ��Ŀ�ȣ���λΪ����
	auto imageHeight = infohead.biHeight;//ͼ��ĸ߶�
	auto imageBitCount = infohead.biBitCount;//ָ����ɫʱ�õ���λ��

	if (filehead.bfOffBits > 54) // ˵���е�ɫ������
	{
		fseek(fp, 54, 0);
		pcolortable = new RGBQUAD[(filehead.bfOffBits - 54)/sizeof(RGBQUAD)];
		fread(pcolortable, filehead.bfOffBits - 54, 1, fp);
	}
		
	fseek(fp, filehead.bfOffBits, 0);//����ͷ�ļ�������ʵ�ʵ�λͼ����
	pbmpBuf = new unsigned char[infohead.biSizeImage];//����ͼ������
	fread(pbmpBuf, infohead.biSizeImage, 1, fp);

	fclose(fp);//�ر��ļ�
	return TRUE;
}

bool IPcode1::CheckInImage(int x, int y)
{
	if ((x >= 0 && x < infohead.biWidth) && (y >= 0 && y < infohead.biHeight))
		return TRUE;
	else
		return FALSE;
}

vector<unsigned long> IPcode1::GetPixel_Index(int x, int y, int & bitlocation)
//ֻҪ���أ��϶�����ֵ����
{
	vector<unsigned long> result;
	bitlocation = -1;
	if (CheckInImage(x, y) == FALSE)
	{
		cout << __FILE__ << __LINE__;
		throw exception("���ܶ�ȡ��ͼ��Χ֮������ص�����");
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
			throw exception("δ����bitBitCount,������");
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
		throw("δ���崦���bitBitCount�ĳ���");
	return result;
}

vector<unsigned char *> IPcode1::GetPixel(int x, int y, int & bitlocation)
//ֻҪ���أ��϶�����ֵ����
{
	vector<unsigned char *> result;
	bitlocation = -1;
	vector<unsigned long> resultIndex = GetPixel_Index(x, y, bitlocation);
	for (int i = 0; i < resultIndex.size(); i++)
		result.push_back(pbmpBuf + resultIndex[i]);
	return result;
}

void IPcode1::CropBmp(int locx, int locy, int width, int height)//��ȡBMP��ĳ������
//width����Ϊ�� height����Ϊ��
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
		throw exception("�����߼�����");

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
				if (bitlocation == -1) //˵����8λ��
				{
					newpbmpBuf[indexbmpbuf] = *data[0];
					indexbmpbuf++;
					ByteCount++;
				}
				else
				{
					cout << __FILE__ << "/" << __LINE__;
					throw exception("��ʱ��д��������Ĵ���֮���пտ��Բ���");
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
				throw exception("�����߼�����,���˲�Ӧ�ó���dataΪ�յ����");
			}
		}
		if (ByteCount % 4 != 0)
		{
			for (int i = 0; i < 4 - (ByteCount % 4); i++)
			{
				newpbmpBuf[indexbmpbuf] = 0x00; //����4�ֽڵı����Ͳ�ȫ0x00
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
		throw exception("�����߼�����");
	}
}

IPcode1::IPcode1(const IPcode1 & input)
	:pbmpBuf(NULL), pcolortable(NULL),infohead(input.infohead), filehead(input.filehead)
{
	int linebytes = (input.infohead.biWidth * input.infohead.biBitCount / 8 + 3) / 4 * 4;
	auto sizebytes = input.infohead.biHeight*linebytes;
	pbmpBuf = new unsigned char [sizebytes];
	
	//if (input.infohead.biHeight*linebytes != input.infohead.biSizeImage)
	//	cout << "warning��input.infohead.biHeight*linebytes != input.infohead.biSizeImage" << input.infohead.biHeight*linebytes << " " << input.infohead.biSizeImage;
	////������ʱ�ǺϷ��ģ�����input��pbmpbuf����biSizeImage
	
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
	////��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0
	//int colorTablesize = filehead.bfOffBits-54;

	//if (infohead.biBitCount == 8)
	//{
	//	if (colorTablesize != 1024)
	//	{
	//		cout << __FILE__ << __LINE__ << endl;
	//		throw exception("�Ҷ�ͼ�����ɫ��Ϊ1024�ֽ�");//��������лҶ�ͼ�����ɫ��Ϊ1024�ֽ�
	//	}
	//	else
	//		cout << "�Ҷ�ͼ�����ɫ��ȷʵΪ1024�ֽ�" << endl;
	//}
	//	
	//int lineBytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4; //���Ӧ�����ò��� 

	//FILE *fp = fopen(bmpname, "wb");
	//if (fp == NULL)
	//	return;
	////if (infohead.biHeight*lineBytes != infohead.biSizeImage)
	//	//throw exception("�����ط��϶�����������");

	//filehead.bfType = 0x4d42;//bmp���� ���������������0x4b42 ��ô����ʾ��֧�ִ�����
	//filehead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + infohead.biSizeImage;
	//filehead.bfReserved1 = 0;
	//filehead.bfReserved2 = 0;
	//filehead.bfOffBits = 54 + colorTablesize;
	//fwrite(&filehead, sizeof(BITMAPFILEHEADER), 1, fp);

	////infohead���޸�ͼ����������ʱ��Ӧ�ñ��޸Ĺ�һ����,ͬʱ�����޸ĵ�Ӧ�ñ���ԭ���Բ���
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
					throw exception("����˵�ǲ�Ӧ�ó��ֲ���3�������");
				}
				else
					pf(pixelvalue[0], pixelvalue[1], pixelvalue[2]); //������ں���ָ�������Ӧת��
			}
		}
	}
	else
		throw exception("��ʱ�޷�����������ʽ��ͼƬ");
}

void IPcode1::RGB2YIQ(unsigned char *a, unsigned char* b, unsigned char* c)
{
	unsigned char d = 0.299 * (*a) + 0.587* (*b) + 0.114* (*c);
	unsigned char e = 0.596 * (*a) - 0.274* (*b) - 0.322* (*c);
	unsigned char f = 0.211 * (*a) - 0.523* (*b) + 0.312* (*c);
	if (!(CheckRange0_255(d) && CheckRange0_255(e) && CheckRange0_255(f)))
		throw exception("ת���㷨�����⣬�õ���ֵ������ 0-255");
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
		throw exception("ת���㷨�����⣬�õ���ֵ������ 0-255");
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
		throw exception("ת���㷨�����⣬�õ���ֵ������ 0-255");
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
		throw exception("ת���㷨�����⣬�õ���ֵ������ 0-255");
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
		throw exception("ת���㷨�����⣬�õ���ֵ������ 0-255");
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
		throw exception("ת���㷨�����⣬�õ���HSIֵ������");
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
			throw exception("�Ҷ�ͼֻ����channel 0����");
	}
	else if (infohead.biBitCount == 24)
	{
		if (channel != 0 && channel != 1 && channel != 2)
			throw exception("��ͨ�������ܳ�����ͨ�� 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("��ʱ�޷�������ʱ�����");
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
		throw exception("�����߼�д��������");
}

void IPcode1::SaveChannel(char * path, int channel)
{
	DWORD sizeimage;
	unsigned char * Channeldata = extractDataChannel(channel, sizeimage);
	SaveBmpTool(path, Channeldata, infohead.biWidth, infohead.biHeight, 8);//����Ҷ�ͼ
	delete[]Channeldata;
}

void IPcode1::SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount)const
//���뱣֤imgBuf��ֵ����lineByte*height
{
	if (imgBuf == NULL)
		return;
	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0
	int colorTablesize;
	if (biBitCount == 24)
		colorTablesize = 0;
	else if (biBitCount == 8)
		colorTablesize = 1024;
	else
		throw exception("��ʱ�޷���������λ���Ĵ���");

	////���洢ͼ������ÿ���ֽ���Ϊ4�ı���
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//�Զ�����д�ķ�ʽ���ļ�
	FILE *fp = fopen(path, "wb");

	if (fp == 0)
		throw exception("�޷��򿪶�Ӧ�������ļ�");

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;//bmp����
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	fileHead.bfOffBits = 54 + colorTablesize;
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ
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

	////����Ҷ�ͼ������ɫ��д���ļ� 
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
		

	//дλͼ���ݽ��ļ�
	fwrite(imgBuf, lineByte*height, 1, fp);

	//�ر��ļ�
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

void IPcode1::makeBmpTimesof(unsigned int widthtimes, unsigned int heighttimes)
{
	if (widthtimes <= 0 || heighttimes <= 0 || widthtimes > infohead.biWidth || heighttimes > infohead.biHeight)
		throw exception("widthtimes��heighttimes���ò���");
	if (infohead.biBitCount == 8 || infohead.biBitCount == 24)
	{
		if (infohead.biWidth % widthtimes == 0 && infohead.biHeight % heighttimes == 0) //�����������widthtimes*heighttimes�������ģ���ôʲô������Ҫ��
			return;
		else
		{
			WORD linebytes = (infohead.biWidth * infohead.biBitCount / 8 + 3) / 4 * 4;
			if (infohead.biSizeImage != linebytes*infohead.biHeight)
				cout << "warning:ͼƬ���ݵ�ÿ���ֽ�������4�ı���" << infohead.biSizeImage << " " << linebytes*infohead.biHeight << endl;
				/*throw exception("��Ҫ�ȱ�֤ͼƬ���ݵ�ÿ���ֽ�����4�ı���");*/
			LONG newbiWidth = (infohead.biWidth + widthtimes-1) / widthtimes * widthtimes;
			WORD newLinebytes = (newbiWidth * infohead.biBitCount / 8 + 3) / 4 * 4; //��֤�ֽ�����4�ı���
			LONG newbiHeight = (infohead.biHeight + heighttimes-1) / heighttimes * heighttimes;
			if (newLinebytes < linebytes)
				throw exception("��Ҫ����˼���±ߵ��߼�������˵��Ӧ�ó����������");

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
			delete[]pbmpBuf;//�Ȱ�ԭ�����ͷ���
			pbmpBuf = result;
			infohead.biHeight = newbiHeight;
			infohead.biSizeImage = newbiHeight*newLinebytes;
			infohead.biWidth = newbiWidth;
		}
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("��ʱ�޷�������ʱ�����");
	}
}

void IPcode1::Kernel_image(int channel,unsigned int width,unsigned int height, string Operation, string inmode,string outmode,double Numin ) //mode�����Ǳ�����ֵ���Ǳ�����λ 0��ʾ������ֵ1��ʾ������λ
{
	if (infohead.biBitCount == 8)
	{
		if (channel != 0)
			throw exception("�Ҷ�ͼֻ����channel 0����");
	}
	else if (infohead.biBitCount == 24)
	{
		if (channel != 0 && channel != 1 && channel != 2)
			throw exception("��ͨ�������ܳ�����ͨ�� 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("��ʱ�޷�������ʱ�����");
	}

	if (channel == 0 || channel == 1 || channel == 2)
	{
		makeBmpTimesof(width, height); //�ȱ�֤ͼƬ�ĳ��Ϳ�������������ı���
		unsigned int * amplitudeStore = NULL;
		double * phaseStore = NULL;
		if (outmode == "ampl")
		{
			amplitudeStore = new unsigned int[infohead.biWidth*infohead.biHeight];
		}
		else if (outmode == "phase")
		{
			phaseStore = new double[infohead.biWidth*infohead.biHeight];
		}
		else
			throw exception("δ����outmode");
		
		vector<complex<double>> DFTpart(width*height); // ��СӦ����width*height
		for (LONG j = 0; j < infohead.biHeight / height; j++)
		{
			for (LONG i = 0; i < infohead.biWidth / width; i++)
			{
				auto xoffset = i*width;
				auto yoffset = j*height;
				int DFTCount = 0;
				
				for (unsigned int y = 0; y < height; y++)
				{
					for (unsigned int x = 0; x < width; x++)
					{
						int bitlocation;
						auto pixel = GetPixel(xoffset + x, yoffset + y, bitlocation);
						DFTpart[DFTCount++] = *pixel[channel];
					}
				}
				if (DFTpart.size() != width*height )
					throw exception("�����߼�����");
				if (Operation == "DFT")
					DFTpart = Operate::FFT_2D(DFTpart, width, height);
				else if (Operation == "IDFT")
				{	
					if (inmode == "phase")//ֻ��IDFT��עinmode 
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
						throw exception("������δ�����inmode");
					DFTpart = Operate::IFFT_2D(DFTpart, width, height);
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
					DFTpart = Operate::FFT_2D(DFTpart, width, height);
					DFTpart = Operate::IFFT_2D(DFTpart, width, height);//����FFT��û��д�� ����һ��һ�����û���κθı�Ļ�˵���ͶԵ�
					//cout << "����FFT��û��д�� ����һ��һ�����û���κθı�Ļ�˵���ͶԵ�";
				}
				else if (Operation == "DCT") 
				{
					if (width != height)
						throw exception("�޷�����width != height�����");
					if (Numin > width*width)
						throw exception("������DCTϵ����������");
					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::DCT_2D(tmpDCT, width);

					for (int i = 0; i < tmpDCT.size(); i++)
						tmpDCT[i] = abs(tmpDCT[i]);
					//���㵽������ȥ,��ʾ��DCTԽ����˵������Խ��

					Operate::zigzagRetainDCT(tmpDCT, width, Numin);
					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);
				}
				else if (Operation == "IDCT") //����ʵ��������ģ���Ϊ������DCT��������ֵ�任���ͼ��ֻ�����������Ⲣ������IDCT����㷨���ԣ��㷨�ǶԵ�
				{
					throw exception("IDCT����ʾ�ڵ�ǰ����ͼ�����������unsigned char����������ģ���Ϊ������DCT��������ֵ�任���ͼ��ֻ������");
					if (width != height)
						throw exception("�޷�����width != height�����");
					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::IDCT_2D(tmpDCT, width);

					auto minelement = *min_element(tmpDCT.begin(), tmpDCT.end());
					if (minelement < 0)
					{
						minelement = abs(minelement);
						for (int i = 0; i < tmpDCT.size(); i++)
							tmpDCT[i] += minelement;
					}//���㵽������ȥ

					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);
				}
				else if (Operation == "DCT_Numin_IDCT")
				{
					if (width != height)
						throw exception("�޷�����width != height�����");
					if (Numin > width*width)
						throw exception("������DCTϵ����������");

					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::DCT_2D(tmpDCT, width);
					Operate::zigzagRetainDCT(tmpDCT, width, Numin);
					tmpDCT = Operate::IDCT_2D(tmpDCT, width);
					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);
				}
				else if (Operation == "normalDCT")
				{
					vector<double> tmpDCT;
					Operate::TransvecComplex2Double(DFTpart, tmpDCT);
					tmpDCT = Operate::DCT_2D(tmpDCT, width);
					tmpDCT = Operate::IDCT_2D(tmpDCT, width);
					Operate::TransvecDouble2Complex(tmpDCT, DFTpart);  //���ڲ���û�������
				}
				else
					throw exception("�����Operation���ԣ�ֻ�����ض���Χ��ѡ");

				DFTCount = 0;
				for (unsigned int y = 0; y < height; y++)
				{
					for (unsigned int x = 0; x < width; x++)
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
			unsigned int * begin = amplitudeStore;
			unsigned int * end = amplitudeStore + (infohead.biWidth*infohead.biHeight - 1);
			vector<double> normal_list = normalization_zk<unsigned int>(begin, end);
			Denormalization_zk<unsigned int>(begin, end, 0, 255, normal_list);
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
		throw exception("�߼��д�");
}

void IPcode1::ChangeSingleChannelInputNEWptrData(unsigned char * newbmpdataptr, LONG width, LONG height, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, unsigned char value)
//������ĵ�ͨ������newbmpdataptr��������ĵ�ַ�����Ա�����޹أ�Ϊ��㣬ld lu rd ruΪ���޸ĵ����ط���,valueΪ��Ҫ�޸ĵ�ֵ 
//width heightΪnewbmpdataptr�����ͼ��ĳ��Ϳ�
{
	if (ld[0] != lu[0] || rd[0] != ru[0] || lu[1] != ru[1] || ld[1] != rd[1])
		throw exception("���Ƿ���");
	if (ld[0] > rd[0] || ld[1] > lu[1])
		throw exception("���õķ���ֵ����");
	if (ld[0] < 0 || rd[0] >= width || ld[1] < 0 || lu[1] >= height)
		throw exception("���õķ���ֵ�����ܵ�ͼƬ�Ĵ�С��");

	WORD linebytes = (width * 8 / 8 + 3) / 4 * 4; //ֻ��Ϊ��ͨ������
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			newbmpdataptr[x + y*linebytes] = value;
		}
	}
}
void IPcode1::plot_DCTkernel_Image(int N) //Ҳ���Դ˺���Ϊģ�壬������������ı任�˻�ͼ�� ʱ�����⣬���Ǽ������� ʱ������
{
	makeBmpTimesof(N, N);
	DWORD sizeimage;
	unsigned char * singelchannel = extractDataChannel(0, sizeimage);
	double* Value = new double[N*N]; //������ ���µ��ϴ���ÿ���ֿ�ͼ���ֵ

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
			Denormalization_zk<double>(begin, end, 0, 255, normalization_zk<double>(begin, end)); //��һ��������0-255
			
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
			SaveBmpTool((char*)filename.data(), singelchannel, infohead.biWidth, infohead.biHeight, 8);//����Ϊ�Ҷ�ͼ,���޸ı�����ͼƬ
		}
	}
	delete[]singelchannel;
	delete[]Value;
}

IPcode1 IPcode1::operator-(const IPcode1& input)const
{
	if (input.infohead.biBitCount != infohead.biBitCount)
		throw exception("�������������ͼƬλ����һ��");
	if (input.infohead.biWidth != infohead.biWidth || input.infohead.biHeight != infohead.biHeight)
		throw exception("�������������ͼƬ�ߴ粻һ��");

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
		throw exception("PSNR������ͼƬλ����һ��");
	if (input.infohead.biWidth != infohead.biWidth || input.infohead.biHeight != infohead.biHeight)
		throw exception("PSNR������ͼƬ�ߴ粻һ��");

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
		throw exception("��ʱ���޷���������λ����PSNR");
	
}

vector<double> IPcode1::Histogram(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru)
{
	if (infohead.biBitCount == 8)
	{
		if (Channel != 0)
			throw exception("�Ҷ�ͼֻ����channel 0����");
	}
	else if (infohead.biBitCount == 24)
	{
		if (Channel != 0 && Channel != 1 && Channel != 2)
			throw exception("��ͨ�������ܳ�����ͨ�� 0 1 2 ");
	}
	else
	{
		cout << __FILE__ << __LINE__;
		throw exception("��ʱ�޷�������ʱ�����");
	}

	if (ld[0] != lu[0] || rd[0] != ru[0] || lu[1] != ru[1] || ld[1] != rd[1])
		throw exception("���Ƿ���");
	if (ld[0] > rd[0] || ld[1] > lu[1])
		throw exception("���õķ���ֵ����");
	if (ld[0] < 0 || rd[0] >= infohead.biWidth || ld[1] < 0 || lu[1] >= infohead.biHeight)
		throw exception("���õķ���ֵ�����ܵ�ͼƬ�Ĵ�С��");

	vector <double> result(256,0);
	LONG CountAll = 0;
	for (LONG y = ld[1]; y <= lu[1]; y++)
	{
		for (LONG x = ld[0]; x <= rd[0]; x++)
		{
			int bitlocation;
			auto pix = GetPixel(x, y, bitlocation);
			result[*pix[Channel]]++; //ͳ�Ƹ���
			CountAll++;
		}
	}
	for (size_t i = 0; i < result.size(); i++)
	{
		result[i] /= CountAll;
	}
	return result;
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
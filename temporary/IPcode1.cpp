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
			throw exception("δ����bitBitCount,������");
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
		throw("δ���崦���bitBitCount�ĳ���");
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
		throw exception("�����߼�����");

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
				cout << __FILE__ << "/" << __LINE__;
				throw exception("�����߼�����,���˲�Ӧ�ó���dataΪ�յ����");
			}
		}
		if (ByteCount % 4 != 0)
		{
			for (int i = 0; i < 4 - ByteCount % 4; i++)
			{
				newpbmpBuf[indexbmpbuf] = 0x00; //����4�ֽڵı����Ͳ�ȫ0x00
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
		throw exception("�����߼�����");
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
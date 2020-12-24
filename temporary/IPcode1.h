#pragma once
#include "main.h"
class IPcode1
{
public:
	~IPcode1()
	{
		delete[] pbmpBuf;
		delete[] pcolortable;
	}
	explicit IPcode1(char* input):pbmpBuf(NULL),pcolortable(NULL){ ReadBmp(input); }
	IPcode1() = delete;
	//explicit IPcode1(unsigned char * ptmp) :pbmpBuf(ptmp) {} //��������һ�����ݵ�ַ��ֻ�ܴ�path�ж�
	
	void CropBmp(int locx,int locy,int width, int height);//��ȡBMP��ĳ������

private:
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//��ɫ��ָ�� 
	BITMAPINFOHEADER infohead; //40�ֽڵ���Ϣͷ
	BITMAPFILEHEADER filehead; // 14�ֽڵ��ļ�ͷ

	bool ReadBmp(char * name); //ֻ���ڹ���ʱ����һ��
	bool CheckInImage(int x, int y);//����Ƿ���BMPͼ��Χ��
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation);
	// �õ�ĳһλ���µ�����ֵ�Ĵ����ַ��������3���ֽڣ�������1�ֽڣ������ǰ���ֽڣ�Ҳ������1λ
	//bitlocation ��ʾ�����ְ���ֽ�ʱ��ǰ��λ��Ǻ��Σ����Ƕ�ֵ��ͼʱ��ʾ����һλ0-7

};
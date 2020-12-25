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
	IPcode1(const IPcode1 & input);//�������캯����Ӧ�ÿ���pbmpBuf��pcolortable��ָ�������
	IPcode1() = delete;
	IPcode1 & operator=(const IPcode1 &) = delete;//������ֵ������ܿ�������ڴ�й©
	//explicit IPcode1(unsigned char * ptmp) :pbmpBuf(ptmp) {} //��������һ�����ݵ�ַ��ֻ�ܴ�path�ж�
	
	void CropBmp(int locx,int locy,int width, int height);//��ȡBMP��ĳ������
	void SaveBmp(char * path); //���Ϊ��ǰ�����ݵ�ĳһ·���µ�ĳһbmp
	void SaveChannel(char * path,int channel); //�����3ͨ���ģ����ĳһͨ���ĻҶ�ͼ

	void Transfer(void(*pf)(unsigned char*, unsigned char*, unsigned char*));//��������XXXͼת����XXͼ �����ɺ���ָ�����
	static void RGB2YIQ(unsigned char *, unsigned char *, unsigned char *);//���óɾ�̬�������ú���ָ��,�����漰����Ա��ָ��ο�C++primer P741
	static void YIQ2RGB(unsigned char *, unsigned char *, unsigned char *);//�⼸��������Ӧ����Ĵ������ı䣬����ʺ�����Ϊstatic
	static void RGB2HSI(unsigned char *, unsigned char *, unsigned char *);
	//static constexpr int sdas = 4;
private:
	
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//��ɫ��ָ�� 
	BITMAPINFOHEADER infohead; //40�ֽڵ���Ϣͷ
	BITMAPFILEHEADER filehead; // 14�ֽڵ��ļ�ͷ

	unsigned char * extractDataChannel(int channel, DWORD &sizeimage);
	void SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount);

	bool ReadBmp(char * name); //ֻ���ڹ���ʱ����һ��
	bool CheckInImage(int x, int y);//����Ƿ���BMPͼ��Χ��
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation);
	// �õ�ĳһλ���µ�����ֵ�Ĵ����ַ��������3���ֽڣ�������1�ֽڣ������ǰ���ֽڣ�Ҳ������1λ
	//bitlocation ��ʾ�����ְ���ֽ�ʱ��ǰ��λ��Ǻ��Σ����Ƕ�ֵ��ͼʱ��ʾ����һλ0-7

};
//constexpr int IPcode1::sdas;
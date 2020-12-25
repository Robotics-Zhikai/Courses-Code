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
	IPcode1(const IPcode1 & input);//拷贝构造函数，应该拷贝pbmpBuf和pcolortable所指向的内容
	IPcode1() = delete;
	IPcode1 & operator=(const IPcode1 &) = delete;//不允许赋值，否则很可能造成内存泄漏
	//explicit IPcode1(unsigned char * ptmp) :pbmpBuf(ptmp) {} //不能外界给一个数据地址，只能从path中读
	
	void CropBmp(int locx,int locy,int width, int height);//截取BMP的某块区域
	void SaveBmp(char * path); //另存为当前的内容到某一路径下的某一bmp
	void SaveChannel(char * path,int channel); //真彩是3通道的，输出某一通道的灰度图

	void Transfer(void(*pf)(unsigned char*, unsigned char*, unsigned char*));//将数据由XXX图转化到XX图 具体由函数指针决定
	static void RGB2YIQ(unsigned char *, unsigned char *, unsigned char *);//设置成静态变量调用函数指针,具体涉及到成员的指针参考C++primer P741
	static void YIQ2RGB(unsigned char *, unsigned char *, unsigned char *);//这几个函数不应对象的创建而改变，因此适合声明为static
	static void RGB2HSI(unsigned char *, unsigned char *, unsigned char *);
	//static constexpr int sdas = 4;
private:
	
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//颜色表指针 
	BITMAPINFOHEADER infohead; //40字节的信息头
	BITMAPFILEHEADER filehead; // 14字节的文件头

	unsigned char * extractDataChannel(int channel, DWORD &sizeimage);
	void SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount);

	bool ReadBmp(char * name); //只能在构造时调用一次
	bool CheckInImage(int x, int y);//检查是否在BMP图像范围内
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation);
	// 得到某一位置下的像素值的储存地址，可能是3个字节，可能是1字节，可能是半个字节，也可能是1位
	//bitlocation 表示当出现半个字节时是前半段还是后半段；当是二值化图时表示在哪一位0-7

};
//constexpr int IPcode1::sdas;
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
	void SaveBmp(char * path); //另存为当前的内容到某一路径

private:
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//颜色表指针 
	BITMAPINFOHEADER infohead; //40字节的信息头
	BITMAPFILEHEADER filehead; // 14字节的文件头

	bool ReadBmp(char * name); //只能在构造时调用一次
	bool CheckInImage(int x, int y);//检查是否在BMP图像范围内
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation);
	// 得到某一位置下的像素值的储存地址，可能是3个字节，可能是1字节，可能是半个字节，也可能是1位
	//bitlocation 表示当出现半个字节时是前半段还是后半段；当是二值化图时表示在哪一位0-7

};
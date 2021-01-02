#pragma once
#include "main.h"
#include "Operate.h"


class IPcode1:private Operate //私有继承，IPcode1的用户不应该看到FFT等非常底层的操作
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

	IPcode1 operator-(const IPcode1& input)const;//得到两图像之间的差异，相减并取绝对值
	double PSNR(const IPcode1& Origin)const;//计算峰值信噪比，输入参数应为this的原始图像 这个值越高，说明与原图越相近
	
	void CropBmp(int locx,int locy,int width, int height);//截取BMP的某块区域
	void SaveBmp(char * path); //另存为当前的内容到某一路径下的某一bmp
	void SaveChannel(char * path,int channel); //真彩是3通道的，输出某一通道的灰度图
	vector<unsigned char> ReadPixel(int x, int y, int & bitlocation);//读取pixel的值，不进行修改

	void Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, string Operation, string inmode, string outmode, double Num=0);
	//对第channel号通道进行分块的DFT,直接修改类本身的数据 mode表明是保留幅值还是保留相位 0表示保留幅值1表示保留相位
	//默认的步长是width height
	void Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, bool ChangeDimension,const vector<double>& KernalTemplate,int stepx,int stepy);
	//用width和height的模板核对channel通道的图像进行处理 整体逻辑和上边的Kernel_image是差不多的 step表示每次在各方向上移动的步长
	//KernalTemplate也是从左到右从下到上的数字排列的。 ChangeDimension表示变换后的量纲是否有改变 比如均值滤波和中值滤波量纲就没有改变
	void Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, bool ChangeDimension, string StatisticalMode, int stepx, int stepy);
	//统计排序滤波器的KernelImage

	double KernalTemplate_Multiple_subimage(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const;
	//用Kernelwidth*Kernelheight的KernalTemplate作用到以CenterX和CenterY为中心的小图块中,返回对应结果，以double返回
	double KernalTemplate_Multiple_subimage_y(int channel, LONG Globaly, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const;
	//给定Globaly 得到该行的值，上一个函数是连续调用这个子函数来得到的
	double KernalTemplate_Multiple_subimage_x(int channel, LONG Globalx, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const;
	vector<double> AVGKernelTemplate(unsigned int M, unsigned int N);

	void MakeRGB24_TO_Gray8();
	void plot_DCTkernel_Image(int N); //绘制DCT正向变换核的图像 以当前图片的大小为基础，绘制正向变换核基图像
	vector<double> Histogram(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru); 
	//得到某一通道分布在给定范围内的本类图像数据的频率分布直方图
	void Equalization_Image(int Channel, vector<LONG> ld , vector<LONG> lu , vector<LONG> rd, vector<LONG> ru);
	//均衡化某一channel的直方图，并将效果显示在图片上
	void AddNoise(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, string NoiseMode, double par0=0, double par1=0, double par2=0, double par3=0);
	//默认情况时是所有通道都加

	LONG ReadWidth()const { return infohead.biWidth; }
	LONG ReadHeight()const { return infohead.biHeight; }

	

	void Transfer(void(*pf)(unsigned char*, unsigned char*, unsigned char*));//将数据由XXX图转化到XX图 具体由函数指针决定
	static void RGB2YIQ(unsigned char *, unsigned char *, unsigned char *);//设置成静态变量调用函数指针,具体涉及到成员的指针参考C++primer P741
	static void YIQ2RGB(unsigned char *, unsigned char *, unsigned char *);//这几个函数不应对象的创建而改变，因此适合声明为static
	static void RGB2HSI(unsigned char *, unsigned char *, unsigned char *);
	static void RGB2YCrCb(unsigned char *, unsigned char*, unsigned char*);
	static void RGB2XYZ(unsigned char *, unsigned char*, unsigned char*);
	static void RGB2GrayValue(unsigned char *, unsigned char*, unsigned char*); //RGB转化为灰度图 任保留24位
	//static constexpr int sdas = 4;
private:
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//颜色表指针 
	BITMAPINFOHEADER infohead; //40字节的信息头
	BITMAPFILEHEADER filehead; // 14字节的文件头

	void makeBmpTimesof(unsigned int widthtimes, unsigned int heighttimes);//使得BMP图的各通道的长宽是widthtimes heighttimes的倍数,不一定字节数是8的倍数
	unsigned char * extractDataChannel(int channel, DWORD &sizeimage);//如果要调用这个，必须记得delete[]
	void SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount)const;

	bool ReadBmp(char * name); //只能在构造时调用一次
	bool CheckInImage(int x, int y)const;//检查是否在BMP图像范围内
	void CheckRectangleInBigRect(vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, LONG width, LONG height)const;
	// 检测方框是否在图像中，如果不是方框或者不在图像范围内，则throw

	static bool CheckRange0_255(double a);
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation)const;
	// 得到某一位置下的像素值的储存地址，可能是3个字节，可能是1字节，可能是半个字节，也可能是1位
	//bitlocation 表示当出现半个字节时是前半段还是后半段；当是二值化图时表示在哪一位0-7
	vector<unsigned long> GetPixel_Index(int x, int y, int & bitlocation)const; //得到x y像素对应的索引数值,基本上和上边的函数逻辑相似
	
	void ChangeSingleChannelInputNEWptrData(unsigned char * newbmpdataptr,LONG width,LONG height, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru,unsigned char value);
	//以输入的单通道数据newbmpdataptr（新申请的地址，与成员数据无关）为起点，ld lu rd ru为待修改的像素方框,value为需要修改的值 
	//width height为newbmpdataptr代表的图像的长和宽

	template<typename Ty>
	Ty max_zk(Ty*begin, Ty*end) //从begin到end地址下的所有元素的最大值 区间是前闭后闭
	{
		Ty maxelement = *begin;
		//begin->dasda = 1; //随便写了这个 是会报错的
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			if (*iterator > maxelement)
				maxelement = *iterator;
		}
		return maxelement;
	}
	template<typename Ty>
	Ty min_zk(Ty*begin, Ty*end) //从begin到end地址下的所有元素的最小值 区间是前闭后闭
	{
		Ty minelement = *begin;
		//begin->dasda = 1;
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			if (*iterator < minelement)
				minelement = *iterator;
		}
		return minelement;
	}
	template<typename Ty>
	vector<double> normalization_zk(Ty*begin, Ty*end) //归一化 在[begin,end]内的数值
	{
		Ty maxelement = max_zk<Ty>(begin, end);
		Ty minelement = min_zk<Ty>(begin, end);
		vector<double> result;
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			if (maxelement != minelement)
				result.push_back(abs(double(*iterator - minelement)) / abs(double(maxelement - minelement)));
			else
				result.push_back(0.5);
			/**iterator = ; *///归一化
		}
		return result;
	}

	//template<typename Ty>
	//void Denormalization_zk(Ty*begin, Ty*end, Ty lower, Ty upper) //逆归一化 在[begin,end]内的数值肯定为[0,1]，将他们逆归一化从lower到upper
	//{
	//	Ty maxelement = max_zk<Ty>(begin, end);
	//	Ty minelement = min_zk<Ty>(begin, end);
	//	if (maxelement > 1 || minelement < 0)
	//		throw exception("输入的数据不在[0,1]");
	//	if (lower > upper)
	//		throw exception("输入lower>upper");
	//	for (auto iterator = begin; iterator != end + 1; iterator++)
	//	{
	//		*iterator = lower + double(upper - lower)*double(*iterator); //逆归一化
	//	}
	//}
	template<typename Ty>
	void Denormalization_zk(Ty*begin, Ty*end, Ty lower, Ty upper,const vector<double>& normal_list) 
		//逆归一化 在[begin,end]内的数值,将他们逆归一化从lower到upper normal_list为归一化后的数据
	{

		double maxelement = *max_element(normal_list.begin(),normal_list.end());
		double minelement = *min_element(normal_list.begin(), normal_list.end());
		if (maxelement > 1 || minelement < 0)
			throw exception("输入的数据不在[0,1]");
		if (lower > upper)
			throw exception("输入lower>upper");
		size_t count = 0;
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			*iterator = lower + double(upper - lower)*normal_list[count++]; //逆归一化
		}
	}
};
//constexpr int IPcode1::sdas;
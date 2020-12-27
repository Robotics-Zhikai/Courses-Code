#pragma once
#include "main.h"
class ComplexEXP
{
public:
	ComplexEXP() = default;
	ComplexEXP(double amplitude1, double phase1):comp(TransExp2Complex(amplitude1, phase1)), amplitude(amplitude1),phase(phase1){}
	ComplexEXP(complex<double> input) :
		comp(input), amplitude(std::sqrt(std::pow(input.imag(), 2) + std::pow(input.real(), 2))), phase(std::atan(input.imag() / input.real())) {}

	const complex<double>& readcomp()const { return comp; }
	const double & read_amplitude()const { return amplitude; }
	const double & read_phase()const { return phase; }
	//const complex<double>& readcomp() { return comp+complex<double>(1,1); } //这是重载，因是否是const类型而调用不同的类型函数

	void pow(double zhishu)
	{
		ComplexEXP tmp(amplitude, zhishu*phase);
		comp = tmp.comp;
		phase = zhishu*phase;
	}

private:
	complex<double> comp;
	double amplitude = 0;
	double phase = 0;

	complex<double> TransExp2Complex(double amplitude, double phase)//phase必须是弧度数
	{
		complex<double> result(amplitude*cos(phase), amplitude*sin(phase));
		return result;
	}	
};

class Operate
{
public:
	vector<complex<double>> FFT_2D(const vector<complex<double>> & input,int M,int N) //二维的FFT M为width N为Height 从左到右 从下到上
	{
		if (input.size() != M*N)
			throw exception("输入数据大小不符合给定的M N");
		vector<complex<double>> result(M*N); //最终的大小应该是M*N
		//vector<vector<complex<double>>> linefft(N);
		for (int i = 0; i < N; i++)
		{
			int offset = i*M;
			vector<complex<double>> tobefftline(0); //储存等待fft的当前行的数据
			for (int j = offset; j < offset+M; j++)
				tobefftline.push_back(input[j]);
			tobefftline = FFT(tobefftline);
			int count = 0;
			for (int j = offset; j < offset + M; j++)
				result[j] = tobefftline[count++];
		}
		for (int i = 0; i < M; i++)
		{
			vector<complex<double>> tobefftcol(0);
			for (int j = i; j < N*M; j = j + M)
				tobefftcol.push_back(result[j]);
			tobefftcol = FFT(tobefftcol);
			int count = 0;
			for (int j = i; j < N*M; j = j + M)
				result[j] = tobefftcol[count++];
		}
		return result;
	}
	vector<complex<double>> IFFT_2D(const vector<complex<double>> & input, int M, int N)//二维的IFFT M为width N为Height 从左到右 从下到上
	{
		if (input.size() != M*N)
			throw exception("输入数据大小不符合给定的M N");
		vector<complex<double>> result(M*N); //最终的大小应该是M*N
											 //vector<vector<complex<double>>> linefft(N);
		for (int i = 0; i < N; i++)
		{
			int offset = i*M;
			vector<complex<double>> tobeifftline(0); //储存等待fft的当前行的数据
			for (int j = offset; j < offset + M; j++)
				tobeifftline.push_back(input[j]);
			tobeifftline = IFFT(tobeifftline);
			int count = 0;
			for (int j = offset; j < offset + M; j++)
				result[j] = tobeifftline[count++];
		}
		for (int i = 0; i < M; i++)
		{
			vector<complex<double>> tobeifftcol(0);
			for (int j = i; j < N*M; j = j + M)
				tobeifftcol.push_back(result[j]);
			tobeifftcol = IFFT(tobeifftcol);
			int count = 0;
			for (int j = i; j < N*M; j = j + M)
				result[j] = tobeifftcol[count++];
		}
		return result;
	}
	vector<complex<double>> IFFT(const vector<complex<double>> & input)
	{
		auto N = input.size();
		vector<complex<double>> result(N);
		if (N == 0)
			return result;

		static auto NOmiga = N;
		static int flagfirst = 1;
		static vector<complex<double>> Omiga;
		if (NOmiga != N || flagfirst == 1) //防止重复调用FFT时反复调用getOmiga
										   //有可能在实际调用FFT时input数据每次都不一样，有时间的话可以考虑搞一个Omiga Set，只要曾经用过的Omiga，就存在Set里 每次只需要付出查询的成本即可
										   //很简单的查询的时间成本可以降到O(1) 实现这个的话就是堆时间，没有什么技术含量
		{
			flagfirst = 0;
			NOmiga = N;
			Omiga = GetOmigaIFFT(N);
		}
		if (Omiga.size() != pow(N - 1, 2) + 1)
			throw exception("代码逻辑出错，Omiga的维度与input不匹配");
		
		for (int i = 0; i < N; i++)
		{
			result[i] = 0;
			for (int j = 0; j <= N - 1; j++)
			{
				result[i] = result[i] + Omiga[j*i] * input[j];
			}
		}
		for (int i = 0; i < result.size(); i++)
			result[i] /= N ;
		return result;
	}
	vector<complex<double>> FFT(const vector<complex<double>> & input)//一维的FFT
	{
		auto N = input.size();
		vector<complex<double>> result(N);
		if (N == 0)
			return result;

		static auto NOmiga = N;
		static int flagfirst = 1;
		static vector<complex<double>> Omiga;
		if (NOmiga != N || flagfirst == 1) //防止重复调用FFT时反复调用getOmiga
										   //有可能在实际调用FFT时input数据每次都不一样，有时间的话可以考虑搞一个Omiga Set，只要曾经用过的Omiga，就存在Set里 每次只需要付出查询的成本即可
										   //很简单的查询的时间成本可以降到O(1) 实现这个的话就是堆时间，没有什么技术含量
		{
			flagfirst = 0;
			NOmiga = N;
			Omiga = GetOmiga(N);
		}
		if (Omiga.size() != pow(N - 1, 2) + 1)
			throw exception("代码逻辑出错，Omiga的维度与input不匹配");
		
		for (int i = 0; i < N; i++)
		{
			result[i] = 0;
			for (int j = 0; j <= N - 1; j++)
			{
				result[i] = result[i] + Omiga[j*i] * input[j];
			}
		}
		return result;
	}
	vector<complex<double>> FFT(const vector<ComplexEXP> &input) //保证不进行输入数据的任何修改且不额外开销耗费计算资源
	{
		vector<complex<double>> transinput;
		for (int i = 0; i < input.size(); i++)
		{
			transinput.push_back(input[i].readcomp());
		}
		return FFT(transinput);
	}
	vector<complex<double>> FFT(const vector<double>&input)
	{
		vector<complex<double>> input1;
		for (int i = 0; i < input.size(); i++)
		{
			complex<double> tmp = input[i];
			input1.push_back(tmp);
		}
		return FFT(input1);
	}
private:
	vector<complex<double>> GetOmiga(int N)//输入为数据个数,在特定的N下，此函数如果重复多次调用的话会计算量非常大
	{
		vector<complex<double>> Omiga(pow(N - 1, 2) + 1);
		Omiga[0] = 1;
		for (int i = 1; i < pow(N - 1, 2) + 1; i++)
		{
			Omiga[i] = Omiga[i - 1] * (ComplexEXP(1, -2 * pi / N).readcomp());
		}
		return Omiga;
	}
	vector<complex<double>> GetOmigaIFFT(int N)//输入为数据个数,在特定的N下，此函数如果重复多次调用的话会计算量非常大
	{
		vector<complex<double>> Omiga(pow(N - 1, 2) + 1);
		Omiga[0] = 1;
		for (int i = 1; i < pow(N - 1, 2) + 1; i++)
		{
			Omiga[i] = Omiga[i - 1] * (ComplexEXP(1, 2 * pi / N).readcomp());
		}
		return Omiga;
	}
};

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
	
	void CropBmp(int locx,int locy,int width, int height);//截取BMP的某块区域
	void SaveBmp(char * path); //另存为当前的内容到某一路径下的某一bmp
	void SaveChannel(char * path,int channel); //真彩是3通道的，输出某一通道的灰度图
	vector<unsigned char> ReadPixel(int x, int y, int & bitlocation);//读取pixel的值，不进行修改

	void DFT_image(int channel, unsigned int width, unsigned int height, int mode);
	//对第channel号通道进行分块的DFT,直接修改类本身的数据 mode表明是保留幅值还是保留相位 0表示保留幅值1表示保留相位

	void Transfer(void(*pf)(unsigned char*, unsigned char*, unsigned char*));//将数据由XXX图转化到XX图 具体由函数指针决定
	static void RGB2YIQ(unsigned char *, unsigned char *, unsigned char *);//设置成静态变量调用函数指针,具体涉及到成员的指针参考C++primer P741
	static void YIQ2RGB(unsigned char *, unsigned char *, unsigned char *);//这几个函数不应对象的创建而改变，因此适合声明为static
	static void RGB2HSI(unsigned char *, unsigned char *, unsigned char *);
	static void RGB2YCrCb(unsigned char *, unsigned char*, unsigned char*);
	static void RGB2XYZ(unsigned char *, unsigned char*, unsigned char*);
	//static constexpr int sdas = 4;
private:
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//颜色表指针 
	BITMAPINFOHEADER infohead; //40字节的信息头
	BITMAPFILEHEADER filehead; // 14字节的文件头

	void makeBmpTimesof(unsigned int widthtimes, unsigned int heighttimes);//使得BMP图的各通道的长宽是8的倍数,不一定字节数是8的倍数
	unsigned char * extractDataChannel(int channel, DWORD &sizeimage);//如果要调用这个，必须记得delete[]
	void SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount);

	bool ReadBmp(char * name); //只能在构造时调用一次
	bool CheckInImage(int x, int y);//检查是否在BMP图像范围内
	static bool CheckRange0_255(double a);
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation);
	// 得到某一位置下的像素值的储存地址，可能是3个字节，可能是1字节，可能是半个字节，也可能是1位
	//bitlocation 表示当出现半个字节时是前半段还是后半段；当是二值化图时表示在哪一位0-7
	vector<unsigned long> GetPixel_Index(int x, int y, int & bitlocation); //得到x y像素对应的索引数值,基本上和上边的函数逻辑相似
	
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
			result.push_back(abs(double(*iterator - minelement)) / abs(double(maxelement - minelement)));
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
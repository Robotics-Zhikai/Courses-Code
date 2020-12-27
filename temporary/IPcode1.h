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
	//const complex<double>& readcomp() { return comp+complex<double>(1,1); } //�������أ����Ƿ���const���Ͷ����ò�ͬ�����ͺ���

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

	complex<double> TransExp2Complex(double amplitude, double phase)//phase�����ǻ�����
	{
		complex<double> result(amplitude*cos(phase), amplitude*sin(phase));
		return result;
	}	
};

class Operate
{
public:
	vector<complex<double>> FFT_2D(const vector<complex<double>> & input,int M,int N) //��ά��FFT MΪwidth NΪHeight ������ ���µ���
	{
		if (input.size() != M*N)
			throw exception("�������ݴ�С�����ϸ�����M N");
		vector<complex<double>> result(M*N); //���յĴ�СӦ����M*N
		//vector<vector<complex<double>>> linefft(N);
		for (int i = 0; i < N; i++)
		{
			int offset = i*M;
			vector<complex<double>> tobefftline(0); //����ȴ�fft�ĵ�ǰ�е�����
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
	vector<complex<double>> IFFT_2D(const vector<complex<double>> & input, int M, int N)//��ά��IFFT MΪwidth NΪHeight ������ ���µ���
	{
		if (input.size() != M*N)
			throw exception("�������ݴ�С�����ϸ�����M N");
		vector<complex<double>> result(M*N); //���յĴ�СӦ����M*N
											 //vector<vector<complex<double>>> linefft(N);
		for (int i = 0; i < N; i++)
		{
			int offset = i*M;
			vector<complex<double>> tobeifftline(0); //����ȴ�fft�ĵ�ǰ�е�����
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
		if (NOmiga != N || flagfirst == 1) //��ֹ�ظ�����FFTʱ��������getOmiga
										   //�п�����ʵ�ʵ���FFTʱinput����ÿ�ζ���һ������ʱ��Ļ����Կ��Ǹ�һ��Omiga Set��ֻҪ�����ù���Omiga���ʹ���Set�� ÿ��ֻ��Ҫ������ѯ�ĳɱ�����
										   //�ܼ򵥵Ĳ�ѯ��ʱ��ɱ����Խ���O(1) ʵ������Ļ����Ƕ�ʱ�䣬û��ʲô��������
		{
			flagfirst = 0;
			NOmiga = N;
			Omiga = GetOmigaIFFT(N);
		}
		if (Omiga.size() != pow(N - 1, 2) + 1)
			throw exception("�����߼�����Omiga��ά����input��ƥ��");
		
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
	vector<complex<double>> FFT(const vector<complex<double>> & input)//һά��FFT
	{
		auto N = input.size();
		vector<complex<double>> result(N);
		if (N == 0)
			return result;

		static auto NOmiga = N;
		static int flagfirst = 1;
		static vector<complex<double>> Omiga;
		if (NOmiga != N || flagfirst == 1) //��ֹ�ظ�����FFTʱ��������getOmiga
										   //�п�����ʵ�ʵ���FFTʱinput����ÿ�ζ���һ������ʱ��Ļ����Կ��Ǹ�һ��Omiga Set��ֻҪ�����ù���Omiga���ʹ���Set�� ÿ��ֻ��Ҫ������ѯ�ĳɱ�����
										   //�ܼ򵥵Ĳ�ѯ��ʱ��ɱ����Խ���O(1) ʵ������Ļ����Ƕ�ʱ�䣬û��ʲô��������
		{
			flagfirst = 0;
			NOmiga = N;
			Omiga = GetOmiga(N);
		}
		if (Omiga.size() != pow(N - 1, 2) + 1)
			throw exception("�����߼�����Omiga��ά����input��ƥ��");
		
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
	vector<complex<double>> FFT(const vector<ComplexEXP> &input) //��֤�������������ݵ��κ��޸��Ҳ����⿪���ķѼ�����Դ
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
	vector<complex<double>> GetOmiga(int N)//����Ϊ���ݸ���,���ض���N�£��˺�������ظ���ε��õĻ���������ǳ���
	{
		vector<complex<double>> Omiga(pow(N - 1, 2) + 1);
		Omiga[0] = 1;
		for (int i = 1; i < pow(N - 1, 2) + 1; i++)
		{
			Omiga[i] = Omiga[i - 1] * (ComplexEXP(1, -2 * pi / N).readcomp());
		}
		return Omiga;
	}
	vector<complex<double>> GetOmigaIFFT(int N)//����Ϊ���ݸ���,���ض���N�£��˺�������ظ���ε��õĻ���������ǳ���
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

class IPcode1:private Operate //˽�м̳У�IPcode1���û���Ӧ�ÿ���FFT�ȷǳ��ײ�Ĳ���
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
	vector<unsigned char> ReadPixel(int x, int y, int & bitlocation);//��ȡpixel��ֵ���������޸�

	void DFT_image(int channel, unsigned int width, unsigned int height, int mode);
	//�Ե�channel��ͨ�����зֿ��DFT,ֱ���޸��౾������� mode�����Ǳ�����ֵ���Ǳ�����λ 0��ʾ������ֵ1��ʾ������λ

	void Transfer(void(*pf)(unsigned char*, unsigned char*, unsigned char*));//��������XXXͼת����XXͼ �����ɺ���ָ�����
	static void RGB2YIQ(unsigned char *, unsigned char *, unsigned char *);//���óɾ�̬�������ú���ָ��,�����漰����Ա��ָ��ο�C++primer P741
	static void YIQ2RGB(unsigned char *, unsigned char *, unsigned char *);//�⼸��������Ӧ����Ĵ������ı䣬����ʺ�����Ϊstatic
	static void RGB2HSI(unsigned char *, unsigned char *, unsigned char *);
	static void RGB2YCrCb(unsigned char *, unsigned char*, unsigned char*);
	static void RGB2XYZ(unsigned char *, unsigned char*, unsigned char*);
	//static constexpr int sdas = 4;
private:
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//��ɫ��ָ�� 
	BITMAPINFOHEADER infohead; //40�ֽڵ���Ϣͷ
	BITMAPFILEHEADER filehead; // 14�ֽڵ��ļ�ͷ

	void makeBmpTimesof(unsigned int widthtimes, unsigned int heighttimes);//ʹ��BMPͼ�ĸ�ͨ���ĳ�����8�ı���,��һ���ֽ�����8�ı���
	unsigned char * extractDataChannel(int channel, DWORD &sizeimage);//���Ҫ�������������ǵ�delete[]
	void SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount);

	bool ReadBmp(char * name); //ֻ���ڹ���ʱ����һ��
	bool CheckInImage(int x, int y);//����Ƿ���BMPͼ��Χ��
	static bool CheckRange0_255(double a);
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation);
	// �õ�ĳһλ���µ�����ֵ�Ĵ����ַ��������3���ֽڣ�������1�ֽڣ������ǰ���ֽڣ�Ҳ������1λ
	//bitlocation ��ʾ�����ְ���ֽ�ʱ��ǰ��λ��Ǻ��Σ����Ƕ�ֵ��ͼʱ��ʾ����һλ0-7
	vector<unsigned long> GetPixel_Index(int x, int y, int & bitlocation); //�õ�x y���ض�Ӧ��������ֵ,�����Ϻ��ϱߵĺ����߼�����
	
	template<typename Ty>
	Ty max_zk(Ty*begin, Ty*end) //��begin��end��ַ�µ�����Ԫ�ص����ֵ ������ǰ�պ��
	{
		Ty maxelement = *begin;
		//begin->dasda = 1; //���д����� �ǻᱨ���
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			if (*iterator > maxelement)
				maxelement = *iterator;
		}
		return maxelement;
	}
	template<typename Ty>
	Ty min_zk(Ty*begin, Ty*end) //��begin��end��ַ�µ�����Ԫ�ص���Сֵ ������ǰ�պ��
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
	vector<double> normalization_zk(Ty*begin, Ty*end) //��һ�� ��[begin,end]�ڵ���ֵ
	{
		Ty maxelement = max_zk<Ty>(begin, end);
		Ty minelement = min_zk<Ty>(begin, end);
		vector<double> result;
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			result.push_back(abs(double(*iterator - minelement)) / abs(double(maxelement - minelement)));
			/**iterator = ; *///��һ��
		}
		return result;
	}
	//template<typename Ty>
	//void Denormalization_zk(Ty*begin, Ty*end, Ty lower, Ty upper) //���һ�� ��[begin,end]�ڵ���ֵ�϶�Ϊ[0,1]�����������һ����lower��upper
	//{
	//	Ty maxelement = max_zk<Ty>(begin, end);
	//	Ty minelement = min_zk<Ty>(begin, end);
	//	if (maxelement > 1 || minelement < 0)
	//		throw exception("��������ݲ���[0,1]");
	//	if (lower > upper)
	//		throw exception("����lower>upper");
	//	for (auto iterator = begin; iterator != end + 1; iterator++)
	//	{
	//		*iterator = lower + double(upper - lower)*double(*iterator); //���һ��
	//	}
	//}
	template<typename Ty>
	void Denormalization_zk(Ty*begin, Ty*end, Ty lower, Ty upper,const vector<double>& normal_list) 
		//���һ�� ��[begin,end]�ڵ���ֵ,���������һ����lower��upper normal_listΪ��һ���������
	{

		double maxelement = *max_element(normal_list.begin(),normal_list.end());
		double minelement = *min_element(normal_list.begin(), normal_list.end());
		if (maxelement > 1 || minelement < 0)
			throw exception("��������ݲ���[0,1]");
		if (lower > upper)
			throw exception("����lower>upper");
		size_t count = 0;
		for (auto iterator = begin; iterator != end + 1; iterator++)
		{
			*iterator = lower + double(upper - lower)*normal_list[count++]; //���һ��
		}
	}
};
//constexpr int IPcode1::sdas;
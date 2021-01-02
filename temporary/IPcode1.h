#pragma once
#include "main.h"
#include "Operate.h"


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

	IPcode1 operator-(const IPcode1& input)const;//�õ���ͼ��֮��Ĳ��죬�����ȡ����ֵ
	double PSNR(const IPcode1& Origin)const;//�����ֵ����ȣ��������ӦΪthis��ԭʼͼ�� ���ֵԽ�ߣ�˵����ԭͼԽ���
	
	void CropBmp(int locx,int locy,int width, int height);//��ȡBMP��ĳ������
	void SaveBmp(char * path); //���Ϊ��ǰ�����ݵ�ĳһ·���µ�ĳһbmp
	void SaveChannel(char * path,int channel); //�����3ͨ���ģ����ĳһͨ���ĻҶ�ͼ
	vector<unsigned char> ReadPixel(int x, int y, int & bitlocation);//��ȡpixel��ֵ���������޸�

	void Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, string Operation, string inmode, string outmode, double Num=0);
	//�Ե�channel��ͨ�����зֿ��DFT,ֱ���޸��౾������� mode�����Ǳ�����ֵ���Ǳ�����λ 0��ʾ������ֵ1��ʾ������λ
	//Ĭ�ϵĲ�����width height
	void Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, bool ChangeDimension,const vector<double>& KernalTemplate,int stepx,int stepy);
	//��width��height��ģ��˶�channelͨ����ͼ����д��� �����߼����ϱߵ�Kernel_image�ǲ��� step��ʾÿ���ڸ��������ƶ��Ĳ���
	//KernalTemplateҲ�Ǵ����Ҵ��µ��ϵ��������еġ� ChangeDimension��ʾ�任��������Ƿ��иı� �����ֵ�˲�����ֵ�˲����پ�û�иı�
	void Kernel_image(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, bool ChangeDimension, string StatisticalMode, int stepx, int stepy);
	//ͳ�������˲�����KernelImage

	double KernalTemplate_Multiple_subimage(int channel, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const;
	//��Kernelwidth*Kernelheight��KernalTemplate���õ���CenterX��CenterYΪ���ĵ�Сͼ����,���ض�Ӧ�������double����
	double KernalTemplate_Multiple_subimage_y(int channel, LONG Globaly, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const;
	//����Globaly �õ����е�ֵ����һ��������������������Ӻ������õ���
	double KernalTemplate_Multiple_subimage_x(int channel, LONG Globalx, unsigned int Kernelwidth, unsigned int Kernelheight, const vector<double>& KernalTemplate, LONG CenterX, LONG CenterY)const;
	vector<double> AVGKernelTemplate(unsigned int M, unsigned int N);

	void MakeRGB24_TO_Gray8();
	void plot_DCTkernel_Image(int N); //����DCT����任�˵�ͼ�� �Ե�ǰͼƬ�Ĵ�СΪ��������������任�˻�ͼ��
	vector<double> Histogram(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru); 
	//�õ�ĳһͨ���ֲ��ڸ�����Χ�ڵı���ͼ�����ݵ�Ƶ�ʷֲ�ֱ��ͼ
	void Equalization_Image(int Channel, vector<LONG> ld , vector<LONG> lu , vector<LONG> rd, vector<LONG> ru);
	//���⻯ĳһchannel��ֱ��ͼ������Ч����ʾ��ͼƬ��
	void AddNoise(int Channel, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, string NoiseMode, double par0=0, double par1=0, double par2=0, double par3=0);
	//Ĭ�����ʱ������ͨ������

	LONG ReadWidth()const { return infohead.biWidth; }
	LONG ReadHeight()const { return infohead.biHeight; }

	

	void Transfer(void(*pf)(unsigned char*, unsigned char*, unsigned char*));//��������XXXͼת����XXͼ �����ɺ���ָ�����
	static void RGB2YIQ(unsigned char *, unsigned char *, unsigned char *);//���óɾ�̬�������ú���ָ��,�����漰����Ա��ָ��ο�C++primer P741
	static void YIQ2RGB(unsigned char *, unsigned char *, unsigned char *);//�⼸��������Ӧ����Ĵ������ı䣬����ʺ�����Ϊstatic
	static void RGB2HSI(unsigned char *, unsigned char *, unsigned char *);
	static void RGB2YCrCb(unsigned char *, unsigned char*, unsigned char*);
	static void RGB2XYZ(unsigned char *, unsigned char*, unsigned char*);
	static void RGB2GrayValue(unsigned char *, unsigned char*, unsigned char*); //RGBת��Ϊ�Ҷ�ͼ �α���24λ
	//static constexpr int sdas = 4;
private:
	unsigned char * pbmpBuf ;
	RGBQUAD *pcolortable;//��ɫ��ָ�� 
	BITMAPINFOHEADER infohead; //40�ֽڵ���Ϣͷ
	BITMAPFILEHEADER filehead; // 14�ֽڵ��ļ�ͷ

	void makeBmpTimesof(unsigned int widthtimes, unsigned int heighttimes);//ʹ��BMPͼ�ĸ�ͨ���ĳ�����widthtimes heighttimes�ı���,��һ���ֽ�����8�ı���
	unsigned char * extractDataChannel(int channel, DWORD &sizeimage);//���Ҫ�������������ǵ�delete[]
	void SaveBmpTool(char * path, unsigned char *imgBuf, int width, int height, int biBitCount)const;

	bool ReadBmp(char * name); //ֻ���ڹ���ʱ����һ��
	bool CheckInImage(int x, int y)const;//����Ƿ���BMPͼ��Χ��
	void CheckRectangleInBigRect(vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru, LONG width, LONG height)const;
	// ��ⷽ���Ƿ���ͼ���У�������Ƿ�����߲���ͼ��Χ�ڣ���throw

	static bool CheckRange0_255(double a);
	vector<unsigned char *> GetPixel(int x, int y,int & bitlocation)const;
	// �õ�ĳһλ���µ�����ֵ�Ĵ����ַ��������3���ֽڣ�������1�ֽڣ������ǰ���ֽڣ�Ҳ������1λ
	//bitlocation ��ʾ�����ְ���ֽ�ʱ��ǰ��λ��Ǻ��Σ����Ƕ�ֵ��ͼʱ��ʾ����һλ0-7
	vector<unsigned long> GetPixel_Index(int x, int y, int & bitlocation)const; //�õ�x y���ض�Ӧ��������ֵ,�����Ϻ��ϱߵĺ����߼�����
	
	void ChangeSingleChannelInputNEWptrData(unsigned char * newbmpdataptr,LONG width,LONG height, vector<LONG> ld, vector<LONG> lu, vector<LONG> rd, vector<LONG> ru,unsigned char value);
	//������ĵ�ͨ������newbmpdataptr��������ĵ�ַ�����Ա�����޹أ�Ϊ��㣬ld lu rd ruΪ���޸ĵ����ط���,valueΪ��Ҫ�޸ĵ�ֵ 
	//width heightΪnewbmpdataptr�����ͼ��ĳ��Ϳ�

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
			if (maxelement != minelement)
				result.push_back(abs(double(*iterator - minelement)) / abs(double(maxelement - minelement)));
			else
				result.push_back(0.5);
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
#pragma once
#include "main.h"
#include "IPcode1.h"
#include <random>

class ComplexEXP
{
public:
	ComplexEXP() = default;
	ComplexEXP(double amplitude1, double phase1) :comp(TransExp2Complex(amplitude1, phase1)), amplitude(amplitude1), phase(phase1) {}
	ComplexEXP(complex<double> input) :
		comp(input), amplitude(std::sqrt(std::pow(input.imag(), 2) + std::pow(input.real(), 2))), phase(std::atan2(input.imag(), input.real())) {}

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
	double GaussianNoiseGenerator(double mean, double variance) //输入参数为均值和方差 可以多次重复调用
	{
		static double meanstatic = mean;
		static double variancestatic = variance;
		static default_random_engine e;
		static normal_distribution<double> n(meanstatic, variancestatic);
		if (meanstatic != mean || variance != variancestatic)
		{
			meanstatic = mean;
			variancestatic = variance;
			normal_distribution<double> ntmp(meanstatic, variancestatic);
			n = ntmp;
		}
		return n(e);
	}
	double ImpulseNoiseGenerator(double a, double b, double Pa,double Pb)
	{
		if (Pa < 0 || Pb < 0)
			throw exception("概率不能小于0");
		if (Pa + Pb > 1)
			throw exception("Pa+Pb>1");

		static default_random_engine e;
		static uniform_real_distribution<double> n(0, 1);
		double randomnum = n(e);
		if (randomnum < Pa)
		{
			return a;
		}
		else if (randomnum >= Pa&&randomnum < Pa + Pb)
		{
			return b;
		}
		else
			return 0;
	}
	void TransvecComplex2Double(const vector<complex<double>>& input,vector<double> & output)
	{
		output.resize(input.size());
		for (int i = 0; i < input.size(); i++)
			output[i] = input[i].real();
	}
	void TransvecDouble2Complex(const vector<double>& input, vector<complex<double>> & output)
	{
		output.resize(input.size());
		for (int i = 0; i < input.size(); i++)
			output[i] = input[i];
	}
	void zigzagRetainDCT(vector<double> & DCT, int N_DCT, int RetainN)
		//zigzag保留DCT系数,保留RetainN个 N_DCT表示DCT的长或者宽
	{
		if (N_DCT <= 0)
			throw exception("N_DCT数值不能小于等于0");
		if (DCT.size() != N_DCT*N_DCT)
			throw exception("DCT应该是N_DCT*N_DCT维的，之前调用的函数肯定出错了");
		int stepRight = 1;
		int stepLeftUp = N_DCT - 1;
		int stepUp = N_DCT;
		int stepRightDown = -(N_DCT - 1);

		int zigzagseqcount = 0;
		vector<int> zigzagseq(N_DCT*N_DCT);

		int Num = 0;
		zigzagseq[zigzagseqcount++] = Num;
		if (N_DCT > 1)
		{
			Num += stepRight;
			zigzagseq[zigzagseqcount++] = Num;
			while (Num != N_DCT *N_DCT - 1)
			{
				while (1)
				{
					Num += stepLeftUp;
					zigzagseq[zigzagseqcount++] = Num;
					if (Num%N_DCT == 0)
						break;
					if (Num >= (N_DCT - 1)*N_DCT)
						break;
				}
				if (Num < (N_DCT - 1)*N_DCT)
				{
					Num += stepUp;
					zigzagseq[zigzagseqcount++] = Num;
				}
				else
				{
					Num += stepRight;
					zigzagseq[zigzagseqcount++] = Num;
				}
				if (Num == N_DCT*N_DCT - 1)
					break;
				while (1)
				{
					Num += stepRightDown;
					zigzagseq[zigzagseqcount++] = Num;
					if (Num < N_DCT )
					{
						if (Num + stepRight < N_DCT)
						{
							Num += stepRight;
							zigzagseq[zigzagseqcount++] = Num;
						}
						else
						{
							Num += stepUp;
							zigzagseq[zigzagseqcount++] = Num;
						}
						break;
					}
					else if ((Num + 1) % N_DCT == 0)
					{
						Num += stepUp;
						zigzagseq[zigzagseqcount++] = Num;
						break;
					}
				}
			}
		}
		if (zigzagseq.size() != N_DCT*N_DCT)
			throw exception("zigzag逻辑有问题");

		for (int i = RetainN; i < DCT.size(); i++)
		{
			DCT[zigzagseq[i]] = 0;
		}

	}
	vector<double> IDCT_2D(const vector<double> & input, int N)//N*N的DCT逆变换
	{
		if (N == 0)
			throw ("N不能等于0");
		if (input.size() != N*N)
			throw exception("input必须是方阵"); //也有可能不是方阵，但时间有限，就弄成方阵吧

		double Cu, Cv;
		vector<double> store(2);
		store[0] = 1 / std::sqrt(N);
		store[1] = std::sqrt(2) / std::sqrt(N);

		vector<double> result(N*N);
		for (int y = 0; y < N; y++)
		{
			for (int x = 0; x < N; x++)
			{
				result[x + y*N] = 0;
				for (size_t v = 0; v < N; v++)
				{
					for (size_t u = 0; u < N; u++)
					{
						if (u == 0)
							Cu = store[0];
						else
							Cu = store[1];
						if (v == 0)
							Cv = store[0];
						else
							Cv = store[1];

						result[x + y*N] += Cu*Cv*input[u + v*N] * cos((x + 0.5)*pi*u / N)*cos((y + 0.5)*pi*v / N);
					}
				}
				result[x + y*N] *= 1;
			}
		}
		//output = result;
		return result;
	}
	/*void IDCT_2D(vector<complex<double>> & input, int N)
	{
		vector<double> trans(input.size());
		for (int i = 0; i < input.size(); i++)
			trans[i] = input[i].real();
		trans = IDCT_2D(trans, N);
		for (int i = 0; i < trans.size(); i++)
		{
			input[i] = trans[i];
		}
	}*/
	//void DCT_2D(vector<complex<double>> & input, int N)
	//{
	//	vector<double> trans(input.size());
	//	for (int i = 0; i < input.size(); i++)
	//		trans[i] = input[i].real();
	//	trans = DCT_2D(trans, N);
	//	for (int i = 0; i < trans.size(); i++)
	//	{
	//		input[i] = trans[i];
	//	}
	//}
	vector<double> DCT_2D(const vector<double> & input, int N) //N*N的DCT变换
	{
		if (N == 0)
			throw ("N不能等于0");
		if (input.size() != N*N)
			throw exception("input必须是方阵"); //也有可能不是方阵，但时间有限，就弄成方阵吧

		vector<double> store(2);
		store[0] = 1 / std::sqrt(N);
		store[1] = std::sqrt(2) / std::sqrt(N);
		double Cu, Cv;

		vector<double> result(N*N);
		for (int v = 0; v < N; v++)
		{
			for (int u = 0; u < N; u++)
			{
				if (u == 0)
					Cu = store[0];
				else 
					Cu = store[1];
				if (v == 0)
					Cv = store[0];
				else
					Cv = store[1];

				result[u + v*N] = 0;
				for (size_t y = 0; y < N; y++)
				{
					for (size_t x = 0; x < N; x++)
					{
						result[u + v*N] += input[x + y*N] * cos((x + 0.5)*pi*u / N)*cos((y + 0.5)*pi*v / N);
					}
				}
				result[u + v*N] *= (Cu*Cv);
			}
		}
		
		return result;
	}
	vector<complex<double>> FFT_2D(const vector<complex<double>> & input, int M, int N) //二维的FFT M为width N为Height 从左到右 从下到上
	{
		if (input.size() != M*N)
			throw exception("输入数据大小不符合给定的M N");
		vector<complex<double>> result(M*N); //最终的大小应该是M*N
											 //vector<vector<complex<double>>> linefft(N);
		for (int i = 0; i < N; i++)
		{
			int offset = i*M;
			vector<complex<double>> tobefftline(0); //储存等待fft的当前行的数据
			for (int j = offset; j < offset + M; j++)
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
	vector<complex<double>> IDFT(const vector<complex<double>> & input)
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
			result[i] /= N;
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

		if (IsInteger(log2(N)))
		{
			for (int i = 0; i < N; i++)
				result[i] = input[i];
			unsigned int IterateTimes = log2(N);
			int k;
			for (unsigned int s = 1; s <= IterateTimes; s++)
			{
				k = N / pow(2, s); //对偶节点的间隔k
				int offset = 0;
				while (1)
				{
					unsigned int p = 0;
					for (int x = offset; x < offset + k; x++)
					{
						if (x == offset)
							p = GetFFT_Omiga_p(x, s, IterateTimes);
						auto store = Omiga[p] * result[x + k];
						auto newresultx = result[x] + store;
						result[x + k] = result[x] - store;
						result[x] = newresultx;
					}
					offset += 2 * k;
					if (offset > N - 1)
						break;
				}
			}
			vector<complex<double>> resultTmp(N);
			for (int i = 0; i < N; i++)
			{
				resultTmp[i] = result[Inverse_uint(i, IterateTimes)];
				resultTmp[i] /= N;
			}
			return resultTmp;
		}
		else
		{
			cout << "IFFT的输入数据长度不是2的整数次幂，计算效率低" << endl;
			return IDFT(input);
		}
	}
	vector<complex<double>> DFT(const vector<complex<double>> & input)
	{
		auto N = input.size();
		vector<complex<double>> result(N);
		if (N == 0)
			return result;

		static auto NOmiga = N;
		static int flagfirst = 1;
		static vector<complex<double>> Omiga;
		if (NOmiga != N || flagfirst == 1) //防止重复调用DFT时反复调用getOmiga
										   //有可能在实际调用DFT时input数据每次都不一样，有时间的话可以考虑搞一个Omiga Set，只要曾经用过的Omiga，就存在Set里 每次只需要付出查询的成本即可
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
										   //有可能在实际调用DFT时input数据每次都不一样，有时间的话可以考虑搞一个Omiga Set，只要曾经用过的Omiga，就存在Set里 每次只需要付出查询的成本即可
										   //很简单的查询的时间成本可以降到O(1) 实现这个的话就是堆时间，没有什么技术含量
		{
			flagfirst = 0;
			NOmiga = N;
			Omiga = GetOmiga(N);
		}
		if (Omiga.size() != pow(N - 1, 2) + 1)
			throw exception("代码逻辑出错，Omiga的维度与input不匹配");

		if (IsInteger(log2(N)))
		{
			for (int i = 0; i < N; i++)
				result[i] = input[i];
			unsigned int IterateTimes = log2(N);
			int k;
			for (unsigned int s = 1; s <= IterateTimes; s++)
			{
				k = N / pow(2, s); //对偶节点的间隔k
				int offset = 0;
				while (1)
				{
					unsigned int p = 0;
					for (int x = offset; x < offset+k; x++)
					{
						if (x==offset)
							p = GetFFT_Omiga_p(x, s, IterateTimes);
						auto store = Omiga[p] * result[x + k];
						auto newresultx = result[x] + store;
						result[x + k] = result[x] - store;
						result[x] = newresultx;
					}
					offset += 2 * k;
					if (offset > N - 1)
						break;
				}
			}
			vector<complex<double>> resultTmp(N);
			for (int i = 0; i < N; i++)
			{
				resultTmp[i] = result[Inverse_uint(i, IterateTimes)];
			}
			return resultTmp;
		}
		else
		{
			cout << "FFT的输入数据长度不是2的整数次幂，计算效率低" << endl;
			return DFT(input);
		}
		
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
	double sinxDividex(double x)
	{
		if (abs(x) < dt)
			return 1;
		else
			return sin(x) / x;
	}
private:
	unsigned int Inverse_uint(unsigned int x, unsigned int LimitBitCount)//限制低LimitBitCount位进行倒序
	{
		unsigned int bitCount = sizeof(unsigned int) * 8;
		if (LimitBitCount > bitCount)
			throw exception("LimitBitCount>bitCount");

		vector<int> bitInverse;
		unsigned int base = 1;
		for (int i = 0; i < LimitBitCount; i++)
		{
			auto basethis = base << i;
			unsigned int thisbit = x&basethis;
			if (thisbit == 0)
				bitInverse.push_back(0);
			else
				bitInverse.push_back(1);
		}

		unsigned int result = 0;
		auto bitInverseN = bitInverse.size();
		base = 1;
		for (int i = 1; i <= bitInverseN; i++)
		{
			result += bitInverse[bitInverseN - i] * base;
			base = base << 1;
		}
		return result;
	}
	unsigned int GetFFT_Omiga_p(unsigned int x,int s, unsigned int Log2N)//得到omiga的p值(指数值)
	{
		x = (x >> (Log2N - s));
		x = Inverse_uint(x, Log2N);
		return x;
	}
	bool IsInteger(double in) //判断输入参数是否是整数
	{
		if (abs(round(in) - in) < 0.000000000000001)
			return 1;
		else
			return 0;
	}
	vector<complex<double>> GetOmiga(int N)//输入为数据个数,在特定的N下，此函数如果重复多次调用的话会计算量非常大
	{
		vector<complex<double>> Omiga(pow(N - 1, 2) + 1);
		Omiga[0] = 1;
		for (int i = 1; i < pow(N - 1, 2) + 1; i++)
		{
			if (i < N)
				Omiga[i] = Omiga[i - 1] * (ComplexEXP(1, -2 * pi / N).readcomp());
			else
				Omiga[i] = Omiga[i%N];
		}
		return Omiga;
	}
	vector<complex<double>> GetOmigaIFFT(int N)//输入为数据个数,在特定的N下，此函数如果重复多次调用的话会计算量非常大
	{
		vector<complex<double>> Omiga(pow(N - 1, 2) + 1);
		Omiga[0] = 1;
		for (int i = 1; i < pow(N - 1, 2) + 1; i++)
		{
			if (i < N)
				Omiga[i] = Omiga[i - 1] * (ComplexEXP(1, 2 * pi / N).readcomp());
			else
				Omiga[i] = Omiga[i%N];
		}
		return Omiga;
	}
};

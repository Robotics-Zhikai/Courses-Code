#pragma once
#include "main.h"
#include "IPcode1.h"

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
		//zigzag����DCTϵ��,����RetainN�� N_DCT��ʾDCT�ĳ����߿�
	{
		if (N_DCT <= 0)
			throw exception("N_DCT��ֵ����С�ڵ���0");
		if (DCT.size() != N_DCT*N_DCT)
			throw exception("DCTӦ����N_DCT*N_DCTά�ģ�֮ǰ���õĺ����϶�������");
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
			throw exception("zigzag�߼�������");

		for (int i = RetainN; i < DCT.size(); i++)
		{
			DCT[zigzagseq[i]] = 0;
		}

	}
	vector<double> IDCT_2D(const vector<double> & input, int N)//N*N��DCT��任
	{
		if (N == 0)
			throw ("N���ܵ���0");
		if (input.size() != N*N)
			throw exception("input�����Ƿ���"); //Ҳ�п��ܲ��Ƿ��󣬵�ʱ�����ޣ���Ū�ɷ����

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
	vector<double> DCT_2D(const vector<double> & input, int N) //N*N��DCT�任
	{
		if (N == 0)
			throw ("N���ܵ���0");
		if (input.size() != N*N)
			throw exception("input�����Ƿ���"); //Ҳ�п��ܲ��Ƿ��󣬵�ʱ�����ޣ���Ū�ɷ����

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
	vector<complex<double>> FFT_2D(const vector<complex<double>> & input, int M, int N) //��ά��FFT MΪwidth NΪHeight ������ ���µ���
	{
		if (input.size() != M*N)
			throw exception("�������ݴ�С�����ϸ�����M N");
		vector<complex<double>> result(M*N); //���յĴ�СӦ����M*N
											 //vector<vector<complex<double>>> linefft(N);
		for (int i = 0; i < N; i++)
		{
			int offset = i*M;
			vector<complex<double>> tobefftline(0); //����ȴ�fft�ĵ�ǰ�е�����
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
			result[i] /= N;
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

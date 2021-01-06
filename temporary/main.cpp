#include "main.h"
#include "IPcode1.h"

//---
//���¸�ģ�������BMPͼ��(��ɫͼ����24bit RGB��8bit)�����ػ�ȡ���������ļ���Ϊxiang_su_zhi.txt��
unsigned char *pBmpBuf;//����ͼ�����ݵ�ָ��

int bmpWidth;//ͼ��Ŀ�
int bmpHeight;//ͼ��ĸ�
RGBQUAD *pColorTable;//��ɫ��ָ��

int biBitCount;//ͼ�����ͣ�ÿ����λ��

			   //-------
			   //��ͼ���λͼ���ݡ����ߡ���ɫ��ÿ����λ�������ݽ��ڴ棬�������Ӧ��ȫ�ֱ�����
bool readBmp(char *bmpName)
{
	FILE *fp = fopen(bmpName, "rb");//�����ƶ���ʽ��ָ����ͼ���ļ�

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹBITMAPFILEHEADER
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//����λͼ��Ϣͷ�ṹ��������ȡλͼ��Ϣͷ���ڴ棬����ڱ���head��
	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //��ȡͼ����ߡ�ÿ������ռλ������Ϣ

	bmpWidth = head.biWidth;

	bmpHeight = head.biHeight;

	biBitCount = head.biBitCount;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256

	if (biBitCount == 8)
	{

		//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�

		pColorTable = new RGBQUAD[256];

		fread(pColorTable, sizeof(RGBQUAD), 256, fp);

	}

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	fclose(fp);//�ر��ļ�

	return 1;//��ȡ�ļ��ɹ�
}

//-----
//����һ��ͼ��λͼ���ݡ����ߡ���ɫ��ָ�뼰ÿ������ռ��λ������Ϣ,����д��ָ���ļ���
bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{

	//���λͼ����ָ��Ϊ0����û�����ݴ��룬��������
	if (!imgBuf)
		return 0;

	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ1024�ֽڣ���ɫͼ����ɫ���СΪ0
	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//�Զ�����д�ķ�ʽ���ļ�
	FILE *fp = fopen(bmpName, "wb");

	if (fp == 0)
		return 0;

	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ
	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp����

							 //bfSize��ͼ���ļ�4����ɲ���֮��
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��
	fileHead.bfOffBits = 54 + colorTablesize;

	//д�ļ�ͷ���ļ�
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ
	BITMAPINFOHEADER head;

	head.biBitCount = biBitCount;

	head.biClrImportant = 0;

	head.biClrUsed = 0;

	head.biCompression = 0;

	head.biHeight = height;

	head.biPlanes = 1;

	head.biSize = 40;

	head.biSizeImage = lineByte*height;

	head.biWidth = width;

	head.biXPelsPerMeter = 0;

	head.biYPelsPerMeter = 0;

	//дλͼ��Ϣͷ���ڴ�
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//����Ҷ�ͼ������ɫ��д���ļ� 
	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//дλͼ���ݽ��ļ�
	fwrite(imgBuf, height*lineByte, 1, fp);

	//�ر��ļ�
	fclose(fp);

	return 1;

}

//----
//����Ϊ���صĶ�ȡ����
void doIt()
{

	//����ָ��BMP�ļ����ڴ�
	char readPath[] = "D:\\Study\\˶ʿ\\�ҵĹ���\\˶ʿѧλ�γ�\\����ͼ����\\Code\\lena512color.bmp";

	readBmp(readPath);

	//���ͼ�����Ϣ
	cout << "width=" << bmpWidth << " height=" << bmpHeight << " biBitCount=" << biBitCount << endl;

	//ѭ��������ͼ�������
	//ÿ���ֽ���
	int lineByte = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;

	//ѭ����������Բ�ɫͼ�񣬱���ÿ���ص���������
	int m = 0, n = 0, count_xiang_su = 0;

	//��ͼ�����½�1/4�����óɺ�ɫ
	ofstream outfile("ͼ������.txt", ios::in | ios::trunc);

	if (biBitCount == 8) //���ڻҶ�ͼ��
	{
		//
		//�������ͼ��ķָ��8*8С��Ԫ����������ֵ�洢��ָ���ı��С�����BMPͼ������������Ǵ�
		//���½ǣ��������ң�������������ɨ���
		int L1 = 0;
		int hang = 63;
		int lie = 0;
		//int L2=0;
		//int fen_ge=8;
		for (int fen_ge_hang = 0; fen_ge_hang<8; fen_ge_hang++)//64*64������ѭ��
		{
			for (int fen_ge_lie = 0; fen_ge_lie<8; fen_ge_lie++)//64*64�о���ѭ��
			{
				//--------
				for (L1 = hang; L1>hang - 8; L1--)//8*8������
				{
					for (int L2 = lie; L2<lie + 8; L2++)//8*8������
					{
						m = *(pBmpBuf + L1*lineByte + L2);
						outfile << m << " ";
						count_xiang_su++;
						if (count_xiang_su % 8 == 0)//ÿ8*8��������ı��ļ�
						{
							outfile << endl;
						}
					}
				}
				//---------
				hang = 63 - fen_ge_hang * 8;//64*64�����б任
				lie += 8;//64*64�����б任
						 //��һ�У�64����8��8*8����������
			}
			hang -= 8;//64*64������б任
			lie = 0;//64*64juzhen
		}
	}

	//double xiang_su[2048];
	//ofstream outfile("xiang_su_zhi.txt",ios::in|ios::trunc);
	if (!outfile)
	{
		cout << "open error!" << endl;
		exit(1);
	}
	else if (biBitCount == 24)
	{//��ɫͼ��
		for (int i = 0; i<bmpHeight; i++)
		{
			for (int j = 0; j<bmpWidth; j++)
			{
				for (int k = 0; k<3; k++)//ÿ����RGB���������ֱ���0�ű�ɺ�ɫ
				{
					//*(pBmpBuf+i*lineByte+j*3+k)-=40;
					m = *(pBmpBuf + i*lineByte + j * 3 + k);
					outfile << m << " ";
					count_xiang_su++;
					if (count_xiang_su % 8 == 0)
					{
						outfile << endl;
					}
					//n++;
				}
				n++;
			}


		}
		cout << "�ܵ����ظ���Ϊ:" << n << endl;
		cout << "----" << endl;
	}

	//��ͼ�����ݴ���
	char writePath[] = "nvcpy.BMP";//ͼƬ������ٴ洢

	saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

	//�����������pBmpBuf��pColorTable��ȫ�ֱ��������ļ�����ʱ����Ŀռ�
	delete[]pBmpBuf;

	if (biBitCount == 8)
		delete[]pColorTable;
}

char path[] = "D:\\Study\\˶ʿ\\�ҵĹ���\\˶ʿѧλ�γ�\\����ͼ����\\Code\\lena512color.bmp";

void Experiment1()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//ʵ��һ
	IPcode1 ip0(path);
	IPcode1 ip1(path);
	IPcode1 ip3(path);
	IPcode1 ip4(path);
	IPcode1 ip5(path);

	int bitloc;
	vector<unsigned char> vecpixel = ip0.ReadPixel(231, 231, bitloc);
	for (int i = 0; i < 3; i++)
	{
		cout << int(vecpixel[i]) << " ";
	}
	cout << endl;

	ip0.SaveBmp("origin.bmp");
	ip1.Transfer(IPcode1::RGB2YIQ);
	IPcode1 ip2 = ip1;
	ip2.Transfer(IPcode1::YIQ2RGB);
	ip3.Transfer(IPcode1::RGB2HSI);
	ip4.Transfer(IPcode1::RGB2YCrCb);
	ip5.Transfer(IPcode1::RGB2XYZ);

	ip1.SaveBmp("YIQ.bmp");
	ip1.SaveChannel("YIQ_Y.bmp", 0);
	ip1.SaveChannel("YIQ_I.bmp", 1);
	ip1.SaveChannel("YIQ_Q.bmp", 2);

	ip2.SaveBmp("RGB.bmp");
	ip2.SaveChannel("RGB_R.bmp", 0);
	ip2.SaveChannel("RGB_G.bmp", 1);
	ip2.SaveChannel("RGB_B.bmp", 2);

	ip3.SaveBmp("HSI.bmp");
	ip3.SaveChannel("HSI_H.bmp", 0);
	ip3.SaveChannel("HSI_S.bmp", 1);
	ip3.SaveChannel("HSI_I.bmp", 2);

	ip4.SaveBmp("YCrCb.bmp");
	ip4.SaveChannel("YCrCb_Y.bmp", 0);
	ip4.SaveChannel("YCrCb_Cr.bmp", 1);
	ip4.SaveChannel("YCrCb_Cb.bmp", 2);

	ip5.SaveBmp("XYZ.bmp");
	ip5.SaveChannel("XYZ_X.bmp", 0);
	ip5.SaveChannel("XYZ_Y.bmp", 1);
	ip5.SaveChannel("XYZ_Z.bmp", 2);
	///////////////////////////////////////////////////////////////////////////////////////////////
}

void Experiment2()
{

}

void main()
{
	try
	{
		unsigned char grhd=0;
		grhd += 1232.1;
		grhd = 255;
		grhd++;
		Operate stast;
		vector<double> DCTtest = { 1 ,2,3,4 };
		vector<double> ssdafa = stast.IDCT_2D(DCTtest, 2);
		Operate ttt;
		ttt.zigzagRetainDCT(vector<double>(64), 8, 1);
		cout << sin(5.7) << endl;
		vector<complex<double>> testtp = { complex<double>(3,2),complex<double>(-3,3),complex<double>(-3,-4),complex<double>(3,-5) };
		
		vector<complex<double>> result = stast.IFFT_2D(testtp, 2, 2);



		///////////////////////////////////////////////////////////////////////////////////////////////
		//ʵ���
		auto tbegin = clock();
		IPcode1 ex2_origin(path);
		

		IPcode1 ex2_origin_gray8 = ex2_origin;
		ex2_origin_gray8.MakeRGB24_TO_Gray8();
		ex2_origin_gray8.SaveBmp("ex2_origin_gray8.bmp");

		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_phase512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_phase512.Kernel_image(0, 512, 512, "DFT_IDFT", "phase", "phase"); //����ؽ�
		ex2_origin_gray8_DFT_phase_IDFT_phase512.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_phase512.bmp");

		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_ampl512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_ampl512.Kernel_image(0, 512, 512, "DFT_IDFT", "phase", "ampl"); //����ؽ�
		ex2_origin_gray8_DFT_phase_IDFT_ampl512.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_ampl512.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_ampl512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_ampl512.Kernel_image(0, 512, 512, "DFT_IDFT", "ampl", "ampl"); //���ؽ�
		ex2_origin_gray8_DFT_ampl_IDFT_ampl512.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_ampl512.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_phase512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_phase512.Kernel_image(0, 512, 512, "DFT_IDFT", "ampl", "phase"); //���ؽ�
		ex2_origin_gray8_DFT_ampl_IDFT_phase512.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_phase512.bmp");


		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_phase = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_phase.Kernel_image(0, 8, 8, "DFT_IDFT", "phase", "phase"); //����ؽ�
		ex2_origin_gray8_DFT_phase_IDFT_phase.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_phase.bmp");

		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_ampl = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_ampl.Kernel_image(0, 8, 8, "DFT_IDFT", "phase", "ampl"); //����ؽ�
		ex2_origin_gray8_DFT_phase_IDFT_ampl.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_ampl.bmp");
		//��ʵ����ؽ���ͼphase�����ampl��������������յ� ��ǲ���0����pi -pi ����phase phaseͼ���Կ������������Ҷȼ� �ֱ����-pi 0 pi

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_ampl = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_ampl.Kernel_image(0, 8, 8, "DFT_IDFT", "ampl", "ampl"); //���ؽ�
		ex2_origin_gray8_DFT_ampl_IDFT_ampl.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_ampl.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_phase = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_phase.Kernel_image(0, 8, 8, "DFT_IDFT", "ampl", "phase"); //���ؽ�
		ex2_origin_gray8_DFT_ampl_IDFT_phase.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_phase.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl.Kernel_image(0, 8, 8, "DFT", "whatever", "ampl");
		ex2_origin_gray8_DFT_ampl.SaveBmp("ex2_origin_gray8_DFT_ampl.bmp");//����Ǿ��������궨��

		IPcode1 ex2_origin_gray8_DFT_phase = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase.Kernel_image(0, 8, 8, "DFT", "whatever", "phase");
		ex2_origin_gray8_DFT_phase.SaveBmp("ex2_origin_gray8_DFT_phase.bmp");

		IPcode1 ex2_origin_gray8_Sobel_gui1 = ex2_origin_gray8;
		ex2_origin_gray8_Sobel_gui1.Kernel_image(0, 3, 3, 1, "sobel", 1, 1);
		ex2_origin_gray8_Sobel_gui1.SaveBmp("ex2_origin_gray8_Sobel_gui1.bmp");
		//IPcode1 ex2_origin_gray8_Sobel_notgui1 = ex2_origin_gray8;
		//ex2_origin_gray8_Sobel_notgui1.Kernel_image(0, 3, 3, 0, "sobel", 1, 1);
		//ex2_origin_gray8_Sobel_notgui1.SaveBmp("ex2_origin_gray8_Sobel_notgui1.bmp");
		clock_t tend = clock();
		cout << "������" << (double)(tend - tbegin) / CLOCKS_PER_SEC << "��" << endl;

		IPcode1 ex2_origin_gray8_addNoise_guass = ex2_origin_gray8;
		auto width = ex2_origin_gray8_addNoise_guass.ReadWidth();
		auto height = ex2_origin_gray8_addNoise_guass.ReadHeight();
		ex2_origin_gray8_addNoise_guass.AddNoise(0, { 0,0 }, { 0,height - 1 }, { width - 1,0 }, { width - 1,height - 1 }, "Gaussian", 15, 5);
		ex2_origin_gray8_addNoise_guass.SaveBmp("ex2_origin_gray8_addNoise_guass.bmp");
		IPcode1 ex2_origin_gray8_addNoise_guass_medianFilter = ex2_origin_gray8_addNoise_guass;
		ex2_origin_gray8_addNoise_guass_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
		ex2_origin_gray8_addNoise_guass_medianFilter.SaveBmp("ex2_origin_gray8_addNoise_guass_medianFilter.bmp");
		cout << "ex2_origin_gray8_addNoise_guass_medianFilter SSIM" << ex2_origin_gray8_addNoise_guass_medianFilter.SSIM(ex2_origin_gray8, 0) << endl;
		cout << "ex2_origin_gray8_addNoise_guass_medianFilter PSNR" << ex2_origin_gray8_addNoise_guass_medianFilter.PSNR(ex2_origin_gray8) << endl;

		ex2_origin_gray8_addNoise_guass.Kernel_image(0, 3, 3, 0, ex2_origin_gray8_addNoise_guass.AVGKernelTemplate(3, 3), 1, 1);
		ex2_origin_gray8_addNoise_guass.SaveBmp("ex2_origin_gray8_addNoise_guass_AVGKernelTemplate3_3.bmp");
		cout << "ex2_origin_gray8_addNoise_guass_AVGKernelTemplate3_3 SSIM" << ex2_origin_gray8_addNoise_guass.SSIM(ex2_origin_gray8, 0) << endl;
		cout << "ex2_origin_gray8_addNoise_guass_AVGKernelTemplate3_3 PSNR" << ex2_origin_gray8_addNoise_guass.PSNR(ex2_origin_gray8) << endl;
		

		IPcode1 ex2_origin_gray8_addNoise_Impulse = ex2_origin_gray8;
		ex2_origin_gray8_addNoise_Impulse.AddNoise(0, { 0,0 }, { 0,height - 1 }, { width - 1,0 }, { width - 1,height - 1 }, "Impulse", 1000,-1000,0.01,0.01);
		ex2_origin_gray8_addNoise_Impulse.SaveBmp("ex2_origin_gray8_addNoise_Impulse.bmp");
		IPcode1 ex2_origin_gray8_addNoise_Impulse_medianFilter = ex2_origin_gray8_addNoise_Impulse;
		ex2_origin_gray8_addNoise_Impulse_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
		ex2_origin_gray8_addNoise_Impulse_medianFilter.SaveBmp("ex2_origin_gray8_addNoise_Impulse_medianFilter.bmp");
		cout << "ex2_origin_gray8_addNoise_Impulse_medianFilter SSIM" << ex2_origin_gray8_addNoise_Impulse_medianFilter.SSIM(ex2_origin_gray8, 0) << endl;
		cout << "ex2_origin_gray8_addNoise_Impulse_medianFilter PSNR" << ex2_origin_gray8_addNoise_Impulse_medianFilter.PSNR(ex2_origin_gray8) << endl;

		ex2_origin_gray8_addNoise_Impulse.Kernel_image(0, 3, 3, 0, ex2_origin_gray8_addNoise_guass.AVGKernelTemplate(3, 3), 1, 1);
		ex2_origin_gray8_addNoise_Impulse.SaveBmp("ex2_origin_gray8_addNoise_Impulse_AVGKernelTemplate3_3.bmp");
		cout << "ex2_origin_gray8_addNoise_Impulse_AVGKernelTemplate3_3 SSIM" << ex2_origin_gray8_addNoise_Impulse.SSIM(ex2_origin_gray8, 0) << endl;
		cout << "ex2_origin_gray8_addNoise_Impulse_AVGKernelTemplate3_3 PSNR" << ex2_origin_gray8_addNoise_Impulse.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_Equalization_Image = ex2_origin_gray8;
		width = ex2_origin_gray8_Equalization_Image.ReadWidth();
		height = ex2_origin_gray8_Equalization_Image.ReadHeight();
		ex2_origin_gray8_Equalization_Image.Equalization_Image(0, { 0,0 }, { 0,height - 1 }, { width - 1,0 }, { width - 1,height - 1 });
		ex2_origin_gray8_Equalization_Image.SaveBmp("ex2_origin_gray8_Equalization_Image.bmp");

		IPcode1 ex2_origin_gray8_Minus_origin_gray8 = ex2_origin_gray8 - ex2_origin_gray8;
		ex2_origin_gray8_Minus_origin_gray8.SaveBmp("ex2_origin_gray8_Minus_origin_gray8.bmp");

		IPcode1 ex2_origin_gray8_DCT1_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT1_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 1);
		ex2_origin_gray8_DCT1_IDCT.SaveBmp("ex2_origin_gray8_DCT1_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT1_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT1_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT1_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT1_IDCT.bmp");
		cout<<"PSNR_DCT1_IDCT:"<<ex2_origin_gray8_DCT1_IDCT.PSNR(ex2_origin_gray8)<<endl;

		IPcode1 ex2_origin_gray8_DCT2_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT2_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 2);
		ex2_origin_gray8_DCT2_IDCT.SaveBmp("ex2_origin_gray8_DCT2_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT2_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT2_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT2_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT2_IDCT.bmp");
		cout << "PSNR_DCT2_IDCT:" << ex2_origin_gray8_DCT2_IDCT.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_DCT4_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT4_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 4);
		ex2_origin_gray8_DCT4_IDCT.SaveBmp("ex2_origin_gray8_DCT4_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT4_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT4_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT4_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT4_IDCT.bmp");
		cout << "PSNR_DCT4_IDCT:" << ex2_origin_gray8_DCT4_IDCT.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_DCT6_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT6_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 6);
		ex2_origin_gray8_DCT6_IDCT.SaveBmp("ex2_origin_gray8_DCT6_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT6_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT6_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT6_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT6_IDCT.bmp");
		cout << "PSNR_DCT6_IDCT:" << ex2_origin_gray8_DCT6_IDCT.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_DCT8_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT8_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 8);
		ex2_origin_gray8_DCT8_IDCT.SaveBmp("ex2_origin_gray8_DCT8_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT8_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT8_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT8_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT8_IDCT.bmp");
		cout << "PSNR_DCT8_IDCT:" << ex2_origin_gray8_DCT8_IDCT.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_DCT10_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT10_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 10);
		ex2_origin_gray8_DCT10_IDCT.SaveBmp("ex2_origin_gray8_DCT10_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT10_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT10_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT10_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT10_IDCT.bmp");
		cout << "PSNR_DCT10_IDCT:" << ex2_origin_gray8_DCT10_IDCT.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_DCT3_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT3_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 3);
		ex2_origin_gray8_DCT3_IDCT.SaveBmp("ex2_origin_gray8_DCT3_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT3_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT3_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT3_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT3_IDCT.bmp");
		cout << "PSNR_DCT3_IDCT:" << ex2_origin_gray8_DCT3_IDCT.PSNR(ex2_origin_gray8) << endl;

		IPcode1 ex2_origin_gray8_DCT30_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT30_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "", "ampl", 30);
		ex2_origin_gray8_DCT30_IDCT.SaveBmp("ex2_origin_gray8_DCT30_IDCT.bmp");
		IPcode1 ex2_origin_gray8_Minus_origin_gray8_DCT30_IDCT = ex2_origin_gray8 - ex2_origin_gray8_DCT30_IDCT;
		ex2_origin_gray8_Minus_origin_gray8_DCT30_IDCT.SaveBmp("ex2_origin_gray8_Minus_origin_gray8_DCT30_IDCT.bmp");
		cout << "PSNR_DCT30_IDCT:" << ex2_origin_gray8_DCT30_IDCT.PSNR(ex2_origin_gray8) << endl;

		


		IPcode1 ex2_origin_gray8_DCT30 = ex2_origin_gray8;
		ex2_origin_gray8_DCT30.Kernel_image(0, 8, 8, "DCT", "", "ampl", 30);
		ex2_origin_gray8_DCT30.SaveBmp("ex2_origin_gray8_DCT30.bmp");

		

		IPcode1 ex2_origin_gray8_DCT_IDCT = ex2_origin_gray8;
		ex2_origin_gray8_DCT_IDCT.Kernel_image(0, 8, 8, "normalDCT", "", "ampl");
		ex2_origin_gray8_DCT_IDCT.SaveBmp("ex2_origin_gray8_DCT_IDCT.bmp");

		
		IPcode1 ex2_origin_gray8_DCT1 = ex2_origin_gray8;
		ex2_origin_gray8_DCT1.Kernel_image(0, 8, 8, "DCT", "", "ampl", 1);
		IPcode1 ex2_origin_gray8_DCT2 = ex2_origin_gray8;
		ex2_origin_gray8_DCT2.Kernel_image(0, 8, 8, "DCT", "", "ampl", 2);
		IPcode1 ex2_origin_gray8_DCT4 = ex2_origin_gray8;
		ex2_origin_gray8_DCT4.Kernel_image(0, 8, 8, "DCT", "", "ampl", 4);
		IPcode1 ex2_origin_gray8_DCT6 = ex2_origin_gray8;
		ex2_origin_gray8_DCT6.Kernel_image(0, 8, 8, "DCT", "", "ampl", 6);
		IPcode1 ex2_origin_gray8_DCT8 = ex2_origin_gray8;
		ex2_origin_gray8_DCT8.Kernel_image(0, 8, 8, "DCT", "", "ampl", 8);
		IPcode1 ex2_origin_gray8_DCT10 = ex2_origin_gray8;
		ex2_origin_gray8_DCT10.Kernel_image(0, 8, 8, "DCT", "", "ampl", 10);
		IPcode1 ex2_origin_gray8_DCT40 = ex2_origin_gray8;
		ex2_origin_gray8_DCT40.Kernel_image(0, 8, 8, "DCT", "", "ampl", 40);
		IPcode1 ex2_origin_gray8_DCT50 = ex2_origin_gray8;
		ex2_origin_gray8_DCT50.Kernel_image(0, 8, 8, "DCT", "", "ampl", 50);
		

		ex2_origin_gray8_DCT1.SaveBmp("ex2_origin_gray8_DCT1.bmp");
		ex2_origin_gray8_DCT2.SaveBmp("ex2_origin_gray8_DCT2.bmp");
		ex2_origin_gray8_DCT4.SaveBmp("ex2_origin_gray8_DCT4.bmp");
		ex2_origin_gray8_DCT6.SaveBmp("ex2_origin_gray8_DCT6.bmp");
		ex2_origin_gray8_DCT8.SaveBmp("ex2_origin_gray8_DCT8.bmp");
		ex2_origin_gray8_DCT10.SaveBmp("ex2_origin_gray8_DCT10.bmp");
		ex2_origin_gray8_DCT40.SaveBmp("ex2_origin_gray8_DCT40.bmp");
		ex2_origin_gray8_DCT50.SaveBmp("ex2_origin_gray8_DCT50.bmp");
		

		

		

		//IPcode1 ex2_origin_black = ex2_origin;
		//ex2_origin_black.Kernel_image(0, 8, 8, "Black", "whatever", "ampl");
		//ex2_origin_black.SaveBmp("black.bmp");
		//ex2_origin_black.SaveChannel("black_channelY.bmp",0);

		ex2_origin.plot_DCTkernel_Image(8);
		ex2_origin.SaveBmp("����plot_DCTkernel_Image���ͼƬ.bmp");
		
		IPcode1 ex2_origin_gray = ex2_origin;
		ex2_origin_gray.Transfer(IPcode1::RGB2GrayValue);

		IPcode1 ex2_origin_crop = ex2_origin;
		ex2_origin_crop.CropBmp(0, 0, 510, 501);

		IPcode1 ex2_origin_crop_YCrCb = ex2_origin_crop;
		ex2_origin_crop_YCrCb.Transfer(IPcode1::RGB2YCrCb);


		/*����DFT2d IDFT2d�Ƿ���ȷ ���Խ������ȷ��*/
		ex2_origin_crop_YCrCb.SaveChannel("ex2_origin_crop_YCrCb_ChannelY.bmp", 0);
		IPcode1 ex2_origin_crop_YCrCb_normal_ampl = ex2_origin_crop_YCrCb;
		ex2_origin_crop_YCrCb_normal_ampl.Kernel_image(0, 8, 8, "normal", "s", "ampl");
		ex2_origin_crop_YCrCb_normal_ampl.SaveChannel("ex2_origin_crop_YCrCb_normal_ampl_channelY.bmp", 0);

		IPcode1 ex2_origin_crop_YCrCb_normal_phase = ex2_origin_crop_YCrCb;
		ex2_origin_crop_YCrCb_normal_phase.Kernel_image(0, 8, 8, "normal", "s", "phase");
		ex2_origin_crop_YCrCb_normal_phase.SaveChannel("ex2_origin_crop_YCrCb_normal_phase_channelY.bmp", 0);




		IPcode1 ex2_origin_crop_YCrCb_DFT_Y_ampl = ex2_origin_crop_YCrCb;
		ex2_origin_crop_YCrCb_DFT_Y_ampl.Kernel_image(0, 8, 8, "DFT", "whatever", "ampl");

		IPcode1 ex2_origin_crop_YCrCb_DFT_Y_phase = ex2_origin_crop_YCrCb;
		ex2_origin_crop_YCrCb_DFT_Y_phase.Kernel_image(0, 8, 8, "DFT", "whatever", "phase");

		IPcode1 ex2_origin_crop_YCrCb_IDFT_ampl_ampl = ex2_origin_crop_YCrCb_DFT_Y_ampl;
		ex2_origin_crop_YCrCb_IDFT_ampl_ampl.Kernel_image(0, 8, 8, "IDFT", "ampl", "ampl");

		IPcode1 ex2_origin_crop_YCrCb_IDFT_ampl_phase = ex2_origin_crop_YCrCb_DFT_Y_ampl;
		ex2_origin_crop_YCrCb_IDFT_ampl_phase.Kernel_image(0, 8, 8, "IDFT", "ampl", "phase");

		IPcode1 ex2_origin_crop_YCrCb_IDFT_phase_ampl = ex2_origin_crop_YCrCb_DFT_Y_phase;
		ex2_origin_crop_YCrCb_IDFT_phase_ampl.Kernel_image(0, 8, 8, "IDFT", "phase", "ampl");

		IPcode1 ex2_origin_crop_YCrCb_IDFT_phase_phase = ex2_origin_crop_YCrCb_DFT_Y_phase;
		ex2_origin_crop_YCrCb_IDFT_phase_phase.Kernel_image(0, 8, 8, "IDFT", "phase", "phase");

		ex2_origin.SaveBmp("origin.bmp");
		ex2_origin_crop.SaveBmp("ex2_origin_crop.bmp");

		ex2_origin_crop_YCrCb.SaveBmp("ex2_origin_crop_YCrCb.bmp");
		ex2_origin_crop_YCrCb.SaveChannel("ex2_origin_crop_YCrCb_channelY.bmp", 0);

		ex2_origin_crop_YCrCb_DFT_Y_ampl.SaveBmp("ex2_origin_crop_YCrCb_DFT_Y_ampl.bmp");
		ex2_origin_crop_YCrCb_DFT_Y_ampl.SaveChannel("ex2_origin_crop_YCrCb_DFT_Y_ampl_channelY.bmp", 0);

		ex2_origin_crop_YCrCb_DFT_Y_phase.SaveBmp("ex2_origin_crop_YCrCb_DFT_Y_phase.bmp");
		ex2_origin_crop_YCrCb_DFT_Y_phase.SaveChannel("ex2_origin_crop_YCrCb_DFT_Y_phase_channelY.bmp", 0);

		ex2_origin_crop_YCrCb_IDFT_ampl_ampl.SaveBmp("ex2_origin_crop_YCrCb_IDFT_ampl_ampl.bmp");
		ex2_origin_crop_YCrCb_IDFT_ampl_ampl.SaveChannel("ex2_origin_crop_YCrCb_IDFT_ampl_ampl_channelY.bmp", 0);

		ex2_origin_crop_YCrCb_IDFT_ampl_phase.SaveBmp("ex2_origin_crop_YCrCb_IDFT_ampl_phase.bmp");
		ex2_origin_crop_YCrCb_IDFT_ampl_phase.SaveChannel("ex2_origin_crop_YCrCb_IDFT_ampl_phase_channelY.bmp", 0);

		ex2_origin_crop_YCrCb_IDFT_phase_ampl.SaveBmp("ex2_origin_crop_YCrCb_IDFT_phase_ampl.bmp");
		ex2_origin_crop_YCrCb_IDFT_phase_ampl.SaveChannel("ex2_origin_crop_YCrCb_IDFT_phase_ampl_channelY.bmp", 0);

		ex2_origin_crop_YCrCb_IDFT_phase_phase.SaveBmp("ex2_origin_crop_YCrCb_IDFT_phase_phase.bmp");
		ex2_origin_crop_YCrCb_IDFT_phase_phase.SaveChannel("ex2_origin_crop_YCrCb_IDFT_phase_phase_channelY.bmp", 0);

		IPcode1 ex2_0(path);
		IPcode1 ex2_1(path);
		IPcode1 ex2_3(path);
		IPcode1 ex2_4(path);
		
		ex2_4.MakeRGB24_TO_Gray8();
		ex2_4.SaveBmp("Gray8.bmp");

		ex2_3.Transfer(IPcode1::RGB2GrayValue);
		ex2_3.SaveBmp("Gray24.bmp"); //8��24һģһ�������ۿ���������

		ex2_0.CropBmp(0, 0, 510, 501);
		ex2_0.Transfer(IPcode1::RGB2YCrCb);
		ex2_0.SaveBmp("YCrCb.bmp");
		ex2_0.SaveChannel("YCrCb_Y.bmp", 0);
		ex2_0.Kernel_image(0, 8, 8, "DFT","whatever","ampl");
		ex2_0.SaveBmp("DFT_whatever_ampl.bmp");
		ex2_0.SaveChannel("channelY_DFT_whatever_ampl.bmp",0);
		ex2_0.Kernel_image(0,8,8,"IDFT","ampl","ampl");//��0ͨ��IDFT��������phase�ĻҶ�ͼ
		ex2_0.SaveBmp("IDFT_ampl_ampl.bmp");
		ex2_0.SaveChannel("channelY_IDFT_ampl_ampl.bmp", 0);

		ex2_1.Transfer(IPcode1::RGB2YCrCb);
		ex2_1.Kernel_image(0, 8, 8, "DFT", "whatever", "phase");
		IPcode1 ex2_5 = ex2_1;
		ex2_1.SaveChannel("DFT_whatever_phase.bmp",0);
		ex2_1.Kernel_image(0, 8, 8, "IDFT","phase", "phase");
		ex2_1.SaveBmp("IDFT_phase_phase.bmp");
		ex2_1.SaveChannel("channelY_IDFT_phase_phase.bmp", 0);
		
		ex2_5.Kernel_image(0, 8, 8, "IDFT", "phase", "ampl");
		ex2_5.SaveBmp("IDFT_phase_ampl.bmp");
		ex2_5.SaveChannel("channelY_IDFT_phase_ampl.bmp", 0);
		///////////////////////////////////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////////////////////////////////
		//���Դ���
		IPcode1 ipgray("HSI_H.bmp");
		ipgray.SaveChannel("HSI_H0.bmp", 0);
		IPcode1 ipgray2 = ipgray;
		ipgray2.SaveBmp("ipgray2.bmp");

		IPcode1 ipgray1("HSI_Htmp.bmp");
		ipgray1.CropBmp(0, 0, 1500, 250);
		ipgray1.SaveBmp("HSI_Htmp1.bmp");

		

		//readBmp(path);
		//doIt();
		unsigned char * bps1 = pBmpBuf;
		int s;
		s = 1;
		///////////////////////////////////////////////////////////////////////////////////////////////



		
	}
	catch (exception d)
	{
		cout << "throw an exception��" << d.what();
	}
	
	system("pause");
}

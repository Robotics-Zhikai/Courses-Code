#include "main.h"
#include "IPcode1.h"

//---
//以下该模块是完成BMP图像(彩色图像是24bit RGB各8bit)的像素获取，并存在文件名为xiang_su_zhi.txt中
unsigned char *pBmpBuf;//读入图像数据的指针

int bmpWidth;//图像的宽
int bmpHeight;//图像的高
RGBQUAD *pColorTable;//颜色表指针

int biBitCount;//图像类型，每像素位数

			   //-------
			   //读图像的位图数据、宽、高、颜色表及每像素位数等数据进内存，存放在相应的全局变量中
bool readBmp(char *bmpName)
{
	FILE *fp = fopen(bmpName, "rb");//二进制读方式打开指定的图像文件

	if (fp == 0)
		return 0;

	//跳过位图文件头结构BITMAPFILEHEADER
	fseek(fp, sizeof(BITMAPFILEHEADER), 0);

	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER head;

	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp); //获取图像宽、高、每像素所占位数等信息

	bmpWidth = head.biWidth;

	bmpHeight = head.biHeight;

	biBitCount = head.biBitCount;//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）

	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//灰度图像有颜色表，且颜色表表项为256

	if (biBitCount == 8)
	{

		//申请颜色表所需要的空间，读颜色表进内存

		pColorTable = new RGBQUAD[256];

		fread(pColorTable, sizeof(RGBQUAD), 256, fp);

	}

	//申请位图数据所需要的空间，读位图数据进内存
	pBmpBuf = new unsigned char[lineByte * bmpHeight];

	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);

	fclose(fp);//关闭文件

	return 1;//读取文件成功
}

//-----
//给定一个图像位图数据、宽、高、颜色表指针及每像素所占的位数等信息,将其写到指定文件中
bool saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{

	//如果位图数据指针为0，则没有数据传入，函数返回
	if (!imgBuf)
		return 0;

	//颜色表大小，以字节为单位，灰度图像颜色表为1024字节，彩色图像颜色表大小为0
	int colorTablesize = 0;

	if (biBitCount == 8)
		colorTablesize = 1024;

	//待存储图像数据每行字节数为4的倍数
	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	//以二进制写的方式打开文件
	FILE *fp = fopen(bmpName, "wb");

	if (fp == 0)
		return 0;

	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;

	fileHead.bfType = 0x4D42;//bmp类型

							 //bfSize是图像文件4个组成部分之和
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lineByte*height;

	fileHead.bfReserved1 = 0;

	fileHead.bfReserved2 = 0;

	//bfOffBits是图像文件前3个部分所需空间之和
	fileHead.bfOffBits = 54 + colorTablesize;

	//写文件头进文件
	fwrite(&fileHead, sizeof(BITMAPFILEHEADER), 1, fp);

	//申请位图信息头结构变量，填写信息头信息
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

	//写位图信息头进内存
	fwrite(&head, sizeof(BITMAPINFOHEADER), 1, fp);

	//如果灰度图像，有颜色表，写入文件 
	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 256, fp);

	//写位图数据进文件
	fwrite(imgBuf, height*lineByte, 1, fp);

	//关闭文件
	fclose(fp);

	return 1;

}

//----
//以下为像素的读取函数
void doIt()
{

	//读入指定BMP文件进内存
	char readPath[] = "D:\\Study\\硕士\\我的工作\\硕士学位课程\\数字图像处理\\Code\\lena512color.bmp";

	readBmp(readPath);

	//输出图像的信息
	cout << "width=" << bmpWidth << " height=" << bmpHeight << " biBitCount=" << biBitCount << endl;

	//循环变量，图像的坐标
	//每行字节数
	int lineByte = (bmpWidth*biBitCount / 8 + 3) / 4 * 4;

	//循环变量，针对彩色图像，遍历每像素的三个分量
	int m = 0, n = 0, count_xiang_su = 0;

	//将图像左下角1/4部分置成黑色
	ofstream outfile("图像像素.txt", ios::in | ios::trunc);

	if (biBitCount == 8) //对于灰度图像
	{
		//
		//以下完成图像的分割成8*8小单元，并把像素值存储到指定文本中。由于BMP图像的像素数据是从
		//左下角：由左往右，由上往下逐行扫描的
		int L1 = 0;
		int hang = 63;
		int lie = 0;
		//int L2=0;
		//int fen_ge=8;
		for (int fen_ge_hang = 0; fen_ge_hang<8; fen_ge_hang++)//64*64矩阵行循环
		{
			for (int fen_ge_lie = 0; fen_ge_lie<8; fen_ge_lie++)//64*64列矩阵循环
			{
				//--------
				for (L1 = hang; L1>hang - 8; L1--)//8*8矩阵行
				{
					for (int L2 = lie; L2<lie + 8; L2++)//8*8矩阵列
					{
						m = *(pBmpBuf + L1*lineByte + L2);
						outfile << m << " ";
						count_xiang_su++;
						if (count_xiang_su % 8 == 0)//每8*8矩阵读入文本文件
						{
							outfile << endl;
						}
					}
				}
				//---------
				hang = 63 - fen_ge_hang * 8;//64*64矩阵行变换
				lie += 8;//64*64矩阵列变换
						 //该一行（64）由8个8*8矩阵的行组成
			}
			hang -= 8;//64*64矩阵的列变换
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
	{//彩色图像
		for (int i = 0; i<bmpHeight; i++)
		{
			for (int j = 0; j<bmpWidth; j++)
			{
				for (int k = 0; k<3; k++)//每像素RGB三个分量分别置0才变成黑色
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
		cout << "总的像素个素为:" << n << endl;
		cout << "----" << endl;
	}

	//将图像数据存盘
	char writePath[] = "nvcpy.BMP";//图片处理后再存储

	saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

	//清除缓冲区，pBmpBuf和pColorTable是全局变量，在文件读入时申请的空间
	delete[]pBmpBuf;

	if (biBitCount == 8)
		delete[]pColorTable;
}

char path[] = "D:\\Study\\硕士\\我的工作\\硕士学位课程\\数字图像处理\\Code\\lena512color.bmp";

void Experiment1()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//实验一
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
		//实验二
		auto tbegin = clock();
		IPcode1 ex2_origin(path);
		

		IPcode1 ex2_origin_gray8 = ex2_origin;
		ex2_origin_gray8.MakeRGB24_TO_Gray8();
		ex2_origin_gray8.SaveBmp("ex2_origin_gray8.bmp");

		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_phase512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_phase512.Kernel_image(0, 512, 512, "DFT_IDFT", "phase", "phase"); //相角重建
		ex2_origin_gray8_DFT_phase_IDFT_phase512.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_phase512.bmp");

		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_ampl512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_ampl512.Kernel_image(0, 512, 512, "DFT_IDFT", "phase", "ampl"); //相角重建
		ex2_origin_gray8_DFT_phase_IDFT_ampl512.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_ampl512.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_ampl512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_ampl512.Kernel_image(0, 512, 512, "DFT_IDFT", "ampl", "ampl"); //谱重建
		ex2_origin_gray8_DFT_ampl_IDFT_ampl512.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_ampl512.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_phase512 = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_phase512.Kernel_image(0, 512, 512, "DFT_IDFT", "ampl", "phase"); //谱重建
		ex2_origin_gray8_DFT_ampl_IDFT_phase512.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_phase512.bmp");


		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_phase = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_phase.Kernel_image(0, 8, 8, "DFT_IDFT", "phase", "phase"); //相角重建
		ex2_origin_gray8_DFT_phase_IDFT_phase.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_phase.bmp");

		IPcode1 ex2_origin_gray8_DFT_phase_IDFT_ampl = ex2_origin_gray8;
		ex2_origin_gray8_DFT_phase_IDFT_ampl.Kernel_image(0, 8, 8, "DFT_IDFT", "phase", "ampl"); //相角重建
		ex2_origin_gray8_DFT_phase_IDFT_ampl.SaveBmp("ex2_origin_gray8_DFT_phase_IDFT_ampl.bmp");
		//其实相角重建的图phase输出和ampl结合起来才是最终的 相角不是0就是pi -pi ，从phase phase图可以看到，有三个灰度级 分别代表-pi 0 pi

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_ampl = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_ampl.Kernel_image(0, 8, 8, "DFT_IDFT", "ampl", "ampl"); //谱重建
		ex2_origin_gray8_DFT_ampl_IDFT_ampl.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_ampl.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl_IDFT_phase = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl_IDFT_phase.Kernel_image(0, 8, 8, "DFT_IDFT", "ampl", "phase"); //谱重建
		ex2_origin_gray8_DFT_ampl_IDFT_phase.SaveBmp("ex2_origin_gray8_DFT_ampl_IDFT_phase.bmp");

		IPcode1 ex2_origin_gray8_DFT_ampl = ex2_origin_gray8;
		ex2_origin_gray8_DFT_ampl.Kernel_image(0, 8, 8, "DFT", "whatever", "ampl");
		ex2_origin_gray8_DFT_ampl.SaveBmp("ex2_origin_gray8_DFT_ampl.bmp");//这个是经过对数标定的

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
		cout << "花费了" << (double)(tend - tbegin) / CLOCKS_PER_SEC << "秒" << endl;

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
		ex2_origin.SaveBmp("经过plot_DCTkernel_Image后的图片.bmp");
		
		IPcode1 ex2_origin_gray = ex2_origin;
		ex2_origin_gray.Transfer(IPcode1::RGB2GrayValue);

		IPcode1 ex2_origin_crop = ex2_origin;
		ex2_origin_crop.CropBmp(0, 0, 510, 501);

		IPcode1 ex2_origin_crop_YCrCb = ex2_origin_crop;
		ex2_origin_crop_YCrCb.Transfer(IPcode1::RGB2YCrCb);


		/*测试DFT2d IDFT2d是否正确 测试结果是正确的*/
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
		ex2_3.SaveBmp("Gray24.bmp"); //8和24一模一样，肉眼看不出区别

		ex2_0.CropBmp(0, 0, 510, 501);
		ex2_0.Transfer(IPcode1::RGB2YCrCb);
		ex2_0.SaveBmp("YCrCb.bmp");
		ex2_0.SaveChannel("YCrCb_Y.bmp", 0);
		ex2_0.Kernel_image(0, 8, 8, "DFT","whatever","ampl");
		ex2_0.SaveBmp("DFT_whatever_ampl.bmp");
		ex2_0.SaveChannel("channelY_DFT_whatever_ampl.bmp",0);
		ex2_0.Kernel_image(0,8,8,"IDFT","ampl","ampl");//在0通道IDFT，并保留phase的灰度图
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
		//测试代码
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
		cout << "throw an exception：" << d.what();
	}
	
	system("pause");
}

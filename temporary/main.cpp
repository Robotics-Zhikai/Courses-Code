#include "main.h"
#include "IPcode1.h"

//char path[300] = "D:\\Study\\硕士\\我的工作\\硕士学位课程\\数字图像处理\\Code\\lena512color.bmp";
char path[300];

void Debug()
{
	unsigned int testuint = 4;
	cout << (testuint&(unsigned int)8) << endl;

	unsigned char grhd = 0;
	grhd += 1232.1;
	grhd = 255;
	grhd++;
	Operate stast;
	vector<complex<double>> datatest = { 1 ,2,3,4,5,6,7,8 };
	vector<complex<double>> IDFTresult = stast.IDFT(datatest);
	vector<complex<double>> IFFTresult = stast.IFFT(datatest);
	vector<complex<double>> DFTresult = stast.DFT(datatest);
	vector<complex<double>> FFTresult = stast.FFT(datatest);

	vector<double> DCTtest = { 1 ,2,3,4 };
	vector<double> ssdafa = stast.IDCT_2D(DCTtest, 2);
	Operate ttt;
	ttt.zigzagRetainDCT(vector<double>(64), 8, 1);
	cout << sin(5.7) << endl;
	vector<complex<double>> testtp = { complex<double>(3,2),complex<double>(-3,3),complex<double>(-3,-4),complex<double>(3,-5) };

	vector<complex<double>> result = stast.IFFT_2D(testtp, 2, 2);



	///////////////////////////////////////////////////////////////////////////////////////////////
	IPcode1 robot("D:\\Study\\硕士\\我的工作\\硕士学位课程\\数字图像处理\\Code\\模糊\\随便拍的.bmp");

	//实验二
	auto tbegin = clock();
	IPcode1 ex2_origin(path);


	IPcode1 ex2_origin_gray8 = ex2_origin;
	ex2_origin_gray8.MakeRGB24_TO_Gray8();
	ex2_origin_gray8.SaveBmp("ex2_origin_gray8.bmp");

	IPcode1 origin_gray8_LinearBlur = ex2_origin_gray8;
	origin_gray8_LinearBlur.MotionLinearBlur(0, 0.03, 0, 1);
	origin_gray8_LinearBlur.SaveBmp("origin_gray8_LinearBlurxa0.03.bmp");

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
	ex2_origin_gray8_addNoise_Impulse.AddNoise(0, { 0,0 }, { 0,height - 1 }, { width - 1,0 }, { width - 1,height - 1 }, "Impulse", 1000, -1000, 0.01, 0.01);
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
	cout << "PSNR_DCT1_IDCT:" << ex2_origin_gray8_DCT1_IDCT.PSNR(ex2_origin_gray8) << endl;

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
	ex2_0.Kernel_image(0, 8, 8, "DFT", "whatever", "ampl");
	ex2_0.SaveBmp("DFT_whatever_ampl.bmp");
	ex2_0.SaveChannel("channelY_DFT_whatever_ampl.bmp", 0);
	ex2_0.Kernel_image(0, 8, 8, "IDFT", "ampl", "ampl");//在0通道IDFT，并保留phase的灰度图
	ex2_0.SaveBmp("IDFT_ampl_ampl.bmp");
	ex2_0.SaveChannel("channelY_IDFT_ampl_ampl.bmp", 0);

	ex2_1.Transfer(IPcode1::RGB2YCrCb);
	ex2_1.Kernel_image(0, 8, 8, "DFT", "whatever", "phase");
	IPcode1 ex2_5 = ex2_1;
	ex2_1.SaveChannel("DFT_whatever_phase.bmp", 0);
	ex2_1.Kernel_image(0, 8, 8, "IDFT", "phase", "phase");
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




	///////////////////////////////////////////////////////////////////////////////////////////////

}

void Experiment1()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	//实验一
	cout << "实验一开始" << endl;
	IPcode1 lunaorigin(path);
	lunaorigin.SaveBmp("lunaorigin.bmp");//保存初始图像
	IPcode1 lunaorigin_crop300_460 = lunaorigin;
	lunaorigin_crop300_460.CropBmp(0, 0, 300, 460); //以0,0作为起点，300的宽度，460的高度进行Crop
	lunaorigin_crop300_460.SaveBmp("lunaorigin_crop300_460.bmp"); //保存剪切后的图像

	vector<LONG> ld = { 0,0 };
	vector<LONG> lu = { 0,lunaorigin_crop300_460.ReadHeight()-1 };
	vector<LONG> rd = { lunaorigin_crop300_460.ReadWidth() - 1,0 };
	vector<LONG> ru = { lunaorigin_crop300_460.ReadWidth() - 1,lunaorigin_crop300_460.ReadHeight() - 1 };
	vector<double> Hist = lunaorigin_crop300_460.Histogram(0, ld, lu, rd, ru);

	cout << "直方图如下，纵轴为频率：" << endl;
	for (int i = 0; i < Hist.size(); i++)
		cout << Hist[i] << " ";
	cout << endl;

	int bitloc;
	vector<unsigned char> vecpixel = lunaorigin.ReadPixel(231, 231, bitloc);
	cout << "231,231处的像素值如下：" << endl;
	for (int i = 0; i < 3; i++)
		cout << int(vecpixel[i]) << " ";
	cout << endl;

	lunaorigin.SaveChannel("lunaorigin_R.bmp", 0);
	lunaorigin.SaveChannel("lunaorigin_G.bmp", 1);
	lunaorigin.SaveChannel("lunaorigin_B.bmp", 2);

	IPcode1 lunaorigin_RGB2YIQ = lunaorigin;
	lunaorigin_RGB2YIQ.Transfer(IPcode1::RGB2YIQ);
	IPcode1 lunaorigin_RGB2HSI = lunaorigin;
	lunaorigin_RGB2HSI.Transfer(IPcode1::RGB2HSI);
	IPcode1 lunaorigin_RGB2YCrCb = lunaorigin;
	lunaorigin_RGB2YCrCb.Transfer(IPcode1::RGB2YCrCb);
	IPcode1 lunaorigin_RGB2XYZ = lunaorigin;
	lunaorigin_RGB2XYZ.Transfer(IPcode1::RGB2XYZ);

	lunaorigin_RGB2YIQ.SaveBmp("lunaorigin_RGB2YIQ.bmp");
	lunaorigin_RGB2YIQ.SaveChannel("lunaorigin_RGB2YIQ_Y.bmp", 0);
	lunaorigin_RGB2YIQ.SaveChannel("lunaorigin_RGB2YIQ_I.bmp", 1);
	lunaorigin_RGB2YIQ.SaveChannel("lunaorigin_RGB2YIQ_Q.bmp", 2);

	lunaorigin_RGB2HSI.SaveBmp("lunaorigin_RGB2HSI.bmp");
	lunaorigin_RGB2HSI.SaveChannel("lunaorigin_RGB2HSI_H.bmp", 0);
	lunaorigin_RGB2HSI.SaveChannel("lunaorigin_RGB2HSI_S.bmp", 1);
	lunaorigin_RGB2HSI.SaveChannel("lunaorigin_RGB2HSI_I.bmp", 2);

	lunaorigin_RGB2YCrCb.SaveBmp("lunaorigin_RGB2YCrCb.bmp");
	lunaorigin_RGB2YCrCb.SaveChannel("lunaorigin_RGB2YCrCb_Y.bmp", 0);
	lunaorigin_RGB2YCrCb.SaveChannel("lunaorigin_RGB2YCrCb_Cr.bmp", 1);
	lunaorigin_RGB2YCrCb.SaveChannel("lunaorigin_RGB2YCrCb_Cb.bmp", 2);

	lunaorigin_RGB2XYZ.SaveBmp("lunaorigin_RGB2XYZ.bmp");
	lunaorigin_RGB2XYZ.SaveChannel("lunaorigin_RGB2XYZ_X.bmp", 0);
	lunaorigin_RGB2XYZ.SaveChannel("lunaorigin_RGB2XYZ_Y.bmp", 1);
	lunaorigin_RGB2XYZ.SaveChannel("lunaorigin_RGB2XYZ_Z.bmp", 2);

	cout << "实验一结束" << endl;
	///////////////////////////////////////////////////////////////////////////////////////////////
}

void Experiment2()
{
	cout << "实验二开始" << endl;

	IPcode1 lunaorigin(path);

	lunaorigin.plot_DCTkernel_Image(8);

	IPcode1 lunaorigin_gray8 = lunaorigin;
	lunaorigin_gray8.MakeRGB24_TO_Gray8();
	lunaorigin_gray8.SaveBmp("lunaorigin_gray8.bmp");

	IPcode1 lunaorigin_gray8_DCT_1_IDCT = lunaorigin_gray8;
	lunaorigin_gray8_DCT_1_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "whatever", "ampl", 1);
	cout << "lunaorigin_gray8_DCT_1_IDCT PSNR:" << lunaorigin_gray8_DCT_1_IDCT.PSNR(lunaorigin_gray8) << endl;
	lunaorigin_gray8_DCT_1_IDCT.SaveBmp("lunaorigin_gray8_DCT_1_IDCT.bmp");

	IPcode1 lunaorigin_gray8_DCT_2_IDCT = lunaorigin_gray8;
	lunaorigin_gray8_DCT_2_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "whatever", "ampl", 2);
	cout << "lunaorigin_gray8_DCT_2_IDCT PSNR:" << lunaorigin_gray8_DCT_2_IDCT.PSNR(lunaorigin_gray8) << endl;
	lunaorigin_gray8_DCT_2_IDCT.SaveBmp("lunaorigin_gray8_DCT_2_IDCT.bmp");

	IPcode1 lunaorigin_gray8_DCT_4_IDCT = lunaorigin_gray8;
	lunaorigin_gray8_DCT_4_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "whatever", "ampl", 4);
	cout << "lunaorigin_gray8_DCT_4_IDCT PSNR:" << lunaorigin_gray8_DCT_4_IDCT.PSNR(lunaorigin_gray8) << endl;
	lunaorigin_gray8_DCT_4_IDCT.SaveBmp("lunaorigin_gray8_DCT_4_IDCT.bmp");

	IPcode1 lunaorigin_gray8_DCT_6_IDCT = lunaorigin_gray8;
	lunaorigin_gray8_DCT_6_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "whatever", "ampl", 6);
	cout << "lunaorigin_gray8_DCT_6_IDCT PSNR:" << lunaorigin_gray8_DCT_6_IDCT.PSNR(lunaorigin_gray8) << endl;
	lunaorigin_gray8_DCT_6_IDCT.SaveBmp("lunaorigin_gray8_DCT_6_IDCT.bmp");

	IPcode1 lunaorigin_gray8_DCT_8_IDCT = lunaorigin_gray8;
	lunaorigin_gray8_DCT_8_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "whatever", "ampl", 8);
	cout << "lunaorigin_gray8_DCT_8_IDCT PSNR:" << lunaorigin_gray8_DCT_8_IDCT.PSNR(lunaorigin_gray8) << endl;
	lunaorigin_gray8_DCT_8_IDCT.SaveBmp("lunaorigin_gray8_DCT_8_IDCT.bmp");

	IPcode1 lunaorigin_gray8_DCT_10_IDCT = lunaorigin_gray8;
	lunaorigin_gray8_DCT_10_IDCT.Kernel_image(0, 8, 8, "DCT_Numin_IDCT", "whatever", "ampl", 10);
	cout << "lunaorigin_gray8_DCT_10_IDCT PSNR:" << lunaorigin_gray8_DCT_10_IDCT.PSNR(lunaorigin_gray8) << endl;
	lunaorigin_gray8_DCT_10_IDCT.SaveBmp("lunaorigin_gray8_DCT_10_IDCT.bmp");
		


	IPcode1 lunaorigin_RGB2YCrCb = lunaorigin;
	lunaorigin_RGB2YCrCb.Transfer(IPcode1::RGB2YCrCb);

	IPcode1 lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl_512 = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl_512.Kernel_image(0, 512, 512, "DFT_IDFT", "ampl", "ampl");
	lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl_512.SaveChannel("lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl_512.bmp", 0);

	IPcode1 lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl_512 = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl_512.Kernel_image(0, 512, 512, "DFT_IDFT", "phase", "ampl");
	lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl_512.SaveChannel("lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl_512.bmp", 0);


	IPcode1 lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl.Kernel_image(0, 8, 8, "DFT_IDFT", "ampl", "ampl");
	lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl.SaveChannel("lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_ampl.bmp", 0);
	IPcode1 lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_phase = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_phase.Kernel_image(0, 8, 8, "DFT_IDFT", "ampl", "phase");
	lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_phase.SaveChannel("lunaorigin_RGB2YCrCb_DFT_ampl_IDFT_phase.bmp", 0);

	IPcode1 lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl.Kernel_image(0, 8, 8, "DFT_IDFT", "phase", "ampl");
	lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl.SaveChannel("lunaorigin_RGB2YCrCb_DFT_phase_IDFT_ampl.bmp", 0);
	IPcode1 lunaorigin_RGB2YCrCb_DFT_phase_IDFT_phase = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_DFT_phase_IDFT_phase.Kernel_image(0, 8, 8, "DFT_IDFT", "phase", "phase");
	lunaorigin_RGB2YCrCb_DFT_phase_IDFT_phase.SaveChannel("lunaorigin_RGB2YCrCb_DFT_phase_IDFT_phase.bmp", 0);


	auto tbegin = clock();
	IPcode1 lunaorigin_RGB2YCrCb_YDFT_ampl = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_YDFT_ampl.Kernel_image(0, 8, 8, "DFT", "whatever", "ampl");
	lunaorigin_RGB2YCrCb_YDFT_ampl.SaveChannel("lunaorigin_RGB2YCrCb_YDFT_ampl.bmp", 0);
	clock_t tend = clock();
	cout << "lunaorigin_RGB2YCrCb_YDFT_ampl花费了" << (double)(tend - tbegin) / CLOCKS_PER_SEC << "秒" << endl;

	tbegin = clock();
	IPcode1 lunaorigin_RGB2YCrCb_YDFT_phase = lunaorigin_RGB2YCrCb;
	lunaorigin_RGB2YCrCb_YDFT_phase.Kernel_image(0, 8, 8, "DFT", "whatever", "phase");
	lunaorigin_RGB2YCrCb_YDFT_phase.SaveChannel("lunaorigin_RGB2YCrCb_YDFT_phase.bmp", 0);
	tend = clock();
	cout << "lunaorigin_RGB2YCrCb_YDFT_phase花费了" << (double)(tend - tbegin) / CLOCKS_PER_SEC << "秒" << endl;

	cout << "实验二结束" << endl;
}

void Experiment3()
{
	cout << "实验三开始" << endl;

	IPcode1 lunaorigin(path);

	IPcode1 lunaorigin_gray8 = lunaorigin;
	lunaorigin_gray8.MakeRGB24_TO_Gray8();
	lunaorigin_gray8.SaveBmp("lunaorigin_gray8.bmp");
	vector<LONG> ld = { 0,0 };
	vector<LONG> lu = { 0,lunaorigin.ReadHeight() - 1 };
	vector<LONG> rd = { lunaorigin.ReadWidth() - 1,0 };
	vector<LONG> ru = { lunaorigin.ReadWidth() - 1,lunaorigin.ReadHeight() - 1 };

	IPcode1 lunaorigin_gray8_Sobel = lunaorigin_gray8;
	lunaorigin_gray8_Sobel.Kernel_image(0, 3, 3, 1, "sobel", 1, 1);
	lunaorigin_gray8_Sobel.SaveBmp("lunaorigin_gray8_Sobel.bmp");

	IPcode1 lunaorigin_gray8_Gaussian_5_5 = lunaorigin_gray8;
	lunaorigin_gray8_Gaussian_5_5.AddNoise(0, ld, lu, rd, ru, "Gaussian", 5, 5);
	lunaorigin_gray8_Gaussian_5_5.SaveBmp("lunaorigin_gray8_Gaussian_5_5.bmp");
	IPcode1 lunaorigin_gray8_Gaussian_5_5_medianFilter = lunaorigin_gray8_Gaussian_5_5;
	lunaorigin_gray8_Gaussian_5_5_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
	lunaorigin_gray8_Gaussian_5_5_medianFilter.SaveBmp("lunaorigin_gray8_Gaussian_5_5_medianFilter.bmp");
	IPcode1 lunaorigin_gray8_Gaussian_5_5_avgFilter = lunaorigin_gray8_Gaussian_5_5;
	lunaorigin_gray8_Gaussian_5_5_avgFilter.Kernel_image(0, 3, 3, 0, IPcode1::AVGKernelTemplate(3, 3), 1, 1);
	lunaorigin_gray8_Gaussian_5_5_avgFilter.SaveBmp("lunaorigin_gray8_Gaussian_5_5_avgFilter.bmp");
	cout << "lunaorigin_gray8_Gaussian_5_5_medianFilter SSIM" << lunaorigin_gray8_Gaussian_5_5_medianFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Gaussian_5_5_medianFilter PSNR" << lunaorigin_gray8_Gaussian_5_5_medianFilter.PSNR(lunaorigin_gray8) << endl;
	cout << "lunaorigin_gray8_Gaussian_5_5_avgFilter SSIM" << lunaorigin_gray8_Gaussian_5_5_avgFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Gaussian_5_5_avgFilter PSNR" << lunaorigin_gray8_Gaussian_5_5_avgFilter.PSNR(lunaorigin_gray8) << endl;

	IPcode1 lunaorigin_gray8_Gaussian_10_5 = lunaorigin_gray8;
	lunaorigin_gray8_Gaussian_10_5.AddNoise(0, ld, lu, rd, ru, "Gaussian", 10, 5);
	lunaorigin_gray8_Gaussian_10_5.SaveBmp("lunaorigin_gray8_Gaussian_10_5.bmp");
	IPcode1 lunaorigin_gray8_Gaussian_10_5_medianFilter = lunaorigin_gray8_Gaussian_10_5;
	lunaorigin_gray8_Gaussian_10_5_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
	lunaorigin_gray8_Gaussian_10_5_medianFilter.SaveBmp("lunaorigin_gray8_Gaussian_10_5_medianFilter.bmp");
	IPcode1 lunaorigin_gray8_Gaussian_10_5_avgFilter = lunaorigin_gray8_Gaussian_10_5;
	lunaorigin_gray8_Gaussian_10_5_avgFilter.Kernel_image(0, 3, 3, 0, IPcode1::AVGKernelTemplate(3, 3), 1, 1);
	lunaorigin_gray8_Gaussian_10_5_avgFilter.SaveBmp("lunaorigin_gray8_Gaussian_10_5_avgFilter.bmp");
	cout << "lunaorigin_gray8_Gaussian_10_5_medianFilter SSIM" << lunaorigin_gray8_Gaussian_10_5_medianFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Gaussian_10_5_medianFilter PSNR" << lunaorigin_gray8_Gaussian_10_5_medianFilter.PSNR(lunaorigin_gray8) << endl;
	cout << "lunaorigin_gray8_Gaussian_10_5_avgFilter SSIM" << lunaorigin_gray8_Gaussian_10_5_avgFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Gaussian_10_5_avgFilter PSNR" << lunaorigin_gray8_Gaussian_10_5_avgFilter.PSNR(lunaorigin_gray8) << endl;

	IPcode1 lunaorigin_gray8_Gaussian_15_5 = lunaorigin_gray8;
	lunaorigin_gray8_Gaussian_15_5.AddNoise(0, ld, lu, rd, ru, "Gaussian", 15, 5);
	lunaorigin_gray8_Gaussian_15_5.SaveBmp("lunaorigin_gray8_Gaussian_15_5.bmp");
	IPcode1 lunaorigin_gray8_Gaussian_15_5_medianFilter = lunaorigin_gray8_Gaussian_15_5;
	lunaorigin_gray8_Gaussian_15_5_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
	lunaorigin_gray8_Gaussian_15_5_medianFilter.SaveBmp("lunaorigin_gray8_Gaussian_15_5_medianFilter.bmp");
	IPcode1 lunaorigin_gray8_Gaussian_15_5_avgFilter = lunaorigin_gray8_Gaussian_15_5;
	lunaorigin_gray8_Gaussian_15_5_avgFilter.Kernel_image(0, 3, 3, 0, IPcode1::AVGKernelTemplate(3, 3), 1, 1);
	lunaorigin_gray8_Gaussian_15_5_avgFilter.SaveBmp("lunaorigin_gray8_Gaussian_15_5_avgFilter.bmp");
	cout << "lunaorigin_gray8_Gaussian_15_5_medianFilter SSIM" << lunaorigin_gray8_Gaussian_15_5_medianFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Gaussian_15_5_medianFilter PSNR" << lunaorigin_gray8_Gaussian_15_5_medianFilter.PSNR(lunaorigin_gray8) << endl;
	cout << "lunaorigin_gray8_Gaussian_15_5_avgFilter SSIM" << lunaorigin_gray8_Gaussian_15_5_avgFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Gaussian_15_5_avgFilter PSNR" << lunaorigin_gray8_Gaussian_15_5_avgFilter.PSNR(lunaorigin_gray8) << endl;



	IPcode1 lunaorigin_gray8_Impulse_1_1 = lunaorigin_gray8;
	lunaorigin_gray8_Impulse_1_1.AddNoise(0, ld, lu, rd, ru, "Impulse", 1000, -1000, 0.01, 0.01);
	lunaorigin_gray8_Impulse_1_1.SaveBmp("lunaorigin_gray8_Impulse_1_1.bmp");
	IPcode1 lunaorigin_gray8_Impulse_1_1_medianFilter = lunaorigin_gray8_Impulse_1_1;
	lunaorigin_gray8_Impulse_1_1_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
	lunaorigin_gray8_Impulse_1_1_medianFilter.SaveBmp("lunaorigin_gray8_Impulse_1_1_medianFilter.bmp");
	IPcode1 lunaorigin_gray8_Impulse_1_1_avgFilter = lunaorigin_gray8_Impulse_1_1;
	lunaorigin_gray8_Impulse_1_1_avgFilter.Kernel_image(0, 3, 3, 0, IPcode1::AVGKernelTemplate(3, 3), 1, 1);
	lunaorigin_gray8_Impulse_1_1_avgFilter.SaveBmp("lunaorigin_gray8_Impulse_1_1_avgFilter.bmp");
	cout << "lunaorigin_gray8_Impulse_1_1_medianFilter SSIM" << lunaorigin_gray8_Impulse_1_1_medianFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Impulse_1_1_medianFilter PSNR" << lunaorigin_gray8_Impulse_1_1_medianFilter.PSNR(lunaorigin_gray8) << endl;
	cout << "lunaorigin_gray8_Impulse_1_1_avgFilter SSIM" << lunaorigin_gray8_Impulse_1_1_avgFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Impulse_1_1_avgFilter PSNR" << lunaorigin_gray8_Impulse_1_1_avgFilter.PSNR(lunaorigin_gray8) << endl;

	IPcode1 lunaorigin_gray8_Impulse_5_5 = lunaorigin_gray8;
	lunaorigin_gray8_Impulse_5_5.AddNoise(0, ld, lu, rd, ru, "Impulse", 1000, -1000, 0.05, 0.05);
	lunaorigin_gray8_Impulse_5_5.SaveBmp("lunaorigin_gray8_Impulse_5_5.bmp");
	IPcode1 lunaorigin_gray8_Impulse_5_5_medianFilter = lunaorigin_gray8_Impulse_5_5;
	lunaorigin_gray8_Impulse_5_5_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
	lunaorigin_gray8_Impulse_5_5_medianFilter.SaveBmp("lunaorigin_gray8_Impulse_5_5_medianFilter.bmp");
	IPcode1 lunaorigin_gray8_Impulse_5_5_avgFilter = lunaorigin_gray8_Impulse_5_5;
	lunaorigin_gray8_Impulse_5_5_avgFilter.Kernel_image(0, 3, 3, 0, IPcode1::AVGKernelTemplate(3, 3), 1, 1);
	lunaorigin_gray8_Impulse_5_5_avgFilter.SaveBmp("lunaorigin_gray8_Impulse_5_5_avgFilter.bmp");
	cout << "lunaorigin_gray8_Impulse_5_5_medianFilter SSIM" << lunaorigin_gray8_Impulse_5_5_medianFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Impulse_5_5_medianFilter PSNR" << lunaorigin_gray8_Impulse_5_5_medianFilter.PSNR(lunaorigin_gray8) << endl;
	cout << "lunaorigin_gray8_Impulse_5_5_avgFilter SSIM" << lunaorigin_gray8_Impulse_5_5_avgFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Impulse_5_5_avgFilter PSNR" << lunaorigin_gray8_Impulse_5_5_avgFilter.PSNR(lunaorigin_gray8) << endl;

	IPcode1 lunaorigin_gray8_Impulse_10_10 = lunaorigin_gray8;
	lunaorigin_gray8_Impulse_10_10.AddNoise(0, ld, lu, rd, ru, "Impulse", 1000, -1000, 0.10, 0.10);
	lunaorigin_gray8_Impulse_10_10.SaveBmp("lunaorigin_gray8_Impulse_10_10.bmp");
	IPcode1 lunaorigin_gray8_Impulse_10_10_medianFilter = lunaorigin_gray8_Impulse_10_10;
	lunaorigin_gray8_Impulse_10_10_medianFilter.Kernel_image(0, 3, 3, 0, "median", 1, 1);
	lunaorigin_gray8_Impulse_10_10_medianFilter.SaveBmp("lunaorigin_gray8_Impulse_10_10_medianFilter.bmp");
	IPcode1 lunaorigin_gray8_Impulse_10_10_avgFilter = lunaorigin_gray8_Impulse_10_10;
	lunaorigin_gray8_Impulse_10_10_avgFilter.Kernel_image(0, 3, 3, 0, IPcode1::AVGKernelTemplate(3, 3), 1, 1);
	lunaorigin_gray8_Impulse_10_10_avgFilter.SaveBmp("lunaorigin_gray8_Impulse_10_10_avgFilter.bmp");
	cout << "lunaorigin_gray8_Impulse_10_10_medianFilter SSIM" << lunaorigin_gray8_Impulse_10_10_medianFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Impulse_10_10_medianFilter PSNR" << lunaorigin_gray8_Impulse_10_10_medianFilter.PSNR(lunaorigin_gray8) << endl;
	cout << "lunaorigin_gray8_Impulse_10_10_avgFilter SSIM" << lunaorigin_gray8_Impulse_10_10_avgFilter.SSIM(lunaorigin_gray8, 0) << endl;
	cout << "lunaorigin_gray8_Impulse_10_10_avgFilter PSNR" << lunaorigin_gray8_Impulse_10_10_avgFilter.PSNR(lunaorigin_gray8) << endl;

	
	
	IPcode1 lunaorigin_gray8_Normalization = lunaorigin_gray8;
	lunaorigin_gray8_Normalization.Normalization_Image(0, ld, lu, rd, ru);;
	lunaorigin_gray8_Normalization.SaveBmp("lunaorigin_gray8_Normalization.bmp");

	IPcode1 lunaorigin_gray8_Equalization = lunaorigin_gray8;
	lunaorigin_gray8_Equalization.Equalization_Image(0, ld, lu, rd, ru);
	lunaorigin_gray8_Equalization.SaveBmp("lunaorigin_gray8_Equalization.bmp");

	


	IPcode1 lunaorigin_gray8_HalfTone_binary = lunaorigin_gray8;
	lunaorigin_gray8_HalfTone_binary.MakeGray8_TO_BinaryImage_HalfTone();
	lunaorigin_gray8_HalfTone_binary.SaveBmp("lunaorigin_gray8_HalfTone_binary.bmp");

	IPcode1 lunaorigin_gray8_DirectThreshold_binary = lunaorigin_gray8;
	lunaorigin_gray8_DirectThreshold_binary.MakeGray8_TO_BinaryImage_DirectThreshold();
	lunaorigin_gray8_DirectThreshold_binary.SaveBmp("lunaorigin_gray8_DirectThreshold_binary.bmp");

	cout << "实验三结束" << endl;
}

void Experiment4()
{
	cout << "实验四开始" << endl;

	IPcode1 lunaorigin(path);

	IPcode1 lunaorigin_gray8 = lunaorigin;
	lunaorigin_gray8.MakeRGB24_TO_Gray8();
	lunaorigin_gray8.SaveBmp("lunaorigin_gray8.bmp");

	lunaorigin_gray8.MotionLinearBlur(0, 0.01, 0.01, 1);
	lunaorigin_gray8.SaveBmp("lunaorigin_gray8_MotionLinearBlur.bmp");

	cout << "实验四结束" << endl;
}

void main()
{
	try
	{
		cout << "输入待处理的bmp图像路径（绝对路径或相对路径）,如果路径不对会报错读取失败，输出的所有文件在当前可执行文件所在文件夹下" << endl;
		cin >> path;
		Experiment1();
		Experiment2();
		Experiment3();
		Experiment4();
	}
	catch (exception d)
	{
		cout << "throw an exception：" << d.what();
	}
	
	system("pause");
}

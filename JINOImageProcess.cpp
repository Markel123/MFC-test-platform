#include"stdafx.h"
#include"JINOImageProcess.h"

#pragma region 如果你需要全局变量的话

struJINOFeature JINOFeature;

#pragma endregion

CJINOItem1* CJINOItem1::GetInstance() {
	return (CJINOItem1*)CJINOItem1::GetSingleton<CJINOItem1>();
}

bool CJINOItem1::JINOMainProcess(Mat &src, Mat &outImg) {
	return true;
}

/*
@目前只针对灰度图处理
*/
bool CJINOItem1::JINOImgProcess1(Mat &src, Mat &outImg) {

#pragma region 预处理 安全判断
	//src为resizedImg,out为尺寸相同的resultImg;
	if (src.data == NULL) {
		return false;
	}
	if (src.channels() > 1) {
		extractChannel(src, b, 0);
		extractChannel(src, g, 1);
		extractChannel(src, r, 2);
		cvtColor(src, outImg, CV_BGR2GRAY);
	}
	Rect ROI1 = Rect(min(JINOFeature.startX, JINOFeature.endX), min(JINOFeature.startY, JINOFeature.endY), abs(JINOFeature.endX - JINOFeature.startX), abs(JINOFeature.endY - JINOFeature.startY));
	if (ROI1.empty()) {
		AfxMessageBox(_T("ROI Not Set!"));
		JINOFeature.roiSetSuccess = false;
		JINOFeature.analyzing = false;
		return false;
	}
	if (!JINOSafeRect(outImg, ROI1)) {
		return false;
	}

#pragma endregion

#pragma region 这里放图像处理逻辑 针对的是图像roiImg 
	Mat roiImg = outImg(ROI1);

	vector<int>mMaxIndVec; vector<int>mMinIndVec;
	vector<double>mMaxValueVec; vector<double>mMinValueVec;
	GetMulGradient(roiImg, 8, mMaxIndVec, mMinIndVec, mMinValueVec, mMaxValueVec, 20, 0, 0, 20);
	auto LineIndex = 1;
	if (LineIndex > mMaxIndVec.size() || LineIndex < 1)
	{
		AfxMessageBox(_T("Grab Line Index exceedes the true range!!!"));
		return false;
	}
	auto LineX1 = mMaxIndVec[LineIndex - 1] + ROI1.x;
	line(src, Point(LineX1, ROI1.y), Point(LineX1, ROI1.y + ROI1.height), Scalar(0, 255, 0), 3);
	/*for (int i = 0; i < mMaxIndVec.size(); i++){
		line(imgout, Point(mMaxIndVec[i] + CamRect.x, CamParaVal().nStartY2), Point(mMaxIndVec[i] + CamRect.x, CamParaVal().nStartY2 + CamParaVal().nYrange2), Scalar(0, 255, 0), 3);
	}*/

#pragma endregion

#pragma region 后处理 等
	outImg = src;
	JINOFeature.analyzing = true;

#pragma endregion
	return true;
}

bool CJINOItem1::JINOImgProcess2(Mat &src, Mat &outImg) {
	return true;
}

bool CJINOItem1::JINOImgProcess3(Mat &src, Mat &outImg) {
	return true;
}

bool CJINOItem1::JINOSafeRect(Mat &src, Rect &rect) {
	if (src.data == NULL) {
		return false;
	}
	Mat srcX = src.clone();
	if (src.channels() > 1) {
		cvtColor(src, srcX, CV_BGR2GRAY);
	}
	if (rect.x<0 || (rect.x + rect.width)>srcX.cols||rect.y<0||(rect.y+rect.height)>srcX.rows) {
		AfxMessageBox(_T("Please Check if ROI Is Set Properly!"));
		return false;
	}
	return true;
}

bool CJINOItem1::JINOScaleRect(Mat &src, Rect &rect, int size) {
	if (src.data == NULL) {
		return false;
	}
	Mat srcX = src.clone();
	if (src.channels() > 1) {
		cvtColor(src, srcX, CV_BGR2GRAY);
	}
	rect.x = rect.x < 0 ? 0 : rect.x;
	rect.y = rect.y < 0 ? 0 : rect.y;
	if (rect.x+rect.width > srcX.rows) {
		auto offset = rect.x + rect.width - srcX.rows;
		rect.x = rect.x - offset-4;
	}
	if (rect.y + rect.height > srcX.cols) {
		auto offset = rect.y + rect.height - srcX.cols;
		rect.y = rect.y - offset - 4;
	}
	return true;
}

bool CJINOItem1::JINOWindowAttach(int littleWindowSize, int ID, const char* pWindowName, const char* cWindowName) {
	cv::namedWindow(cWindowName);
	cv::resizeWindow(cWindowName, cv::Size(littleWindowSize,littleWindowSize));
	HWND hcWnd = (HWND)cvGetWindowHandle(cWindowName);
	HWND hpWnd = (HWND)cvGetWindowHandle(pWindowName);
	HWND hParent = ::GetParent(hcWnd);
	::SetParent(hcWnd, hpWnd);
	::ShowWindow(hParent, SW_HIDE);
	return true;
}

#pragma region 手动阈值
bool CJINOItem1::GetMulGradient(Mat& src,
	int n,					/*左个各采集n个*/
	vector<int> &MaxOrder,			/*最大梯度的竖线*/
	vector<int> &MinOrder,			/*最小梯度的竖线*/
	vector<double> &MinGradient,	/*最小平均梯度*/
	vector<double> &MaxGradient,    /*最大平均梯度*/
	int Threshold,          /*灰度阈值*/
	int direction,          /*检测方向：1找水平线,0找竖直线*/
	int direction1,         /*检测方向：0正向,1反向*/
	int intervalNum             /*相邻边之间的像素间隔*/
)
{
	if (!src.data)
		return FALSE;
	vector<int>	subgray;//一列的灰度值之和
	vector<double> gradient;//梯度
	double dGradient = 0;
	bool startJump = false;
	int  jumpI = 0;

	if (direction == 0) //
	{
		for (int col = 0; col < src.cols; ++col)
		{
			int tmp = 0;
			for (int row = 0; row < src.rows; ++row)
			{
				tmp += src.at<uchar>(row, col);
			}
			subgray.push_back(tmp);
		}
		if (subgray.size() > 10)
		{
			for (int i = 0; i < subgray.size(); i++)
			{
				dGradient = 0;
				if (i - n >= 0 && i + n - 1 < subgray.size())
				{
					double rSubGradient = 0;
					double lSubGradient = 0;
					for (int j = 0; j < n; j++)
					{
						rSubGradient += subgray[i + j];
						lSubGradient += subgray[i - j - 1];
					}
					dGradient = rSubGradient - lSubGradient;
				}
				gradient.push_back(dGradient / (double)n / (double)src.rows);
			}
			if (direction1 == 0)
			{
				for (int i = 0; i < gradient.size(); i++)
				{
					if (gradient[i] > Threshold&&startJump == false)
					{
						MaxOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
				for (int i = 0; i < gradient.size(); i++)
				{
					if (gradient[i] < -Threshold && startJump == false)
					{
						MinOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
			}
			if (direction1 == 1)
			{
				for (int i = gradient.size() - 1; i > 0; i--)   //找第一个最大梯度
				{
					if (gradient[i] > Threshold&&startJump == false)
					{
						MaxOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
				for (int i = gradient.size() - 1; i > 0; i--)
				{
					if (gradient[i] < -Threshold && startJump == false)
					{
						MinOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
			}

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		for (int row = 0; row < src.rows; ++row)
		{
			int tmp = 0;
			for (int col = 0; col < src.cols; ++col)
			{
				tmp += src.at<uchar>(row, col);
			}
			subgray.push_back(tmp);
		}
		if (subgray.size() > 10)
		{
			for (int i = 0; i < subgray.size(); i++)
			{
				dGradient = 0;
				if (i - n >= 0 && i + n - 1 < subgray.size())
				{
					double rSubGradient = 0;
					double lSubGradient = 0;
					for (int j = 0; j < n; j++)
					{
						rSubGradient += subgray[i + j];
						lSubGradient += subgray[i - j - 1];
					}
					dGradient = rSubGradient - lSubGradient;
				}
				gradient.push_back(dGradient / (double)n / (double)src.cols);
			}
			if (direction1 == 0)
			{
				for (int i = 0; i < gradient.size(); i++)   //找第一个最大梯度
				{
					if (gradient[i] > Threshold&&startJump == false)
					{
						MaxOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
				for (int i = 0; i < gradient.size(); i++)
				{
					if (gradient[i] < -Threshold && startJump == false)
					{
						MinOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
			}
			else
			{
				for (int i = gradient.size() - 1; i > 0; i--)
				{
					if (gradient[i] > Threshold&&startJump == false)
					{
						MaxOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
				for (int i = gradient.size() - 1; i > 0; i--)
				{
					if (gradient[i] < -Threshold && startJump == false)
					{
						MinOrder.push_back(i);
						startJump = true;
					}
					if (startJump == true)
					{
						if (jumpI == intervalNum)
						{
							startJump = false;
						}
						jumpI += 1;
					}
				}
			}

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

#pragma endregion

#pragma region 返回自动阈值 1.灰度阈值2.梯度阈值
/* src：roiImg
 *@
*/
bool CJINOItem1::GetThresholdValueBin(Mat &src,double &BinThreshReturn) {

	return true;
}
/*
*@
*/
bool CJINOItem1::GetThresholdValueGrad(Mat &src, double &GradThreshReturn) {
	
	return true;
}
#pragma endregion

bool CJINOItem1::Preprocess(Mat &src) {
	if (src.data == NULL) {
		return false;
	}
	if (src.channels() > 1) {
		extractChannel(src, b, 0);
		extractChannel(src, g, 1);
		extractChannel(src, r, 2);
		cvtColor(src, src, CV_BGR2GRAY);
	}
	Rect ROI1 = Rect(min(JINOFeature.startX, JINOFeature.endX), min(JINOFeature.startY, JINOFeature.endY), abs(JINOFeature.endX - JINOFeature.startX), abs(JINOFeature.endY - JINOFeature.startY));
	if (ROI1.empty()) {
		AfxMessageBox(_T("ROI Not Set!"));
		JINOFeature.roiSetSuccess = false;
		JINOFeature.analyzing = false;
		return false;
	}
	if (!JINOSafeRect(src, ROI1)) {
		return false;
	}
	return true;
}


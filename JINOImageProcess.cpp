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
@ 目前只针对灰度图处理
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
	JINOFeature.ROIX = ROI1.x;
	JINOFeature.ROIY = ROI1.y;
	Mat roiImg = outImg(ROI1);

	vector <double>gradVector;
	vector <double>gradVector2;
	vector<int>peakIndexVec;
	vector <double>peakValueVec;

	int HV = 0, FB = 1;
	GetGradVector(roiImg, gradVector,5,HV,FB);
	GetGradVector(roiImg, gradVector2, 5, HV, 1-FB);
	GetMulPeaksEx(gradVector, peakIndexVec, peakValueVec,5);

	int templine1 = 0;
	int templine2 = 0;
	int tempIndex1 = JINOFeature.NumNLine;
	int tempIndex2 = JINOFeature.NumNLine2;
	auto scaleX = JINOFeature.gradScale;
	GetLineWithoutTreshold(roiImg, templine1, 0, 1, 1, 1, scaleX);
	GetLineWithoutTreshold(roiImg, templine2, 0, 0, 2, 1, scaleX);
	int maxValue = *max_element(peakValueVec.begin(), peakValueVec.end());
	int maxValIndex= max_element(gradVector.begin(), gradVector.end())- gradVector.begin();
	Scalar colorX1(0, 255, 0);
	Scalar colorX2(0, 0, 255);
	HistShow(gradVector, maxValue, HV, "dispaly1",colorX1,0.2);
	HistShow(gradVector2, maxValue, HV,"dispaly2",colorX2,0.2);
	//int temp = peakIndexVec[1];
	//如果
	/*if (peakValueVec[1] < 3) {
		peakIndVec.clear();
		peakValueVec.clear();
		GetGradVector(roiImg, gradVector, 5, HV, 1-FB);
		if (peakValueVec[1] < temp) {
			AfxMessageBox(_T("No Line Exists or Lines Too Weak!"));
			return false;
		}
	}*/
	//
	/*vector<int>mMaxIndVec; vector<int>mMinIndVec;
	vector<double>mMaxValueVec; vector<double>mMinValueVec;
	GetMulGradient(roiImg, 8, mMaxIndVec, mMinIndVec, mMinValueVec, mMaxValueVec, tt, 0, 0, 20);
	auto LineIndex = 1;
	if (LineIndex > mMaxIndVec.size() || LineIndex < 1)
	{
		AfxMessageBox(_T("Grab Line Index exceedes the true range!!!"));
		return false;
	}*/

	//auto LineX1 = peakIndexVec[1] + ROI1.x;
	//auto LineY1 = peakIndexVec[1] + ROI1.y;

	//竖边
	/*auto LineX1 = templine1 + ROI1.x;
	auto LineX2 = templine2 + ROI1.x;
	line(src, Point(LineX1, ROI1.y), Point(LineX1, ROI1.y + ROI1.height), Scalar(0, 0, 255), 2);
	line(src, Point(LineX2, ROI1.y), Point(LineX2, ROI1.y + ROI1.height), Scalar(0, 0, 255), 2);*/

	//横边
	auto LineY1 = templine1 + ROI1.y;
	auto LineY2 = templine2 + ROI1.y;
	line(src, Point(ROI1.x, LineY1), Point(ROI1.x + ROI1.width, LineY1), colorX1, 2);
	line(src, Point(ROI1.x, LineY2), Point(ROI1.x + ROI1.width, LineY2), colorX2, 2);
/*
	if (HV == 1) {
		line(src, Point(maxValIndex + ROI1.x-1, ROI1.y), Point(maxValIndex + ROI1.x-1, ROI1.y + ROI1.height), Scalar(0, 0, 255), 2);
		for (int i = 0; i < peakIndexVec.size(); i++) {

			if (peakIndexVec[i] > 3) {
				line(src, Point(peakIndexVec[i] + ROI1.x, ROI1.y), Point(peakIndexVec[i] + ROI1.x, ROI1.y + ROI1.height), Scalar(0, 255, 0), 2);
			}
		}
	}
	else if (HV == 0) {
		line(src, Point(ROI1.x, maxValIndex + ROI1.y-1), Point(ROI1.x+ROI1.width, maxValIndex + ROI1.y-1), Scalar(0, 0, 255), 2);
		for (int i = 0; i < peakIndexVec.size(); i++) {
			if (peakIndexVec[i] > 3) {
				line(src, Point(ROI1.x, peakIndexVec[i] + ROI1.y), Point(ROI1.x + ROI1.width, peakIndexVec[i] + ROI1.y), Scalar(0, 255, 0), 2);
			}
		}
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


bool CJINOItem1::GetGradient(Mat& src,
	int n,					/*左个各采集n个*/
	int &MaxOrder,			/*最大梯度的竖线*/
	int &MinOrder,			/*最小梯度的竖线*/
	double &MinGradient,	/*最小平均梯度*/
	double &MaxGradient,    /*最大平均梯度*/
	int direction           /*检测方向*/
)
{
	if (!src.data)
		return FALSE;
	vector<int>	subgray;//一列的灰度值之和
	vector<double> gradient;//梯度
	int minCol = 0;
	int maxCol = 0;
	double	maxGradient = 0;
	double minGradient = 0;
	double dGradient = 0;

	if (direction == 0) //如果0是横向检测，否则是纵向找梯度
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

			for (int i = 0; i < gradient.size(); i++)
			{
				if (gradient[i] > maxGradient)
				{
					maxGradient = gradient[i];
					maxCol = i;
				}
				if (gradient[i] < minGradient)
				{
					minGradient = gradient[i];
					minCol = i;
				}
			}

			MaxOrder = maxCol;
			MinOrder = minCol;
			MinGradient = minGradient;
			MaxGradient = maxGradient;
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

			for (int i = 0; i < gradient.size(); i++)
			{
				if (gradient[i] > maxGradient)
				{
					maxGradient = gradient[i];
					maxCol = i;
				}
				if (gradient[i] < minGradient)
				{
					minGradient = gradient[i];
					minCol = i;
				}
			}

			MaxOrder = maxCol;
			MinOrder = minCol;
			MinGradient = minGradient;
			MaxGradient = maxGradient;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

bool CJINOItem1::GetFirstGradient(Mat& src,
	int n,					/*左个各采集n个*/
	int &MaxOrder,			/*最大梯度的竖线*/
	int &MinOrder,			/*最小梯度的竖线*/
	double &MinGradient,	/*最小平均梯度*/
	double &MaxGradient,    /*最大平均梯度*/
	int Threshold,          /*灰度阈值*/
	int direction,          /*检测方向：1找水平线,0找竖直线*/
	int direction1          /*检测方向：0正向,1反向*/
)
{
	if (!src.data)
		return FALSE;
	vector<int>	subgray;//一列的灰度值之和
	vector<double> gradient;//梯度
	int minCol = 0;
	int maxCol = 0;
	double	maxGradient = 0;
	double minGradient = 0;
	double dGradient = 0;

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
				for (int i = 0; i < gradient.size(); i++)   //找第一个最大梯度
				{
					if (gradient[i] > Threshold)
					{
						maxGradient = gradient[i];
						maxCol = i;
						break;
					}
				}
				for (int i = 0; i < gradient.size(); i++)
				{
					if (gradient[i] < -Threshold)
					{
						minGradient = gradient[i];
						minCol = i;
						break;
					}
				}
			}
			else
			{
				for (int i = gradient.size() - 1; i > 0; i--)   //找第一个最大梯度
				{
					if (gradient[i] > Threshold)
					{
						maxGradient = gradient[i];
						maxCol = i;
						break;
					}
				}
				for (int i = gradient.size() - 1; i > 0; i--)
				{
					if (gradient[i] < -Threshold)
					{
						minGradient = gradient[i];
						minCol = i;
						break;
					}
				}
			}
			MaxOrder = maxCol;
			MinOrder = minCol;
			MinGradient = minGradient;
			MaxGradient = maxGradient;
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
					if (gradient[i] > Threshold)
					{
						maxGradient = gradient[i];
						maxCol = i;
						break;
					}
				}
				for (int i = 0; i < gradient.size(); i++)
				{
					if (gradient[i] < -Threshold)
					{
						minGradient = gradient[i];
						minCol = i;
						break;
					}
				}
			}
			else
			{
				for (int i = gradient.size() - 1; i > 0; i--)   //找第一个最大梯度
				{
					if (gradient[i] > Threshold)
					{
						maxGradient = gradient[i];
						maxCol = i;
						break;
					}
				}
				for (int i = gradient.size() - 1; i > 0; i--)
				{
					if (gradient[i] < -Threshold)
					{
						minGradient = gradient[i];
						minCol = i;
						break;
					}
				}
			}

			MaxOrder = maxCol;
			MinOrder = minCol;
			MinGradient = minGradient;
			MaxGradient = maxGradient;
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
/* 
@ src：roiImg
*/
bool CJINOItem1::GetThresholdValueBin(Mat &src,double &BinThreshReturn) {
	
	return true;
}
/* ver 0.1 
@ 对于多条线的情况，返回一个能够找出所有线的梯度阈值
@ 将灰度值转换为单行/列的一维梯度数组
@ 从梯度数组中 搜索/生成 合适的值作为阈值输出
@ 搜索方法：1.均值+offset 2.中值+offset 3.面积最大法/滑动窗口 4.大窗口滤波处理之后求二阶导数 5.前几个方法一个都没用到
@ ver 0.2 此函数只负责返回梯度数组
@ (Mat &src, vector<double>&gradVec, int n, int directionHV, int directionFB)
*/
bool CJINOItem1::GetGradVector(Mat &src, vector<double>&gradVec, int n, int directionHV, int directionFB) {
	if (src.data == NULL) {
		return false;
	}
	vector<int> toGradVec;
	//V Line
	if (directionHV==1) {
		for (int col = 0; col < src.cols; col++) {
			int sumX = 0;
			for (int row = 0; row < src.rows; row++) {
				sumX += src.at<uchar>(row, col);
			}
			toGradVec.push_back(sumX);
		}
	}
	//H Line
	else if (directionHV == 0) {
		for (int row = 0; row < src.rows; row++) {
			int sumX = 0;
			for (int col = 0; col < src.cols; col++) {
				sumX += src.at<uchar>(row, col);
			}
			toGradVec.push_back(sumX);
		}
	}
	for (int i = 0; i < toGradVec.size(); i++) {
		double leftSum = 0, rightSum = 0, grad = 0;
		for (int j = 0; j < n; j++) {
			if (i >= n && i + n - 1 < toGradVec.size()) {//认为前/后n个不存在目标，所以不处理前n个
				leftSum += toGradVec[i - j - 1];
				rightSum += toGradVec[i + j];
			}
			grad =((1-directionFB)? (rightSum - leftSum):(leftSum - rightSum)) / double(n) / double(src.cols);
		}
		gradVec.push_back(grad);
	}

#pragma region 求值//返回值丢到vector
	////均值 效果不好
	//int gradMean=std::accumulate(gradVec.begin()+n, gradVec.end()-n-1, 0)/(gradVec.size()-2*n);
	////面积 效果不好 逻辑不如求梯度最大值简单
	////1.因为我们知道ROI内应该有几条线(假设有n条)，所以返回前n个最大面积处的梯度值到一个vector
	//auto mKernelX=cv::getGaussianKernel(5, 5);//Mat mKernel = Mat::ones(5, 1, src.type());
	//Mat inX = Mat(gradVec);
	//auto outX= gradVec;
	//cv::GaussianBlur(gradVec, outX, Size(5, 1), 5);
	//
	////二阶导数//需要先高斯滤波
	//vector<double>seGrad;
	//for (int i = 0; i < outX.size()-1; i++) {
	//	seGrad.push_back(outX[i + 1] - outX[i]);
	//}
	//求多个峰值//目前的做法是认为ROI内最多有三条线
	//vector<double>peakValueVec;

	//GetMulPeaks(gradVec, peakIndVec, peakValueVec, directionHV,3);
#pragma endregion
	return true;
}

/*
@ 如果两个峰之间的最小值大于0，并且峰之间距离过近(黏连，主要是两条True边之间的灰度分布不均明暗变化干扰了梯度法抓边，如下图所示)，
  则认为其中至少有一个峰为fake的(即使峰值会比较大)，将其中较小的峰值丢弃(暂时)
*/
bool CJINOItem1::GetFiltGradVec(vector<double>&gradVec,vector<double>&returnGradVec,int peakDist) {
	return true;
}

/*
@ 获取自动阈值
*/
bool CJINOItem1::GetAutoThreshold(vector<double>&gradVec,double scale) {
	vector<int>trueLineVec; vector<double>trueValueVec;
	vector<int>peakIndexVec; vector<double>peakValueVec;
	auto maxV = *max_element(peakValueVec.begin(), peakValueVec.end());
	GetMulPeaksEx(gradVec, peakIndexVec, peakValueVec, 5);
	for (int i = 0; i < peakIndexVec.size(); i++) {
		if (peakValueVec[i] > maxV * scale) {
			trueLineVec.push_back(peakIndexVec[i]);
			trueValueVec.push_back(peakValueVec[i]);
		}
	}
	sort(trueValueVec.begin(), trueValueVec.end());

	return true;
}
#pragma endregion

bool CJINOItem1::GetThresholdValueGradEx(Mat &src, double &GradThreshReturn, int n, int directionLR, int directionFB) {
	return true;
}

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

/*
@ ver 0.1.0 
@ 最简单的做法是认为ROI内最多不超过3条线，将三个梯度最大的索引找到并降序排序
@ 先找全局最大值，之后以最大值为界，前半部分找一个最大值，后半部分找一个最大值
@ (vector<double>&inputVec, vector<int>&peakIndVec, vector<double>&peakValueVec,int directionHV,int lineNum,int interval,int n) {
*/
bool CJINOItem1::GetMulPeaks(vector<double>&inputVec, vector<int>&peakIndVec, vector<double>&peakValueVec,int directionHV,int lineNum,int interval,int n) {
	int maxValue = *max_element(inputVec.begin(), inputVec.end());
	int maxInd = max_element(inputVec.begin(), inputVec.end()) - inputVec.begin();
	Scalar colorX(0, 0, 255);
	HistShow(inputVec, maxValue, directionHV,"displayxx",colorX,0.2);
	JINOFeature.directionHV = directionHV;
	int maxVLeft = 0,maxVRight =0, maxIndLeft =0, maxIndRight = 0;
	if (lineNum>1) {
		//左侧//最大梯度的左/右侧可能会不存在线，最大梯度太靠左/右边界了
		if (maxInd - interval > n) {
			vector<int>newVectorL;
			for (int i = 0; i < maxInd - interval; i++) {
				newVectorL.push_back(inputVec[i]);
			}

			maxVLeft = *max_element(newVectorL.begin(), newVectorL.end());
			maxIndLeft = max_element(newVectorL.begin(), newVectorL.end()) - newVectorL.begin();
		}
		//右侧
		if (maxInd + interval < inputVec.size() - n) {
			vector<int>newVectorR;
			for (int i = maxInd + interval; i < inputVec.size(); i++) {
				newVectorR.push_back(inputVec[i]);
			}
			maxVRight = *max_element(newVectorR.begin(), newVectorR.end());
			maxIndRight = max_element(newVectorR.begin(), newVectorR.end()) - newVectorR.begin() + maxInd + interval + 1;
		}
		peakIndVec.push_back(maxIndLeft);peakIndVec.push_back(maxInd);peakIndVec.push_back(maxVLeft);
		peakValueVec.push_back(maxVLeft);peakValueVec.push_back(maxValue);peakValueVec.push_back(maxVRight);
	}

	return true;
}
/*
@ ver 0.2.0 (in use)
@ 如果当前最大值localMax往后 {range} 范围内没有比localMax大的值，则认为localMax为一个局部最大值
@ 按照上->下 || 左->右 的顺序输出 局部最大值索引到peakIndex，值到peakValue
@ (vector<int>&gradVec, vector<int>&peakIndex, vector<double>&peakValue, int range = 5)
*/
bool CJINOItem1::GetMulPeaksEx(vector<double>&gradVec, vector<int>&peakIndex, vector<double>&peakValue, int range) {

	int localCnt = 0;
	int trueCnt = 0;
	double localMax = -1000;
	int localMaxIndex = 0;
	bool findOneMax = false;
	for (int i = 0; i < gradVec.size(); i++) {
		if (gradVec[i] > localMax) {
			localMax = gradVec[i];
			localCnt = 0;
		}
		else if(gradVec[i] < localMax) {
			localCnt += 1;
		}
		if (localCnt == range) {//防止处于山坡的右侧时求得fakeMax//计数到达range的时候，需要判断是否是fakeMax
			if (trueCnt > 0) {
				if (!(localMax > gradVec[i - range-1] && localMax > gradVec[i - range - 2])){
					localMax = gradVec[i];
					localCnt = 0;
					continue;
				}
			}
			if (localMax > 3) {//初步过滤不合理的peaks
				localMaxIndex = i - range;
				peakIndex.push_back(localMaxIndex);
				peakValue.push_back(localMax);
				trueCnt += 1;
				localMax = gradVec[i];
				localCnt = 0;
			}
		}
	}
	return true;
}

/*
@ 正反向各求一次求交集
@ (vector<double>&gradVec, vector<int>&tempPeakIndexF, vector<double>&tempGradVecF, vector<int>&tempPeakIndexB, vector<double>&tempGradVecB ,int range)
*/
bool CJINOItem1::GetMulPeaksEx2(vector<double>&gradVec, vector<int>&tempPeakIndexF, vector<double>&tempGradVecF, vector<int>&tempPeakIndexB, vector<double>&tempGradVecB ,int range) {

	int localCnt = 0;
	double localMax = -1000;
	int localMaxIndex = 0;
	bool findOneMax = false;
	//正向找一次
	for (int i = 0; i < gradVec.size(); i++) {
		if (gradVec[i] > localMax) {
			localMax = gradVec[i];
			localCnt = 0;
		}
		else if (gradVec[i] < localMax) {
			//localMaxIndex = i-1;
			localCnt += 1;
		}
		if (localCnt == range) {
			localMaxIndex = i - range;
			tempPeakIndexF.push_back(localMaxIndex);
			tempGradVecF.push_back(localMax);
			localMax = -1000;
			localCnt = 0;
		}
	}
	localCnt = 0;
	localMax = -1000;
	localMaxIndex = 0;
	//反向找一次
	for (int j = gradVec.size()-1; j > 0; j--) {
		if (gradVec[j] > localMax) {
			localMax = gradVec[j];
			localCnt = 0;
		}
		else if (gradVec[j] < localMax) {
			//localMaxIndex = i-1;
			localCnt += 1;
		}
		if (localCnt == range) {
			localMaxIndex = j - range;
			tempPeakIndexB.push_back(localMaxIndex);
			tempGradVecB.push_back(localMax);
			localMax = -1000;
			localCnt = 0;
		}
	}
	return true;
}


bool CJINOItem1::HistShow(vector<double>&inputVec, int maxValue,int directionHV,const char* name,Scalar color,double scale) {
	Scalar baseColor(255, 0, 0);
	int histMaxV = maxValue;
	if (directionHV == 0) {
		if (maxValue < 30) {
			histMaxV = maxValue + 30;
		}
		Mat histImg = Mat::ones(inputVec.size(), 2*(histMaxV + 2), CV_8UC3);
		for (int i = 0; i < inputVec.size(); i++) {
			Rect histI = Rect(histMaxV + 2 - inputVec[i], i, inputVec[i], 1);
			cv::rectangle(histImg, histI, color, 1);
		}
		for (int i = 0; i < inputVec.size(); i++) {
			if (inputVec[i] > maxValue*scale) {
				Rect histI = Rect(histMaxV,i,inputVec[i],1);
				cv::rectangle(histImg, histI, baseColor + color, 1);
			}
			else {
				continue;
			}
		}
		JINOFeature.histWindowWidth = inputVec.size();
		destroyWindow(name);//删除上一张图像的数据
		imshow(name, histImg);
	}
	if (directionHV == 1) {
		Mat histImg = Mat::ones(histMaxV + 2, inputVec.size(), CV_8UC3);
		for (int i = 0; i < inputVec.size(); i++) {
			Rect histI = Rect(i, histMaxV + 2 - inputVec[i], 1, inputVec[i]);
			cv::rectangle(histImg, histI, color, 1);
		}
		for (int i = 0; i < inputVec.size(); i++) {
			if (inputVec[i] > maxValue*scale) {
				Rect histI = Rect(i, histMaxV, 1, inputVec[i]);
			}
			else {
				continue;
			}
		}
		JINOFeature.histWindowHeight = histMaxV + 2;
		destroyWindow(name);
		imshow(name, histImg);
	}
	JINOFeature.directionHV = directionHV;
	return true;
}

bool CJINOItem1::HistMat(Mat &src) {
	return true;
}

/*
@ mjk
@ (Mat &roiImg, int &returnIndex, int directionHV, int directionFB, int lineNo,int fromTo, double scale)
*/
bool CJINOItem1::GetLineWithoutTreshold(Mat &roiImg, int &returnIndex, int directionHV, int directionFB, int lineNo,int fromTo, double scale) {

	vector<double>GradVec; vector<int>trueLineVec;
	vector<int>peakIndexVec; vector<double>peakValueVec;
	GetGradVector(roiImg, GradVec, 5, directionHV, directionFB);
	GetMulPeaksEx(GradVec, peakIndexVec, peakValueVec, 5);
	for (int i = 0; i < peakIndexVec.size(); i++) {
		if (peakValueVec[i] > (*max_element(peakValueVec.begin(), peakValueVec.end()))*scale) {
			trueLineVec.push_back(peakIndexVec[i]);
		}
	}
	if (lineNo > trueLineVec.size() || lineNo < 1)
	{
		AfxMessageBox(_T("Grab Line Index exceedes the range!!!"));
		return false;
	}
	returnIndex = (fromTo == 0) ? trueLineVec[lineNo - 1]: trueLineVec[trueLineVec.size() - lineNo];
	return true;
}

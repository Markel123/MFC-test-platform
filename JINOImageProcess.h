#pragma once
#include "JINOBase.h"
#include "JINORun.h"
#include "JINORunDlg.h"
#include <numeric>
#include <cmath>
#include "Resource.h"
#include "afxdialogex.h"

#include<vector>

/////////////////项目类/////////////////
class CJINOItem1 : public CJINOBase {
public:
	Mat mImage, resultImage;
	Mat b, g, r;
	Mat grayImg;

public:
	static CJINOItem1* GetInstance();

public:
	virtual bool JINOMainProcess(Mat &src, Mat &outImg);
	virtual bool JINOImgProcess1(Mat &src, Mat &outImg);
	virtual bool JINOImgProcess2(Mat &src, Mat &outImg);
	virtual bool JINOImgProcess3(Mat &src, Mat &outImg);
	virtual bool JINOSafeRect(Mat &src, Rect &rect);
	virtual bool JINOScaleRect(Mat &src, Rect &rect, int size);
	virtual bool JINOWindowAttach(int littleWindowSize, int ID = 0, const char* pWindowName = "", const char* cWindowName = "");

public:
	virtual bool GetMulGradient(Mat& src, int n, vector<int> &MaxOrder, vector<int> &MinOrder, vector<double> &MinGradient, vector<double> &MaxGradient, int Threshold, int direction, int direction1, int intervalNum);
	virtual bool GetGradient(Mat& src, int n, int &MaxOrder, int &MinOrder, double &MinGradient, double &MaxGradient, int direction);
	virtual bool GetFirstGradient(Mat& src, int n, int &MaxOrder, int &MinOrder, double &MinGradient, double &MaxGradient, int Threshold, int direction, int direction1);
	virtual bool GetThresholdValueBin(Mat &src,double &BinThreshReturn);
	virtual bool GetGradVector(Mat &src, vector<double>&gradVec,int n,int directionLR,int directionFB);
	virtual bool GetThresholdValueGradEx(Mat &src, double &GradThreshReturn, int n, int directionLR, int directionFB);
	virtual bool GetMulPeaks(vector<double>&gradVec, vector<int>&peakindex,vector<double>&peakValue,int directionHV = 0,int linNum = 1,int interval=10,int n=5);
	virtual bool GetMulPeaksEx(vector<double>&gradVec, vector<int>&peakIndex, vector<double>&peakValue, int range=5);
	virtual bool GetMulPeaksEx2(vector<double>&gradVec, vector<int>&tempPeakIndexF, vector<double>&tempGradVecF, vector<int>&tempPeakIndexB, vector<double>&tempGradVecB, int range);
	virtual bool HistShow(vector<double>&inputVec, int maxValue,int directionHV);
	virtual bool Preprocess(Mat &src);
public:

};
//////////////////结构体数据///////////////////
struct struJINOFeature
{
public:
	Mat mImage, resizedImg, reslutImage, showImg;
	Mat littleWindowImg;
	Mat markedImg;
	bool picExist = false;
	bool analyzing = false;//为了修复Bug:Analyze之后，鼠标点击左上角的小图，窗口又立马显示了原图
	bool littleWindowExist = false;//小窗口存在的时候，为true//暂时不用
	bool roiSetSuccess = false;
	int clickCnt = 0;//为了修复Bug:图片刚打开的时候，左上角与鼠标最后一次坐标之间生成了一个框、/、点击图片次数超过1次才启用画ROI
	int scaleRectSize = 10;
	int littleWindowSize = 100;
	int startX, endX, startY, endY;
	int histWindowWidth,histWindowHeight;
	int directionHV = 0;
	const char* littleWindowName = "JINOBounce";
	const char* histWindowName = "HIST";

	enum curWindowPicture {Raw,Resized,Result,Show};
	enum lineNum { one = 1,two,three }lineNum;

public:
	CComboBox ComboMethode;
	int curMethode = 0;
};
///////////////////分号保护////////////////////

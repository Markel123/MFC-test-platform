#pragma once
#include<opencv2\highgui.hpp>
#include<opencv.hpp>

using namespace cv;
using namespace std;

//////////////////////////基类//////////////////////////
///////////////最主要的函数 声明放在这里////////////////
class CJINOBase {
public:
	////////////////这些暂时没用到///////////////
	Mat mImage,resultImage;
	Mat b, g, r;
	Mat grayImg;
protected:
	static CMutex mutex;//类的静态成员初始化之前是不会分配内存空间的，也即是不存在的；此时会提示LINK error 变量不存在(无法解析的外部符号)
	static CJINOBase *Singleton;
public:
	template<class _tp> static CJINOBase *GetSingleton() {
		if (Singleton == NULL) {
			mutex.Lock();
			if (Singleton == NULL) {
				Singleton = new _tp;//C++ new返回指针
			}
			mutex.Unlock();
		}
		return Singleton;
	}
	~CJINOBase(){};

public:
	virtual bool JINOMainProcess(Mat &src, Mat &outImg) = 0;
	virtual bool JINOImgProcess1(Mat &src, Mat &outImg) = 0;
	virtual bool JINOImgProcess2(Mat &src, Mat &outImg) = 0;
	virtual bool JINOImgProcess3(Mat &src, Mat &outImg) = 0;
	virtual bool JINOSafeRect(Mat &src,Rect &rect) = 0;
	virtual bool JINOWindowAttach(int littleWindowSize, int ID = 0, const char* pWindowName="",const char* cWindowName="") = 0;

public:
	
};



// JINORunDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "JINORun.h"
#include "JINORunDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ROI绘图变量

//opencv绘图变量
int startX, startY, endX, endY;
bool picExist;
extern struJINOFeature JINOFeature;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CJINORunDlg 对话框



CJINORunDlg::CJINORunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJINORunDlg::IDD, pParent)
	, m_NumNLine(0)
	, m_NumNLine2(0)
	, m_gradScale(0)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJINORunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Methode_Type, ComboMethode);
	//  DDX_Text(pDX, IDC_EDIT_NumNLine, m_NumNLine);
	DDX_Text(pDX, IDC_EDIT_NumNLine, m_NumNLine);
	DDX_Text(pDX, IDC_EDIT_NumNLine2, m_NumNLine2);
	//  DDX_Text(pDX, IDC_EDIT_gradScale, m_radScale);
	//  DDX_Text(pDX, IDC_EDIT_gradScale, m_gradScale);
	DDX_Text(pDX, IDC_EDIT_gradScale, m_gradScale);
}

BEGIN_MESSAGE_MAP(CJINORunDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_ICON, &CJINORunDlg::OnStnClickedIcon)
	ON_BN_CLICKED(IDC_BUTTON_Open_Pic, &CJINORunDlg::OnClickedButtonOpenPic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Analyze_Pic, &CJINORunDlg::OnClickedButtonAnalyzePic)
	ON_BN_CLICKED(IDC_BUTTON_Reset_ROI, &CJINORunDlg::OnClickedButtonResetRoi)
	ON_CBN_SELCHANGE(IDC_COMBO_Methode_Type, &CJINORunDlg::OnSelchangeComboMethodeType)
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_BN_CLICKED(IDC_BUTTON_Execute, &CJINORunDlg::OnClickedButtonExecute)
END_MESSAGE_MAP()


// CJINORunDlg 消息处理程序

BOOL CJINORunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
#pragma region	新建大窗口并绑定
	attachWindow("Display", IDC_STATIC_Show_Pic);//可以换成和小窗口一样的做法
	myMouseAction = &CJINORunDlg::onMouseAction;//函数指针赋值,静态/非静态成员函数右值都需要加域//类成员函数指针赋值要加&
	setMouseCallback("Display", myMouseAction);
#pragma endregion

#pragma region 新建小窗口并绑定
	CJINOItem1::GetInstance()->JINOWindowAttach(JINOFeature.littleWindowSize,0, "Display", JINOFeature.littleWindowName);

#pragma endregion

#pragma region 控件状态初始化

	initMethodeCombobox();
#pragma endregion
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CJINORunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJINORunDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CJINORunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CJINORunDlg::OnStnClickedIcon()
{
	// TODO:  在此添加控件通知处理程序代码
}


void CJINORunDlg::OnClickedButtonOpenPic()
{
	// TODO:  打开图片
	//选择图片
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
	//打开文件选择窗体
	if (fileDlg.DoModal() == IDCANCEL)
	{
		JINOFeature.picExist = false;
		return; //如果点击“取消”按钮，直接退出
	}
	//获取图片路径（包含名称）
	CString strFilePath = fileDlg.GetPathName();//既有路径又有文件名，如D:/lena.jpg
	//CString strFileName = fileDlg.GetFileName();//只获取文件名，如lena.jpg
	//判断路径不为空
	if (strFilePath == _T(""))
	{
		JINOFeature.picExist = false;
		return;
	}
	JINOFeature.picExist = true;
	//使用CImage的Load、Draw函数显示图像

	string filePath=CW2A(strFilePath.GetString());//CString与string互转
	JINOFeature.mImage = imread(filePath);	//c_str();
	JINOFeature.clickCnt = 0;
	JINOFeature.analyzing = false;

	CRect rect;
	GetDlgItem(IDC_STATIC_Show_Pic)->GetClientRect(&rect);
	cv::resize(JINOFeature.mImage, JINOFeature.resizedImg, cv::Size(rect.Width(), rect.Height()), CV_INTER_CUBIC);
	JINOFeature.markedImg = JINOFeature.resizedImg;
	imshow("Display", JINOFeature.resizedImg);

#pragma region 使用MFC函数来刷图
	///////////////////////////
	//CImage image;
	//image.Load(strFilePath);
	//cImage = image;
	////获取控件的矩形
	//CRect rectControl;  //控件矩形对象
	//CWnd  *pWnd = GetDlgItem(IDC_STATIC_Show_Pic); //Picture Control的ID为IDC_IMAGE
	//pWnd->GetClientRect(&rectControl);
	////以控件为画布，在其上画图
	//CDC *pDc = GetDlgItem(IDC_STATIC_Show_Pic)->GetDC();
	//SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重
	////画图（以下两种方法都可）
	////image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
	//image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	//image.Destroy();
	//pWnd->ReleaseDC(pDc);
#pragma endregion
}


void CJINORunDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//if (picExist){
	//	GetCursorPos(&selPos);
	//	GetDlgItem(IDC_STATIC_Show_Pic)->GetWindowRect(picRect);
	//	if (picRect.PtInRect(selPos)) {
	//		ClipCursor(&picRect);//限制鼠标在picRect范围内移动，参数为NULL则取消限制
	//		auto rect_width = picRect.right - picRect.left;
	//		auto rect_height = picRect.bottom - picRect.top;
	//		xScale = cImage.GetHeight() / rect_width;
	//		yScale = cImage.GetWidth() / rect_height;

	//		startPos.x = (selPos.x - picRect.left)*xScale;
	//		startPos.y = (selPos.y - picRect.top)*yScale;
	//	}
	//	moveFlag = true;
	//	CDialogEx::OnLButtonDown(nFlags, point);
	//}
}


void CJINORunDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//if (picExist) {
	//	if (upFlag) {
	//		GetCursorPos(&selPos);
	//		GetDlgItem(IDC_STATIC_Show_Pic)->GetWindowRect(&picRect);
	//		if (picRect.PtInRect(selPos)) {
	//			ClipCursor(NULL);
	//			auto rect_width = picRect.right - picRect.left;
	//			auto rect_height = picRect.bottom - picRect.top;

	//			endPos.x = (selPos.x - picRect.left)*xScale;
	//			endPos.x = (selPos.y - picRect.top)*yScale;
	//		}
	//	}
	//	upFlag = false;
	//	roiRect.x = startPos.x;
	//	roiRect.y = startPos.y;
	//	roiRect.width = endPos.x - startPos.x;
	//	roiRect.height = endPos.y - startPos.y;

	//	auto mImageX = mImage.clone();
	//	rectangle(mImage, roiRect, Scalar(255, 255, 0));
	//	//attachWindow("Display", IDC_STATIC_Show_Pic);

	//	CRect rect;
	//	GetDlgItem(IDC_STATIC_Show_Pic)->GetClientRect(&rect);
	//	cv::resize(mImageX, mImageX, cv::Size(rect.Width(), rect.Height()), CV_INTER_CUBIC);
	//	imshow("Display", mImageX);
	//	mImageX.release();

	//	CDialogEx::OnLButtonUp(nFlags, point);
	//}
}


void CJINORunDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*if (picExist) {
		if (moveFlag) {
			GetCursorPos(&selPos);
			GetDlgItem(IDC_STATIC_Show_Pic)->GetWindowRect(&picRect);
			if (picRect.PtInRect(selPos)) {
				auto rect_width = picRect.right - picRect.left;
				auto rect_height = picRect.bottom - picRect.top;
				endPos.x = (selPos.x - picRect.left)*xScale;
				endPos.y = (selPos.y - picRect.top)*yScale;
			}
			moveFlag = false;
		}
		upFlag = true;
		CDialogEx::OnMouseMove(nFlags, point);
	}*/
}

BOOL CJINORunDlg::attachWindow(const char* wndName,int ID){//因为调用的时候第一个参数是直接输入常量字符串，下面有些函数用不了，转换很麻烦
	cv::namedWindow(string(wndName));
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	cv::resizeWindow(string(wndName), cv::Size(rect.Width(), rect.Height()));
	HWND hWnd = (HWND)cvGetWindowHandle(wndName);
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(ID)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	return true;
}

void CJINORunDlg::onMouseAction(int event,int x,int y,int flags,void* userdata) {
	///////////////////////所有的Mouse Action应该都是针对当前窗口中显示的图片/////////////////////
	/*图片分析之后就不能再画框了，但是应该显示坐标，并且放大功能存在*/
	if (JINOFeature.picExist) {
		//////////////////////////Common_Mouse_Action/////////////////////////
		if (event == CV_EVENT_MOUSEMOVE&&flags != CV_EVENT_FLAG_LBUTTON && flags != CV_EVENT_FLAG_RBUTTON) {
			string curCoord = "";
			auto curX = x;
			auto curY = y;
			curCoord +=(to_string(curX)+","+to_string(curY));
			if (!JINOFeature.analyzing) {
				Mat mImageX = JINOFeature.markedImg.clone();
				putText(mImageX, curCoord, Point(curX, curY), CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 255));

				imshow("Display", mImageX);
				mImageX.release();
			}
			else if (JINOFeature.analyzing) {
				Mat mImageX = JINOFeature.reslutImage.clone();
				putText(mImageX, curCoord, Point(curX, curY), CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 255));

				imshow("Display", mImageX);
				mImageX.release();
			}
		}
		///////////////////////Mouse_Left_Button_Action///////////////////////
		if (event == CV_EVENT_LBUTTONDOWN) {
			JINOFeature.clickCnt += 1;
			if (!JINOFeature.analyzing) {
				startX = x;
				startY = y;
			}
		}
		if (event == CV_EVENT_MOUSEMOVE&&flags==CV_EVENT_FLAG_LBUTTON&& JINOFeature.clickCnt>1) {
			if (!JINOFeature.analyzing) {//
				endX = x;
				endY = y;
				Mat mImageX = JINOFeature.resizedImg.clone();
				rectangle(mImageX, Point(startX, startY), Point(endX, endY), Scalar(0, 180, 255));

				imshow("Display", mImageX);
				mImageX.release();
			}
		}
		if (event == CV_EVENT_LBUTTONUP&& JINOFeature.clickCnt>1) {
			if (!JINOFeature.analyzing) {
				endX = x;
				endY = y;
				Mat mImageX = JINOFeature.resizedImg.clone();
				rectangle(mImageX, Point(startX, startY), Point(endX, endY), Scalar(0, 255, 0));
				//startX = startX < endX ? startX : endX;
				//startY = startY < endY ? startY : endY;
				putText(mImageX, "ROI", Point(min(startX, endX), min(startY, endY) - 10), CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 255, 255));

				JINOFeature.markedImg = mImageX;
				imshow("Display", mImageX);
				mImageX.release();
				JINOFeature.roiSetSuccess = true;
			}
		}
		//修复Bug:刷坐标和刷ROI框应该有一个优先级关系

		///////////////////////Mouse_Right_Button_Action///////////////////////
		/*if (event == CV_EVENT_RBUTTONDOWN) {
			if (!JINOFeature.analyzing) {
				startX = x;
				startY = y;
			}
		}*/
		if (event == CV_EVENT_MOUSEMOVE && flags==CV_EVENT_FLAG_RBUTTON) {
			if (true/*!JINOFeature.analyzing*/) {
				if (JINOFeature.littleWindowExist) {//小窗口是否存在，若存在，那么鼠标右键移动的时候，小窗口应该销毁//正确的应该是移动而不是销毁
					/*cv::destroyWindow(JINOFeature.littleWindowName);
					JINOFeature.littleWindowExist = false;*/
				}
				if (true) {
					//cv::namedWindow(JINOFeature.littleWindowName);
					//HWND hcWnd = (HWND)cvGetWindowHandle(JINOFeature.littleWindowName);
					//HWND hpWnd = (HWND)cvGetWindowHandle("Display");
					//HWND hParent = ::GetParent(hcWnd);
					//::SetParent(hcWnd, hpWnd);
					//::ShowWindow(hParent, SW_HIDE);

					auto curX = x;
					auto curY = y;
					Mat mImageX = JINOFeature.analyzing?JINOFeature.reslutImage.clone():JINOFeature.resizedImg.clone();
					Rect scaleRect = Rect(curX - JINOFeature.scaleRectSize / 2, curY - JINOFeature.scaleRectSize / 2, JINOFeature.scaleRectSize, JINOFeature.scaleRectSize);
					CJINOItem1::GetInstance()->JINOScaleRect(mImageX, scaleRect, JINOFeature.scaleRectSize);
					Mat littleWindowImg;
					cv::resize(mImageX(scaleRect), littleWindowImg, cv::Size(JINOFeature.littleWindowSize, JINOFeature.littleWindowSize));

					//cv::moveWindow(JINOFeature.littleWindowName, curX, curY);
					imshow(JINOFeature.littleWindowName, littleWindowImg);
					JINOFeature.littleWindowExist = true;
				}
			}
			if (event == CV_EVENT_RBUTTONUP) {
				if (!JINOFeature.analyzing) {
					if (JINOFeature.littleWindowExist) {
						cv::destroyWindow(JINOFeature.littleWindowName);
						JINOFeature.littleWindowExist = false;
					}
				}
			}
		}
	}
}

void CJINORunDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (AfxMessageBox(_T("   是否关闭窗口？"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK) {
		CDialogEx::OnClose();
	}
}


BOOL CJINORunDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == 27) {
		return NULL;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CJINORunDlg::OnClickedButtonAnalyzePic()
{
	// TODO: 在此添加控件通知处理程序代码
	CJINOItem1::GetInstance()->JINOImgProcess1(JINOFeature.resizedImg,JINOFeature.reslutImage);
	if (JINOFeature.analyzing) {
		imshow("Display", JINOFeature.reslutImage);
	}
}


void CJINORunDlg::OnClickedButtonResetRoi()
{
	// TODO: 在此添加控件通知处理程序代码
	if (JINOFeature.roiSetSuccess) {
		JINOFeature.startX = startX;
		JINOFeature.startY = startY;
		JINOFeature.endX = endX;
		JINOFeature.endY = endY;
		JINOFeature.roiSetSuccess = false;
		AfxMessageBox(_T("ROI Reset Succeed!"));
	}
}


void CJINORunDlg::OnSelchangeComboMethodeType()
{
	// TODO: 在此添加控件通知处理程序代码
	onMethodeComboboxChange();
}


void CJINORunDlg::initMethodeCombobox() {
	ComboMethode.AddString(_T("Method1"));
	ComboMethode.AddString(_T("Method2"));
	ComboMethode.AddString(_T("Method3"));
	ComboMethode.SetCurSel(0);
}

void CJINORunDlg::onMethodeComboboxChange() {
	UpdateData(TRUE);//True string->variable;False variale->string
	int curSel = ComboMethode.GetCurSel();
	JINOFeature.curMethode = curSel + 1;
}

void CJINORunDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	
	// TODO: 在此处添加消息处理程序代码

}


void CJINORunDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	CRect windowRect;
	GetWindowRect(&windowRect);
	if (JINOFeature.directionHV == 0) {
		//cv::moveWindow(JINOFeature.histWindowName, windowRect.left - JINOFeature.histWindowWidth-5, windowRect.top + JINOFeature.ROIX);
	}
	if (JINOFeature.directionHV == 1) {
		//cv::moveWindow(JINOFeature.histWindowName, windowRect.left+JINOFeature.ROIY, windowRect.top - JINOFeature.histWindowHeight - 45);
	}
}


void CJINORunDlg::OnClickedButtonExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	JINOFeature.NumNLine = m_NumNLine;
	JINOFeature.NumNLine2 = m_NumNLine2;
	JINOFeature.gradScale = m_gradScale;
}

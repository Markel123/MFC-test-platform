
// JINORunDlg.h : 头文件
//

#pragma once
#include"JINOBase.h"
#include"JINOImageProcess.h"
#include "stdafx.h"

// CJINORunDlg 对话框
class CJINORunDlg : public CDialogEx
{
// 构造
public:
	CJINORunDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_JINORUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedIcon();
	afx_msg void OnClickedButtonOpenPic();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	BOOL attachWindow(const char* wndName,int ID);
	static void onMouseAction(int event, int x, int y, int flags, void* userdata);
	void(*myMouseAction)(int , int , int , int , void* );//指向非静态成员函数需要加上类域
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void initMethodeCombobox();
	void onMethodeComboboxChange();
public:

	afx_msg void OnClickedButtonAnalyzePic();
	afx_msg void OnClickedButtonResetRoi();
	// 选择图像处理的方法
	CComboBox ComboMethode;
	afx_msg void OnSelchangeComboMethodeType();
};

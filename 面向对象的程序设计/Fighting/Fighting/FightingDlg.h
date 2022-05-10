
// FightingDlg.h: 头文件
//

#pragma once

#include "Object.h"
#include "Shell.h"
#include "wall.h"
#include<vector>
#include "EneTank.h"
#include "MyTank.h"
#include <thread>
#include <mutex>
#include <Windows.h>

using namespace std;


// CFightingDlg 对话框
class CFightingDlg : public CDialogEx
{
// 构造
public:
	CFightingDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIGHTING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	static bool work;
	static MyTank myTank;
	static vector<EneTank>eneTank;
	static vector<Shell>myvecShell;
	static vector<Shell>enevecShell;
	static vector<wall>vecWall;

	static mutex mut;
	static void Collide1();
	static void Collide2();
	//static void threadTT();
	//static void threadSS();
	//static void threadTS();


	


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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};



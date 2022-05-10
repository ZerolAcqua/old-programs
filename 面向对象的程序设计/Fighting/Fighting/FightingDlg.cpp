
// FightingDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Fighting.h"
#include "FightingDlg.h"
#include "afxdialogex.h"
#include "Shell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPoint EneTank::birth[3] = { CPoint(100,100),CPoint(300,100),CPoint(500,100) };

MyTank CFightingDlg::myTank;
vector<EneTank> CFightingDlg::eneTank;
vector<Shell> CFightingDlg::myvecShell;
vector<Shell> CFightingDlg::enevecShell;
vector<wall> CFightingDlg::vecWall;
CString str;
int shell_num ;
bool CFightingDlg::work = true;
mutex CFightingDlg::mut;

void CFightingDlg::Collide1()
{
	int i, j;
	CRect tank;
	while (work)
	{
		if (mut.try_lock())
		{
		//////炮弹碰撞//////
			for (i = 0; i < myvecShell.size(); i++)
			{
				//我方炮弹出界
				if (!myvecShell[i].m_inbox)
				{
					myvecShell.erase(myvecShell.begin() + i);
					i--;
					continue;
				}
				//敌我炮弹碰撞
				for (j = 0; j < enevecShell.size(); j++)
				{				
					if (myvecShell[i].SSisOverlap(enevecShell[j]))
					{
						myvecShell.erase(myvecShell.begin() + i);
						enevecShell.erase(enevecShell.begin() + j);
						i--;
						j--;
						break;
					}
				}
			}
			//敌方炮弹出界
			for(i=0;i<enevecShell.size();i++)
				if (!enevecShell[i].m_inbox)
				{
					enevecShell.erase(enevecShell.begin() + i);
					i--;
				}
		//////敌方坦克与我方炮弹//////
			for (i = 0; i < eneTank.size(); i++)
			{
				tank = eneTank[i].GetPos();
				for (j = 0; j < myvecShell.size(); j++)
				{
					if (myvecShell[j].IsOverLap(tank))
					{
						myvecShell.erase(myvecShell.begin() + j);
						eneTank.erase(eneTank.begin() + i);
						EneTank::eme_down();
						i--;

						break;
					}
				}
			}
		//玩家坦克与敌方炮弹
			for (i = 0; i < enevecShell.size(); i++)
			{
				tank = myTank.GetPos();
				if (enevecShell[i].IsOverLap(tank))
				{
					myTank.lifedecrease();
					myTank.respan();
					enevecShell.erase(enevecShell.begin() + i);
					i--;
				}
			}
		//////坦克碰撞//////
			for (i = 0; i < eneTank.size(); i++)
			{
				if (myTank.TOisOverlap(eneTank[i]))
				{
					eneTank[i].re_time();
				}
			}
			myTank.IsOut();
			for (i = 0; i < eneTank.size(); i++)
			{
				for (j = 0; j < eneTank.size(); j++)
				{
					if (i != j)
					{
						if (eneTank[i].TOisOverlap(eneTank[j]))
						{
							eneTank[i].re_time();
							eneTank[j].re_time();
						}
					}
				}
				if (eneTank[i].TOisOverlap(myTank) || eneTank[i].IsOut())
				{
					eneTank[i].re_time();
				}
			}
			mut.unlock();
		}
	}
}

void CFightingDlg::Collide2()
{
	int i=0, j=0;
	bool check = false;

	while (work)
	{
		if (mut.try_lock())
		{
			//炮弹与墙
			for (i = 0; i < myvecShell.size(); i++)
			{
				for (j = 0, check = false; j < vecWall.size(); j++)
				{
					if (vecWall[j].IsOverLap(myvecShell[i].GetPos()))
					{
						if (vecWall[j].type == BASE)
						{
							myTank.gameover();
							work = false;
						}
						if (vecWall[j].ShellPass)
						{
							continue;
						}
						else
						{
							check = true;
							if (vecWall[j].destroyable)
							{
								vecWall.erase(vecWall.begin() + j);
								j--;		
							}
						}
					}
				}
				if (check)
				{
					myvecShell.erase(myvecShell.begin() + i);
					i--;
				}
			}
			for (i = 0; i < enevecShell.size(); i++)
			{
				for (j = 0, check = false; j < vecWall.size(); j++)
				{
					if (vecWall[j].IsOverLap(enevecShell[i].GetPos()))
					{
						if (vecWall[j].type == BASE)
						{
							myTank.gameover();
							work = false;
						}
						if(vecWall[j].ShellPass)
						{
							continue;
						}
						else
						{
							check = true;
							if (vecWall[j].destroyable)
							{
								vecWall.erase(vecWall.begin() + j);
								j--;
							}
						}
					}
				}
				if (check)
				{
					enevecShell.erase(enevecShell.begin() + i);
					i--;
				}
			}
			//坦克与墙
			//敌方
			for (i = 0; i < eneTank.size(); i++)
			{
				for (j = 0; j < vecWall.size(); j++)
				{
					if (vecWall[j].type == BASE)
					{
						if (vecWall[j].IsOverLap(eneTank[i].GetPos()))
						{
							myTank.gameover();
							work = false;
						}
					}
					if (vecWall[j].type!=GRASS&&eneTank[i].TOisOverlap(vecWall[j]))
					{
						eneTank[i].re_time();
						break;
					}
				}
			}
			//玩家
			for (j = 0; j < vecWall.size(); j++)
			{
				if (!vecWall[j].TankPass)
				{
					if (myTank.TOisOverlap(vecWall[j]))
					{
						break;
					}
				}
			}
			//墙与墙
			for (j = 0; j < vecWall.size(); j++)
			{
				for (i = 0; i < vecWall.size(); i++)
				{
					if (vecWall[j].m_movable && j != i)
					{
						if (vecWall[j].IsOverLap(vecWall[i].GetPos())&& vecWall[i].type!=GRASS)
						{
							vecWall.erase(vecWall.begin() + j);
							j--;
							break;
						}
						vecWall[j].IsOut();
					}
				}

			}
			mut.unlock();
		}
	}
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFightingDlg 对话框



CFightingDlg::CFightingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FIGHTING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFightingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFightingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
//	ON_WM_ERASEBKGND()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_KEYUP()
END_MESSAGE_MAP()


// CFightingDlg 消息处理程序

BOOL CFightingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// TODO: 在此添加额外的初始化代码


	SetTimer(1, 40, NULL);
	SetTimer(2, 20000, NULL);
	SetTimer(3, 20, NULL);
	srand(time(NULL) * time(NULL));
	shell_num = 2;//坦克可发射炮弹数

	////////贴图导入////////
	CString strFileName;
	TCHAR cPath[1024];
	GetModuleFileName(NULL, cPath, 1024);
	strFileName = cPath;
	CString str;
	//对炮弹的静态成员变量赋值
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\shell.png";
	Shell::s_img.Load(str);	
	//对敌方坦克的静态成员变量赋值
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\eneTank0.png";
	EneTank::ene_img[0].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\eneTank1.png";
	EneTank::ene_img[1].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\eneTank2.png";
	EneTank::ene_img[2].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\eneTank3.png";
	EneTank::ene_img[3].Load(str);
	//墙
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\0.png";
	wall::w_img[BRICK].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\1.png";
	wall::w_img[METAL].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\2.png";
	wall::w_img[GRASS].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\3.png";
	wall::w_img[WATER].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\4.png";
	wall::w_img[WOOD].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\5.png";
	wall::w_img[BASE].Load(str);
	//对坦克的静态成员变量赋值
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\mytank0.png";
	MyTank::m_img[0].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\mytank1.png";
	MyTank::m_img[1].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\mytank2.png";
	MyTank::m_img[2].Load(str);
	str = strFileName.Left(strFileName.ReverseFind('\\') + 1) + L"material\\mytank3.png";
	MyTank::m_img[3].Load(str);

	////////////线程/////////////
	thread T(Collide1);
	T.detach();
	thread W(Collide2);
	W.detach();

	//////////地图载入///////////
	for(int j = 0; j < Y_MAX / 100; j++)
		for (int i = 0; i < X_MAX / 50; i++)
		{	
			if((i+j)%4)
			vecWall.push_back(wall(CPoint(i * 50, j * 100), (i + j)% 5));
		}
	vecWall.push_back(wall(CPoint(250, 450),BASE));
	vecWall.push_back(wall(CPoint(200, 400), BRICK));
	vecWall.push_back(wall(CPoint(200, 450), WOOD));
	vecWall.push_back(wall(CPoint(300, 450), WOOD));
	///////////敌方坦克初始化///////////
	for(int i=0;i<3;i++)
	{
		EneTank eme(i);
		eneTank.push_back(eme);
	}
	//其他
	SetWindowPos(NULL, 0, 0, X_MAX + 200, Y_MAX + 40+10, SWP_NOMOVE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFightingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFightingDlg::OnPaint()
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

	//CMemDC memDC(*GetDC(), this);
	//CDC* pDC = &memDC.GetDC();





	int i = 0, j = 0;



	CRect   rc;
	//CWnd* pWnd = GetDlgItem(IDC_IMAGE_BOX);
	//pWnd->GetClientRect();
	GetClientRect(&rc);// 获取客户区  

	CDC* pDC = GetDC();//这里通过在CmemDC中绘制，以解决闪烁问题
	CMemDC dcMem(*pDC, this);
	CDC& dc = dcMem.GetDC();
	
	//////敌方坦克//////

	if (myTank.m_life > 0)
	{
		if (eneTank.size() < 3)
		{
			EneTank eme;
			eneTank.push_back(eme);
		}

		for (i = 0; i < eneTank.size(); i++)
		{
			eneTank[i].AutoMove();
			if (eneTank[i].fireClock())
				enevecShell.push_back(eneTank[i].fire());
		}

		/////////////////////////绘制部分/////////////////////////////

			//////绘制背景//////
		dc.FillSolidRect(&Range, RGB(0, 0, 0));

		for (i = 0; i < vecWall.size(); i++)
			if (vecWall[i].type == WATER
				|| vecWall[i].type == BASE
				|| vecWall[i].type == WOOD)
			{
				vecWall[i].Show(&dc);
			}

		//////炮弹//////
		for (i = 0; i < myvecShell.size(); i++)
			myvecShell[i].Show(&dc);
		for (i = 0; i < enevecShell.size(); i++)
			enevecShell[i].Show(&dc);

		//////敌方坦克//////
		for (i = 0; i < eneTank.size(); i++)
			eneTank[i].Show(&dc);

		//////玩家坦克//////
		myTank.Show(&dc);


		for (i = 0; i < vecWall.size(); i++)
		{
			if (vecWall[i].type != WATER 
				&& vecWall[i].type != BASE
				&& vecWall[i].type!=WOOD)
			{
				vecWall[i].Show(&dc);
			}
		}
		//生命显示
		CRect rct(CPoint(620, 0), CPoint(620 + 30, 0 + 30));
		myTank.show_life(&dc,rct);

	}
	else
	{
		work = false;
		str.Format(L"游戏结束!您消灭了%d个敌人", EneTank::total_num);
		dc.TextOut(250, 270,str);
		eneTank.clear();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFightingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFightingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		InvalidateRect(Range, true); 
		break;
	case 2:
		if(EneTank::level<9)
		EneTank::level++;
		break;
	case 3:
		myTank.myMove();
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CFightingDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	static bool used = false;
	CRect rct;
	CPoint pt;
	if (myTank.m_life == 0)
		return;
	switch (nChar)
	{
	case 'w':
	case 'W':
		myTank.setDir(0);
		break;	
	case 's':
	case 'S':
		myTank.setDir(1);
		break;
	case 'a':
	case 'A':
		myTank.setDir(2);
		break;
	case 'd':
	case 'D':
		myTank.setDir(3);
		break;
	case 'p':
	case 'P':
		if (myTank.m_life < 30)
		{
			myTank.m_life += 5;
		}
		if (!used)
		{
			vecWall.push_back(wall(CPoint(200, 400), METAL));
			vecWall.push_back(wall(CPoint(250, 400), METAL));
			vecWall.push_back(wall(CPoint(300, 400), METAL));
			vecWall.push_back(wall(CPoint(200, 450), METAL));
			vecWall.push_back(wall(CPoint(300, 450), METAL));
			shell_num = 1000;
			myTank.setSpeed(10);
			used = true;
		}
		break;
	case 'k':
	case 'K':
		myTank.gameover();
		break;
	case VK_SPACE:
		if (myvecShell.size() < shell_num)
		{
			myvecShell.push_back(myTank.Fire());
		}
		break;
	default:
		break;
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}





BOOL CFightingDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return false;
}


void CFightingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	work = false;
}




BOOL CFightingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CFightingDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (myTank.m_life == 0)
		return;
	switch (nChar)
	{
	case 'w':
	case 'W':
	case 's':
	case 'S':
	case 'a':
	case 'A':
	case 'd':
	case 'D':
		myTank.setDir(5);
		break;
	}
	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}

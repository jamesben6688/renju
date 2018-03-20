#include "stdafx.h"
#include "renju.h"
#include "renjuDlg.h"
#include "newgame.h"
#include "MoveGenerator.h"
#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRenjuDlg::CRenjuDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRenjuDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRenjuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenjuDlg)
	DDX_Control(pDX, IDC_THINKPRG, m_ThinkProgress);
	DDX_Control(pDX, IDC_NODECOUNT, m_OutputInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRenjuDlg, CDialog)
	//{{AFX_MSG_MAP(CRenjuDlg)
	ON_WM_PAINT()		//调用paint函数绘制界面
	ON_WM_QUERYDRAGICON()	//设置图标
	ON_WM_LBUTTONDOWN()		//等待左键按下并调用处理函数
	//ON_BN_CLICKED(IDC_NEWGAME, OnNewgame)	//New Game函数的调用
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &CRenjuDlg::OnBnClickedMfcmenubutton1)
	ON_COMMAND(ID_FILE_ABOUT32772, &CRenjuDlg::OnFileAbout32772)
	ON_COMMAND(ID_FILE_ABOUT, &CRenjuDlg::OnFileAbout)
	ON_STN_CLICKED(IDC_NODECOUNT, &CRenjuDlg::OnStnClickedNodecount)
END_MESSAGE_MAP()


BOOL CRenjuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	pMenu = GetSystemMenu(FALSE);//获取系统菜单的指针
	pMenu->AppendMenu(MF_SEPARATOR);
	pMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, "ABOUT");
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	memset(m_RenjuBoard, NOSTONE, GRID_COUNT);//初始化棋盘

	CMoveGenerator *pMG;
	CEveluation *pEvel;

	m_nUserStoneColor = BLACK;

	m_pSE = new CNegaScout_TT_HH;
	pMG = new CMoveGenerator;
	pEvel = new CEveluation;
	m_pSE->SetThinkProgress(&m_ThinkProgress);

	//设定搜索深度
	m_pSE->SetSearchDepth(3);
	m_pSE->SetMoveGenerator(pMG);
	m_pSE->SetEveluator(pEvel);
	return TRUE;  // returnTRUE  unless you set the focus to a control
}


//绘制棋盘棋子
void CRenjuDlg::OnPaint()
{

	CRect   rect;
	CPaintDC   dc(this);
	GetClientRect(rect);
	rect.bottom = 520;
	dc.FillSolidRect(rect, RGB(215,128,86));   //设置为绿色背景
	
	//画横轴；
	for (int i = 0; i < GRID_NUM; i++)
	{
		dc.MoveTo(BOARD_POS_X, BOARD_POS_Y + i*BOARD_WIDTH);
		dc.LineTo(BOARD_POS_X + 14 * BOARD_WIDTH, BOARD_POS_Y + i*BOARD_WIDTH);
	}

	//画竖轴；
	for (int j = 0; j < GRID_NUM; j++)
	{
		dc.MoveTo(BOARD_POS_X + j*BOARD_WIDTH, BOARD_POS_Y);
		dc.LineTo(BOARD_POS_X + j*BOARD_WIDTH, BOARD_POS_Y + 14 * BOARD_WIDTH);
	}

	////画横坐标数字
	//char ch[2];
	//ch[0] = 'a'-1;
	//ch[1] = 0;
	//dc.SetBkMode(TRANSPARENT);
	//for (int i = 0; i < GRID_NUM; i++)
	//{
	//	ch[0] ++;
	//	dc.TextOut(BOARD_POS_X + i*BOARD_WIDTH - 5, BOARD_POS_Y + 14 * BOARD_WIDTH + 20, ch);
	//}

	////画纵坐标数字
	//ch[0] = 'O';
	//ch[1] = 0;

	//for (int i = 0; i < GRID_NUM; i++)
	//{
	//	dc.TextOut(BOARD_POS_X - BOARD_WIDTH, BOARD_POS_Y + i*BOARD_WIDTH - 10, ch);
	//	ch[0] -= 1;
	//}

	//画标记点，选择白色
	dc.SelectStockObject(BLACK_BRUSH);
	dc.Ellipse(BOARD_POS_X + 3 * BOARD_WIDTH - 3, BOARD_POS_Y + 3 * BOARD_WIDTH - 3, BOARD_POS_X + 3 * BOARD_WIDTH + 3, BOARD_POS_Y + 3 * BOARD_WIDTH + 3);
	dc.Ellipse(BOARD_POS_X + 11 * BOARD_WIDTH - 3, BOARD_POS_Y + 3 * BOARD_WIDTH - 3, BOARD_POS_X + 11 * BOARD_WIDTH + 3, BOARD_POS_Y + 3 * BOARD_WIDTH + 3);
	dc.Ellipse(BOARD_POS_X + 7 * BOARD_WIDTH - 3, BOARD_POS_Y + 7 * BOARD_WIDTH - 3, BOARD_POS_X + 7 * BOARD_WIDTH + 3, BOARD_POS_Y + 7 * BOARD_WIDTH + 3);
	dc.Ellipse(BOARD_POS_X + 3 * BOARD_WIDTH - 3, BOARD_POS_Y + 11 * BOARD_WIDTH - 3, BOARD_POS_X + 3 * BOARD_WIDTH + 3, BOARD_POS_Y + 11 * BOARD_WIDTH + 3);
	dc.Ellipse(BOARD_POS_X + 11 * BOARD_WIDTH - 3, BOARD_POS_Y + 11 * BOARD_WIDTH - 3, BOARD_POS_X + 11 * BOARD_WIDTH + 3, BOARD_POS_Y + 11 * BOARD_WIDTH + 3);

	//画棋子
	int x;
	int y;
	for (x = 0; x < GRID_NUM; x++)
	{
		for (y = 0; y < GRID_NUM; y++)
		{
			if (m_RenjuBoard[y][x] == BLACK)
			{
				//设置黑色棋子
				dc.SelectStockObject(BLACK_BRUSH);
				dc.Ellipse(BOARD_POS_X + x*BOARD_WIDTH - BOARD_WIDTH / 2 + 1, BOARD_POS_Y + y*BOARD_WIDTH - BOARD_WIDTH / 2 + 1, BOARD_POS_X + x*BOARD_WIDTH + BOARD_WIDTH / 2 - 1, BOARD_POS_Y + y*BOARD_WIDTH + BOARD_WIDTH / 2 - 1);
			}
			else if (m_RenjuBoard[y][x] == WHITE)
			{
				//设置白色棋子
				dc.SelectStockObject(WHITE_BRUSH);
				dc.Ellipse(BOARD_POS_X + x*BOARD_WIDTH - BOARD_WIDTH / 2 + 1, BOARD_POS_Y + y*BOARD_WIDTH - BOARD_WIDTH / 2 + 1, BOARD_POS_X + x*BOARD_WIDTH + BOARD_WIDTH / 2 - 1, BOARD_POS_Y + y*BOARD_WIDTH + BOARD_WIDTH / 2 - 1);
			}
		}
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRenjuDlg::OnQueryDragIcon()
{
	return(HCURSOR)m_hIcon;
}

extern int count;//记录估值次数

//左键按下的响应函数
void CRenjuDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//this code does not content in books
	if (m_pSE->GameOver())
		return;

	CDC *pDC;
	pDC = GetDC();
	CRect rect(BOARD_POS_X - 5, BOARD_POS_Y - 5, BOARD_POS_X + 14 * BOARD_WIDTH + 5, BOARD_POS_Y + 14 * BOARD_WIDTH + 5);
	int i = (point.x + BOARD_WIDTH / 2 - 1 - BOARD_POS_X) / BOARD_WIDTH;
	int j = (point.y + BOARD_WIDTH / 2 - 1 - BOARD_POS_Y) / BOARD_WIDTH;
	if (IsValidPos(i, j) && m_RenjuBoard[j][i] == (BYTE)NOSTONE)
	{

		m_RenjuBoard[j][i] = m_nUserStoneColor;
		switch (m_nUserStoneColor)
		{
		case BLACK:
			pDC->SelectStockObject(BLACK_BRUSH);
			break;
		case WHITE:
			pDC->SelectStockObject(WHITE_BRUSH);
			break;
		}
		pDC->Ellipse(BOARD_POS_X + i*BOARD_WIDTH - BOARD_WIDTH / 2 + 1, BOARD_POS_Y + j*BOARD_WIDTH - BOARD_WIDTH / 2 + 1, BOARD_POS_X + i*BOARD_WIDTH + BOARD_WIDTH / 2 - 1, BOARD_POS_Y + j*BOARD_WIDTH + BOARD_WIDTH / 2 - 1);
		m_ThinkProgress.SetPos(0);
		m_OutputInfo.SetWindowText("Computer is thinking about how to move,Please wait... ");
		int timecount;
		timecount = GetTickCount();
		count = 0;
		//设置棋局中棋子的颜色
		if (m_nUserStoneColor == BLACK)
			InvertRenjuBroad();
		m_pSE->SearchAGoodMove(m_RenjuBoard, BLACK);//!m_nUserStoneColor);	
		if (m_nUserStoneColor == BLACK)
			InvertRenjuBroad();
		CString sNodeCount;
		sNodeCount.Format(" Cost %d ms. %d Nodes were eveluated.", GetTickCount() - timecount, count);
		m_OutputInfo.SetWindowText(sNodeCount);

	}
	InvalidateRect(NULL, TRUE);
	UpdateWindow();//更新窗口
	CDialog::OnLButtonDown(nFlags, point);
}

////响应New Game按钮的消息
//void CRenjuDlg::OnNewgame()
//{
//	// TODO: Add your control notification handler code here
//	CNewGame NewGame;
//
//	if (NewGame.DoModal() == IDOK)
//	{
//		m_pSE->SetSearchDepth(NewGame.GetSelectedPly());
//
//
//		m_nUserStoneColor = NewGame.GetStoneColor();
//		memset(m_RenjuBoard, NOSTONE, GRID_COUNT);//初始化棋盘
//		if (m_nUserStoneColor == WHITE)
//			m_RenjuBoard[7][7] = BLACK;//Black First
//
//		InvalidateRect(NULL, TRUE);
//		UpdateWindow();
//
//		//this code does not content in books
//		m_pSE->BeginGame();
//		//this code does not content in books
//	}
//	else
//		return;
//}

//反转棋子
void CRenjuDlg::InvertRenjuBroad()
{
	int x, y;
	for (x = 0; x < GRID_NUM; x++)
	{
		for (y = 0; y < GRID_NUM; y++)
		{
			switch (m_RenjuBoard[y][x])
			{
			case WHITE:
				m_RenjuBoard[y][x] = BLACK;
				break;
			case BLACK:
				m_RenjuBoard[y][x] = WHITE;
				break;
			default:
				break;
			}
		}
	}
}


void CRenjuDlg::OnFileAbout32772()
{
	CAboutDlg box;
	box.DoModal();
}


void CRenjuDlg::OnFileAbout()
{
	// TODO:  在此添加命令处理程序代码
	CNewGame NewGame;

	if (NewGame.DoModal() == IDOK)
	{
		m_pSE->SetSearchDepth(NewGame.GetSelectedPly());
		m_nUserStoneColor = NewGame.GetStoneColor();
		memset(m_RenjuBoard, NOSTONE, GRID_COUNT);//初始化棋盘
		if (m_nUserStoneColor == WHITE)
			m_RenjuBoard[7][7] = BLACK;//Black First

		InvalidateRect(NULL, TRUE);
		UpdateWindow();
		m_pSE->BeginGame();
	}
	else
		return;
}


void CRenjuDlg::OnStnClickedNodecount()
{
	// TODO:  在此添加控件通知处理程序代码
}

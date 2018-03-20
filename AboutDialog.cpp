#include "stdafx.h"
#include "renju.h"
#include "AboutDialog.h"
#include "afxdialogex.h"
#include "NewGame.h"


// CAboutDialog 对话框

IMPLEMENT_DYNAMIC(CAboutDialog, CDialogEx)

CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAboutDialog::IDD, pParent)
{

}

CAboutDialog::~CAboutDialog()
{
}

void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialogEx)
	ON_COMMAND(ID_FILE_ABOUT, &CAboutDialog::OnFileAbout)
END_MESSAGE_MAP()


// CAboutDialog 消息处理程序


void CAboutDialog::OnFileAbout()
{
	// TODO:  在此添加命令处理程序代码
	// TODO: Add your control notification handler code here
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

		//this code does not content in books
		m_pSE->BeginGame();
		//this code does not content in books
	}
	else
		return;
}

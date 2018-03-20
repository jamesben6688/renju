#include "stdafx.h"
#include "renju.h"
#include "NewGame.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CNewGame::CNewGame(CWnd* pParent /*=NULL*/)
: CDialog(CNewGame::IDD, pParent)
{
}


void CNewGame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGame)
	DDX_Control(pDX, IDC_PLY, m_SetPly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGame, CDialog)
	//{{AFX_MSG_MAP(CNewGame)
	ON_BN_CLICKED(IDC_BLACKSTONE, OnBlackstone)
	ON_BN_CLICKED(IDC_WHITESTONE, OnWhitestone)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_ABOUT, &CNewGame::OnFileAbout)
	ON_BN_CLICKED(IDOK, &CNewGame::OnBnClickedOk)
END_MESSAGE_MAP()


void CNewGame::OnOK()
{
	// TODO: Add extra validation here
	m_nSelectedPly = m_SetPly.GetPos();
	CDialog::OnOK();
}

BOOL CNewGame::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	((CButton*)GetDlgItem(IDC_BLACKSTONE))->SetCheck(TRUE);
	m_nStoneColor = BLACK;
	m_SetPly.SetRange(1, 4);
	m_SetPly.SetPos(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNewGame::OnBlackstone()
{
	// TODO: Add your control notification handler code here
	m_nStoneColor = BLACK;

}

void CNewGame::OnWhitestone()
{
	// TODO: Add your control notification handler code here
	m_nStoneColor = WHITE;

}


void CNewGame::OnFileAbout()
{
	// TODO:  在此添加命令处理程序代码
	CAboutDlg box;
	box.DoModal();
}

void CNewGame::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnOK();
}

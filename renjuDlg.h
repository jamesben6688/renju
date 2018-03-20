#if !defined(AFX_RENJUDLG_H__2B09B234_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_RENJUDLG_H__2B09B234_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "SearchEngine.h"
#include "Resource.h"
#include "NegaScout_TT_HH.h"
typedef struct _movestone
{
	BYTE nRenjuID;
	POINT ptMovePoint;
}MOVESTONE;

class CRenjuDlg : public CDialog
{
// Construction
public:

	CRenjuDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRenjuDlg)
	enum { IDD = IDD_RENJU_DIALOG };
	CProgressCtrl	m_ThinkProgress;
	CStatic	m_OutputInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenjuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void InvertRenjuBroad();
	// Generated message map functions
	//{{AFX_MSG(CRenjuDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM IPara);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnNewgame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BYTE m_RenjuBoard[GRID_NUM][GRID_NUM];
	int m_nUserStoneColor;
	CSearchEngine *m_pSE;
	CMenu* pMenu;
public:
	//afx_msg void OnBnClickedMfcmenubutton1();
	afx_msg void OnFileAbout32772();
	afx_msg void OnFileAbout();
	afx_msg void OnStnClickedNodecount();
};

#endif // !defined(AFX_RENJUDLG_H__2B09B234_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_)

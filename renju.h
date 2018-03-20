#if !defined(AFX_RENJU_H__2B09B232_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_RENJU_H__2B09B232_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "define.h"


class CRenjuApp : public CWinApp
{
public:
	CRenjuApp();

	public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_RENJU_H__2B09B232_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_)

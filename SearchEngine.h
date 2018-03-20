#if !defined(AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MoveGenerator.h"
#include "Eveluation.h"

class CSearchEngine  
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();
	//博弈接口，为当前局面走出下一步
	virtual void SearchAGoodMove(BYTE position[][GRID_NUM], int Type)=0;
	//设定最大搜索深度
	virtual void SetSearchDepth(int nDepth)
	{
		m_nSearchDepth = nDepth;
	};
	//设定搜索引擎
	virtual void SetEveluator(CEveluation *pEval)
	{
		m_pEval = pEval;
	};
	//设定走法产生器
	virtual void SetMoveGenerator(CMoveGenerator *pMG)
	{
		m_pMG = pMG;
	};
	//设定显示思考进度的进度条
	void SetThinkProgress(CProgressCtrl	*pThinkProgress)
	{
		m_pThinkProgress = pThinkProgress;
	};
	//
	BOOL GameOver()
	{
		return m_bGameOver;
	};
	void BeginGame()
	{
		m_bGameOver = FALSE;
	};
protected:
	CProgressCtrl	*m_pThinkProgress;//用以显示思考进度的进度条指针
	virtual BYTE MakeMove(STONEMOVE* move,int type);//此函数用于根据某一走法产生走了之后的棋盘
	virtual void UnMakeMove(STONEMOVE* move);//此函数用于恢复某一走法所产生棋盘为走过之前的
	virtual int IsGameOver(BYTE position[GRID_NUM][GRID_NUM], int nDepth);//此函数用于判断当前局面是否已分出胜负
	BYTE CurPosition[GRID_NUM][GRID_NUM];//搜索时用于当前节点棋盘状态的数组
	STONEMOVE m_cmBestMove;//记录最佳走法的变量
	CMoveGenerator *m_pMG;//走法产生器指针
	CEveluation *m_pEval;//估值核心指针
	int m_nSearchDepth;//最大搜索深度
	int m_nMaxDepth;//当前搜索的最大搜索深度
	BOOL m_bGameOver;
};

#endif // !defined(AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)

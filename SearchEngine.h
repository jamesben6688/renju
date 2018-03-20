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
	//���Ľӿڣ�Ϊ��ǰ�����߳���һ��
	virtual void SearchAGoodMove(BYTE position[][GRID_NUM], int Type)=0;
	//�趨����������
	virtual void SetSearchDepth(int nDepth)
	{
		m_nSearchDepth = nDepth;
	};
	//�趨��������
	virtual void SetEveluator(CEveluation *pEval)
	{
		m_pEval = pEval;
	};
	//�趨�߷�������
	virtual void SetMoveGenerator(CMoveGenerator *pMG)
	{
		m_pMG = pMG;
	};
	//�趨��ʾ˼�����ȵĽ�����
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
	CProgressCtrl	*m_pThinkProgress;//������ʾ˼�����ȵĽ�����ָ��
	virtual BYTE MakeMove(STONEMOVE* move,int type);//�˺������ڸ���ĳһ�߷���������֮�������
	virtual void UnMakeMove(STONEMOVE* move);//�˺������ڻָ�ĳһ�߷�����������Ϊ�߹�֮ǰ��
	virtual int IsGameOver(BYTE position[GRID_NUM][GRID_NUM], int nDepth);//�˺��������жϵ�ǰ�����Ƿ��ѷֳ�ʤ��
	BYTE CurPosition[GRID_NUM][GRID_NUM];//����ʱ���ڵ�ǰ�ڵ�����״̬������
	STONEMOVE m_cmBestMove;//��¼����߷��ı���
	CMoveGenerator *m_pMG;//�߷�������ָ��
	CEveluation *m_pEval;//��ֵ����ָ��
	int m_nSearchDepth;//����������
	int m_nMaxDepth;//��ǰ����������������
	BOOL m_bGameOver;
};

#endif // !defined(AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)

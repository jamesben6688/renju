#include "stdafx.h"
#include "renju.h"
#include "NegaScout_TT_HH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CNegaScout_TT_HH::CNegaScout_TT_HH()
{
	ResetHistoryTable();
	m_pThinkProgress = NULL;
	m_bGameOver = FALSE;
}

CNegaScout_TT_HH::~CNegaScout_TT_HH()
{

}

void CNegaScout_TT_HH::SearchAGoodMove(BYTE position[GRID_NUM][GRID_NUM], int Type)
{
	int Score;
	memcpy(CurPosition, position, GRID_COUNT);
	m_nMaxDepth = m_nSearchDepth;	//设定搜索深度
	CalculateInitHashKey(CurPosition);	//计算初始棋盘的hash值
	ResetHistoryTable();			//初始化历史记录表
	//使用NegaScout搜索，返回最大分数
	Score = NegaScout(m_nMaxDepth, -20000, 20000);
	//判断游戏是否结束
	if (CurPosition[m_cmBestMove.StonePos.y][m_cmBestMove.StonePos.x] != NOSTONE)
	{
		//玩家获胜
		if (Score < 0)
			MessageBox(NULL, "Game Over! You Win!!!:(", "Renju", MB_OK);
		//AI获胜
		else
			MessageBox(NULL, "Game Over! You Loss...:)", "Renju", MB_OK);

		m_bGameOver = TRUE;
	}
	//走出最佳走法
	MakeMove(&m_cmBestMove, Type);
	//传出走过了的棋盘
	memcpy(position, CurPosition, GRID_COUNT);
}

//核心搜索函数
int CNegaScout_TT_HH::NegaScout(int depth, int alpha, int beta)
{
	int Count, i;
	BYTE type;
	int a, b, t;
	int side;
	int score;

	if (depth > 0)
	{
		i = IsGameOver(CurPosition, depth);
		if (i != 0)
			return i;	//已经分出胜负，返回极值
	}
	side = (m_nMaxDepth - depth) % 2;//计算当前节点的类型

	score = LookUpHashTable(alpha, beta, depth, side); //查找置换表看是否有当前节点的有效数据
	if (score != 66666)
		return score;//命中，直接返回

	if (depth <= 0)	//叶子节点取估值
	{
		score = m_pEval->Eveluate(CurPosition, side);//取出估值
		EnterHashTable(exact, score, depth, side);//将估值存入置换表
		return score;//返回估值
	}


	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);//产生下一步可能的走法
	//根节点设定进度条
	if (depth == m_nMaxDepth)
	{
		m_pThinkProgress->SetRange(0, Count);
		m_pThinkProgress->SetStep(1);
	}

	//取所有走法的历史得分
	for (i = 0; i < Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
	}

	//将走法按历史得分排序
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);

	int bestmove = -1;

	a = alpha;
	b = beta;
	int eval_is_exact = 0;
	for (i = 0; i < Count; i++)
	{
		if (depth == m_nMaxDepth)
			m_pThinkProgress->StepIt();//进度条走动
		//产生子节点
		type = MakeMove(&m_pMG->m_MoveList[depth][i], side);
		//产生子节点hash值
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);

		//递归搜索子节点，对第一个节点是全窗口，其后是空窗探测
		t = -NegaScout(depth - 1, -b, -a);

		if (t > a && t < beta && i > 0)
		{
			a = -NegaScout(depth - 1, -beta, -t);     /* re-search */
			eval_is_exact = 1;	//设数据类型为精确值
			if (depth == m_nMaxDepth)	//保留最佳走法
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			bestmove = i;		//记住最佳走法的位置
		}
		//恢复当前结点的hash值
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);
		//撤销子节点
		UnMakeMove(&m_pMG->m_MoveList[depth][i]);
		if (a < t)
		{
			//第一次搜索命中
			eval_is_exact = 1;
			a = t;
			//保留最佳走法
			if (depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}
		if (a >= beta)
		{
			//将下边界存入置换表
			EnterHashTable(lower_bound, a, depth, side);
			//将当前走法汇入历史记录
			EnterHistoryScore(&m_pMG->m_MoveList[depth][i], depth);
			return a;	//剪枝
		}
		b = a + 1;                      /* set new null window */
	}

	//最佳走法汇入历史记录
	if (bestmove != -1)
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);
	if (eval_is_exact)
		EnterHashTable(exact, a, depth, side);
	else
		EnterHashTable(upper_bound, a, depth, side);
	return a;		//返回最佳值
}
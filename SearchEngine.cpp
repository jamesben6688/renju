#include "stdafx.h"
#include "renju.h"
#include "SearchEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CSearchEngine::CSearchEngine()
{
}

CSearchEngine::~CSearchEngine()
{
	delete m_pMG;
	delete m_pEval;
}

//根据传入的走法改变棋盘
//move 是要进行的走法
//type 是要下的棋子类型
BYTE CSearchEngine::MakeMove(STONEMOVE* move, int type)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = type;
	return 0;
}

//根据传入的走法恢复棋盘
//move 是要恢复的走法
void CSearchEngine::UnMakeMove(STONEMOVE* move)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = NOSTONE;
}

//用以检查给定局面游戏是否结束
//未结束返回0，否则返回极大值/极小值
int CSearchEngine::IsGameOver(BYTE position[][GRID_NUM], int nDepth)
{
	int score, i;
	//计算要下子的棋子颜色
	i = (m_nMaxDepth - nDepth) % 2;
	//调用估值函数
	score = m_pEval->Eveluate(position, i);
	//如果估值函数返回极值，给定局面游戏结束
	if (abs(score) >= 9990)
		return score; //返回极值
	return 0;//返回未结束
}

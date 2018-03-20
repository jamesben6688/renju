#include "stdafx.h"
#include "renju.h"
#include "MoveGenerator.h"
#include "HistoryHeuristic.h"
#include "Eveluation.h"

#ifdef _DEBUG
#undef THIS_FILE
static BYTE THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CMoveGenerator::CMoveGenerator()
{

}

CMoveGenerator::~CMoveGenerator()
{

}


// 在 m_MoveList 中插入一个走法
// nToX 是目标位置横坐标
// nToY 是目标位置纵坐标
// nPly 是此走法所在的层次
int CMoveGenerator::AddMove(int nToX, int nToY, int nPly)
{
	m_MoveList[nPly][m_nMoveCount].StonePos.x = nToX;
	m_MoveList[nPly][m_nMoveCount].StonePos.y = nToY;
	m_nMoveCount++;
	//使用位置价值表评估当前走法的价值
	m_MoveList[nPly][m_nMoveCount].Score = PosValue[nToY][nToX];
	return m_nMoveCount;
}

//用以产生局面 position 中所有可能的走法
//position 是包含了所有棋子位置信息的二维数组
//nPly指明当前搜索的层数，每层将走法存在不同的位置，以免覆盖
//nSide指明产生哪一方的走法，WHITE为白方，BLACK是黑方
int CMoveGenerator::CreatePossibleMove(BYTE position[GRID_NUM][GRID_NUM], int nPly, int nSide)
{
	int	i, j;
	m_nMoveCount = 0;
	for (i = 0; i < GRID_NUM; i++)
	for (j = 0; j < GRID_NUM; j++)
	{
		if (position[i][j] == (BYTE)NOSTONE)
		{
			AddMove(j, i, nPly);
		}
	}
	//使用历史启发类中的静态归并排序函数对走法队列进行排序
	//这是为了提高剪枝效率
	CHistoryHeuristic::MergeSort(m_MoveList[nPly], m_nMoveCount, 0);
	return m_nMoveCount;//返回合法走法的数量
}
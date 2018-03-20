#include "stdafx.h"
#include "renju.h"
#include "Eveluation.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//静态局面函数，越往边缘分数越低
int PosValue[GRID_NUM][GRID_NUM] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0 },
	{ 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0 },
	{ 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};


CEveluation::CEveluation()
{
}

CEveluation::~CEveluation()
{

}
//全局变量，统计估值函数的执行遍数
int count = 0;

/*
*position是要估值的棋盘
*bIsBlackTurn 轮到谁走棋
*/
int CEveluation::Eveluate(BYTE position[][GRID_NUM], BOOL bIsBlackTurn)
{
	int i, j, k;
	BYTE nStoneType;
	count++;
	//清空上一次棋子的分析结果
	memset(TypeRecord, TOBEANALSIS, GRID_COUNT * 4 * 4);
	memset(TypeCount, 0, 40 * 4);

	//对4个方向进行分析
	for (i = 0; i < GRID_NUM; i++)
	for (j = 0; j < GRID_NUM; j++)
	{
		if (position[i][j] != NOSTONE)
		{
			if (TypeRecord[i][j][0] == TOBEANALSIS)
				AnalysisHorizon(position, i, j);
			if (TypeRecord[i][j][1] == TOBEANALSIS)
				AnalysisVertical(position, i, j);
			if (TypeRecord[i][j][2] == TOBEANALSIS)
				AnalysisLeft(position, i, j);
			if (TypeRecord[i][j][3] == TOBEANALSIS)
				AnalysisRight(position, i, j);
		}
	}

	for (i = 0; i < GRID_NUM; i++)
	for (j = 0; j < GRID_NUM; j++)
	for (k = 0; k < 4; k++)
	{
		nStoneType = position[i][j];
		if (nStoneType != NOSTONE)
		{
			switch (TypeRecord[i][j][k])
			{
			case FIVE:
				TypeCount[nStoneType][FIVE] ++;//五子相连
				break;
			case FOUR:
				TypeCount[nStoneType][FOUR] ++;//活四
				break;
			case SFOUR:
				TypeCount[nStoneType][SFOUR] ++;//冲四
				break;
			case THREE:
				TypeCount[nStoneType][THREE] ++;//活三
				break;
			case STHREE:
				TypeCount[nStoneType][STHREE] ++;//眠三
				break;
			case TWO:
				TypeCount[nStoneType][TWO] ++;//活二
				break;
			case STWO:
				TypeCount[nStoneType][STWO] ++;//眠二
				break;
			default:
				break;

			}
		}

	}
	//轮到黑子(玩家)走棋
	if (bIsBlackTurn)
	{
		//玩家获胜，返回极值
		if (TypeCount[BLACK][FIVE])
			return -9999;
		//AI获胜，返回极值
		if (TypeCount[WHITE][FIVE])
			return 9999;
	}
	//白子走棋(AI)
	else
	{
		//同上
		if (TypeCount[BLACK][FIVE])
			return 9999;
		if (TypeCount[WHITE][FIVE])
			return -9999;
	}
	//AI活四数量加一
	if (TypeCount[WHITE][SFOUR] > 1)
		TypeCount[WHITE][FOUR]++;
	//玩家活四数量加一
	if (TypeCount[BLACK][SFOUR] > 1)
		TypeCount[BLACK][FOUR]++;


	int WValue = 0, BValue = 0;
	//轮到玩家下棋
	if (bIsBlackTurn)
	{

		if (TypeCount[WHITE][FOUR])//白胜
		{
			return 9990;
		}

		if (TypeCount[WHITE][SFOUR])//白胜
		{
			return 9980;
		}

		if (TypeCount[BLACK][FOUR])//黑胜
		{
			return -9970;
		}

		//玩家已经形成眠四或者活三，AI输
		if (TypeCount[BLACK][SFOUR] && TypeCount[BLACK][THREE])//黑胜
		{
			return -9960;
		}
		//AI形成活三或者眠四，此时返回一个较大的分数
		if (TypeCount[WHITE][THREE] && TypeCount[BLACK][SFOUR] == 0)
		{
			return 9950;
		}
		//玩家形成了2个以上的活三，AI未形成眠四，活三，眠三，玩家占优
		if (TypeCount[BLACK][THREE] > 1 &&
			TypeCount[WHITE][SFOUR] == 0 &&
			TypeCount[WHITE][THREE] == 0 &&
			TypeCount[WHITE][STHREE] == 0)
		{
			return -9940;
		}

		//AI形成了2个以上的活三
		if (TypeCount[WHITE][THREE] > 1)
			WValue += 2000;
		else{
			//一个活三
			if (TypeCount[WHITE][THREE])
				WValue += 200;
		}
		//玩家形成了2个以上的活三
		if (TypeCount[BLACK][THREE] > 1)
			BValue += 500;
		else{
			//玩家形成了一个活三
			if (TypeCount[BLACK][THREE])
				BValue += 100;
		}

		//AI形成了1个以上的眠三
		if (TypeCount[WHITE][STHREE])
			WValue += TypeCount[WHITE][STHREE] * 10;
		//玩家形成了1个以上的眠三
		if (TypeCount[BLACK][STHREE])
			BValue += TypeCount[BLACK][STHREE] * 10;
		//AI形成了一个以上的活二
		if (TypeCount[WHITE][TWO])
			WValue += TypeCount[WHITE][TWO] * 4;
		//玩家形成了1个以上的活二
		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][TWO] * 4;
		//AI形成了一个以上的眠二
		if (TypeCount[WHITE][STWO])
			WValue += TypeCount[WHITE][STWO];
		//玩家形成了1个以上的眠二
		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][STWO];
	}
	//轮到AI下棋
	else
	{
		if (TypeCount[BLACK][FOUR])
		{
			return 9990;
		}

		if (TypeCount[BLACK][SFOUR])
		{
			return 9980;
		}

		if (TypeCount[WHITE][FOUR])
		{
			return -9970;
		}

		if (TypeCount[WHITE][SFOUR] && TypeCount[WHITE][THREE])
		{
			return -9960;
		}

		if (TypeCount[BLACK][THREE] && TypeCount[WHITE][SFOUR] == 0)
		{
			return 9950;
		}

		if (TypeCount[WHITE][THREE] > 1 &&
			TypeCount[BLACK][SFOUR] == 0 &&
			TypeCount[BLACK][THREE] == 0 &&
			TypeCount[BLACK][STHREE] == 0)
		{
			return -9940;
		}

		if (TypeCount[BLACK][THREE] > 1)
			BValue += 2000;
		else{
			if (TypeCount[BLACK][THREE])
				BValue += 200;
		}

		if (TypeCount[WHITE][THREE] > 1)
			WValue += 500;
		else{
			if (TypeCount[WHITE][THREE])
				WValue += 100;
		}



		if (TypeCount[WHITE][STHREE])
			WValue += TypeCount[WHITE][STHREE] * 10;

		if (TypeCount[BLACK][STHREE])
			BValue += TypeCount[BLACK][STHREE] * 10;

		if (TypeCount[WHITE][TWO])
			WValue += TypeCount[WHITE][TWO] * 4;

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][TWO] * 4;

		if (TypeCount[WHITE][STWO])
			WValue += TypeCount[WHITE][STWO];

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][STWO];

	}

	//加上位置分数，越往外分数越低
	for (i = 0; i < GRID_NUM; i++)
	for (j = 0; j < GRID_NUM; j++)
	{
		nStoneType = position[i][j];
		if (nStoneType != NOSTONE)
		{
			if (nStoneType == BLACK)
				BValue += PosValue[i][j];
			else
				WValue += PosValue[i][j];
		}
	}

	//返回总分数估值
	if (!bIsBlackTurn)
		return BValue - WValue;
	else
		return WValue - BValue;

}


int CEveluation::AnalysisHorizon(BYTE position[][GRID_NUM], int i, int j)
{
	//直接调用直线分析函数
	AnalysisLine(position[i], 15, j);
	//拿到分析结果，0表示水平方向
	for (int s = 0; s < 15; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[i][s][0] = m_LineRecord[s];

	}
	return TypeRecord[i][j][0];

}


int CEveluation::AnalysisVertical(BYTE position[][GRID_NUM], int i, int j)
{
	BYTE tempArray[GRID_NUM];

	//将一列的旗型转入一维数组
	for (int k = 0; k < GRID_NUM; k++)
		tempArray[k] = position[k][j];
	AnalysisLine(tempArray, GRID_NUM, i);
	//得到分析结果
	for (int s = 0; s < GRID_NUM; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[s][j][1] = m_LineRecord[s];

	}
	return TypeRecord[i][j][1];
}

int CEveluation::AnalysisLeft(BYTE position[][GRID_NUM], int i, int j)
{
	BYTE tempArray[GRID_NUM];
	int x, y;
	int k;
	if (i < j)
	{
		y = 0;
		x = j - i;
	}
	else
	{
		x = 0;
		y = i - j;
	}

	//将左斜45度的旗形转入一维数组
	for (k = 0; k < GRID_NUM; k++)
	{
		if (x + k > 14 || y + k > 14)
			break;
		tempArray[k] = position[y + k][x + k];
	}
	AnalysisLine(tempArray, k, j - x);
	//返回分析结果
	for (int s = 0; s < k; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y + s][x + s][2] = m_LineRecord[s];

	}
	return TypeRecord[i][j][2];
}

int CEveluation::AnalysisRight(BYTE position[][GRID_NUM], int i, int j)
{
	BYTE tempArray[GRID_NUM];
	int x, y, realnum;
	int k;
	if (14 - i < j)
	{
		y = 14;
		x = j - 14 + i;
		realnum = 14 - i;
	}
	else
	{
		x = 0;
		y = i + j;
		realnum = j;
	}

	//将右斜45度的棋型转入一维数组
	for (k = 0; k < GRID_NUM; k++)
	{
		if (x + k > 14 || y - k < 0)
			break;
		tempArray[k] = position[y - k][x + k];
	}
	AnalysisLine(tempArray, k, j - x);
	//返回分析结果
	for (int s = 0; s < k; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y - s][x + s][3] = m_LineRecord[s];

	}
	return TypeRecord[i][j][3];
}

/**
*分析给定一维数组中的五四三二一等棋形的数量
*position 一维数组的头部
*GridNum 数组的长度，15
*StonePos 待分析的棋子的位置
*/
int CEveluation::AnalysisLine(BYTE *position, int GridNum, int StonePos)
{
	BYTE StoneType;
	BYTE AnalyLine[30];
	int nAnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;
	//数组长度不能小于5
	if (GridNum < 5)
	{
		memset(m_LineRecord, ANALSISED, GridNum);
		return 0;
	}

	nAnalyPos = StonePos;
	memset(m_LineRecord, TOBEANALSIS, 30);
	memset(AnalyLine, 0x0F, 30);
	//传入的数组复制到AnalyLine
	memcpy(&AnalyLine, position, GridNum);

	GridNum--;

	StoneType = AnalyLine[nAnalyPos];
	LeftEdge = nAnalyPos;
	RightEdge = nAnalyPos;
	//计算连续棋子的左边界
	while (LeftEdge > 0)
	{
		//左边没有棋子
		if (AnalyLine[LeftEdge - 1] != StoneType)
			break;
		LeftEdge--;
	}
	//计算连续棋子的右边界
	while (RightEdge < GridNum)
	{
		//右边无子
		if (AnalyLine[RightEdge + 1] != StoneType)
			break;
		RightEdge++;
	}

	LeftRange = LeftEdge;
	RightRange = RightEdge;

	//计算棋子可下的范围
	while (LeftRange > 0)
	{
		//StoneType是待分析的位置的棋子类型
		//从连续左边界再往左，直到碰到非棋子
		if (AnalyLine[LeftRange - 1] == !StoneType)
			break;
		LeftRange--;
	}
	while (RightRange < GridNum)
	{
		if (AnalyLine[RightRange + 1] == !StoneType)
			break;
		RightRange++;
	}

	if (RightRange - LeftRange < 4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			m_LineRecord[k] = ANALSISED;
		return FALSE;
	}

	//将连续区域设为分析过的，防止重复分析此一区域
	for (int k = LeftEdge; k <= RightEdge; k++)
		m_LineRecord[k] = ANALSISED;

	if (RightEdge - LeftEdge > 3)//五连
	{
		m_LineRecord[nAnalyPos] = FIVE;
		return FIVE;
	}

	if (RightEdge - LeftEdge == 3)//4连
	{
		BOOL Leftfour = FALSE;
		if (LeftEdge > 0)
		{
			if (AnalyLine[LeftEdge - 1] == NOSTONE)
				Leftfour = TRUE;
		}

		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge + 1] == NOSTONE)
			{
				//活四
				if (Leftfour == TRUE)
					m_LineRecord[nAnalyPos] = FOUR;
				else
					//眠四
					m_LineRecord[nAnalyPos] = SFOUR;
			}
			else
			{
				if (Leftfour == TRUE)
					m_LineRecord[nAnalyPos] = SFOUR;
			}

		}
		else
		{
			if (Leftfour = TRUE)
				m_LineRecord[nAnalyPos] = SFOUR;
		}

		return m_LineRecord[nAnalyPos];
	}



	if (RightEdge - LeftEdge == 2)//3连
	{
		BOOL LeftThree = FALSE;
		if (LeftEdge > 0)
		{

			if (AnalyLine[LeftEdge - 1] == NOSTONE)
			{
				if (LeftEdge > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					m_LineRecord[LeftEdge] = SFOUR;
					m_LineRecord[LeftEdge - 2] = ANALSISED;
				}
				else
					LeftThree = TRUE;
			}
			else
			{
				if (RightEdge == GridNum || AnalyLine[RightEdge + 1] != NOSTONE)
					return FALSE;
			}
		}

		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge + 1] == NOSTONE)
			{
				if (RightEdge < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					m_LineRecord[RightEdge] = SFOUR;
					m_LineRecord[RightEdge + 2] = ANALSISED;

				}
				else
				{
					if (LeftThree == TRUE)
						m_LineRecord[RightEdge] = THREE;
					else
						m_LineRecord[RightEdge] = STHREE;
				}
			}
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)
					return m_LineRecord[LeftEdge];
				if (LeftThree == TRUE)
					m_LineRecord[nAnalyPos] = STHREE;
			}

		}
		else
		{
			if (m_LineRecord[LeftEdge] == SFOUR)
				return m_LineRecord[LeftEdge];
			if (LeftThree == TRUE)
				m_LineRecord[nAnalyPos] = STHREE;

		}
		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 1)//2连
	{
		BOOL Lefttwo = FALSE;
		BOOL Leftthree = FALSE;
		if (LeftEdge > 2)
		{
			if (AnalyLine[LeftEdge - 1] == NOSTONE)
			{
				if (LeftEdge - 1 > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					if (AnalyLine[LeftEdge - 3] == AnalyLine[LeftEdge])
					{
						m_LineRecord[LeftEdge - 3] = ANALSISED;
						m_LineRecord[LeftEdge - 2] = ANALSISED;
						m_LineRecord[LeftEdge] = SFOUR;
					}
					else
					if (AnalyLine[LeftEdge - 3] == NOSTONE)
					{
						m_LineRecord[LeftEdge - 2] = ANALSISED;
						m_LineRecord[LeftEdge] = STHREE;
					}
				}
				else
					Lefttwo = TRUE;
			}
		}
		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge + 1] == NOSTONE)
			{
				if (RightEdge + 1 < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge])
					{
						m_LineRecord[RightEdge + 3] = ANALSISED;
						m_LineRecord[RightEdge + 2] = ANALSISED;
						m_LineRecord[RightEdge] = SFOUR;
					}
					else
					if (AnalyLine[RightEdge + 3] == NOSTONE)
					{
						m_LineRecord[RightEdge + 2] = ANALSISED;
						if (Lefttwo == TRUE)
							m_LineRecord[RightEdge] = THREE;
						else
							m_LineRecord[RightEdge] = STHREE;
					}

				}
				else
				{
					if (m_LineRecord[LeftEdge] == SFOUR)
						return m_LineRecord[LeftEdge];
					if (m_LineRecord[LeftEdge] == STHREE)
					{
						m_LineRecord[LeftEdge] = THREE;
						return m_LineRecord[LeftEdge];
					}

					if (Lefttwo == TRUE)
						m_LineRecord[nAnalyPos] = TWO;
					else
						m_LineRecord[nAnalyPos] = STWO;
				}
			}
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)
					return m_LineRecord[LeftEdge];
				if (Lefttwo == TRUE)
					m_LineRecord[nAnalyPos] = STWO;
			}

		}
		return m_LineRecord[nAnalyPos];
	}

	return 0;

}

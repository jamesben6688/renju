#if !defined(AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define  STWO      1   //眠二
#define  STHREE    2   //眠三
#define  SFOUR     3   //冲四
#define  TWO       4   //活二
#define  THREE     5   //活三
#define  FOUR      6   //活四
#define  FIVE      7   //五连
#define  DFOUR     8   //双四
#define  FOURT     9   //四三
#define  DTHREE   10   //三三
#define  NOTYPE   11   //未定义
#define  ANALSISED 255  //已分析过的
#define  TOBEANALSIS 0  //已分析过的

//棋子位置价值表
extern int PosValue[15][15];

class CEveluation  
{
public:
	CEveluation();
	virtual ~CEveluation();
	
	//估值函数，对传入的棋盘打分，bIsRed表明轮到谁走棋
	virtual int Eveluate(BYTE position[][GRID_NUM], BOOL bIsRedTurn);

protected:
	//分析水平方向上的某点及其周边的旗型
	int AnalysisHorizon(BYTE position[][GRID_NUM], int i, int j);
	//分析垂直方向上某点及其周边旗型
	int AnalysisVertical(BYTE position[][GRID_NUM], int i, int j);
	//分析左斜45度方向
	int AnalysisLeft(BYTE position[][GRID_NUM], int i, int j);
	//分析右斜45方向
	int AnalysisRight(BYTE position[][GRID_NUM], int i, int j);
	//分析
	int AnalysisLine(BYTE *position, int GridNum, int StonePos);
	//存放AnalysisLine分析结果的数组
	BYTE m_LineRecord[30];
	//存放全部分析结果的数组，用于存放水平、垂直、左斜、右斜源个方向上所有棋型，分析结果
	int TypeRecord[GRID_NUM][GRID_NUM][4];
	//存放统计过的分析结果的数组
	int TypeCount[2][20];
};

#endif // !defined(AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)

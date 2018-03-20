#if !defined(AFX_MOVEGENERATOR_H__54A88FC2_CAFC_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_MOVEGENERATOR_H__54A88FC2_CAFC_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMoveGenerator  
{
public:
	CMoveGenerator();
	virtual ~CMoveGenerator();
	//产生给定局面的下一步的所有合法的走法
	int CreatePossibleMove(BYTE position[GRID_NUM][GRID_NUM], int nPly,int nSide);
	//用以记录走法的数组
	STONEMOVE m_MoveList[10][225];
protected:
	int AddMove(int nFromX,int nToX,int nPly);//向走法数组中添加走法
	int m_nMoveCount;//记录走法的总数
};

#endif // !defined(AFX_MOVEGENERATOR_H__54A88FC2_CAFC_11D5_AEC7_5254AB2E22C7__INCLUDED_)

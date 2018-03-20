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


// �� m_MoveList �в���һ���߷�
// nToX ��Ŀ��λ�ú�����
// nToY ��Ŀ��λ��������
// nPly �Ǵ��߷����ڵĲ��
int CMoveGenerator::AddMove(int nToX, int nToY, int nPly)
{
	m_MoveList[nPly][m_nMoveCount].StonePos.x = nToX;
	m_MoveList[nPly][m_nMoveCount].StonePos.y = nToY;
	m_nMoveCount++;
	//ʹ��λ�ü�ֵ��������ǰ�߷��ļ�ֵ
	m_MoveList[nPly][m_nMoveCount].Score = PosValue[nToY][nToX];
	return m_nMoveCount;
}

//���Բ������� position �����п��ܵ��߷�
//position �ǰ�������������λ����Ϣ�Ķ�ά����
//nPlyָ����ǰ�����Ĳ�����ÿ�㽫�߷����ڲ�ͬ��λ�ã����⸲��
//nSideָ��������һ�����߷���WHITEΪ�׷���BLACK�Ǻڷ�
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
	//ʹ����ʷ�������еľ�̬�鲢���������߷����н�������
	//����Ϊ����߼�֦Ч��
	CHistoryHeuristic::MergeSort(m_MoveList[nPly], m_nMoveCount, 0);
	return m_nMoveCount;//���غϷ��߷�������
}
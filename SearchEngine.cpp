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

//���ݴ�����߷��ı�����
//move ��Ҫ���е��߷�
//type ��Ҫ�µ���������
BYTE CSearchEngine::MakeMove(STONEMOVE* move, int type)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = type;
	return 0;
}

//���ݴ�����߷��ָ�����
//move ��Ҫ�ָ����߷�
void CSearchEngine::UnMakeMove(STONEMOVE* move)
{
	CurPosition[move->StonePos.y][move->StonePos.x] = NOSTONE;
}

//���Լ�����������Ϸ�Ƿ����
//δ��������0�����򷵻ؼ���ֵ/��Сֵ
int CSearchEngine::IsGameOver(BYTE position[][GRID_NUM], int nDepth)
{
	int score, i;
	//����Ҫ���ӵ�������ɫ
	i = (m_nMaxDepth - nDepth) % 2;
	//���ù�ֵ����
	score = m_pEval->Eveluate(position, i);
	//�����ֵ�������ؼ�ֵ������������Ϸ����
	if (abs(score) >= 9990)
		return score; //���ؼ�ֵ
	return 0;//����δ����
}

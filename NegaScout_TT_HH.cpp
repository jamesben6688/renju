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
	m_nMaxDepth = m_nSearchDepth;	//�趨�������
	CalculateInitHashKey(CurPosition);	//�����ʼ���̵�hashֵ
	ResetHistoryTable();			//��ʼ����ʷ��¼��
	//ʹ��NegaScout����������������
	Score = NegaScout(m_nMaxDepth, -20000, 20000);
	//�ж���Ϸ�Ƿ����
	if (CurPosition[m_cmBestMove.StonePos.y][m_cmBestMove.StonePos.x] != NOSTONE)
	{
		//��һ�ʤ
		if (Score < 0)
			MessageBox(NULL, "Game Over! You Win!!!:(", "Renju", MB_OK);
		//AI��ʤ
		else
			MessageBox(NULL, "Game Over! You Loss...:)", "Renju", MB_OK);

		m_bGameOver = TRUE;
	}
	//�߳�����߷�
	MakeMove(&m_cmBestMove, Type);
	//�����߹��˵�����
	memcpy(position, CurPosition, GRID_COUNT);
}

//������������
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
			return i;	//�Ѿ��ֳ�ʤ�������ؼ�ֵ
	}
	side = (m_nMaxDepth - depth) % 2;//���㵱ǰ�ڵ������

	score = LookUpHashTable(alpha, beta, depth, side); //�����û����Ƿ��е�ǰ�ڵ����Ч����
	if (score != 66666)
		return score;//���У�ֱ�ӷ���

	if (depth <= 0)	//Ҷ�ӽڵ�ȡ��ֵ
	{
		score = m_pEval->Eveluate(CurPosition, side);//ȡ����ֵ
		EnterHashTable(exact, score, depth, side);//����ֵ�����û���
		return score;//���ع�ֵ
	}


	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);//������һ�����ܵ��߷�
	//���ڵ��趨������
	if (depth == m_nMaxDepth)
	{
		m_pThinkProgress->SetRange(0, Count);
		m_pThinkProgress->SetStep(1);
	}

	//ȡ�����߷�����ʷ�÷�
	for (i = 0; i < Count; i++)
	{
		m_pMG->m_MoveList[depth][i].Score =
			GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
	}

	//���߷�����ʷ�÷�����
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);

	int bestmove = -1;

	a = alpha;
	b = beta;
	int eval_is_exact = 0;
	for (i = 0; i < Count; i++)
	{
		if (depth == m_nMaxDepth)
			m_pThinkProgress->StepIt();//�������߶�
		//�����ӽڵ�
		type = MakeMove(&m_pMG->m_MoveList[depth][i], side);
		//�����ӽڵ�hashֵ
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);

		//�ݹ������ӽڵ㣬�Ե�һ���ڵ���ȫ���ڣ�����ǿմ�̽��
		t = -NegaScout(depth - 1, -b, -a);

		if (t > a && t < beta && i > 0)
		{
			a = -NegaScout(depth - 1, -beta, -t);     /* re-search */
			eval_is_exact = 1;	//����������Ϊ��ȷֵ
			if (depth == m_nMaxDepth)	//��������߷�
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			bestmove = i;		//��ס����߷���λ��
		}
		//�ָ���ǰ����hashֵ
		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);
		//�����ӽڵ�
		UnMakeMove(&m_pMG->m_MoveList[depth][i]);
		if (a < t)
		{
			//��һ����������
			eval_is_exact = 1;
			a = t;
			//��������߷�
			if (depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}
		if (a >= beta)
		{
			//���±߽�����û���
			EnterHashTable(lower_bound, a, depth, side);
			//����ǰ�߷�������ʷ��¼
			EnterHistoryScore(&m_pMG->m_MoveList[depth][i], depth);
			return a;	//��֦
		}
		b = a + 1;                      /* set new null window */
	}

	//����߷�������ʷ��¼
	if (bestmove != -1)
		EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);
	if (eval_is_exact)
		EnterHashTable(exact, a, depth, side);
	else
		EnterHashTable(upper_bound, a, depth, side);
	return a;		//�������ֵ
}
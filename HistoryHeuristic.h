#if !defined(AFX_HISTORYHEURISTIC_H__5870AB20_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_HISTORYHEURISTIC_H__5870AB20_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHistoryHeuristic  
{
public:
	CHistoryHeuristic();
	virtual ~CHistoryHeuristic();
	
	//��ʱ��¼�����
	void ResetHistoryTable();
	//ȡĳһ�߷�����ʷ�÷�
	int GetHistoryScore(STONEMOVE *move);
	//��ĳһ����߷�������ʷ��¼��
	void EnterHistoryScore(STONEMOVE *move,int depth);
	//�Ե�ǰ�߷����н��й鲢����
	static void MergeSort(STONEMOVE *source, int n, BOOL direction);
protected:
	//���ںϲ�����õ��������ݶΣ�MergeSort����
	static void Merge(STONEMOVE *source, STONEMOVE *target, int l,int m, int r);
	//��С��������MergePass����
	static void MergePass(STONEMOVE *source, STONEMOVE *target, const  int s, const  int n, const BOOL direction);
	//�Ӵ�С����	MergePass����
	static void Merge_A(STONEMOVE *source, STONEMOVE *target, int l,int m, int r);
	static STONEMOVE m_TargetBuff[225]; //��ʷ�÷ֱ�
	int m_HistoryTable[GRID_NUM][GRID_NUM]; //�����õĻ������

};

#endif // !defined(AFX_HISTORYHEURISTIC_H__5870AB20_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_)

#if !defined(AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ö���͵��������͡���ȷ���±߽硢�ϱ߽�
enum ENTRY_TYPE{ exact , lower_bound, upper_bound}; 

//��ϣ����Ԫ�صĽṹ����
typedef struct HASHITEM{
    LONGLONG checksum;	// or long long might be even better
	ENTRY_TYPE  entry_type;//��������
    short depth;	//ȡ����ֵʱ�Ĳ��
    short eval;		//�ڵ��ֵ
}HashItem;

class CTranspositionTable  
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

	//����������̵�hashֵ
	void CalculateInitHashKey(BYTE CurPosition[GRID_NUM][GRID_NUM]);
	//�����߷������������µ�hashֵ
	void Hash_MakeMove(STONEMOVE* move,BYTE CurPosition[GRID_NUM][GRID_NUM]);
	//���������߷��Ĺ�ϣֵ����ԭ���߹�֮ǰ��
	void Hash_UnMakeMove(STONEMOVE* move,BYTE CurPosition[GRID_NUM][GRID_NUM]);
	//��ѯ��ϣ���е�ǰ�ڵ�����
	int LookUpHashTable(int alpha, int beta, int depth,int TableNo);
	//����ǰ�ڵ��ֵ�����ϣ��
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth,int TableNo);
	//32λ������飬��������ԳԲλ��ϣֵ
	UINT m_nHashKey32[2][GRID_NUM][GRID_NUM];
	//64λ������飬��������64λ��ϣֵ
	ULONGLONG m_ulHashKey64[2][GRID_NUM][GRID_NUM];
	//�û���ͷָ��
	HashItem *m_pTT[2];
	//��ʼ��������飬������ϣ��
	void InitializeHashKey();
	UINT m_HashKey32;//32λhashֵ
	LONGLONG m_HashKey64;//64λhashֵ
};

#endif // !defined(AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_)

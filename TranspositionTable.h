#if !defined(AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//枚举型的数据类型、精确、下边界、上边界
enum ENTRY_TYPE{ exact , lower_bound, upper_bound}; 

//哈希表中元素的结构定义
typedef struct HASHITEM{
    LONGLONG checksum;	// or long long might be even better
	ENTRY_TYPE  entry_type;//数据类型
    short depth;	//取到此值时的层次
    short eval;		//节点的值
}HashItem;

class CTranspositionTable  
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

	//计算给定棋盘的hash值
	void CalculateInitHashKey(BYTE CurPosition[GRID_NUM][GRID_NUM]);
	//根据走法，增量生成新的hash值
	void Hash_MakeMove(STONEMOVE* move,BYTE CurPosition[GRID_NUM][GRID_NUM]);
	//撤销所给走法的哈希值，还原成走过之前的
	void Hash_UnMakeMove(STONEMOVE* move,BYTE CurPosition[GRID_NUM][GRID_NUM]);
	//查询哈希表中当前节点数据
	int LookUpHashTable(int alpha, int beta, int depth,int TableNo);
	//将当前节点的值存入哈希表
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth,int TableNo);
	//32位随机数组，用以生成猿圆位哈希值
	UINT m_nHashKey32[2][GRID_NUM][GRID_NUM];
	//64位随机数组，用以生成64位哈希值
	ULONGLONG m_ulHashKey64[2][GRID_NUM][GRID_NUM];
	//置换表头指针
	HashItem *m_pTT[2];
	//初始化随机数组，创建哈希表
	void InitializeHashKey();
	UINT m_HashKey32;//32位hash值
	LONGLONG m_HashKey64;//64位hash值
};

#endif // !defined(AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_)

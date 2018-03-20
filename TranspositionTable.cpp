#include "stdafx.h"
#include "renju.h"
#include "TranspositionTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//生成64位随机数
LONGLONG rand64(void)
{
	return rand() ^ ((LONGLONG)rand() << 15) ^ ((LONGLONG)rand() << 30) ^
		((LONGLONG)rand() << 45) ^ ((LONGLONG)rand() << 60);
}

//产生32位随机数
LONG rand32(void)
{
	return rand() ^ ((LONG)rand() << 15) ^ ((LONG)rand() << 30);
}

//建立哈希表，创建随机数组
CTranspositionTable::CTranspositionTable()
{
	InitializeHashKey();
}

//释放空间
CTranspositionTable::~CTranspositionTable()
{
	delete m_pTT[0];
	delete m_pTT[1];
}

//生成用于计算哈希值的随机数组
void CTranspositionTable::InitializeHashKey()
{
	int i, j, k;

	srand((unsigned)time(NULL));//设定随机数的种子

	//设定随机数的种子
	for (k = 0; k < 2; k++)
	for (i = 0; i < GRID_NUM; i++)
	for (j = 0; j < GRID_NUM; j++)
	{
		m_nHashKey32[k][i][j] = rand32();
		m_ulHashKey64[k][i][j] = rand64();
	}

	//申请置换表所用空间
	m_pTT[0] = new HashItem[1024 * 1024];
	m_pTT[1] = new HashItem[1024 * 1024];
}

//根据传入的棋盘计算出64位及32位哈希值
//CurPosition  要计算哈希值的棋盘
void CTranspositionTable::CalculateInitHashKey(BYTE CurPosition[GRID_NUM][GRID_NUM])
{
	int i, j, nStoneType;
	m_HashKey32 = 0;
	m_HashKey32 = 0;

	//将所有棋子对应的哈希值加总
	for (i = 0; i < GRID_NUM; i++)
	for (j = 0; j < GRID_NUM; j++)
	{
		nStoneType = CurPosition[i][j];
		if (nStoneType != NOSTONE)
		{
			m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nStoneType][i][j];
			m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nStoneType][i][j];
		}
	}
}

//根据传入的走法，修改相应的哈希值为走过以后的
//move  要进行的走法
//CurPositionc 当前棋盘
void CTranspositionTable::Hash_MakeMove(STONEMOVE* move, BYTE CurPosition[GRID_NUM][GRID_NUM])
{
	int type;
	type = CurPosition[move->StonePos.y][move->StonePos.x];
	//将棋子在目标位置的随机数添入
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move->StonePos.y][move->StonePos.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move->StonePos.y][move->StonePos.x];
}

//恢复hash_makemove改变的hash值
//move 要取消的走法，CurPosition当前的棋盘
void CTranspositionTable::Hash_UnMakeMove(STONEMOVE* move, BYTE CurPosition[GRID_NUM][GRID_NUM])
{
	int type;
	type = CurPosition[move->StonePos.y][move->StonePos.x];
	//将棋子现在位置上的随机数从哈希值当中去除
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[type][move->StonePos.y][move->StonePos.x];
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[type][move->StonePos.y][move->StonePos.x];
}

//查找hash表
//alpha 是alpha-beta搜索的上边界
//beta 是alpha-beta搜索的下边界
//depth当前搜索的层次
int CTranspositionTable::LookUpHashTable(int alpha, int beta, int depth, int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;
	pht = &m_pTT[TableNo][x];

	if (pht->depth >= depth && pht->checksum == m_HashKey64)
	{
		switch (pht->entry_type)
		{
		case exact:
			return pht->eval;
		case lower_bound:
			if (pht->eval >= beta)
				return (pht->eval);
			else
				break;
		case upper_bound:
			if (pht->eval <= alpha)
				return (pht->eval);
			else
				break;
		}
	}

	return 66666;
}

//
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth, int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;//二十位哈希地址
	pht = &m_pTT[TableNo][x];

	pht->checksum = m_HashKey64;
	pht->entry_type = entry_type;
	pht->eval = eval;
	pht->depth = depth;

}

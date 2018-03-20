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
	
	//及时记录表清空
	void ResetHistoryTable();
	//取某一走法的历史得分
	int GetHistoryScore(STONEMOVE *move);
	//将某一最佳走法汇入历史记录表
	void EnterHistoryScore(STONEMOVE *move,int depth);
	//对当前走法队列进行归并排序
	static void MergeSort(STONEMOVE *source, int n, BOOL direction);
protected:
	//用于合并排序好的相邻数据段，MergeSort调用
	static void Merge(STONEMOVE *source, STONEMOVE *target, int l,int m, int r);
	//从小到大排序，MergePass调用
	static void MergePass(STONEMOVE *source, STONEMOVE *target, const  int s, const  int n, const BOOL direction);
	//从大到小排序	MergePass调用
	static void Merge_A(STONEMOVE *source, STONEMOVE *target, int l,int m, int r);
	static STONEMOVE m_TargetBuff[225]; //历史得分表
	int m_HistoryTable[GRID_NUM][GRID_NUM]; //排序用的缓冲队列

};

#endif // !defined(AFX_HISTORYHEURISTIC_H__5870AB20_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_)

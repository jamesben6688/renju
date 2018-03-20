#if !defined(AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define  STWO      1   //�߶�
#define  STHREE    2   //����
#define  SFOUR     3   //����
#define  TWO       4   //���
#define  THREE     5   //����
#define  FOUR      6   //����
#define  FIVE      7   //����
#define  DFOUR     8   //˫��
#define  FOURT     9   //����
#define  DTHREE   10   //����
#define  NOTYPE   11   //δ����
#define  ANALSISED 255  //�ѷ�������
#define  TOBEANALSIS 0  //�ѷ�������

//����λ�ü�ֵ��
extern int PosValue[15][15];

class CEveluation  
{
public:
	CEveluation();
	virtual ~CEveluation();
	
	//��ֵ�������Դ�������̴�֣�bIsRed�����ֵ�˭����
	virtual int Eveluate(BYTE position[][GRID_NUM], BOOL bIsRedTurn);

protected:
	//����ˮƽ�����ϵ�ĳ�㼰���ܱߵ�����
	int AnalysisHorizon(BYTE position[][GRID_NUM], int i, int j);
	//������ֱ������ĳ�㼰���ܱ�����
	int AnalysisVertical(BYTE position[][GRID_NUM], int i, int j);
	//������б45�ȷ���
	int AnalysisLeft(BYTE position[][GRID_NUM], int i, int j);
	//������б45����
	int AnalysisRight(BYTE position[][GRID_NUM], int i, int j);
	//����
	int AnalysisLine(BYTE *position, int GridNum, int StonePos);
	//���AnalysisLine�������������
	BYTE m_LineRecord[30];
	//���ȫ��������������飬���ڴ��ˮƽ����ֱ����б����бԴ���������������ͣ��������
	int TypeRecord[GRID_NUM][GRID_NUM][4];
	//���ͳ�ƹ��ķ������������
	int TypeCount[2][20];
};

#endif // !defined(AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)

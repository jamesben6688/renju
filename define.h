#ifndef define_h_
#define define_h_

#define BOARD_POS_X   60
#define BOARD_POS_Y   30

#define BOARD_WIDTH   32

#define GRID_NUM 15
#define GRID_COUNT 225


#define BLACK  0
#define WHITE  1

#define NOSTONE 0xFF    //没有棋子

#define IsValidPos(x,y)  ((x>=0&&x<GRID_NUM)&&(y>=0&&y<GRID_NUM))  
typedef struct _stoneposition
{
	BYTE		x;
	BYTE		y;
}STONEPOS;

//用于记录走法，将在走法产生及搜索部分用到
//落子位置和落子的分数
typedef struct _stonemove
{
	STONEPOS	StonePos;		
	int		    Score;	
}STONEMOVE;

#endif //define_h_
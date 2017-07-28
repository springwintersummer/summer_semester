#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "tetris.h"

//窗口中每一块都可以用坐标来表示，所有的方块我们都可以在一个4*4的体现
//每一小部分代表了每一个图形，每一行代表了每一个图形的旋转过程中所有的图形
struct B Blocks[][4] =
{
	//正7
	0, 0, 1, 0, 1, 1, 1, 2,    2, 0, 0, 1, 1, 1, 2, 1,   1, 0, 1, 1, 1, 2, 2, 2,    0, 1, 1, 1, 2, 1, 0, 2,
	//反7
	1, 0, 2, 0, 1, 1, 1, 2,    0, 1, 1, 1, 2, 1, 2, 2,   1, 0, 1, 1, 0, 2, 1, 2,    0, 0, 0, 1, 1, 1, 2, 1,
	//长方体
	1, 0, 1, 1, 1, 2, 1, 3,    0, 0, 1, 0, 2, 0, 3, 0,   1, 0, 1, 1, 1, 2, 1, 3,    0, 0, 1, 0, 2, 0, 3, 0,
	//正Z
	0, 0, 1, 0, 1, 1, 2, 1,    2, 0, 1, 1, 2, 1, 1, 2,   0, 0, 1, 0, 1, 1, 2, 1,    2, 0, 1, 1, 2, 1, 1, 2,
	//反Z
	1, 0, 2, 0, 0, 1, 1, 1,    1, 0, 1, 1, 2, 1, 2, 2,   1, 0, 2, 0, 0, 1, 1, 1,    1, 0, 1, 1, 2, 1, 2, 2,
	//正方体
	0, 0, 1, 0, 0, 1, 1, 1,    0, 0, 1, 0, 0, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 1,    0, 0, 1, 0, 0, 1, 1, 1,
	//凸
	1, 0, 0, 1, 1, 1, 2, 1,    0, 0, 0, 1, 1, 1, 0, 2,   0, 0, 1, 0, 2, 0, 1, 1,    1, 0, 0, 1, 1, 1, 1, 2
};

/*******************************************************************************
判断方块是否可以进行上下左右移动的函数
此时我们将整个窗口看成是一个二维数组
在这个二维数组中，我们把边界和已经有了方块的地方看成是true，也就是1.
此时我们先把整个方块变成一个空的，整个向旁边来移动，来进行移动后的判断
如果移动后碰到了边界或者是已经有了一个方块则操作就不能继续进进行了
反之就是能够进行移动和变换
********************************************************************************/

//判断方块是否可以下落
bool CanDown(POINT pt[])
{
	bool result = true;
	//方块所在的地方我们假设这里没有方格，然后进行下落移动
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	for (int i = 0; i < 4; ++i)
	{
		//假如继续落下后的图形超过下底边界，或者这个方块下落一格已经有方块，结果为false
	if (pt[i].y + 1 == NUMLINEBLOCKS || GameClient[pt[i].x][pt[i].y + 1])
    {
			result = false;
			break;
	 }
	}
	//恢复方块所在格子为有方块
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}


//判断是否可以左移
bool CanLeft(POINT pt[])
{
	bool result = true;
	//方块所在的地方我们假设这里没有方格，然后进行向左移动
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	for (int i = 0; i < 4; ++i)
	{
		//假如继续左移超过左边边界，或者这个小方块左移一格已经有方块，结果为false
		if (!pt[i].x || GameClient[pt[i].x - 1][pt[i].y])
		{
			result = false;
			break;
		}
	}
	//恢复方块所在格子为有方块
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}

//判断是否可以右移
bool CanRight(POINT pt[])
{
	bool result = true;
	//方块所在的地方我们假设这里没有方格，然后进行向右移动
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	for (int i = 0; i < 4; ++i)
	{
		//假如继续左移超过左边边界，或者这个小方块左移一格已经有方块，结果为false
		if (pt[i].x + 1 == NUMCOLUMNBLOCKS || GameClient[pt[i].x + 1][pt[i].y])
		{
			result = false;
			break;
		}
	}
	//恢复方块所在格子为有方块
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}

//判断是否可以旋转
bool CanChange(POINT pt[])
{
	bool result = true;
	//方块所在的地方我们假设这里没有方格，然后进行旋转
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	int t = (cS + 1) % 4;
	for (int k = 0; k < 4; ++k)
	{
		int x = Blocks[cF][t].pt[k].x - Blocks[cF][cS].pt[k].x,
			y = Blocks[cF][t].pt[k].y - Blocks[cF][cS].pt[k].y;
		if (GameClient[pt[k].x + x][pt[k].y + y] ||     //该方格已经有方块
			pt[k].x + x > NUMCOLUMNBLOCKS - 1 ||        //x坐标超越了右边界
			pt[k].x + x < 0 ||                          //x坐标超越了左边界
			pt[k].y + y > NUMLINEBLOCKS - 1)            //y坐标超越了下边界
		{
			result = false;
			break;
		}
	}

	//恢复方块所在格子为有方块
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}

// 提高速度。
bool SpeedUp(HWND hwnd)
{
	// 调整计时器到时时间
	dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return TRUE;
}

//实现旋转的过程
void Change(POINT pt[])
{
	//调整Blocks里的值
	int t = (cS + 1) % 4;
	for (int i = 0; i < 4; ++i)
	{
		int x1 = Blocks[cF][t].pt[i].x - Blocks[cF][cS].pt[i].x,
			y1 = Blocks[cF][t].pt[i].y - Blocks[cF][cS].pt[i].y;
		GameClient[pt[i].x][pt[i].y] = false;
		pt[i].x += x1;
		pt[i].y += y1;
		GameClient[pt[i].x][pt[i].y] = true;
	}
	cS = t;
}

//实现右移的过程
void Right(POINT pt[])
{
	for (int i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = false;
		++pt[i].x;
	}
	for (int k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = true;
}

//实现左移的过程
void Left(POINT pt[])
{
	for (int i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = false;
		--pt[i].x;
	}
	for (int k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = true;
}

//实现方块的下落过程
void Down(POINT pt[])
{
	for (int i = 0; i < 4; ++i)
	{
		GameClient[pt[i].x][pt[i].y] = false;
		++pt[i].y;
	}
	for (int k = 0; k < 4; ++k)
		GameClient[pt[k].x][pt[k].y] = true;
}


//满行的消除以及分数的计算
void DelSqure(HWND hwnd)
{
	int line = 0, temp;
	for (int x = NUMLINEBLOCKS - 1; x >= 0; --x)
	{
		bool result = true;
		for (int y = 0; y < NUMCOLUMNBLOCKS; ++y)
		{
			if (!GameClient[y][x])
			{
				result = false;
				break;
			}
		}
		//判断是否可以消行
		if (result)
		{
			temp = x;
			++line;
			//移动消行以后上面的方块的位置
			while (x > 0)
			{
				for (int y = 0; y < NUMCOLUMNBLOCKS; ++y)
				{
					GameClient[y][x] = GameClient[y][x - 1];
				}
				--x;
			}
			for (int y = 0; y < NUMCOLUMNBLOCKS; ++y)
				GameClient[y][0] = false;
			x = temp + 1;
		}
	}
    Score +=line;
	InvalidateRect(hwnd, NULL, TRUE);
}

unsigned Random(int n)
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	srand(st.wMilliseconds);
	return rand() % n;
}
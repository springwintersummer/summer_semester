#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <Windows.h>
#include <time.h>

typedef int bool;
#define false 0   //代表没有方块
#define true  1   //代表无方块

/************** 宏定义 ****************/
// 计时器ID。
#define TIMER_ID 12340
// 判断游戏是否达到升级条件
#define IS_SPEEDUP(s) (s % dwOneLevelScores) == 0
//将每一个窗口看成是有规律的方块，这里表示了方块的大小
#define BLOCKWIDTH                      20  
//窗口的行数
#define NUMLINEBLOCKS                   25  
//窗口的列数
#define NUMCOLUMNBLOCKS                 11
//定时器的ID
#define ID_TIMER                        1    
//方块的种类数
#define BLOCKSTYLES                     7
// 此变量表示多少个积分上升一个级别
#define dwOneLevelScores                10
//方块的颜色
#define BLOCKCOLOR                      RGB(23.45.67)
//初始速度
#define UNITSPED                        600

/************** 全局变量 ****************/
// 计时器到时间隔
DWORD dwTimerElapse;
// 每提高一级，计时器时钟周期缩短的比例。
DOUBLE dbLevelSpeedupRatio;
//游戏区各方格顶点布尔值，代表该方格是否有方块
bool   GameClient[NUMCOLUMNBLOCKS][NUMLINEBLOCKS];
//随机方块图形对应的位置，F和cF代表了图形的种类，S和cS代表了旋转过程中的图形
int    F, S, cF, cS;
//得到的分数
int    Score;             

//定义各方块形状,用点POINT来描述位置，用X,Y来表示坐标
struct B
{
	POINT     pt[4];
};
										 
//随机数函数定制版，用于随机出现的方块
unsigned Random(int n);

//判断是否可以下落，可以则返回true
bool CanDown(POINT pt[]);

//实现下落的过程
void Down(POINT pt[]);

//判断是否可以左移
bool CanLeft(POINT pt[]);

//实现左移的过程
void Left(POINT pt[]);

//判断是否可以右移
bool CanRight(POINT pt[]);

//实现右移的过程
void Right(POINT pt[]);

//判断是否可以变形
bool CanChange(POINT pt[]);

//实现变形的过程
void Change(POINT pt[]);

//对已经满行的地方进行消除处理以及分数的计算
void DelSqure(HWND);

//随机生成方块
unsigned Random(int n);

//随机生成数，使之不能大于n
unsigned Random(int n);

#endif
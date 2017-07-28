#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "tetris.h"

//������ÿһ�鶼��������������ʾ�����еķ������Ƕ�������һ��4*4������
//ÿһС���ִ�����ÿһ��ͼ�Σ�ÿһ�д�����ÿһ��ͼ�ε���ת���������е�ͼ��
struct B Blocks[][4] =
{
	//��7
	0, 0, 1, 0, 1, 1, 1, 2,    2, 0, 0, 1, 1, 1, 2, 1,   1, 0, 1, 1, 1, 2, 2, 2,    0, 1, 1, 1, 2, 1, 0, 2,
	//��7
	1, 0, 2, 0, 1, 1, 1, 2,    0, 1, 1, 1, 2, 1, 2, 2,   1, 0, 1, 1, 0, 2, 1, 2,    0, 0, 0, 1, 1, 1, 2, 1,
	//������
	1, 0, 1, 1, 1, 2, 1, 3,    0, 0, 1, 0, 2, 0, 3, 0,   1, 0, 1, 1, 1, 2, 1, 3,    0, 0, 1, 0, 2, 0, 3, 0,
	//��Z
	0, 0, 1, 0, 1, 1, 2, 1,    2, 0, 1, 1, 2, 1, 1, 2,   0, 0, 1, 0, 1, 1, 2, 1,    2, 0, 1, 1, 2, 1, 1, 2,
	//��Z
	1, 0, 2, 0, 0, 1, 1, 1,    1, 0, 1, 1, 2, 1, 2, 2,   1, 0, 2, 0, 0, 1, 1, 1,    1, 0, 1, 1, 2, 1, 2, 2,
	//������
	0, 0, 1, 0, 0, 1, 1, 1,    0, 0, 1, 0, 0, 1, 1, 1,   0, 0, 1, 0, 0, 1, 1, 1,    0, 0, 1, 0, 0, 1, 1, 1,
	//͹
	1, 0, 0, 1, 1, 1, 2, 1,    0, 0, 0, 1, 1, 1, 0, 2,   0, 0, 1, 0, 2, 0, 1, 1,    1, 0, 0, 1, 1, 1, 1, 2
};

/*******************************************************************************
�жϷ����Ƿ���Խ������������ƶ��ĺ���
��ʱ���ǽ��������ڿ�����һ����ά����
�������ά�����У����ǰѱ߽���Ѿ����˷���ĵط�������true��Ҳ����1.
��ʱ�����Ȱ�����������һ���յģ��������Ա����ƶ����������ƶ�����ж�
����ƶ��������˱߽�������Ѿ�����һ������������Ͳ��ܼ�����������
��֮�����ܹ������ƶ��ͱ任
********************************************************************************/

//�жϷ����Ƿ��������
bool CanDown(POINT pt[])
{
	bool result = true;
	//�������ڵĵط����Ǽ�������û�з���Ȼ����������ƶ�
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	for (int i = 0; i < 4; ++i)
	{
		//����������º��ͼ�γ����µױ߽磬���������������һ���Ѿ��з��飬���Ϊfalse
	if (pt[i].y + 1 == NUMLINEBLOCKS || GameClient[pt[i].x][pt[i].y + 1])
    {
			result = false;
			break;
	 }
	}
	//�ָ��������ڸ���Ϊ�з���
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}


//�ж��Ƿ��������
bool CanLeft(POINT pt[])
{
	bool result = true;
	//�������ڵĵط����Ǽ�������û�з���Ȼ����������ƶ�
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	for (int i = 0; i < 4; ++i)
	{
		//����������Ƴ�����߽߱磬�������С��������һ���Ѿ��з��飬���Ϊfalse
		if (!pt[i].x || GameClient[pt[i].x - 1][pt[i].y])
		{
			result = false;
			break;
		}
	}
	//�ָ��������ڸ���Ϊ�з���
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}

//�ж��Ƿ��������
bool CanRight(POINT pt[])
{
	bool result = true;
	//�������ڵĵط����Ǽ�������û�з���Ȼ����������ƶ�
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	for (int i = 0; i < 4; ++i)
	{
		//����������Ƴ�����߽߱磬�������С��������һ���Ѿ��з��飬���Ϊfalse
		if (pt[i].x + 1 == NUMCOLUMNBLOCKS || GameClient[pt[i].x + 1][pt[i].y])
		{
			result = false;
			break;
		}
	}
	//�ָ��������ڸ���Ϊ�з���
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}

//�ж��Ƿ������ת
bool CanChange(POINT pt[])
{
	bool result = true;
	//�������ڵĵط����Ǽ�������û�з���Ȼ�������ת
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = false;
	int t = (cS + 1) % 4;
	for (int k = 0; k < 4; ++k)
	{
		int x = Blocks[cF][t].pt[k].x - Blocks[cF][cS].pt[k].x,
			y = Blocks[cF][t].pt[k].y - Blocks[cF][cS].pt[k].y;
		if (GameClient[pt[k].x + x][pt[k].y + y] ||     //�÷����Ѿ��з���
			pt[k].x + x > NUMCOLUMNBLOCKS - 1 ||        //x���곬Խ���ұ߽�
			pt[k].x + x < 0 ||                          //x���곬Խ����߽�
			pt[k].y + y > NUMLINEBLOCKS - 1)            //y���곬Խ���±߽�
		{
			result = false;
			break;
		}
	}

	//�ָ��������ڸ���Ϊ�з���
	for (int i = 0; i < 4; ++i)
		GameClient[pt[i].x][pt[i].y] = true;
	return result;
}

// ����ٶȡ�
bool SpeedUp(HWND hwnd)
{
	// ������ʱ����ʱʱ��
	dwTimerElapse = (DWORD)(dwTimerElapse * dbLevelSpeedupRatio);
	KillTimer(hwnd, TIMER_ID);
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);
	return TRUE;
}

//ʵ����ת�Ĺ���
void Change(POINT pt[])
{
	//����Blocks���ֵ
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

//ʵ�����ƵĹ���
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

//ʵ�����ƵĹ���
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

//ʵ�ַ�����������
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


//���е������Լ������ļ���
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
		//�ж��Ƿ��������
		if (result)
		{
			temp = x;
			++line;
			//�ƶ������Ժ�����ķ����λ��
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
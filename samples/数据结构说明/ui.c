#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "tetris.h"

extern struct B Blocks[][4];
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
       
int WINAPI WinMain(
	HINSTANCE hinstance,     // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
							 //这两个不用管，就是句柄，有点像this
	LPSTR lpCmdLine,         // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)            // 用于指明窗口是否需要显示的参数。
					         //程序启动没有用到
{
	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	TCHAR szClassName[] = TEXT("Terics"),
		  szWindowName[] = TEXT("俄罗斯方块");
	static POINT Block[4];

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.hCursor = LoadCursor(hinstance, IDC_ARROW);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hIcon = LoadIcon(hinstance,
		IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hInstance = hinstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szClassName;
	//wc是一个结构体，使用这样一个结构体来表示，和c++的类有区别
	wc.lpszMenuName = NULL;
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	if (RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。
	HWND hwnd = CreateWindow(szClassName, szWindowName, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, (NUMCOLUMNBLOCKS + 10) * BLOCKWIDTH,
		(NUMLINEBLOCKS + 3) * BLOCKWIDTH,
		NULL, NULL, hinstance, NULL);

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	MSG msg;
	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT            TericsBorder[] = { -1, -1,NUMCOLUMNBLOCKS * BLOCKWIDTH + 1, NUMLINEBLOCKS * BLOCKWIDTH + 1 };
	HDC              hdc;
	PAINTSTRUCT      ps;
	TEXTMETRIC       tm;
	TCHAR            szNextTerics[] = TEXT("下一个："),
                     szSCore[] = TEXT("得分："),
                     szLevel[] = TEXT("等级： ");
	TCHAR            szBufferScore[5];
	static int       cxChar, cyChar;
	static POINT     Block[4], NextBlock[4];//代表了当前图形和下一个图形
	int              x, y;

	switch (message)
	{
		//* WM_CREATE		当窗口被创建是会收到此消息
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		//平均宽度
		cxChar = tm.tmAveCharWidth * 2;
		//tmHeight是字体高度，tmExternalLeading是夹在两行之间的空间数目 ，先定义变量然后获得值。
		cyChar = tm.tmExternalLeading + tm.tmHeight;
		SetTimer(hwnd, ID_TIMER,UNITSPED-Score/ dwOneLevelScores*100, NULL);
		//初始化第一个出现的方块
		cS = Random(4);
		cF = Random(BLOCKSTYLES);
		srand((unsigned)time(NULL));
		int rode = rand() % (NUMCOLUMNBLOCKS -4);
		for (int i = 0; i < 4; ++i)
		{
			Block[i].x = Blocks[cF][cS].pt[i].x+rode;
			Block[i].y = Blocks[cF][cS].pt[i].y;
			GameClient[Block[i].x][Block[i].y] = true;
		}
		S = Random(4);
		F = Random(BLOCKSTYLES);
		for (int i = 0; i < 4; ++i)
		{
			NextBlock[i].x = Blocks[F][S].pt[i].x;
			NextBlock[i].y = Blocks[F][S].pt[i].y;
		}

		ReleaseDC(hwnd, hdc);
		return 0;

		//* WM_TIMER		当计时器到时以后
	case WM_TIMER:
		if (CanDown(Block))
		{
			Down(Block);
		}
		//不能下移，需要处理消行判断（结合分数），还需要处理下一个显示，和当前显示的方块
		else
		{
			srand((unsigned)time(NULL));
			int rode1 = rand() % (NUMCOLUMNBLOCKS -4);
			DelSqure(hwnd);
			for (int i = 0; i < 4; ++i)
			{
				Block[i].x = NextBlock[i].x + rode1;
				Block[i].y = NextBlock[i].y;
				//如果最后一块在边界落不下来那么就中止程序，判断游戏结束
				if (GameClient[Block[i].x][Block[i].y])
				{
					KillTimer(hwnd, ID_TIMER);
					MessageBox(0, "你输了，游戏结束！！！", "俄罗斯方块", MB_ICONSTOP);
					exit(0);
				}
				else
					GameClient[Block[i].x][Block[i].y] = true;
			}
			cS = S;     cF = F;
			S = Random(4);
			F = Random(BLOCKSTYLES);
			for (int i = 0; i < 4; ++i)
			{
				NextBlock[i].x = Blocks[F][S].pt[i].x;
				NextBlock[i].y = Blocks[F][S].pt[i].y;
			}
		}
		InvalidateRect(hwnd, NULL, TRUE);
		return 0;
//* WM_KEYDOWN		当键盘的键被按下以后
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (CanLeft(Block))
				Left(Block);
			//无效矩形,是提高速度用的.比如你换了几个字符,就没必要刷新整个屏幕,把字符对应的矩形区域刷新一下就可以了 
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_RIGHT:
			if (CanRight(Block))
				Right(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_UP:
			if (CanChange(Block))
				Change(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		case VK_DOWN:
			while (CanDown(Block))
				Down(Block);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		default:
			break;
		}
		return 0;

		//* WM_PAINT		当窗口应该绘制GDI图形是会收到此消息
	case WM_PAINT:
		//if(pause) return 0;
		hdc = BeginPaint(hwnd, &ps);
		SetViewportOrgEx(hdc, BLOCKWIDTH, BLOCKWIDTH, NULL);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		SelectObject(hdc, GetStockObject(BLACK_PEN));

		//画俄罗斯方块游戏的边框
		Rectangle(hdc, TericsBorder[0].x, TericsBorder[0].y,
			TericsBorder[1].x, TericsBorder[1].y);
		//输出“下一个”字符串
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, 0,
			szNextTerics, lstrlen(szNextTerics));
		//输出“得分”字符串
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 5 * BLOCKWIDTH,
			szSCore, lstrlen(szSCore));

		SetTextAlign(hdc, TA_RIGHT | TA_TOP);
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH + 3 * cxChar, 2 * cyChar + 5 * BLOCKWIDTH,
		szBufferScore, wsprintf(szBufferScore, TEXT("%d"), Score));
		SetTextAlign(hdc, TA_LEFT | TA_TOP);

		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, GetStockObject(WHITE_PEN));


		//显示下一个方块区域的方块
		for (int i = 0; i < 4; ++i)
		{
			Rectangle(hdc, (NextBlock[i].x + NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, NextBlock[i].y * BLOCKWIDTH + cyChar,
				(NextBlock[i].x + NUMCOLUMNBLOCKS + 3) * BLOCKWIDTH, (NextBlock[i].y + 1) * BLOCKWIDTH + cyChar);
		}

		//显示游戏区的方块
		for (x = 0; x < NUMCOLUMNBLOCKS; ++x)
		{
			for (y = 0; y < NUMLINEBLOCKS; ++y)
			{
				if (GameClient[x][y])
				{
					Rectangle(hdc, x * BLOCKWIDTH, y * BLOCKWIDTH,
						(x + 1) * BLOCKWIDTH, (y + 1) * BLOCKWIDTH);
				}
			}
		}

		//输出等级字符
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 10 * BLOCKWIDTH,
			szLevel, lstrlen(szLevel));

		//输出等级
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH + 3 * cxChar, 8 * cyChar + 5 * BLOCKWIDTH,
			szBufferScore, wsprintf(szBufferScore, TEXT("%d"), Score/ dwOneLevelScores));

		//输出游戏规则
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 14 * BLOCKWIDTH,
			"游戏规则：", 10);
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 15 * BLOCKWIDTH,
			"上键↑ 旋转", 11);
		//输出游戏规则
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 16 * BLOCKWIDTH,
			"下键↓ 加速", 11);
		//输出游戏规则
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 17 * BLOCKWIDTH,
			"左键← 左移", 11);
		//输出游戏规则
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 18 * BLOCKWIDTH,
			"右键→ 右移", 11);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
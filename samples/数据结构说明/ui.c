#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "tetris.h"

extern struct B Blocks[][4];
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
       
int WINAPI WinMain(
	HINSTANCE hinstance,     // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
							 //���������ùܣ����Ǿ�����е���this
	LPSTR lpCmdLine,         // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)            // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
					         //��������û���õ�
{
	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	TCHAR szClassName[] = TEXT("Terics"),
		  szWindowName[] = TEXT("����˹����");
	static POINT Block[4];

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.hCursor = LoadCursor(hinstance, IDC_ARROW);
	// GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
	// ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hIcon = LoadIcon(hinstance,
		IDI_APPLICATION);
	// Cursor������꣬�������趨����������ʽ��
	// ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hInstance = hinstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = szClassName;
	//wc��һ���ṹ�壬ʹ������һ���ṹ������ʾ����c++����������
	wc.lpszMenuName = NULL;
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	if (RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�
	HWND hwnd = CreateWindow(szClassName, szWindowName, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, (NUMCOLUMNBLOCKS + 10) * BLOCKWIDTH,
		(NUMLINEBLOCKS + 3) * BLOCKWIDTH,
		NULL, NULL, hinstance, NULL);

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	MSG msg;
	// ���½�����Ϣѭ������ȡ��Ϣ--������Ϣ--������Ϣ���ɴ��ڵ���Ϣ��������������Ϣ��
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
	TCHAR            szNextTerics[] = TEXT("��һ����"),
                     szSCore[] = TEXT("�÷֣�"),
                     szLevel[] = TEXT("�ȼ��� ");
	TCHAR            szBufferScore[5];
	static int       cxChar, cyChar;
	static POINT     Block[4], NextBlock[4];//�����˵�ǰͼ�κ���һ��ͼ��
	int              x, y;

	switch (message)
	{
		//* WM_CREATE		�����ڱ������ǻ��յ�����Ϣ
	case WM_CREATE:
		hdc = GetDC(hwnd);
		GetTextMetrics(hdc, &tm);
		//ƽ�����
		cxChar = tm.tmAveCharWidth * 2;
		//tmHeight������߶ȣ�tmExternalLeading�Ǽ�������֮��Ŀռ���Ŀ ���ȶ������Ȼ����ֵ��
		cyChar = tm.tmExternalLeading + tm.tmHeight;
		SetTimer(hwnd, ID_TIMER,UNITSPED-Score/ dwOneLevelScores*100, NULL);
		//��ʼ����һ�����ֵķ���
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

		//* WM_TIMER		����ʱ����ʱ�Ժ�
	case WM_TIMER:
		if (CanDown(Block))
		{
			Down(Block);
		}
		//�������ƣ���Ҫ���������жϣ���Ϸ�����������Ҫ������һ����ʾ���͵�ǰ��ʾ�ķ���
		else
		{
			srand((unsigned)time(NULL));
			int rode1 = rand() % (NUMCOLUMNBLOCKS -4);
			DelSqure(hwnd);
			for (int i = 0; i < 4; ++i)
			{
				Block[i].x = NextBlock[i].x + rode1;
				Block[i].y = NextBlock[i].y;
				//������һ���ڱ߽��䲻������ô����ֹ�����ж���Ϸ����
				if (GameClient[Block[i].x][Block[i].y])
				{
					KillTimer(hwnd, ID_TIMER);
					MessageBox(0, "�����ˣ���Ϸ����������", "����˹����", MB_ICONSTOP);
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
//* WM_KEYDOWN		�����̵ļ��������Ժ�
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (CanLeft(Block))
				Left(Block);
			//��Ч����,������ٶ��õ�.�����㻻�˼����ַ�,��û��Ҫˢ��������Ļ,���ַ���Ӧ�ľ�������ˢ��һ�¾Ϳ����� 
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

		//* WM_PAINT		������Ӧ�û���GDIͼ���ǻ��յ�����Ϣ
	case WM_PAINT:
		//if(pause) return 0;
		hdc = BeginPaint(hwnd, &ps);
		SetViewportOrgEx(hdc, BLOCKWIDTH, BLOCKWIDTH, NULL);
		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
		SelectObject(hdc, GetStockObject(BLACK_PEN));

		//������˹������Ϸ�ı߿�
		Rectangle(hdc, TericsBorder[0].x, TericsBorder[0].y,
			TericsBorder[1].x, TericsBorder[1].y);
		//�������һ�����ַ���
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, 0,
			szNextTerics, lstrlen(szNextTerics));
		//������÷֡��ַ���
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 5 * BLOCKWIDTH,
			szSCore, lstrlen(szSCore));

		SetTextAlign(hdc, TA_RIGHT | TA_TOP);
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH + 3 * cxChar, 2 * cyChar + 5 * BLOCKWIDTH,
		szBufferScore, wsprintf(szBufferScore, TEXT("%d"), Score));
		SetTextAlign(hdc, TA_LEFT | TA_TOP);

		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		SelectObject(hdc, GetStockObject(WHITE_PEN));


		//��ʾ��һ����������ķ���
		for (int i = 0; i < 4; ++i)
		{
			Rectangle(hdc, (NextBlock[i].x + NUMCOLUMNBLOCKS + 2) * BLOCKWIDTH, NextBlock[i].y * BLOCKWIDTH + cyChar,
				(NextBlock[i].x + NUMCOLUMNBLOCKS + 3) * BLOCKWIDTH, (NextBlock[i].y + 1) * BLOCKWIDTH + cyChar);
		}

		//��ʾ��Ϸ���ķ���
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

		//����ȼ��ַ�
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 10 * BLOCKWIDTH,
			szLevel, lstrlen(szLevel));

		//����ȼ�
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH + 3 * cxChar, 8 * cyChar + 5 * BLOCKWIDTH,
			szBufferScore, wsprintf(szBufferScore, TEXT("%d"), Score/ dwOneLevelScores));

		//�����Ϸ����
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 14 * BLOCKWIDTH,
			"��Ϸ����", 10);
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 15 * BLOCKWIDTH,
			"�ϼ��� ��ת", 11);
		//�����Ϸ����
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 16 * BLOCKWIDTH,
			"�¼��� ����", 11);
		//�����Ϸ����
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 17 * BLOCKWIDTH,
			"����� ����", 11);
		//�����Ϸ����
		TextOut(hdc, (NUMCOLUMNBLOCKS + 1) * BLOCKWIDTH, cyChar + 18 * BLOCKWIDTH,
			"�Ҽ��� ����", 11);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
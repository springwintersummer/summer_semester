#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <Windows.h>
#include <time.h>

typedef int bool;
#define false 0   //����û�з���
#define true  1   //�����޷���

/************** �궨�� ****************/
// ��ʱ��ID��
#define TIMER_ID 12340
// �ж���Ϸ�Ƿ�ﵽ��������
#define IS_SPEEDUP(s) (s % dwOneLevelScores) == 0
//��ÿһ�����ڿ������й��ɵķ��飬�����ʾ�˷���Ĵ�С
#define BLOCKWIDTH                      20  
//���ڵ�����
#define NUMLINEBLOCKS                   25  
//���ڵ�����
#define NUMCOLUMNBLOCKS                 11
//��ʱ����ID
#define ID_TIMER                        1    
//�����������
#define BLOCKSTYLES                     7
// �˱�����ʾ���ٸ���������һ������
#define dwOneLevelScores                10
//�������ɫ
#define BLOCKCOLOR                      RGB(23.45.67)
//��ʼ�ٶ�
#define UNITSPED                        600

/************** ȫ�ֱ��� ****************/
// ��ʱ����ʱ���
DWORD dwTimerElapse;
// ÿ���һ������ʱ��ʱ���������̵ı�����
DOUBLE dbLevelSpeedupRatio;
//��Ϸ�������񶥵㲼��ֵ������÷����Ƿ��з���
bool   GameClient[NUMCOLUMNBLOCKS][NUMLINEBLOCKS];
//�������ͼ�ζ�Ӧ��λ�ã�F��cF������ͼ�ε����࣬S��cS��������ת�����е�ͼ��
int    F, S, cF, cS;
//�õ��ķ���
int    Score;             

//�����������״,�õ�POINT������λ�ã���X,Y����ʾ����
struct B
{
	POINT     pt[4];
};
										 
//������������ư棬����������ֵķ���
unsigned Random(int n);

//�ж��Ƿ�������䣬�����򷵻�true
bool CanDown(POINT pt[]);

//ʵ������Ĺ���
void Down(POINT pt[]);

//�ж��Ƿ��������
bool CanLeft(POINT pt[]);

//ʵ�����ƵĹ���
void Left(POINT pt[]);

//�ж��Ƿ��������
bool CanRight(POINT pt[]);

//ʵ�����ƵĹ���
void Right(POINT pt[]);

//�ж��Ƿ���Ա���
bool CanChange(POINT pt[]);

//ʵ�ֱ��εĹ���
void Change(POINT pt[]);

//���Ѿ����еĵط��������������Լ������ļ���
void DelSqure(HWND);

//������ɷ���
unsigned Random(int n);

//�����������ʹ֮���ܴ���n
unsigned Random(int n);

#endif
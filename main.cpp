// ThreadPool.cpp : Defines the entry point for the console application.

//����ʵ��
#include "stdafx.h"
#include "ThreadPool.h"
#include "Test.h"



//����ʵ��
int _tmain(int argc, _TCHAR* argv[])
{
	//��ʽʹ�õ�ʱ�򣬿��Խ�tmp����Ϊ��̬����,���ϵ�����������Զ����������д���
	ThreadPool tmp(1);
	CTask task;
	tmp.Create();
	tmp.AddTask(&task);//�����ָ�룬�˳��ֲ�����ָ���ʧЧ�ˣ�����ֱ��push_back����
	tmp.Execute();
	while(1)
	{
		Sleep(1000);
	}
	//tmp.Close();

	return 0;
}

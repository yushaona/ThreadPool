// ThreadPool.cpp : Defines the entry point for the console application.

//测试实例
#include "stdafx.h"
#include "ThreadPool.h"
#include "Test.h"



//测试实例
int _tmain(int argc, _TCHAR* argv[])
{
	//正式使用的时候，可以将tmp设置为静态变量,不断的向池中抛入自定义的任务进行处理
	ThreadPool tmp(1);
	CTask task;
	tmp.Create();
	tmp.AddTask(&task);//这个是指针，退出局部变量指针就失效了，可以直接push_back对象
	tmp.Execute();
	while(1)
	{
		Sleep(1000);
	}
	//tmp.Close();

	return 0;
}

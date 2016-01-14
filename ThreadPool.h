#pragma once
#include <vector>
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <vector>
using namespace std;

/*
该类用于管理线程池:
实现思路:
1.<<任务队列>>:用于向其中抛入待处理任务
2.<<线程队列>>:当有任务的时候,从线程队列中取出一个线程，并执行用户自定义的函数
3.如果线程队列中没有线程的时候，自动创建一个线程或者不创建线程(实际的用处是不同的)
4.初始情况下，根据CPU的核心数*2生成初始线程

*/
enum __Status
{
	RUNNING,
	WAITTING
};

class CustomTask;
//线程的当前状态
struct ThreadStatus//这个里面还可以加一个状态，如果超过cpu核数的线程长时间(超过30s)不运行，就自行结束自己,并且线程总数减一 pThreadNum
{
	int dwThreadID;
	CustomTask *pTask;
	__Status pStatus;
	time_t Lasttime;
	ThreadStatus()
	{
		dwThreadID = -1;
		Lasttime = 0;
		pTask = NULL;
		pStatus = WAITTING;
	}
};
//线程池管理类

class ThreadPool
{
public:
	ThreadPool(int ThreadNum = 0);
	~ThreadPool();
public:
	void Create();
	void Close();
	void Execute();
protected:
	void Run();
public:
	void AddTask(CustomTask*ct);
	int GetRemainTask();
protected:
	static  DWORD WINAPI ThreadFunc(LPVOID lpThreadParameter);
	static DWORD WINAPI ThreadPoolFunc(LPVOID lpThreadParameter);
private:
	//主线程的句柄
	HANDLE m_MainHandle;
	HANDLE m_MainEvent;
	vector<HANDLE>pThreadHandle;
	static vector<ThreadStatus> pThreadStatus;
	static bool PStopAll;
	vector<CustomTask*>pCustomTask;//任务队列
	CRITICAL_SECTION cs;//关键段
	int PThreadNum;//线程数量
	static HANDLE arr[20];
};



class CustomTask
{
public:
	CustomTask();
	virtual ~CustomTask();
	virtual void Run() = 0;
	void SetData( void *CustomData){ szData = CustomData;}
private:
	void *szData;
};
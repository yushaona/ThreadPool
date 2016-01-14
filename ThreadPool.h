#pragma once
#include <vector>
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <vector>
using namespace std;

/*
�������ڹ����̳߳�:
ʵ��˼·:
1.<<�������>>:�����������������������
2.<<�̶߳���>>:���������ʱ��,���̶߳�����ȡ��һ���̣߳���ִ���û��Զ���ĺ���
3.����̶߳�����û���̵߳�ʱ���Զ�����һ���̻߳��߲������߳�(ʵ�ʵ��ô��ǲ�ͬ��)
4.��ʼ����£�����CPU�ĺ�����*2���ɳ�ʼ�߳�

*/
enum __Status
{
	RUNNING,
	WAITTING
};

class CustomTask;
//�̵߳ĵ�ǰ״̬
struct ThreadStatus//������滹���Լ�һ��״̬���������cpu�������̳߳�ʱ��(����30s)�����У������н����Լ�,�����߳�������һ pThreadNum
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
//�̳߳ع�����

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
	//���̵߳ľ��
	HANDLE m_MainHandle;
	HANDLE m_MainEvent;
	vector<HANDLE>pThreadHandle;
	static vector<ThreadStatus> pThreadStatus;
	static bool PStopAll;
	vector<CustomTask*>pCustomTask;//�������
	CRITICAL_SECTION cs;//�ؼ���
	int PThreadNum;//�߳�����
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
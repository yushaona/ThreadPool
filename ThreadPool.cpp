#include "stdafx.h"
#include "ThreadPool.h"

//线程池

vector<ThreadStatus> ThreadPool::pThreadStatus;
bool ThreadPool::PStopAll = false;
HANDLE ThreadPool::arr[20]={0};
ThreadPool::ThreadPool( int ThreadNum /*= 0*/ )
{
	PThreadNum = ThreadNum;
	InitializeCriticalSection(&cs);
	m_MainHandle = NULL;
	m_MainEvent = NULL;
}

void ThreadPool::Create()
{
	if (0 == PThreadNum)
	{
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		PThreadNum = 2*sysinfo.dwNumberOfProcessors;
	}
	for (int i=0;i<PThreadNum;i++)
	{
		ThreadStatus pStatus;
		pStatus.dwThreadID = i;
		pThreadStatus.push_back(pStatus);
		HANDLE m_handle = CreateThread(NULL,0,ThreadFunc,&pThreadStatus[i].dwThreadID,0,NULL);
		pThreadHandle.push_back(m_handle);
	}
}




DWORD WINAPI ThreadPool::ThreadFunc( LPVOID lpThreadParameter )
{
	int dwThreadID = *(int*)lpThreadParameter;
	arr[dwThreadID] = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	while(1)
	{
		if (PStopAll)
		{
			::SetEvent(arr[dwThreadID]);
			break;
		}
		ThreadStatus _pStatus = pThreadStatus[dwThreadID];
		if(_pStatus.pStatus == RUNNING)
		{
			if (_pStatus.pTask != NULL)
				_pStatus.pTask->Run();
			//先赋值NULL,再设置为等待
			pThreadStatus[dwThreadID].pTask = NULL;
			pThreadStatus[dwThreadID].pStatus = WAITTING;
		}			
		Sleep(1);
	}
	return 0;
}

void ThreadPool::AddTask( CustomTask*ct )
{
	EnterCriticalSection(&cs);
	pCustomTask.push_back(ct);
	LeaveCriticalSection(&cs);
}

void ThreadPool::Run()
{
	m_MainEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	while(1)
	{
		if (PStopAll)
		{
			WaitForMultipleObjects(PThreadNum,arr,TRUE,INFINITE);
			for(int n = 0;n<PThreadNum;n++)
			{
				::CloseHandle(pThreadHandle[n]);
				pThreadHandle[n] = NULL;
			}
			SetEvent(m_MainEvent);
			break;
		}
		CustomTask *pTask = NULL;
		EnterCriticalSection(&cs);
		vector<CustomTask*>::iterator iter = pCustomTask.begin();
		if (iter != pCustomTask.end())
		{
			pTask = *iter;
			pCustomTask.erase(iter);//抛出
		}
		LeaveCriticalSection(&cs);

		if (pTask != NULL)//分配线程进行处理
		{
			bool success = false;
			for (int i = 0;i<PThreadNum;i++)
			{
				ThreadStatus __pStatus = pThreadStatus[i];
				if (__pStatus.pStatus == WAITTING)
				{
					pThreadStatus[i].pTask = pTask;//先设置对象，再设置状态
					pThreadStatus[i].pStatus = RUNNING;
					success = true;
					break;
				}
			}
			if (!success)//如果所有的线程都在忙,就新增一个线程
			{
				++PThreadNum;
				ThreadStatus pStatus;
				pStatus.pTask = pTask;
				pStatus.pStatus = RUNNING;
				pThreadStatus.push_back(pStatus);
				CreateThread(NULL,0,ThreadFunc,&PThreadNum,0,NULL);
			}
		}
	}
}

void ThreadPool::Execute()
{
	m_MainHandle = CreateThread(NULL,0,ThreadPoolFunc,this,0,NULL);
}

int ThreadPool::GetRemainTask()
{
	return pCustomTask.size();
}

void ThreadPool::Close()
{
	PStopAll = true;
	WaitForSingleObject(m_MainEvent,INFINITE);
	CloseHandle(m_MainEvent);
	CloseHandle(m_MainHandle);
}

ThreadPool::~ThreadPool()
{
	Close();
}

DWORD WINAPI ThreadPool::ThreadPoolFunc( LPVOID lpThreadParameter )
{
	ThreadPool *tp = (ThreadPool*)lpThreadParameter;
	tp->Run();
	return 0;
}



CustomTask::CustomTask()
{

}

CustomTask::~CustomTask()
{

}

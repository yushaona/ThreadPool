#pragma once
#include "ThreadPool.h"

class CTask:public CustomTask
{
public:
	CTask();
	~CTask();
	void Run();
};

CTask::CTask()
{

}

CTask::~CTask()
{

}

void CTask::Run()
{
	int i = 0;
	while(i<10)
	{
		printf("test");
		i++;
	}
}

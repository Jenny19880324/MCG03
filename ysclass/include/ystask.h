/* ////////////////////////////////////////////////////////////

YS Class Library
Copyright (c) 2014 Soji Yamakawa.  All rights reserved.

YS Class Library is an open-source class-library project since 1999.
It has been used and battle-tested in commercial, research, and free programs.
Please visit the following URL for more details.

http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File Name: ystask.h
//////////////////////////////////////////////////////////// */

#ifndef YSTASK_IS_INCLUDED
#define YSTASK_IS_INCLUDED
/* { */

#include "ysdef.h"
#include "ysarray.h"
#include <thread>

/*! YsTask class is a base class for creating a task.  
    A sub-class of this class must implement Run(void) function, which will be called from Start(void *contextPtr) function. */
class YsTask
{
protected:
	/*! Actual task.  A sub-class must implement this function.  */
	virtual void Run(void)=0;
public:
	enum TASKSTATE
	{
		STATE_IDLE,
		STATE_RUNNING,
		STATE_FINISHED
	};

private:
	TASKSTATE state;

public:
	/*! Default constructor. */
	YsTask();

	/*! This function changes the state from STATE_FINISHED to STATE_IDLE.
        It cannot change the state if the thread state is still STATE_RUNNING. 
        It will return YSOK if it succeeds, or will fail and return YSERR if the thread state is STATE_RUNNING. */
	YSRESULT SetIdle(void);

	/*! Returns the state of the task. */
	TASKSTATE GetState(void) const;

	/*! This is an entry-point function that can be called from a thread library or any external library.
	    The first parameter, contextPtr, must be a pointer to a sub-class of YsTask. 
		This function will not start a task unless the task state is STATE_IDLE.

	    Example:

			#include <thread>

			class YsActualTask : public YsTask;

			YsActualTask task;
			std::thread t(YsTask::Start,&task);
			t.detach();

			while(YSOK!=task.SetIdle())
			{
				// Wait until the task is done.
			}
	    */
	static void Start(void *contextPtr);

	/*! This function runs the task in the same thread.
	    This function can be useful to run the task in a single-threaded environment.
	    Or, it can also be used for running a task in the main thread while other tasks are running in the sub-threads. 
	    This function can run the task only when the state is STATE_IDLE, in which case the function will return YSOK.
	    Otherwise, it will return YSERR. */
	YSRESULT StartLocal(void);

	/*! This function runs multiple tasks in parallel. */
	static void RunParallel(int nTask,YsTask *const task[]);

	/*! This function runs multiple tasks in parallel. */
	template <const int N>
	static void RunParallel(const YsArray <YsTask *,N> &taskArray);

	/*! This function runs multiple tasks in parallel.
	    T needs to be a class derived from YsTask.  
	    Tasks in 'taskArray' may be input and output.  Therefore it shouldn't made const. */
	template <class T,const int N>
	static void RunParallelDirect(YsArray <T,N> &taskArray);
};

template <const int N>
/*static*/ void YsTask::RunParallel(const YsArray <YsTask *,N> &taskArray)
{
	YsTask::RunParallel(taskArray.GetN(),taskArray);
}

template <class T,const int N>
/*static*/ void YsTask::RunParallelDirect(YsArray <T,N> &taskArray)
{
	if(1==taskArray.GetN())
	{
		taskArray[0].StartLocal();
	}
	else if(0<taskArray.GetN())
	{
		const auto nTask=taskArray.GetN();
		YsArray <std::thread,64> threadArray;
		threadArray.Set(nTask-1,NULL);
		for(int idx=0; idx<nTask-1; ++idx)
		{
			std::thread t(YsTask::Start,&taskArray[idx]);
			threadArray[idx]=(std::thread &&)t;
		}
		taskArray[nTask-1].StartLocal();
		for(int idx=0; idx<nTask-1; ++idx)
		{
			threadArray[idx].join();
		}
	}
}

/* } */
#endif

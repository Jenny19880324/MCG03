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

File Name: ystask.cpp
//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include "ystask.h"
#include "ysarray.h"
#include <thread>

YsTask::YsTask()
{
	state=STATE_IDLE;
}

YSRESULT YsTask::SetIdle(void)
{
	if(STATE_RUNNING==state)
	{
		return YSERR;
	}
	state=STATE_IDLE;
	return YSOK;
}

YsTask::TASKSTATE YsTask::GetState(void) const
{
	return state;
}

/*static*/ void YsTask::Start(void *contextPtr)
{
	YsTask *taskPtr=(YsTask *)contextPtr;
	if(STATE_IDLE==taskPtr->state)
	{
		taskPtr->state=STATE_RUNNING;
		taskPtr->Run();
		taskPtr->state=STATE_FINISHED;
		// The state may be changed by the controlling thread here.  However, it won't affect anything at this point.
	}
}

YSRESULT YsTask::StartLocal(void)
{
	if(STATE_IDLE==state)
	{
		state=STATE_RUNNING;
		Run();
		state=STATE_FINISHED;
		return YSOK;
	}
	return YSERR;
}

/*static*/ void YsTask::RunParallel(int nTask,YsTask *const task[])
{
	if(1==nTask)
	{
		task[0]->StartLocal();
	}
	else if(0<nTask)
	{
		YsArray <std::thread,64> threadArray;
		threadArray.Set(nTask-1,NULL);
		for(int idx=0; idx<nTask-1; ++idx)
		{
			std::thread t(YsTask::Start,task[idx]);
			threadArray[idx]=(std::thread &&)t;
		}
		task[nTask-1]->StartLocal();
		for(int idx=0; idx<nTask-1; ++idx)
		{
			threadArray[idx].join();
		}
	}
}

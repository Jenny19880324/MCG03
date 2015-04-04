#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "fssimplewindow.h"

extern "C" void FsOpenWindowC(int x0,int y0,int wid,int hei,int useDoubleBuffer);
extern "C" void FsGetWindowSizeC(int *wid,int *hei);
extern "C" void FsPollDeviceC(void);
extern "C" void FsSleepC(int ms);
extern "C" int FsPassedTimeC(void);
extern "C" void FsMouseC(int *lb,int *mb,int *rb,int *mx,int *my);
extern "C" int FsGetMouseEventC(int *lb,int *mb,int *rb,int *mx,int *my);
extern "C" void FsSwapBufferC(void);
extern "C" int FsInkeyC(void);
extern "C" int FsInkeyCharC(void);
extern "C" int FsKeyStateC(int fsKeyCode);
extern "C" void FsChangeToProgramDirC(void);
extern "C" int FsCheckExposureC(void);

void FsOpenWindow(int x0,int y0,int wid,int hei,int useDoubleBuffer)
{
	FsOpenWindowC(x0,y0,wid,hei,useDoubleBuffer);
}

void FsCloseWindow(void)
{
}

void FsGetWindowSize(int &wid,int &hei)
{
	FsGetWindowSizeC(&wid,&hei);
}

void FsPollDevice(void)
{
	FsPollDeviceC();
}

void FsSleep(int ms)
{
	FsSleepC(ms);
}

int FsPassedTime(void)
{
	return FsPassedTimeC();
}

void FsGetMouseState(int &lb,int &mb,int &rb,int &mx,int &my)
{
	FsMouseC(&lb,&mb,&rb,&mx,&my);
}

int FsGetMouseEvent(int &lb,int &mb,int &rb,int &mx,int &my)
{
	return FsGetMouseEventC(&lb,&mb,&rb,&mx,&my);
}

void FsSwapBuffers(void)
{
	FsSwapBufferC();
}

int FsInkey(void)
{
	return FsInkeyC();
}

int FsInkeyChar(void)
{
	return FsInkeyCharC();
}

int FsGetKeyState(int fsKeyCode)
{
	return FsKeyStateC(fsKeyCode);
}

int FsCheckWindowExposure(void)
{
	return FsCheckExposureC();
}

void FsChangeToProgramDir(void)
{
	FsChangeToProgramDirC();
}

void FsClearEventQueue(void)
{
	for(;;)
	{
		int checkAgain=0;

		FsPollDevice();

		int lb,mb,rb,mx,my;
		while(FSMOUSEEVENT_NONE!=FsGetMouseEvent(lb,mb,rb,mx,my) ||
		      FSKEY_NULL!=FsInkey() ||
		      0!=FsInkeyChar() ||
		      0!=FsCheckWindowExposure())
		{
			checkAgain=1;
		}

		if(0!=lb || 0!=rb || 0!=mb)
		{
			checkAgain=1;
		}

		if(0!=FsCheckKeyHeldDown())
		{
			checkAgain=1;
		}

		if(0==checkAgain)
		{
			break;
		}

		FsSleep(50);
	}
}

int FsCheckKeyHeldDown(void)
{
	int keyCode;
	for(keyCode=FSKEY_NULL+1; keyCode<FSKEY_NUM_KEYCODE; keyCode++)
	{
		if(0!=FsGetKeyState(keyCode))
		{
			return 1;
		}
	}
	return 0;
}

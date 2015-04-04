#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "fssimplewindow.h"

extern "C" void FsOpenWindowC(int x0,int y0,int wid,int hei,int useDoubleBuffer,const char windowName[]);
extern "C" void FsGetWindowSizeC(int *wid,int *hei);
extern "C" void FsGetWindowPositionC(int *wid,int *hei);
extern "C" void FsMaximizeWindowC(void);
extern "C" void FsPollDeviceC(void);
extern "C" void FsSleepC(int ms);
extern "C" int FsPassedTimeC(void);
extern "C" long long int FsSubSecondTimerC(void);
extern "C" void FsMouseC(int *lb,int *mb,int *rb,int *mx,int *my);
extern "C" int FsGetMouseEventC(int *lb,int *mb,int *rb,int *mx,int *my);
extern "C" void FsSwapBufferC(void);
extern "C" int FsInkeyC(void);
extern "C" int FsInkeyCharC(void);
extern "C" int FsKeyStateC(int fsKeyCode);
extern "C" void FsChangeToProgramDirC(void);
extern "C" int FsCheckExposureC(void);



static void (*IntervalCallBack)(void *)=NULL;
static void *IntervalCallBackParameter=NULL;

void FsRegisterIntervalCallBack(void (*callback)(void *),void *param)
{
	IntervalCallBack=callback;
	IntervalCallBackParameter=param;
}



static int doubleBuffer=1;
static int windowOpen=0;

void FsOpenWindow(int x0,int y0,int wid,int hei,int useDoubleBuffer)
{
	FsOpenWindow(x0,y0,wid,hei,useDoubleBuffer,NULL);
}

void FsOpenWindow(int x0,int y0,int wid,int hei,int useDoubleBuffer,const char windowName[])
{
	if(0!=windowOpen)
	{
		printf("Error! Window is already open.\n");
		exit(1);
	}

	if(NULL==windowName)
	{
		FsOpenWindowC(x0,y0,wid,hei,useDoubleBuffer,"Main Window");
	}
	else
	{
		FsOpenWindowC(x0,y0,wid,hei,useDoubleBuffer,windowName);
	}

	FsPassedTimeC();  // Resetting the timer.
	doubleBuffer=useDoubleBuffer;
	windowOpen=1;
}

void FsCloseWindow(void)
{
}

void FsGetWindowSize(int &wid,int &hei)
{
	FsGetWindowSizeC(&wid,&hei);
}

void FsGetWindowPosition(int &x0,int &y0)
{
	FsGetWindowPositionC(&x0,&y0);
}

void FsMaximizeWindow(void)
{
	FsMaximizeWindowC();
}

void FsPollDevice(void)
{
	FsPollDeviceC();
}

void FsSleep(int ms)
{
	FsSleepC(ms);
}

long long int FsPassedTime(void)
{
	static long long int lastTick;
	long long int tick;

	static int first=1;
	if(1==first)
	{
		lastTick=FsSubSecondTimer();
		first=0;
	}

	tick=FsSubSecondTimer();
	long long passed=tick-lastTick;
	lastTick=tick;

	return passed;
}

long long int FsSubSecondTimer(void)
{
	return FsSubSecondTimerC();
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
	if(0==doubleBuffer)
	{
		printf("Error! FsSwapBufferes called in a single-buffered application.\n");
		exit(1);
	}

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


#include "../fssimplewindow.h"
#include "fssimplewindowios.h"
#include <stdio.h>

class FsMouseEventLog
{
public:
	int eventType,lb,mb,rb,mx,my;
};

class FsTouchCache
{
public:
	int x,y;
};

// Mouse Emulation
#define NEVTBUF 256
static int nMosBufUsed=0;
static FsMouseEventLog mosBuffer[NEVTBUF];
static FsMouseEventLog lastKnownMos;
static int nTouchCacheUsed=0;
static FsTouchCache touchCache[NEVTBUF];

static int exposure=0;

static int winWid=0,winHei=0;


extern "C" void FsIOSReportWindowSize(int wid,int hei)
{
	winWid=wid;
	winHei=hei;
}

extern "C" void FsIOSReportFirstTouchBegan(int x,int y)
{
	lastKnownMos.eventType=FSMOUSEEVENT_LBUTTONDOWN;
	lastKnownMos.lb=1;
	lastKnownMos.mb=0;
	lastKnownMos.rb=0;
	lastKnownMos.mx=x;
	lastKnownMos.my=y;
	if(NEVTBUF>nMosBufUsed)
	{
		mosBuffer[nMosBufUsed++]=lastKnownMos;
	}
	printf("%s %d %d\n",__FUNCTION__,x,y);
}

extern "C" void FsIOSReportTouchMoved(int x,int y)
{
	lastKnownMos.eventType=FSMOUSEEVENT_MOVE;
	lastKnownMos.lb=1;
	lastKnownMos.mb=0;
	lastKnownMos.rb=0;
	lastKnownMos.mx=x;
	lastKnownMos.my=y;
	if(NEVTBUF>nMosBufUsed)
	{
		mosBuffer[nMosBufUsed++]=lastKnownMos;
	}
	printf("%s %d %d\n",__FUNCTION__,x,y);
}

extern "C" void FsIOSReportLastTouchEnded(int x,int y)
{
	lastKnownMos.eventType=FSMOUSEEVENT_LBUTTONUP;
	lastKnownMos.lb=0;
	lastKnownMos.mb=0;
	lastKnownMos.rb=0;
	lastKnownMos.mx=x;
	lastKnownMos.my=y;
	if(NEVTBUF>nMosBufUsed)
	{
		mosBuffer[nMosBufUsed++]=lastKnownMos;
	}
	printf("%s %d %d\n",__FUNCTION__,x,y);
}

extern "C" void FsIOSClearTouchCache(void)
{
	nTouchCacheUsed=0;
}

extern "C" void FsIOSAddTouchCache(int x,int y)
{
	if(NEVTBUF>nTouchCacheUsed)
	{
		touchCache[nTouchCacheUsed].x=x;
		touchCache[nTouchCacheUsed].y=y;
		++nTouchCacheUsed;
	}
}

extern void FsSimpleWindowIntervalCallBack(void);

extern "C" void FsIOSIntervalCallBack(void)
{
	FsSimpleWindowIntervalCallBack();
}


void FsOpenWindow(int,int,int,int,int)
{
}

void FsOpenWindow(int,int,int,int,int,const char *)
{
}

void FsCloseWindow(void)
{
}

void FsGetWindowSize(int &wid,int &hei)
{
	wid=winWid;
	hei=winHei;
}

void FsGetWindowPosition(int &x0,int y0)
{
	x0=0;
	y0=0;
}

void FsPollDevice(void)
{
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
	lb=lastKnownMos.lb;
	mb=lastKnownMos.mb;
	rb=lastKnownMos.rb;
	mx=lastKnownMos.mx;
	my=lastKnownMos.my;
}

int FsGetMouseEvent(int &lb,int &mb,int &rb,int &mx,int &my)
{
	if(0<nMosBufUsed)
	{
		const int eventType=mosBuffer[0].eventType;
		lb=mosBuffer[0].lb;
		mb=mosBuffer[0].mb;
		rb=mosBuffer[0].rb;
		mx=mosBuffer[0].mx;
		my=mosBuffer[0].my;

		for(int i=0; i<nMosBufUsed; ++i)
		{
			mosBuffer[i]=mosBuffer[i+1];
		}
		--nMosBufUsed;
		return eventType;
	}
	else 
	{
		FsGetMouseState(lb,mb,rb,mx,my);
		return FSMOUSEEVENT_NONE;
	}
}

void FsSwapBuffers(void)
{
	FsSwapBuffersC();
}

int FsInkey(void)
{
}

int FsInkeyChar(void)
{
}

int FsGetKeyState(int fsKeyCode)
{
}

int FsCheckKeyHeldDown(void)
{
}

int FsCheckWindowExposure(void)
{
}

void FsClearEventQueue(void)
{
}

void FsChangeToProgramDir(void)
{
	FsChangeToProgramDirC();
}

static const char *const keyCodeToStr[]=
{
	"NULL",
	"SPACE",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"ESC",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"PRINTSCRN",
	"CAPSLOCK",
	"SCROLLLOCK",
	"PAUSEBREAK",
	"BS",
	"TAB",
	"ENTER",
	"SHIFT",
	"CTRL",
	"ALT",
	"INS",
	"DEL",
	"HOME",
	"END",
	"PAGEUP",
	"PAGEDOWN",
	"UP",
	"DOWN",
	"LEFT",
	"RIGHT",
	"NUMLOCK",
	"TILDA",
	"MINUS",
	"PLUS",
	"LBRACKET",
	"RBRACKET",
	"BACKSLASH",
	"SEMICOLON",
	"SINGLEQUOTE",
	"COMMA",
	"DOT",
	"SLASH",
	"TEN0",
	"TEN1",
	"TEN2",
	"TEN3",
	"TEN4",
	"TEN5",
	"TEN6",
	"TEN7",
	"TEN8",
	"TEN9",
	"TENDOT",
	"TENSLASH",
	"TENSTAR",
	"TENMINUS",
	"TENPLUS",
	"TENENTER",
	"WHEELUP",
	"WHEELDOWN"
};

const char *FsKeyCodeToString(int keyCode)
{
	if(0<=keyCode && keyCode<FSKEY_NUM_KEYCODE)
	{
		return keyCodeToStr[keyCode];
	}
	return "(Undefined keycode)";
}


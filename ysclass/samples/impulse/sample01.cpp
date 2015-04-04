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

File Name: sample01.cpp
//////////////////////////////////////////////////////////// */

//cl sample??.cpp /DWIN32 ysclass.lib kernel32.lib user32.lib gdi32.lib advapi32.lib impls32d.lib

#include <impulse.h>

#include <stdio.h>
#include <stdlib.h>

#include <ysclass.h>

void InitYsClass(void);

class YsClassSample
{
protected:
	YsVec2 pointOfInterest;
	YsVec2 p[6];
	int lastX,lastY;

	void PreparePolygon(YsVec2 p[]);
	void Display(void);
	void Motion(int mx,int my);
public:
	void MainLoop(void);
};

#define BIWORKSIZE 0x60000L
char BiWork[BIWORKSIZE];

int main(int argc, char** argv)
{
	BIPROJ prj;

	BiMemoryInitialize();
	BiCalcInitialize();
	BiGraphInitialize(BiWork,BIWORKSIZE);
	BiDeviceInitialize();
	BiRenderInitialize();

	BiSetWindowName("YsClass Sample Application");
	BiOpenWindow(640,480);

	BiGetStdProjection(&prj);
	BiSetProjection(&prj);
	BiSetShadMode(BI_ON);

	YsClassSample sam;
	sam.MainLoop();

	BiCloseWindow();

	return 0;
}

void YsClassSample::MainLoop(void)
{
	int key;
	int lb,mb,rb;
	long mx,my;

	pointOfInterest.Set(0.0,0.0);
	PreparePolygon(p);

	BiUpdateDevice();
	BiMouse(&lb,&mb,&rb,&mx,&my);

	while((key=BiInkey())!=BIKEY_ESC)
	{
		BIPROJ prj;

		BiUpdateDevice();
		BiGetStdProjection(&prj);
		BiSetProjection(&prj);
		Display();

		lastX=mx;
		lastY=my;
		BiMouse(&lb,&mb,&rb,&mx,&my);
		if(lb==BI_ON && (lastX!=mx || lastY!=my))
		{
			Motion(mx,my);
		}
	}

}

void YsClassSample::PreparePolygon(YsVec2 v[])
{
	v[0].Set( 0.0, 5.0);
	v[1].Set( 5.0, 5.0);
	v[2].Set( 5.0, 0.0);
	v[3].Set( 0.0,-5.0);
	v[4].Set(-5.0,-5.0);
	v[5].Set(-5.0, 0.0);
}

////////////////////////////////////////////////////////////

void YsClassSample::Display(void)
{
	long mx,my;
	int lb,mb,rb;

	int i;
	BIPOINT plg[256],poi;

	BiMouse(&lb,&mb,&rb,&mx,&my);

	BiClearScreen();
	BiStartBuffer(&BiOrgPA);

	for(i=0; i<6; i++)
	{
		BiSetPoint(&plg[i],p[i].x(),p[i].y(),10.0);
	}
	BiOvwPolyg(6,plg,&BiWhite);


	BiSetPoint(&poi,pointOfInterest.x(),pointOfInterest.y(),10.0);
	switch(YsCheckInsidePolygon2(pointOfInterest,6,p))
	{
	case YSINSIDE:
		BiOvwMarker(&poi,&BiGreen,BIMK_CROSS);
		break;
	case YSOUTSIDE:
		BiOvwMarker(&poi,&BiBlue,BIMK_CROSS);
		break;
	case YSBOUNDARY:
		BiOvwMarker(&poi,&BiYellow,BIMK_CROSS);
		break;
	case YSUNKNOWNSIDE:
		BiOvwMarker(&poi,&BiRed,BIMK_CROSS);
		break;
	}

	BiFlushBuffer();
	BiSwapBuffers();
}

void YsClassSample::Motion(int x,int y)
{
	double dx,dy;
	YsVec2 vec;

	dx=double(x-lastX);
	dy=double(lastY-y);

	vec.Set(dx/30.0,dy/30.0);
	pointOfInterest=pointOfInterest+vec;
}


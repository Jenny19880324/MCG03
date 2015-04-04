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

File Name: sample04.cpp
//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <ysclass.h>

char vScreen[24][75];

void ClearVirtualScreen(void);
void DrawLine(int x1,int y1,int x2,int y2,char dot);
void ExposeVirtualScreen(void);

YsDrawLineByDDA ddaDriver;

int main(void)
{
	ClearVirtualScreen();
	DrawLine(3,5,60,20,'A');
	DrawLine(15,3,5,20,'B');
	DrawLine(0,23,50,0,'C');
	DrawLine(60,5,70,15,'D');
	DrawLine(70,5,60,15,'E');
	DrawLine(60,5,70,5,'F');
	DrawLine(70,15,60,15,'G');
	DrawLine(60,5,60,15,'H');
	DrawLine(70,15,70,5,'I');
	ExposeVirtualScreen();
	return 0;
}

void ClearVirtualScreen(void)
{
	int i,j;
	for(i=0; i<24; i++)
	{
		for(j=0; j<75; j++)
		{
			vScreen[i][j]='.';
		}
		vScreen[i][74]=0;
	}
}

void DrawLine(int x1,int y1,int x2,int y2,char dot)
{
	ddaDriver.Set(x1,y1,x2,y2);
	vScreen[y1][x1]=dot;
	while(ddaDriver.ReachedToTheEnd()!=YSTRUE)
	{
		int x,y;
		ddaDriver.MoveOneStep();
		ddaDriver.GetPosition(x,y);
		vScreen[y][x]=dot;
	}
}

void ExposeVirtualScreen(void)
{
	int i;
	for(i=0; i<24; i++)
	{
		printf("%s\n",vScreen[i]);
	}
}

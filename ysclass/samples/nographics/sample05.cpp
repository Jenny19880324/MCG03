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

File Name: sample05.cpp
//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <ysclass.h>

char vScreen[24][75];

void ClearVirtualScreen(void);
void DrawPolygon(int np,int p[],char dot);
void ExposeVirtualScreen(void);

YsScreenPolygon <16,24> plgDriver;

int main(void)
{
	int plg0[]=
	{
		2,2,
		70,2,
		70,21,
		2,21
	};
	int plg1[]=
	{
		10,3,
		60,5,
		30,20
	};
	int plg2[]=
	{
		5,10,
		10,18,
		20,15,
		60,19,
		50,5,
		40,10,
		20,5
	};

	ClearVirtualScreen();
	DrawPolygon(4,plg0,'+');
	DrawPolygon(3,plg1,'*');
	DrawPolygon(7,plg2,'-');
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

void DrawPolygon(int np,int p[],char dot)
{
	int i,n,*buf,x,y;
	int miny,maxy;

	plgDriver.MakeHorizontalIntersectionBuffer(np,p);
	plgDriver.GetRangeOfY(miny,maxy);
	for(y=miny; y<=maxy; y++)
	{
		n=plgDriver.GetNumIntersectionAtY(y);
		buf=plgDriver.GetIntersectionBufferAtY(y);
		for(i=0; i<n; i+=2)
		{
			for(x=buf[i]; x<=buf[i+1]; x++)
			{
				vScreen[y][x]=dot;
			}
		}
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

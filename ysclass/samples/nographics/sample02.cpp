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

File Name: sample02.cpp
//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <ysclass.h>

// For more information of YsMatrix4x4, please refer to the
// online help.

// You may also find interesting features in YsAtt3, YsRotation

void ShowMatrix(YsMatrix4x4 &m,char caption[]);

int main(void)
{
	YsMatrix4x4 a,b,c;

	a.Initialize();
	a.RotateXZ(YsDegToRad(30.0));
	a.RotateZY(YsDegToRad(45.0));
	a.RotateXY(YsDegToRad(60.0));

	b=a;
	b.Invert();

	c=a*b;

	ShowMatrix(a,"Matrix A");
	ShowMatrix(b,"Matrix B is inverse of Matrix A");
	ShowMatrix(c,"Matrix A x Matrix B must be an identity matrix");

	return 0;
}

void ShowMatrix(YsMatrix4x4 &m,char caption[])
{
	int r,c;
	double v;

	printf("[%s]\n",caption);

	for(r=1; r<=4; r++)
	{
		for(c=1; c<=4; c++)
		{
			v=m.v(r,c);
			printf(" %+.4lf",v);
		}
		printf("\n");
	}
}


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

#include <stdio.h>
#include <ysclass.h>

// Example of
//   YsCheckInsidePolygon2
// This function is for 2D polygon. But you also can use
//   YsCheckInsidePolygon3
// in a same manner. Please refer to the function reference.

const int nVtx=5;
double vtxTemplate[nVtx][2]=
{
	{10,10},{40,5},{60,15},{50,20},{10,16}
};


int main(void)
{
	int i,x,y;
	YsVec2 vtx[nVtx];
	YsVec2 poi;  // Point of interest

	for(i=0; i<nVtx; i++)
	{
		vtx[i].Set(vtxTemplate[i]);
	}

	for(y=0; y<24; y++)
	{
		for(x=0; x<75; x++)
		{
			poi.Set(double(x),double(y));
			switch(YsCheckInsidePolygon2(poi,nVtx,vtx))
			{
			case YSINSIDE:
				printf("I");
				break;
			case YSOUTSIDE:
				printf("O");
				break;
			case YSBOUNDARY:
				printf("B");
				break;
			case YSUNKNOWNSIDE:
				printf("?");
				break;
			}
		}
		printf("\n");
	}

	return 0;
}

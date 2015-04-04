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

File Name: ysbishell.cpp
//////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <impulse.h>
#include <ysclass.h>

#include <ysbi.h>


void YsBiDrawShell(const YsShell &shl,const YsColor &col,YSBOOL originalColor)
{
	BIPOINT plg[256];
	BICOLOR c;
	int i,j;
	int nv,idx[256];

	if(originalColor!=YSTRUE)
	{
		c<<col;
	}

	for(i=0; i<shl.GetNumPolygon(); i++)
	{
		YsColor plCol;

		nv=shl.GetNumVertexOfPolygon(i);
		shl.GetVertexListOfPolygon(idx,256,i);
		for(j=0; j<nv; j++)
		{
			YsVec3 v;
			shl.GetVertexPosition(v,idx[j]);
			plg[j]<<v;
		}

		if(originalColor==YSTRUE)
		{
			shl.GetColorOfPolygon(plCol,i);
			c<<plCol;
		}

		BiInsPolyg(nv,plg,&c);
	}
}

void YsBiDrawShellWire(const YsShell &shl,const YsShellEdgeList &edgeList,const YsColor &col)
{
	int i,nEdge;
	BICOLOR c;

	c<<col;

	nEdge=edgeList.GetNumEdge();
	for(i=0; i<nEdge; i++)
	{
		YsShellVertexHandle vtHd1,vtHd2;
		YsVec3 vt1,vt2;

		edgeList.GetEdge(vtHd1,vtHd2,i);
		shl.GetVertexPosition(vt1,vtHd1);
		shl.GetVertexPosition(vt2,vtHd2);

		BIPOINT p1,p2;
		p1<<vt1;
		p2<<vt2;

		BiInsLine(&p1,&p2,&c);
	}
}

void YsBiDrawBoundingBox(const YsVec3 &min,const YsVec3 &max,const YsColor &col,YSBOOL fill)
{
	YsVec3 pos[8];
	pos[0].Set(min.x(),min.y(),min.z());
	pos[1].Set(max.x(),min.y(),min.z());
	pos[2].Set(max.x(),max.y(),min.z());
	pos[3].Set(min.x(),max.y(),min.z());
	pos[4].Set(min.x(),min.y(),max.z());
	pos[5].Set(max.x(),min.y(),max.z());
	pos[6].Set(max.x(),max.y(),max.z());
	pos[7].Set(min.x(),max.y(),max.z());

	if(fill==YSTRUE)
	{
		BIPOINT p[4];
		BICOLOR c;
		c<<col;

		p[0]<<pos[0];
		p[1]<<pos[1];
		p[2]<<pos[2];
		p[3]<<pos[3];
		BiInsPolyg(4,p,&c);

		p[0]<<pos[4];
		p[1]<<pos[5];
		p[2]<<pos[6];
		p[3]<<pos[7];
		BiInsPolyg(4,p,&c);

		p[0]<<pos[0];
		p[1]<<pos[1];
		p[2]<<pos[5];
		p[3]<<pos[4];
		BiInsPolyg(4,p,&c);

		p[0]<<pos[1];
		p[1]<<pos[2];
		p[2]<<pos[6];
		p[3]<<pos[5];
		BiInsPolyg(4,p,&c);

		p[0]<<pos[2];
		p[1]<<pos[3];
		p[2]<<pos[7];
		p[3]<<pos[6];
		BiInsPolyg(4,p,&c);

		p[0]<<pos[3];
		p[1]<<pos[0];
		p[2]<<pos[4];
		p[3]<<pos[7];
		BiInsPolyg(4,p,&c);
	}
	else
	{
		BIPOINT p1,p2;
		BICOLOR c;
		c<<col;

		p1<<pos[0];
		p2<<pos[1];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[1];
		p2<<pos[2];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[2];
		p2<<pos[3];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[3];
		p2<<pos[0];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[4];
		p2<<pos[5];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[5];
		p2<<pos[6];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[6];
		p2<<pos[7];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[7];
		p2<<pos[4];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[0];
		p2<<pos[4];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[1];
		p2<<pos[5];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[2];
		p2<<pos[6];
		BiInsLine(&p1,&p2,&c);

		p1<<pos[3];
		p2<<pos[7];
		BiInsLine(&p1,&p2,&c);
	}
}

void YsBiDrawWireframeSphere(const YsVec3 &cen,const double &rad,const YsColor &col)
{
	BICOLOR c;
	c<<col;

	int i,step;

	double x1,y1,x2,y2;
	YsVec3 v1,v2;
	BIPOINT p1,p2;

	step=30;
	x2=cos(YsDegToRad(double(360-step)))*rad;
	y2=sin(YsDegToRad(double(360-step)))*rad;
	for(i=0; i<360; i+=step)
	{
		x1=x2;
		y1=y2;
		x2=cos(YsDegToRad(double(i)))*rad;
		y2=sin(YsDegToRad(double(i)))*rad;

		v1.Set(x1,y1,0.0);
		v2.Set(x2,y2,0.0);
		p1<<(v1+cen);
		p2<<(v2+cen);
		BiInsLine(&p1,&p2,&c);

		v1.Set(x1,0.0,y1);
		v2.Set(x2,0.0,y2);
		p1<<(v1+cen);
		p2<<(v2+cen);
		BiInsLine(&p1,&p2,&c);

		v1.Set(0.0,x1,y1);
		v2.Set(0.0,x2,y2);
		p1<<(v1+cen);
		p2<<(v2+cen);
		BiInsLine(&p1,&p2,&c);
	}
}

void YsBiDrawWireframeEllipsoid
    (const YsVec3 &cen,const YsMatrix4x4 &rot,const double &a,const double &b,const double &c,const YsColor &col)
{
	YsMatrix4x4 tfm;
	tfm.Initialize();
	tfm.Translate(cen);
	tfm=tfm*rot;
	tfm.Scale(a,b,c);


	BICOLOR bicol;
	bicol<<col;

	int i,step;

	double x1,y1,x2,y2;
	YsVec3 v1,v2;
	BIPOINT p1,p2;

	step=3;
	x2=cos(YsDegToRad(double(360-step)));
	y2=sin(YsDegToRad(double(360-step)));
	for(i=0; i<360; i+=step)
	{
		x1=x2;
		y1=y2;
		x2=cos(YsDegToRad(double(i)));
		y2=sin(YsDegToRad(double(i)));

		v1.Set(x1,y1,0.0);
		v2.Set(x2,y2,0.0);
		p1<<tfm*v1;
		p2<<tfm*v2;
		BiInsLine(&p1,&p2,&bicol);

		v1.Set(x1,0.0,y1);
		v2.Set(x2,0.0,y2);
		p1<<tfm*v1;
		p2<<tfm*v2;
		BiInsLine(&p1,&p2,&bicol);

		v1.Set(0.0,x1,y1);
		v2.Set(0.0,x2,y2);
		p1<<tfm*v1;
		p2<<tfm*v2;
		BiInsLine(&p1,&p2,&bicol);
	}
}



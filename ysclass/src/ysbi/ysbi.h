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

File Name: ysbi.h
//////////////////////////////////////////////////////////// */

#ifndef YSBI_IS_INCLUDED
#define YSBI_IS_INCLUDED
/* { */

inline const BIPOINT &operator<<(BIPOINT &a,const YsVec3 &v)
{
	BiSetPoint(&a,v.x(),v.y(),v.z());
	return a;
}

inline const YsVec3 &operator<<(YsVec3 &v,const BIPOINT &a)
{
	v.Set(a.x,a.y,a.z);
	return v;
}


inline const BICOLOR &operator<<(BICOLOR &c,const YsColor &d)
{
	int r,g,b;
	d.GetIntRGB(r,g,b);
	BiSetColorRGB(&c,r,g,b);
	return c;
}

inline const YsColor &operator<<(YsColor &d,const BICOLOR &c)
{
	d.SetIntRGB(c.r,c.g,c.b);
	return d;
}

inline const BIANGLE &operator<<(BIANGLE &a,const YsAtt3 &b)
{
	BiSetAngleDeg(&a,YsRadToDeg(b.h()),YsRadToDeg(b.p()),YsRadToDeg(b.b()));
	return a;
}

inline const YsAtt3 &operator<<(YsAtt3 &aa,const BIANGLE &a)
{
	double h,p,b;
	h=double(a.h)*YsPi/32768;
	p=double(a.p)*YsPi/32768;
	b=double(a.b)*YsPi/32768;
	aa.Set(h,p,b);
	return aa;
}

inline const BIPOINT2 &operator<<(BIPOINT2 &v,const YsVec2 &a)
{
	BiSetPoint2(&v,a.x(),a.y());
	return v;
}

inline const BIPOINTS &operator<<(BIPOINTS &v,const YsVec2 &a)
{
	BiSetPointS(&v,a.x(),a.y());
	return v;
}

inline void YsBiMakeMatrix4x4(YsMatrix4x4 &mat,const BIPOSATT &posatt)
{
	YsVec3 pos;
	YsAtt3 att;

	pos<<posatt.p;
	att<<posatt.a;

	mat.Translate(pos);
	mat.Rotate(att);
}


// ysbishell.cpp
void YsBiDrawShell(const YsShell &shl,const YsColor &col,YSBOOL originalColor);
void YsBiDrawShellWire(const YsShell &shl,const YsShellEdgeList &edgeList,const YsColor &col);
void YsBiDrawBoundingBox(const YsVec3 &min,const YsVec3 &max,const YsColor &col,YSBOOL fill);
void YsBiDrawWireframeSphere(const YsVec3 &cen,const double &rad,const YsColor &col);
void YsBiDrawWireframeEllipsoid
    (const YsVec3 &cen,const YsMatrix4x4 &mat,const double &a,const double &b,const double &c,const YsColor &col);

/* } */
#endif

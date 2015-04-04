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

File Name: ysveci.h
//////////////////////////////////////////////////////////// */

#ifndef YSVECI_IS_INCLUDED
#define YSVECI_IS_INCLUDED
/* { */



class YsVec2i : protected YsFixedLengthArray <int,2>
{
public:
	inline YsVec2i()
	{
	}
	inline YsVec2i(int x,int y)
	{
		dat[0]=x;
		dat[1]=y;
	}
	YsVec2i(const char *x,const char *y);

	inline const int &operator[](int idx) const
	{
		return dat[idx];
	}
	inline int &operator[](int idx)
	{
		return dat[idx];
	}
	operator const int *() const
	{
		return dat;
	}
	const char *Text(char buf[],const char *fmt="%d %d") const;

	inline void AddX(const int &x)
	{
		dat[0]+=x;
	}
	inline void AddY(const int &y)
	{
		dat[1]+=y;
	}

	inline void SubX(const int &x)
	{
		dat[0]-=x;
	}
	inline void SubY(const int &y)
	{
		dat[1]-=y;
	}

	inline void MulX(const int &x)
	{
		dat[0]*=x;
	}
	inline void MulY(const int &y)
	{
		dat[1]*=y;
	}

	inline void DivX(const int &x)
	{
		dat[0]/=x;
	}
	inline void DivY(const int &y)
	{
		dat[1]/=y;
	}

	inline int x(void) const
	{
		return dat[0];
	}
	inline int y(void) const
	{
		return dat[1];
	}

	inline void Set(int x,int y)
	{
		dat[0]=x;
		dat[1]=y;
	}

	inline void SetX(const int &x)
	{
		dat[0]=x;
	}
	inline void SetY(const int &y)
	{
		dat[1]=y;
	}

	void RotateLeft90(void)
	{
		int x,y;
		x=dat[0];
		y=dat[1];
		dat[0]=-y;
		dat[1]=x;
	}
	void RotateRight90(void)
	{
		int x,y;
		x=dat[0];
		y=dat[1];
		dat[0]=y;
		dat[1]=-x;
	}
};

inline const YsVec2i operator-(const YsVec2i &a)
{
	return YsVec2i(-a.x(),-a.y());
}

inline const YsVec2i operator-(const YsVec2i &a,const YsVec2i &b)
{
	return YsVec2i(a.x()-b.x(),a.y()-b.y());
}

inline const YsVec2i operator+(const YsVec2i &a,const YsVec2i &b)
{
	return YsVec2i(a.x()+b.x(),a.y()+b.y());
}


////////////////////////////////////////////////////////////


class YsVec3i : protected YsFixedLengthArray <int,3>
{
public:
	inline YsVec3i()
	{
	}
	inline YsVec3i(int x,int y,int z)
	{
		dat[0]=x;
		dat[1]=y;
		dat[2]=z;
	}
	YsVec3i(const char *x,const char *y,const char *z);

	inline const int &operator[](int idx) const
	{
		return dat[idx];
	}
	inline int &operator[](int idx)
	{
		return dat[idx];
	}
	operator const int *() const
	{
		return dat;
	}
	const char *Text(char buf[],const char *fmt="%d %d %d") const;

	inline void AddX(const int &x)
	{
		dat[0]+=x;
	}
	inline void AddY(const int &y)
	{
		dat[1]+=y;
	}
	inline void AddZ(const int &z)
	{
		dat[2]+=z;
	}

	inline void SubX(const int &x)
	{
		dat[0]-=x;
	}
	inline void SubY(const int &y)
	{
		dat[1]-=y;
	}
	inline void SubZ(const int &z)
	{
		dat[2]-=z;
	}

	inline void MulX(const int &x)
	{
		dat[0]*=x;
	}
	inline void MulY(const int &y)
	{
		dat[1]*=y;
	}
	inline void MulZ(const int &z)
	{
		dat[2]*=z;
	}

	inline void DivX(const int &x)
	{
		dat[0]/=x;
	}
	inline void DivY(const int &y)
	{
		dat[1]/=y;
	}
	inline void DivZ(const int &z)
	{
		dat[2]/=z;
	}

	inline int x(void) const
	{
		return dat[0];
	}
	inline int y(void) const
	{
		return dat[1];
	}
	inline int z(void) const
	{
		return dat[2];
	}

	inline void Set(int x,int y,int z)
	{
		dat[0]=x;
		dat[1]=y;
		dat[2]=z;
	}

	inline void SetX(const int &x)
	{
		dat[0]=x;
	}
	inline void SetY(const int &y)
	{
		dat[1]=y;
	}
	inline void SetZ(const int &z)
	{
		dat[2]=z;
	}
};

inline const YsVec3i operator-(const YsVec3i &a)
{
	return YsVec3i(-a.x(),-a.y(),-a.z());
}

inline const YsVec3i operator-(const YsVec3i &a,const YsVec3i &b)
{
	return YsVec3i(a.x()-b.x(),a.y()-b.y(),a.z()-b.z());
}

inline const YsVec3i operator+(const YsVec3i &a,const YsVec3i &b)
{
	return YsVec3i(a.x()+b.x(),a.y()+b.y(),a.z()+b.z());
}


/* } */
#endif

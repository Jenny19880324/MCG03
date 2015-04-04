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

File Name: ysrange.h
//////////////////////////////////////////////////////////// */

#ifndef YSRANGE_IS_INCLUDED
#define YSRANGE_IS_INCLUDED
/* { */



/*! This class enables range-based for using an index.  It can be written as:
    YsArray <T> array;
    for(auto idx : array.AllIndex())
    {
        // Do something for array[idx]
    }
    Useful when something needs to be done before and after an array item. */
class YsIndexRange
{
public:
	YSSIZE_T arraySize;

	inline YsIndexRange(YSSIZE_T arraySizeIn) : arraySize(arraySizeIn)
	{
	}

	class iterator
	{
	public:
		YSSIZE_T arrayIndex,dir;
	private:
		inline void Forward(void)
		{
			arrayIndex+=dir;
		}
		inline void Backward(void)
		{
			arrayIndex-=dir;
		}
	public:
		inline iterator &operator++()
		{
			Forward();
			return *this;
		}
		inline iterator operator++(int)
		{
			iterator copy=*this;
			Forward();
			return copy;
		}
		inline iterator &operator--()
		{
			Backward();
			return *this;
		}
		inline iterator operator--(int)
		{
			iterator copy=*this;
			Backward();
			return copy;
		}
		inline bool operator==(const iterator &from)
		{
			return (arrayIndex==from.arrayIndex);
		}
		inline bool operator!=(const iterator &from)
		{
			return (arrayIndex!=from.arrayIndex);
		}
		inline YSSIZE_T &operator*()
		{
			return arrayIndex;
		}
	};

	/*! Support for STL-like iterator */
	inline iterator begin() const
	{
		iterator iter;
		iter.arrayIndex=0;
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	inline iterator end() const
	{
		iterator iter;
		iter.arrayIndex=arraySize;
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	inline iterator rbegin() const
	{
		iterator iter;
		iter.arrayIndex=arraySize-1;
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	inline iterator rend() const
	{
		iterator iter;
		iter.arrayIndex=-1;
		iter.dir=-1;
		return iter;
	}
};

inline YsIndexRange::iterator begin(YsIndexRange allIndex)
{
	return allIndex.begin();
}

inline YsIndexRange::iterator rbegin(YsIndexRange allIndex)
{
	return allIndex.rbegin();
}

inline YsIndexRange::iterator end(YsIndexRange allIndex)
{
	return allIndex.end();
}

inline YsIndexRange::iterator rend(YsIndexRange allIndex)
{
	return allIndex.rend();
}



/* } */
#endif

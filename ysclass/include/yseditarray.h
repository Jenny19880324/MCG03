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

File Name: yseditarray.h
//////////////////////////////////////////////////////////// */

#ifndef YSARRAYFOREDIT_IS_INCLUDED
#define YSARRAYFOREDIT_IS_INCLUDED
/* { */

#include "ysarray.h"
#include "yssegarray.h"


template <class T,const int bitShift>
class YsEditArray;

/* Memo:
  I really want to make YsEditArrayObjectHandle a local class of YsEditArray.
  However, for some reason, Visual C++ cannot deduce
    int operator==(void *ptr,typename const YsEditArray <T,bitShift>::ObjHandle &hd) and
    int operator!=(void *ptr,typename const YsEditArray <T,bitShift>::ObjHandle &hd).
  Forcing me to declare YsEditArrayObjectHandle outside YsEditArray.  I have also added:
    int operator==(int ptr,typename const YsEditArray <T,bitShift>::ObjHandle &hd) and
    int operator!=(int ptr,typename const YsEditArray <T,bitShift>::ObjHandle &hd),
  which cannot be found by Visual C++, either.  Why?
*/


/*!  This is an object handle for YsEditArray.
*/
template <class T,const int bitShift=8>
class YsEditArrayObjectHandle
{
friend class YsEditArray <T,bitShift>;

private:
	enum
	{
		NULLINDEX=0x7fffffff
	};

	YSSIZE_T index;

public:
	YsEditArrayObjectHandle();
	YsEditArrayObjectHandle(void *ptr);
	YsEditArrayObjectHandle(const YsEditArrayObjectHandle <T,bitShift> &incoming);

	inline const YsEditArrayObjectHandle &CopyFrom(void *ptr);
	inline const YsEditArrayObjectHandle &CopyFrom(const YsEditArrayObjectHandle <T,bitShift> &incoming);
	inline int IsEqual(void *ptr) const;
	inline int IsNotEqual(void *ptr) const;
	inline int IsEqual(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
	inline int IsNotEqual(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;

	inline const YsEditArrayObjectHandle &operator=(void *ptr);
	inline int operator==(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
	inline int operator!=(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
	inline int operator==(void *ptr) const;
	inline int operator!=(void *ptr) const;

	inline int operator<(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
	inline int operator<=(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
	inline int operator>(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
	inline int operator>=(const YsEditArrayObjectHandle <T,bitShift> &incoming) const;
};

template <class T,const int bitShift>
YsEditArrayObjectHandle <T,bitShift>::YsEditArrayObjectHandle()
{
	index=NULLINDEX;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle <T,bitShift>::YsEditArrayObjectHandle(void *ptr)
{
	CopyFrom(ptr);
}

template <class T,const int bitShift>
YsEditArrayObjectHandle <T,bitShift>::YsEditArrayObjectHandle(const YsEditArrayObjectHandle <T,bitShift> &incoming)
{
	CopyFrom(incoming);
}

template <class T,const int bitShift>
inline const YsEditArrayObjectHandle <T,bitShift> &YsEditArrayObjectHandle <T,bitShift>::CopyFrom(void *ptr)
{
	if(NULL==ptr)
	{
		this->index=NULLINDEX;
	}
	else
	{
		YsPrintf("%s\n",__FUNCTION__);
		YsPrintf("Only NULL pointer can be directly copied to YsEditArrayObjectHandle\n");
	}
	return *this;
}

template <class T,const int bitShift>
inline const YsEditArrayObjectHandle <T,bitShift> &YsEditArrayObjectHandle <T,bitShift>::CopyFrom(const YsEditArrayObjectHandle <T,bitShift> &incoming)
{
	this->index=incoming.index;
	return *this;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::IsEqual(void *ptr) const
{
	if(NULL==ptr)
	{
		return NULLINDEX==this->index;
	}
	YsPrintf("%s\n",__FUNCTION__);
	YsPrintf("Only NULL pointer can be directly compared with YsEditArrayObjectHandle\n");
	return 0;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::IsNotEqual(void *ptr) const
{
	if(NULL==ptr)
	{
		return NULLINDEX!=this->index;
	}
	YsPrintf("%s\n",__FUNCTION__);
	YsPrintf("Only NULL pointer can be directly compared with YsEditArrayObjectHandle\n");
	return 0;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::IsEqual(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return this->index==incoming.index;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::IsNotEqual(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return this->index!=incoming.index;
}

template <class T,const int bitShift>
inline const YsEditArrayObjectHandle <T,bitShift> &YsEditArrayObjectHandle <T,bitShift>::operator=(void *ptr)
{
	return CopyFrom(ptr);
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator==(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return IsEqual(incoming);
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator!=(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return IsNotEqual(incoming);
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator==(void *ptr) const
{
	return IsEqual(ptr);
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator!=(void *ptr) const
{
	return IsNotEqual(ptr);
}

template <class T,const int bitShift>
inline int operator==(void *ptr,const YsEditArrayObjectHandle <T,bitShift> &obHd)
{
	return obHd.IsEqual(ptr);
}

template <class T,const int bitShift>
inline int operator!=(void *ptr,const YsEditArrayObjectHandle <T,bitShift> &obHd)
{
	return obHd.IsNotEqual(ptr);
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator<(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return this->index<incoming.index;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator<=(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return this->index<=incoming.index;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator>(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return this->index>incoming.index;
}

template <class T,const int bitShift>
inline int YsEditArrayObjectHandle <T,bitShift>::operator>=(const YsEditArrayObjectHandle <T,bitShift> &incoming) const
{
	return this->index>=incoming.index;
}

////////////////////////////////////////////////////////////

// gcc cannot match begin(YsEditArray<T,bitShift>::HandleEnumerator. 
//    I am forced to contaminate the name space by adding YsEditArrayHandleEnumerator.
//    Am I missing anything?

template <class T,const int bitShift>
class YsEditArrayHandleEnumerator;

////////////////////////////////////////////////////////////

/*!  This is a template array class for editing.  Uses YsSegmentedArray as a background data structure.
*/
template <class T,const int bitShift=8>
class YsEditArray
{
public:
	/*! The object in the YsEditArray can be one of ALIVE, DELETED, or FROZEN.
	*/
	enum
	{
		ALIVE=0,
		DELETED=1,
		FROZEN=2
	};

private:
	// Actually I want to add Lock/Unlock

	YSHASHKEY localSearchKeySeed;
	YSHASHKEY *searchKeySeed;
	class ArrayObject
	{
	public:
		unsigned int attrib;  // bit0-1 STATE(ALIVE,DELETED,FROZEN)  bit 2-31 search key
		mutable YSSIZE_T cachedIndex;
		T dat;

		inline void SetSearchKey(unsigned int key)
		{
			attrib&=3;
			attrib|=(key<<2);
		}
		inline unsigned int GetSearchKey(void) const
		{
			return (attrib>>2);
		}
		inline void SetState(unsigned int state)
		{
			attrib&=~(unsigned int)3;
			attrib|=(state&3);
		}
		inline unsigned int GetState(void) const
		{
			return (attrib&3);
		}
	};

	YsSegmentedArray <ArrayObject,bitShift> objArray;
	YsArray <YSSIZE_T> deletedIndex; // <- Ready for recycling
	YSSIZE_T nFrozen;

	YSBOOL searchEnabled;
	YsHashTable <YSSIZE_T> searchKeyToIndex;

	mutable YSSIZE_T maxCachedIndex;
	mutable YsArray <YsEditArrayObjectHandle <T,bitShift> > indexToHandleCache;

public:
	class iterator
	{
	public:
		YsEditArray <T,bitShift> *array;
		YsEditArrayObjectHandle <T,bitShift> index;
		int dir;
	private:
		inline void Forward(void)
		{
			if(0<=dir)
			{
				array->MoveToNext(index);
			}
			else
			{
				array->MoveToPrev(index);;
			}
		}
		inline void Backward(void)
		{
			if(0<=dir)
			{
				array->MoveToPrev(index);
			}
			else
			{
				array->MoveToNext(index);;
			}
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
			return (array==from.array && index==from.index);
		}
		inline bool operator!=(const iterator &from)
		{
			return (array!=from.array || index!=from.index);
		}
		inline T &operator*()
		{
			return *(*array)[index];
		}
	};

	class const_iterator
	{
	public:
		const YsEditArray <T,bitShift> *array;
		YsEditArrayObjectHandle <T,bitShift> index;
		int dir;
	private:
		void Forward(void)
		{
			if(0<=dir)
			{
				array->MoveToNext(index);
			}
			else
			{
				array->MoveToPrev(index);;
			}
		}
		void Backward(void)
		{
			if(0<=dir)
			{
				array->MoveToPrev(index);
			}
			else
			{
				array->MoveToNext(index);;
			}
		}
	public:
		inline const_iterator &operator++()
		{
			Forward();;
			return *this;
		}
		inline const_iterator operator++(int)
		{
			auto copy=*this;
			Forward();
			return copy;
		}
		inline const_iterator &operator--()
		{
			Backward();
			return *this;
		}
		inline const_iterator operator--(int)
		{
			iterator copy=*this;
			Backward();
			return copy;
		}
		inline bool operator==(const const_iterator &from)
		{
			return (array==from.array && index==from.index);
		}
		inline bool operator!=(const const_iterator &from)
		{
			return (array!=from.array || index!=from.index);
		}
		inline const T &operator*()
		{
			return *(*array)[index];
		}
	};

	/*! Support for STL-like iterator */
	iterator begin()
	{
		iterator iter;
		iter.array=this;
		iter.index=this->FindNext(NULL);
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator end()
	{
		iterator iter;
		iter.array=this;
		iter.index=NULL;
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator begin() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=this->FindNext(NULL);
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator end() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=NULL;
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator rbegin()
	{
		iterator iter;
		iter.array=this;
		iter.index=this->FindPrev(NULL);
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator rend()
	{
		iterator iter;
		iter.array=this;
		iter.index=NULL;
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator rbegin() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=this->FindPrev(NULL);
		iter.dir=-1;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator rend() const
	{
		const_iterator iter;
		iter.array=this;
		iter.index=NULL;
		iter.dir=-1;
		return iter;
	}



public:
	/*! Default constructor
	*/
	YsEditArray();

	/*! Clears the array. */
	void CleanUp(void);

	/*! Make this object to use a shared external search-key seed.
	*/
	void UseSharedSearchKeySeed(YSHASHKEY &seedPtr);

	/*! Enable search table.  When the search is enabled, an array object can be
	    seached by FindObject or FindFrozenObject function.
	*/
	void EnableSearch(void);

	/*! Disable search table.
	*/
	void DisableSearch(void);

	/*! Returns number of objects ALIVE in the array.
	*/
	YSSIZE_T GetN(void) const;

	/*! Returns YSTRUE if the object is frozen (or tentatively deleted), or YSFALSE otherwise. */
	YSBOOL IsFrozen(YsEditArrayObjectHandle<T,bitShift> hd) const;

	/*! Create a new object and returns the handle.
	*/
	YsEditArrayObjectHandle<T,bitShift> Create(void);

	/*! Access the object. */
	T *operator[] (YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Access the object. */
	const T *operator[] (YsEditArrayObjectHandle<T,bitShift> hd) const;

	/*! Access the object that is tentatively deleted. */
	T *GetFrozenObject(YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Access the object that is tentatively deleted. */
	const T *GetFrozenObject(YsEditArrayObjectHandle<T,bitShift> hd) const;

	/*! Access the object that can be alive or can be tentatively deleted. */
	const T *GetObjectMaybeFrozen(YsEditArrayObjectHandle<T,bitShift> hd) const;

	/*! Access the object that can be alive or can be tentatively deleted. */
	T *GetObjectMaybeFrozen(YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Appends an object.  T must be safe for the copy operator. */
	YsEditArrayObjectHandle<T,bitShift> Append(const T &incoming);

	/*! Delete an object.  The state of the object pointed by the handle must be ALIVE.
	*/
	YSRESULT Delete(YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Freeze an object.  Frozen object is temporarily deleted and can later be melted (or undeleted.)
	*/
	YSRESULT Freeze(YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Melt (undelete) an object that was temporarily deleted by Freeze function.
	*/
	YSRESULT Melt(YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Delete a frozen object.  The state of the object pointed by the handle must be FROZEN.
	*/
	YSRESULT DeleteFrozen(YsEditArrayObjectHandle<T,bitShift> hd);

	/*! Find an object from a search key.  It can find an object that is ALIVE.
	*/
	YsEditArrayObjectHandle<T,bitShift> FindObject(unsigned int key) const;

	/*! Find a frozen object from a search key.
	*/
	YsEditArrayObjectHandle<T,bitShift> FindFrozenObject(unsigned int key) const;

	/*! Find next object of obHd that is ALIVE.  The object pointed by obHd does not have to be ALIVE.
	    If obHd==NULL, this function finds the next first object ALIVE and returns the handle to the next object.
	*/
	YsEditArrayObjectHandle <T,bitShift> FindNext(const YsEditArrayObjectHandle <T,bitShift> obHd) const;

	/*! Find previous object obHd that is ALIVE.  The object pointed by obHd does not have to be ALIVE.
	    If obHd==NULL, this function finds the last object ALIVE and returns the handle to the previous object.
	*/
	YsEditArrayObjectHandle <T,bitShift> FindPrev(const YsEditArrayObjectHandle <T,bitShift> obHd) const;

	/*! Move obHd to the next object that is ALIVE.  The object pointed by obHd does not have to be ALIVE.
	    If obHd==NULL, this function finds the next first object ALIVE.
	    If the next of obHd exists, this function returns YSOK.
	    If the next of obHd does not exist, this function returns YSERR, and obHd will be NULL.
	*/
	YSRESULT MoveToNext(YsEditArrayObjectHandle <T,bitShift> &obHd) const;

	/*! Move obHd to the previous object that is ALIVE.  The object pointed by obHd does not have to be ALIVE.
	    If obHd==NULL, this function finds the last object ALIVE.
	    If the previous of obHd exists, this function returns YSOK, and obHd will be updated to the handle to the previous object.
	    If the previous of obHd does not exist, this function returns YSERR, and obHd will be NULL.
	*/
	YSRESULT MoveToPrev(YsEditArrayObjectHandle <T,bitShift> &obHd) const;

	/*! Returns a search key (hash key) of the object.  Object pointed by obHd needs to be ALIVE. */
	unsigned int GetSearchKey(YsEditArrayObjectHandle <T,bitShift> &obHd) const;

	/*! Returns a search key (hash key) of the object.  Object pointed by obHd can be FROZEN. */
	unsigned int GetSearchKeyMaybeFrozen(YsEditArrayObjectHandle <T,bitShift> &obHd) const;

	/*! Cache index numbers per object so that the index can be founc quickly from a handle, and vise-versa. */
	void Encache(void) const;

	/*! Clear cache of index numbers per object so that the index can be founc quickly from a handle, and vise-versa. */
	void Decache(void) const;

	/*! Get a zero-based index from a handle. */
	YSSIZE_T GetIndexFromHandle(YsEditArrayObjectHandle <T,bitShift> obHd) const;

	/*! Get a handle from a zero-based index. */
	YsEditArrayObjectHandle <T,bitShift> GetObjectHandleFromIndex(YSSIZE_T index) const;

	/*! Get an all-handle object. 
	    Regular iterator is good, but not always convenient.  Rather than getting a reference to T, 
	    if often is convenient to iterate over the handles.  All-handle object will allow iterating like:
	        for(auto &hdl : array)
	        {
	            // hdl is of YsEditArrayObjectHandle <T,bitShift> &.
	        }
	   */
	YsEditArrayHandleEnumerator <T,bitShift> AllHandle(void) const;
};



template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::iterator begin(YsEditArray <T,bitShift> &array)
{
	return array.begin();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::iterator end(YsEditArray <T,bitShift> &array)
{
	return array.end();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::const_iterator begin(const YsEditArray <T,bitShift> &array)
{
	return array.begin();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::const_iterator end(const YsEditArray <T,bitShift> &array)
{
	return array.end();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::iterator rbegin(YsEditArray <T,bitShift> &array)
{
	return array.rbegin();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::iterator rend(YsEditArray <T,bitShift> &array)
{
	return array.rend();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::const_iterator rbegin(const YsEditArray <T,bitShift> &array)
{
	return array.rbegin();
}

template <class T,const int bitShift>
inline typename YsEditArray <T,bitShift>::const_iterator rend(const YsEditArray <T,bitShift> &array)
{
	return array.rend();
}



template <class T,const int bitShift>
YsEditArray <T,bitShift>::YsEditArray()
{
	localSearchKeySeed=0;
	searchKeySeed=&localSearchKeySeed;

	searchEnabled=YSFALSE;
	nFrozen=0;

	maxCachedIndex=-1;
	indexToHandleCache.CleanUp();
}

template <class T,const int bitShift>
void YsEditArray <T,bitShift>::CleanUp(void)
{
	objArray.Clear();
	deletedIndex.Clear();
	nFrozen=0;
	searchKeyToIndex.PrepareTable();;

	maxCachedIndex=-1;
	indexToHandleCache.CleanUp();
}

template <class T,const int bitShift>
void YsEditArray <T,bitShift>::UseSharedSearchKeySeed(YSHASHKEY &seedPtr)
{
	searchKeySeed=&seedPtr;
}

template <class T,const int bitShift>
void YsEditArray <T,bitShift>::EnableSearch(void)
{
	if(YSTRUE!=searchEnabled)
	{
		searchEnabled=YSTRUE;
		searchKeyToIndex.PrepareTable(1+objArray.GetN()/4);

		for(YsEditArrayObjectHandle<T,bitShift> ptr=NULL; YSOK==MoveToNext(ptr); )
		{
			YSSIZE_T index=ptr.index;
			searchKeyToIndex.AddElement(objArray[index].GetSearchKey(),index);
		}
	}
}

template <class T,const int bitShift>
void YsEditArray <T,bitShift>::DisableSearch(void)
{
	searchEnabled=YSFALSE;
}

template <class T,const int bitShift>
YSSIZE_T YsEditArray <T,bitShift>::GetN(void) const
{
	return objArray.GetN()-deletedIndex.GetN()-nFrozen;
}

template <class T,const int bitShift>
YSBOOL YsEditArray <T,bitShift>::IsFrozen(YsEditArrayObjectHandle<T,bitShift> obHd) const
{
	if(NULL!=obHd &&
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   FROZEN==objArray[obHd.index].GetState())
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle<T,bitShift> YsEditArray <T,bitShift>::Create(void)
{
	YSSIZE_T newIdx;
	if(0<deletedIndex.GetN())
	{
		newIdx=deletedIndex.GetEnd();
		deletedIndex.DeleteLast();
	}
	else
	{
		newIdx=objArray.GetN();
		objArray.Increment();
	}

	YsEditArrayObjectHandle<T,bitShift> obHd;
	obHd.index=newIdx;

	objArray[newIdx].SetState(ALIVE);
	objArray[newIdx].SetSearchKey(*searchKeySeed);

	if(YSTRUE==searchEnabled)
	{
		searchKeyToIndex.AddElement(*searchKeySeed,newIdx);
	}

	++(*searchKeySeed);

	Decache();

	return obHd;
}

template <class T,const int bitShift>
T *YsEditArray <T,bitShift>::operator[] (YsEditArrayObjectHandle<T,bitShift> hd)
{
	if(YSTRUE==objArray.IsInRange(hd.index) &&
	   ALIVE==objArray[hd.index].GetState())
	{
		return &objArray[hd.index].dat;
	}
	return NULL;
}

template <class T,const int bitShift>
const T *YsEditArray <T,bitShift>::operator[] (YsEditArrayObjectHandle<T,bitShift> hd) const
{
	if(YSTRUE==objArray.IsInRange(hd.index) &&
	   ALIVE==objArray[hd.index].GetState())
	{
		return &objArray[hd.index].dat;
	}
	return NULL;
}

template <class T,const int bitShift>
T *YsEditArray <T,bitShift>::GetFrozenObject(YsEditArrayObjectHandle<T,bitShift> hd)
{
	if(YSTRUE==objArray.IsInRange(hd.index) &&
	   FROZEN==objArray[hd.index].GetState())
	{
		return &objArray[hd.index].dat;
	}
	return NULL;
}

template <class T,const int bitShift>
const T *YsEditArray <T,bitShift>::GetFrozenObject(YsEditArrayObjectHandle<T,bitShift> hd) const
{
	if(YSTRUE==objArray.IsInRange(hd.index) &&
	   FROZEN==objArray[hd.index].GetState())
	{
		return &objArray[hd.index].dat;
	}
	return NULL;
}

template <class T,const int bitShift>
T *YsEditArray <T,bitShift>::GetObjectMaybeFrozen(YsEditArrayObjectHandle<T,bitShift> hd)
{
	if(YSTRUE==objArray.IsInRange(hd.index) &&
	   (FROZEN==objArray[hd.index].GetState() || ALIVE==objArray[hd.index].GetState()))
   {
		return &objArray[hd.index].dat;
	}
	return NULL;
}

template <class T,const int bitShift>
const T *YsEditArray <T,bitShift>::GetObjectMaybeFrozen(YsEditArrayObjectHandle<T,bitShift> hd) const
{
	if(YSTRUE==objArray.IsInRange(hd.index) &&
	   (FROZEN==objArray[hd.index].GetState() || ALIVE==objArray[hd.index].GetState()))
   {
		return &objArray[hd.index].dat;
	}
	return NULL;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle<T,bitShift> YsEditArray <T,bitShift>::Append(const T &incoming)
{
	YsEditArrayObjectHandle<T,bitShift> newObjHd=Create();
	(*(*this)[newObjHd])=incoming;
	return newObjHd;
}

template <class T,const int bitShift>
YSRESULT YsEditArray <T,bitShift>::Delete(YsEditArrayObjectHandle<T,bitShift> obHd)
{
	if(NULL!=obHd && 
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   ALIVE==objArray[obHd.index].GetState())
	{
		searchKeyToIndex.DeleteElement(objArray[obHd.index].GetSearchKey(),obHd.index);
		objArray[obHd.index].SetState(DELETED);
		deletedIndex.Append(obHd.index);

		Decache();

		return YSOK;
	}
	return YSERR;
}

template <class T,const int bitShift>
YSRESULT YsEditArray <T,bitShift>::Freeze(YsEditArrayObjectHandle<T,bitShift> obHd)
{
	if(NULL!=obHd &&
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   ALIVE==objArray[obHd.index].GetState())
	{
		objArray[obHd.index].SetState(FROZEN);
		++nFrozen;
		Decache();
		return YSOK;
	}
	return YSERR;
}

template <class T,const int bitShift>
YSRESULT YsEditArray <T,bitShift>::Melt(YsEditArrayObjectHandle<T,bitShift> obHd)
{
	if(NULL!=obHd &&
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   FROZEN==objArray[obHd.index].GetState())
	{
		objArray[obHd.index].SetState(ALIVE);
		--nFrozen;
		Decache();
		return YSOK;
	}
	return YSERR;
}

template <class T,const int bitShift>
YSRESULT YsEditArray <T,bitShift>::DeleteFrozen(YsEditArrayObjectHandle<T,bitShift> obHd)
{
	if(NULL!=obHd &&
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   FROZEN==objArray[obHd.index].GetState())
	{
		Melt(obHd);
		Delete(obHd);
		return YSOK;
	}
	return YSERR;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle<T,bitShift> YsEditArray <T,bitShift>::FindObject(unsigned int key) const
{
	YSSIZE_T index;
	if(YSOK==searchKeyToIndex.FindElement(index,key) &&
	   YSTRUE==objArray.IsInRange(index) &&
	   ALIVE==objArray[index].GetState())
	{
		YsEditArrayObjectHandle<T,bitShift> obHd;
		obHd.index=index;
		return obHd;
	}
	return NULL;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle<T,bitShift> YsEditArray <T,bitShift>::FindFrozenObject(unsigned int key) const
{
	YSSIZE_T index;
	if(YSOK==searchKeyToIndex.FindElement(index,key) &&
	   YSTRUE==objArray.IsInRange(index) &&
	   FROZEN==objArray[index].GetState())
	{
		YsEditArrayObjectHandle<T,bitShift> obHd;
		obHd.index=index;
		return obHd;
	}
	return NULL;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle <T,bitShift> YsEditArray <T,bitShift>::FindNext(const YsEditArrayObjectHandle <T,bitShift> obHd) const
{
	YsEditArrayObjectHandle <T,bitShift> nextObHd=obHd;
	if(YSOK==MoveToNext(nextObHd))
	{
		return nextObHd;
	}
	return NULL;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle <T,bitShift> YsEditArray <T,bitShift>::FindPrev(const YsEditArrayObjectHandle <T,bitShift> obHd) const
{
	YsEditArrayObjectHandle <T,bitShift> prevObHd=obHd;
	if(YSOK==MoveToPrev(prevObHd))
	{
		return prevObHd;
	}
	return NULL;
}

template <class T,const int bitShift>
YSRESULT YsEditArray <T,bitShift>::MoveToNext(YsEditArrayObjectHandle <T,bitShift> &obHd) const
{
	YSSIZE_T i0;
	if(NULL==obHd || YSTRUE!=objArray.IsInRange(obHd.index))
	{
		i0=0;
	}
	else
	{
		i0=obHd.index+1;
	}
	for(YSSIZE_T i=i0; i<objArray.GetN(); ++i)
	{
		if(ALIVE==objArray[i].GetState())
		{
			obHd.index=i;
			return YSOK;
		}
	}
	obHd=NULL;
	return YSERR;
}

template <class T,const int bitShift>
YSRESULT YsEditArray <T,bitShift>::MoveToPrev(YsEditArrayObjectHandle <T,bitShift> &obHd) const
{
	YSSIZE_T i0;
	if(NULL==obHd || YSTRUE!=objArray.IsInRange(obHd.index))
	{
		i0=objArray.GetN()-1;
	}
	else
	{
		i0=obHd.index-1;
	}
	for(YSSIZE_T i=i0; 0<=i; --i)
	{
		if(ALIVE==objArray[i].GetState())
		{
			obHd.index=i;
			return YSOK;
		}
	}
	obHd=NULL;
	return YSERR;
}

template <class T,const int bitShift>
unsigned int YsEditArray <T,bitShift>::GetSearchKey(YsEditArrayObjectHandle <T,bitShift> &obHd) const
{
	if(NULL!=obHd && 
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   ALIVE==objArray[obHd.index].GetState())
	{
		return objArray[obHd.index].GetSearchKey();
	}
	return ~(unsigned int)0;
}

template <class T,const int bitShift>
unsigned int YsEditArray <T,bitShift>::GetSearchKeyMaybeFrozen(YsEditArrayObjectHandle <T,bitShift> &obHd) const
{
	if(NULL!=obHd && 
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   (ALIVE==objArray[obHd.index].GetState() || FROZEN==objArray[obHd.index].GetState()))
	{
		return objArray[obHd.index].GetSearchKey();
	}
	return ~(unsigned int)0;
}

template <class T,const int bitShift>
void YsEditArray <T,bitShift>::Encache(void) const
{
	indexToHandleCache.CleanUp();
	maxCachedIndex=0;

	YSSIZE_T index=0;

	YsEditArrayObjectHandle <T,bitShift> obHd=NULL;
	while(YSOK==MoveToNext(obHd))
	{
		objArray[obHd.index].cachedIndex=index;
		indexToHandleCache.Append(obHd);
		++maxCachedIndex;
	}
}

template <class T,const int bitShift>
void YsEditArray <T,bitShift>::Decache(void) const
{
	maxCachedIndex=-1;
	indexToHandleCache.CleanUp();
}

template <class T,const int bitShift>
YSSIZE_T YsEditArray <T,bitShift>::GetIndexFromHandle(YsEditArrayObjectHandle <T,bitShift> obHd) const
{
	if(NULL!=obHd && 
	   YSTRUE==objArray.IsInRange(obHd.index) &&
	   objArray[obHd.index].cachedIndex<maxCachedIndex)
	{
		return objArray[obHd.index].cachedIndex;
	}
	return -1;
}

template <class T,const int bitShift>
YsEditArrayObjectHandle <T,bitShift> YsEditArray <T,bitShift>::GetObjectHandleFromIndex(YSSIZE_T index) const
{
	if(YSTRUE==indexToHandleCache.IsInRante(index))
	{
		return indexToHandleCache[index];
	}
	return NULL;
}

////////////////////////////////////////////////////////////

// This class is supposed to be YsEditArray <T,bitShift>::HandleEnumerator.
// However, gcc cannot match begin(YsEditArray <T,bitShift>::HandleEnumerator &).
// It actually find the right function as a candidate, but for some wrong reason, it thinks
// the function does not match.  Am I missing something? 
// gcc 4.6.3
template <class T,const int bitShift>
class YsEditArrayHandleEnumerator
{
public:
	const YsEditArray <T,bitShift> *array;

	class iterator : public YsEditArray <T,bitShift>::const_iterator
	{
	public:
		inline YsEditArrayObjectHandle <T,bitShift> &operator*()
		{
			// I don't think any ambiguity of the member variable index.
			// However, gcc cannot find it unless I add
			//   'YsEditArray <T,bitShift>::const_iterator::'
			// in front of index.
			// gcc 4.6.3
			return YsEditArray <T,bitShift>::const_iterator::index;
		}
	};

	/*! Support for STL-like iterator */
	iterator begin() const
	{
		iterator iter;
		iter.array=array;
		iter.index=array->FindNext(NULL);
		iter.dir=1;
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator end() const
	{
		iterator iter;
		iter.array=array;
		iter.index=NULL;
		iter.dir=1;
		return iter;
	}

	/*! Returns an array that contains all handles. */
	YsArray <YsEditArrayObjectHandle <T,bitShift> > Array(void) const
	{
		YsArray <YsEditArrayObjectHandle <T,bitShift> > hdArray(array->GetN(),NULL);
		YSSIZE_T idx=0;
		for(auto hd=begin(); hd!=end(); ++hd)
		{
			hdArray[idx]=*hd;
			++idx;
		}
		return hdArray;
	}
};

template <class T,const int bitShift>
typename YsEditArrayHandleEnumerator <T,bitShift>::iterator begin(const YsEditArrayHandleEnumerator <T,bitShift> &allHandle)
{
	return allHandle.begin();
}

template <class T,const int bitShift>
typename YsEditArrayHandleEnumerator <T,bitShift>::iterator end(const YsEditArrayHandleEnumerator <T,bitShift> &allHandle)
{
	return allHandle.end();
}

template <class T,const int bitShift>
typename YsEditArrayHandleEnumerator <T,bitShift>::iterator rbegin(const YsEditArrayHandleEnumerator <T,bitShift> &allHandle)
{
	return allHandle.rbegin();
}

template <class T,const int bitShift>
typename YsEditArrayHandleEnumerator <T,bitShift>::iterator rend(const YsEditArrayHandleEnumerator <T,bitShift> &allHandle)
{
	return allHandle.rend();
}



////////////////////////////////////////////////////////////

template <class T,const int bitShift>
YsEditArrayHandleEnumerator <T,bitShift> YsEditArray <T,bitShift>::AllHandle(void) const
{
	YsEditArrayHandleEnumerator <T,bitShift> allHandle;
	allHandle.array=this;
	return allHandle;
}


/* } */
#endif

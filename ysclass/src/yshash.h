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

File Name: yshash.h
//////////////////////////////////////////////////////////// */

#ifndef YSHASH_IS_INCLUDED
#define YSHASH_IS_INCLUDED
/* { */

#include "yssort.h"
#include "ysdef.h"
#include "ysarray.h"

class YsHashElementEnumHandle
{
public:
	YSSIZE_T hashIdx;
	YSSIZE_T arrayIdx;
};

// Each element must have an unique key
template <class toFind>
class YsHashTable
{
private:
	class HashElement
	{
	public:
		YSHASHKEY key;
		toFind dat;
	};


public:
	class iterator
	{
	friend class YsHashTable <toFind>;

	private:
		YsHashTable <toFind> *table;
		YsHashElementEnumHandle hd;
		YSBOOL ended;
	public:
		inline iterator &operator++()
		{
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return *this;
		}
		inline iterator operator++(int)
		{
			iterator copy=*this;
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return copy;
		}
		inline bool operator==(const iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return true;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hashIdx==from.hashIdx && this->arrayIdx==from.arrayIdx)
			{
				return true;
			}
			return false;
		}
		inline bool operator!=(const iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return false;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hd.hashIdx==from.hd.hashIdx && this->hd.arrayIdx==from.hd.arrayIdx)
			{
				return false;
			}
			return true;
		}
		inline toFind &operator*()
		{
			return table->GetElement(hd);
		}
	};

	class const_iterator
	{
	friend class YsHashTable <toFind>;

	private:
		const YsHashTable <toFind> *table;
		YsHashElementEnumHandle hd;
		YSBOOL ended;
	public:
		inline const_iterator &operator++()
		{
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return *this;
		}
		inline const_iterator operator++(int)
		{
			const_iterator copy=*this;
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return copy;
		}
		inline bool operator==(const const_iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return true;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hd.hashIdx==from.hd.hashIdx && this->hd.arrayIdx==from.hd.arrayIdx)
			{
				return true;
			}
			return false;
		}
		inline bool operator!=(const const_iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return false;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hashIdx==from.hashIdx && this->arrayIdx==from.arrayIdx)
			{
				return false;
			}
			return true;
		}
		inline const toFind &operator*()
		{
			return table->GetElement(hd);
		}
	};

	/*! Support for STL-like iterator */
	iterator begin()
	{
		iterator iter;
		iter.table=this;
		iter.ended=YSFALSE;
		if(YSOK!=RewindElementEnumHandle(iter.hd))
		{
			iter.ended=YSTRUE;
		}
		return iter;
	}

	/*! Support for STL-like iterator */
	iterator end()
	{
		iterator iter;
		iter.table=this;
		iter.ended=YSTRUE;
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator begin() const
	{
		const_iterator iter;
		iter.table=this;
		iter.ended=YSFALSE;
		if(YSOK!=RewindElementEnumHandle(iter.hd))
		{
			iter.ended=YSTRUE;
		}
		return iter;
	}

	/*! Support for STL-like iterator */
	const_iterator end() const
	{
		const_iterator iter;
		iter.table=this;
		iter.ended=YSTRUE;
		return iter;
	}

protected:
	YSBOOL enableAutoResizing;
	YSSIZE_T autoResizingMin,autoResizingMax;

	YSSIZE_T HashSize;
	YSSIZE_T nElem;
	mutable YsArray <HashElement> *table;

public:
	YsHashTable(YSSIZE_T hashSize=16);
	~YsHashTable();

	YsHashTable(const YsHashTable <toFind> &incoming);
	YsHashTable <toFind> &operator=(const YsHashTable <toFind> &incoming);
	YsHashTable <toFind> &CopyFrom(const YsHashTable <toFind> &incoming);

	YsHashTable(YsHashTable <toFind> &&incoming);
	YsHashTable <toFind> &operator=(YsHashTable <toFind> &&incoming);
	YsHashTable <toFind> &MoveFrom(YsHashTable <toFind> &incoming);

	void CleanUp(void);
	YSRESULT PrepareTable(void);
	YSRESULT PrepareTable(YSSIZE_T hashSize);
	YSRESULT Resize(YSSIZE_T hashSize);
	YSRESULT CollectGarbage(void);
	YSRESULT AddElement(unsigned searchKey,toFind element);
	YSRESULT UpdateElement(unsigned searchKey,toFind element);
	YSRESULT UpdateElement(const YsHashElementEnumHandle &handle,toFind newValue);
	YSRESULT DeleteElement(unsigned int searchKey,toFind element);
	YSRESULT DeleteElementIfExist(unsigned int searchKey);
	YSRESULT FindElement(toFind &elem,unsigned searchKey) const;
	YSRESULT GetKey(YSHASHKEY &key,const YsHashElementEnumHandle &handle) const;
	YSRESULT GetElement(toFind &elem,const YsHashElementEnumHandle &handle) const;
	toFind &GetElement(const YsHashElementEnumHandle &handle);
	const toFind &GetElement(const YsHashElementEnumHandle &handle) const;
	YSBOOL CheckKeyExist(unsigned searchKey) const;

	/*! Returns a null handle.  You can iterate through keys by the following loop.
	    for(auto handle=hash.NullHandle(); YSOK==FindNextKey(handle); )
	    {
	        // Do something useful.
	    }
	*/
	YsHashElementEnumHandle NullHandle(void) const;

	YSBOOL IsHandleValid(const YsHashElementEnumHandle &handle) const;
	YSRESULT RewindElementEnumHandle(YsHashElementEnumHandle &handle) const;
	YSRESULT FindNextElement(YsHashElementEnumHandle &handle) const;

	YSRESULT SelfDiagnostic(void) const;

	void EnableAutoResizing(YSSIZE_T minSize,YSSIZE_T maxSize);
	void EnableAutoResizing(void);
	void DisableAutoResizing(void);

	int GetNumUnused(void) const;

	const char *tableName;

protected:
	YSRESULT CheckAutoResizingGrow(void);
	YSRESULT CheckAutoResizingShrink(void);
	YsArray <HashElement> *FindTableIndex(YSSIZE_T &idx,unsigned searchKey) const;
};

////////////////////////////////////////////////////////////

/*! Support for STL-like iterator */
template <class toFind>
inline typename YsHashTable<toFind>::iterator begin(YsHashTable<toFind> &table)
{
	return table.begin();
}

template <class toFind>
inline typename YsHashTable<toFind>::iterator end(YsHashTable<toFind> &table)
{
	return table.end();
}

template <class toFind>
inline typename YsHashTable<toFind>::const_iterator begin(const YsHashTable<toFind> &table)
{
	return table.begin();
}

template <class toFind>
inline typename YsHashTable<toFind>::const_iterator end(const YsHashTable<toFind> &table)
{
	return table.end();
}

////////////////////////////////////////////////////////////

template <class toFind>
YsHashTable <toFind>::YsHashTable(const YsHashTable <toFind> &incoming)
{
	table=NULL;
	CopyFrom(incoming);
}

template <class toFind>
YsHashTable <toFind> &YsHashTable <toFind>::operator=(const YsHashTable <toFind> &incoming)
{
	return CopyFrom(incoming);
}

template <class toFind>
YsHashTable <toFind> &YsHashTable <toFind>::CopyFrom(const YsHashTable <toFind> &incoming)
{
	if(incoming.HashSize!=HashSize || NULL==table)
	{
		if(NULL!=table)
		{
			delete [] table;
		}
		table=new YsArray <HashElement>[incoming.HashSize];
	}

	tableName=incoming.tableName;
	HashSize=incoming.HashSize;
	nElem=incoming.nElem;

	for(YSSIZE_T idx=0; idx<incoming.HashSize; ++idx)
	{
		table[idx]=incoming.table[idx];
	}

	return *this;
}


template <class toFind>
YsHashTable <toFind>::YsHashTable(YsHashTable <toFind> &&incoming)
{
	table=NULL;
	MoveFrom(incoming);
}

template <class toFind>
YsHashTable <toFind> &YsHashTable <toFind>::operator=(YsHashTable <toFind> &&incoming)
{
	return MoveFrom(incoming);
}

template <class toFind>
YsHashTable <toFind> &YsHashTable <toFind>::MoveFrom(YsHashTable <toFind> &incoming)
{
	if(NULL!=table)
	{
		delete [] table;
	}

	tableName=incoming.tableName;
	HashSize=incoming.HashSize;
	nElem=incoming.nElem;
	table=incoming.table;

	incoming.HashSize=0;
	incoming.nElem=0;
	incoming.table=NULL;

	return *this;
}


template <class toFind>
YsHashTable <toFind> ::YsHashTable(YSSIZE_T hashSize)
{
	HashSize=hashSize;
	table=new YsArray <HashElement>[hashSize];
	nElem=0;

	tableName="";

	enableAutoResizing=YSTRUE;
	autoResizingMin=0;
	autoResizingMax=0;
}

template <class toFind>
YsHashTable <toFind> ::~YsHashTable()
{
	// Don't CleanUp().  CleanUp() will create a table with length of one.
	if(table!=NULL)
	{
		delete [] table;
	}
}

template <class toFind>
void YsHashTable <toFind> ::CleanUp(void)
{
	if(table!=NULL)
	{
		delete [] table;
	}
	HashSize=1;
	table=new YsArray <HashElement>[1];
	nElem=0;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::PrepareTable(void)
{
	YSSIZE_T i;
	for(i=0; i<HashSize; i++)
	{
		table[i].Set(0,NULL);
	}
	nElem=0;
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::PrepareTable(YSSIZE_T hashSize)
{
	if(hashSize<1)
	{
		hashSize=1;
	}
	if(table!=NULL)
	{
		delete [] table;
	}
	HashSize=hashSize;
	table=new YsArray <HashElement>[hashSize];
	nElem=0;
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::Resize(YSSIZE_T hashSize)
{
	int i;
	YSBOOL saveAutoResizing;
	YsArray <HashElement> elemList;
	for(i=0; i<HashSize; i++)
	{
		elemList.Append(table[i].GetN(),table[i]);
	}
	PrepareTable(hashSize);

	saveAutoResizing=enableAutoResizing;
	enableAutoResizing=YSFALSE;
	for(i=0; i<elemList.GetN(); i++)
	{
		AddElement(elemList[i].key,elemList[i].dat);
	}
	enableAutoResizing=saveAutoResizing;

	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::CollectGarbage(void)
{
	Resize(HashSize);
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::AddElement(unsigned searchKey,toFind element)
{
	int hashId;
	YSSIZE_T i1,i2,im;
	YsArray <HashElement> *entry;
	HashElement neo;

	hashId=searchKey%HashSize;
	entry=&table[hashId];

	neo.dat=element;
	neo.key=searchKey;

	if(entry->GetN()==0)
	{
		entry->Append(neo);
	}
	else
	{
		i1=0;
		i2=entry->GetNumItem()-1;

		if(searchKey<(*entry)[i1].key)
		{
			entry->Insert(i1,neo);
		}
		else if((*entry)[i2].key<searchKey)
		{
			entry->Append(neo);
		}
		else
		{
			i1=0;
			i2=entry->GetN()-1;
			while(i2-i1>1)
			{
				im=(i1+i2)/2;
				if(searchKey<(*entry)[im].key)
				{
					i2=im;
				}
				else
				{
					i1=im;
				}
			}
			entry->Insert(i2,neo);
		}
	}
	nElem++;
	CheckAutoResizingGrow();
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::UpdateElement(unsigned searchKey,toFind element)
{
	YsArray <HashElement> *entry;
	HashElement neo;

	int hashId=searchKey%HashSize;
	entry=&table[hashId];

	neo.dat=element;
	neo.key=searchKey;

	if(entry->GetN()==0)
	{
		entry->Append(neo);
	}
	else
	{
		YSSIZE_T i1,i2;
		i1=0;
		i2=entry->GetNumItem()-1;

		if(searchKey<(*entry)[i1].key)
		{
			entry->Insert(i1,neo);
		}
		else if((*entry)[i2].key<searchKey)
		{
			entry->Append(neo);
		}
		else
		{
			i1=0;
			i2=entry->GetN()-1;
			while(i2-i1>1)
			{
				const YSSIZE_T im=(i1+i2)/2;
				if(searchKey<(*entry)[im].key)
				{
					i2=im;
				}
				else
				{
					i1=im;
				}
			}

			if(searchKey==(*entry)[i1].key)
			{
				(*entry)[i1].dat=element;
				return YSOK;  // I must return here because the size doesn't change.
			}
			else if(searchKey==(*entry)[i2].key)
			{
				(*entry)[i2].dat=element;
				return YSOK;  // I must return here because the size doesn't change.
			}

			entry->Insert(i2,neo);
		}
	}
	nElem++;
	CheckAutoResizingGrow();
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::UpdateElement(const YsHashElementEnumHandle &handle,toFind newValue)
{
	const int hashIdx=handle.hashIdx;
	const int arrayIdx=handle.arrayIdx;

	if(0<=hashIdx && hashIdx<HashSize &&
	   0<=arrayIdx && arrayIdx<table[hashIdx].GetN())
	{
		table[hashIdx][arrayIdx].dat=newValue;
		return YSOK;
	}
	return YSERR;
	
}

template <class toFind>
YSRESULT YsHashTable <toFind>::DeleteElement(unsigned int searchKey,toFind)
{
	YsArray <HashElement> *entry;
	YSSIZE_T idx;

	entry=FindTableIndex(idx,searchKey);
	if(entry!=NULL)
	{
		entry->Delete(idx);
		nElem--;
		CheckAutoResizingShrink();
		return YSOK;
	}

	YsErrOut("YsHashTable::DeleteElement()\n");
	YsErrOut("  Element not found\n");
	return YSERR;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::DeleteElementIfExist(unsigned int searchKey)
{
	YsArray <HashElement> *entry;
	YSSIZE_T idx;

	entry=FindTableIndex(idx,searchKey);
	if(entry!=NULL)
	{
		entry->Delete(idx);
		nElem--;
		CheckAutoResizingShrink();
		return YSOK;
	}
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::FindElement(toFind &element,unsigned searchKey) const
{
	YSSIZE_T idx;
	YsArray <HashElement> *entry;

	entry=FindTableIndex(idx,searchKey);
	if(entry!=NULL)
	{
		element=(*entry)[idx].dat;
		return YSOK;
	}
	return YSERR;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::GetKey(YSHASHKEY &key,const YsHashElementEnumHandle &handle) const
{
	const auto hashIdx=handle.hashIdx;
	const auto arrayIdx=handle.arrayIdx;

	if(0<=hashIdx && hashIdx<HashSize &&
	   0<=arrayIdx && arrayIdx<table[hashIdx].GetN())
	{
		key=table[hashIdx][arrayIdx].key;
		return YSOK;
	}
	return YSERR;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::GetElement(toFind &elem,const YsHashElementEnumHandle &handle) const
{
	const auto hashIdx=handle.hashIdx;
	const auto arrayIdx=handle.arrayIdx;

	if(0<=hashIdx && hashIdx<HashSize &&
	   0<=arrayIdx && arrayIdx<table[hashIdx].GetN())
	{
		elem=table[hashIdx][arrayIdx].dat;
		return YSOK;
	}
	return YSERR;
}

template <class toFind>
toFind &YsHashTable<toFind>::GetElement(const YsHashElementEnumHandle &handle)
{
	const auto hashIdx=handle.hashIdx;
	const auto arrayIdx=handle.arrayIdx;

	if(0<=hashIdx && hashIdx<HashSize &&
	   0<=arrayIdx && arrayIdx<table[hashIdx].GetN())
	{
		return table[hashIdx][arrayIdx].dat;
	}
	return table[0][0].dat;  // Good luck
}

template <class toFind>
const toFind &YsHashTable<toFind>::GetElement(const YsHashElementEnumHandle &handle) const
{
	const auto hashIdx=handle.hashIdx;
	const auto arrayIdx=handle.arrayIdx;

	if(0<=hashIdx && hashIdx<HashSize &&
	   0<=arrayIdx && arrayIdx<table[hashIdx].GetN())
	{
		return table[hashIdx][arrayIdx].dat;
	}
	return table[0][0].dat;  // Good luck
}

template <class toFind>
YSBOOL YsHashTable <toFind>::CheckKeyExist(unsigned searchKey) const
{
	YSSIZE_T idx;
	if(FindTableIndex(idx,searchKey)!=NULL)
	{
		return YSTRUE;
	}
	else
	{
		return YSFALSE;
	}

}
template <class toFind>
YsHashElementEnumHandle YsHashTable <toFind>::NullHandle(void) const
{
	YsHashElementEnumHandle handle;
	handle.hashIdx=-1;
	handle.arrayIdx=-1;
	return handle;
}

template <class toFind>
YSBOOL YsHashTable <toFind>::IsHandleValid(const YsHashElementEnumHandle &handle) const
{
	if(0<=handle.hashIdx && handle.hashIdx<HashSize && YSTRUE==table[handle.hashIdx].IsInRange(handle.arrayIdx))
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::RewindElementEnumHandle(YsHashElementEnumHandle &handle) const
{
	// Rewind -> handle points to the first element
	for(YSSIZE_T i=0; i<HashSize; i++)
	{
		if(0<table[i].GetN())
		{
			handle.hashIdx=i;
			handle.arrayIdx=0;
			return YSOK;
		}
	}
	handle.hashIdx=-1;
	handle.arrayIdx=-1;
	return YSERR;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::FindNextElement(YsHashElementEnumHandle &handle) const
{
	if(YSTRUE!=IsHandleValid(handle))
	{
		return RewindElementEnumHandle(handle);
	}

	YSSIZE_T hashIdx=handle.hashIdx;
	YSSIZE_T arrayIdx=handle.arrayIdx;

	++arrayIdx;
	if(arrayIdx<table[hashIdx].GetN())
	{
		handle.hashIdx=hashIdx;
		handle.arrayIdx=arrayIdx;
		return YSOK;
	}

	++hashIdx;
	arrayIdx=0;

	while(hashIdx<HashSize)
	{
		if(0<table[hashIdx].GetN())
		{
			handle.hashIdx=hashIdx;
			handle.arrayIdx=0;
			return YSOK;
		}
		++hashIdx;
	}

	handle.hashIdx=-1;
	handle.arrayIdx=-1;

	return YSERR;
}

template <class toFind>
YsArray <typename YsHashTable<toFind>::HashElement> *YsHashTable <toFind>::FindTableIndex(YSSIZE_T &idx,unsigned searchKey) const
{
	int hashId;
	YSSIZE_T i1,i2,im;
	YsArray <HashElement> *entry;

	hashId=searchKey%HashSize;
	entry=&table[hashId];

	if(entry->GetNumItem()==0)
	{
		idx=0;
		return NULL;
	}
	else
	{
		i1=0;
		i2=entry->GetN()-1;
		while(i2-i1>1)
		{
			im=(i1+i2)/2;
			if(searchKey<(*entry)[im].key)
			{
				i2=im;
			}
			else
			{
				i1=im;
			}
		}

		if((*entry)[i1].key==searchKey)
		{
			idx=i1;
			return entry;
		}
		else if((*entry)[i2].key==searchKey)
		{
			idx=i2;
			return entry;
		}
		else
		{
			return NULL;
		}
	}
}


template <class toFind>
YSRESULT YsHashTable <toFind>::SelfDiagnostic(void) const
{
	// Check all elements in the table are sorted in search key order
	// Search key and hash id is consistent

	YSSIZE_T i,j,nElemSum;
	YSBOOL err;
	YSRESULT res;
	res=YSOK;

	YsPrintf("Self Diagnostic YsHashTable\n");
	nElemSum=0;
	for(i=0; i<HashSize; i++)
	{
		if(i%20==0)
		{
			YsPrintf("%d/%d\r",(int)i,(int)HashSize);
		}

		err=YSFALSE;
		for(j=0; j<table[i].GetN(); j++)
		{
			if(j>0)
			{
				if(table[i][j-1].key>=table[i][j].key)
				{
					res=YSERR;
					err=YSTRUE;
				}
			}
			if(table[i].GetItem(j).key%HashSize!=unsigned(i))
			{
				res=YSERR;
				YsPrintf("##Inconsistencies in hash table %d %d (HashSize=%d)\n",(int)i,table[i][j].key,(int)HashSize);
			}
		}
		if(err==YSTRUE)
		{
			YsPrintf("##Elements are not sorted in key order\n");
			for(j=0; j<table[i].GetN(); j++)
			{
				YsPrintf(" %d",table[i][j].key);
			}
			YsPrintf("\n");
		}
		nElemSum+=table[i].GetN();
	}
	YsPrintf("%d/%d\n",(int)HashSize,(int)HashSize);

	if(nElemSum!=nElem)
	{
		printf("Number of elements does not match.\n");
	}

	return res;
}


template <class toFind>
void YsHashTable <toFind>::EnableAutoResizing(YSSIZE_T minSize,YSSIZE_T maxSize)
{
	enableAutoResizing=YSTRUE;
	autoResizingMin=minSize;
	autoResizingMax=maxSize;
}

template <class toFind>
void YsHashTable <toFind>::EnableAutoResizing(void)
{
	enableAutoResizing=YSTRUE;
	autoResizingMin=0;  // 0,0 -> Unrestricted auto resizing
	autoResizingMax=0;
}

template <class toFind>
void YsHashTable <toFind>::DisableAutoResizing(void)
{
	enableAutoResizing=YSFALSE;
}

template <class toFind>
int YsHashTable <toFind>::GetNumUnused(void) const
{
	YSSIZE_T i,n;
	n=0;
	for(i=0; i<HashSize; i++)
	{
		n+=table[i].GetNUnused();
	}
	return (int)n;
}


template <class toFind>
YSRESULT YsHashTable <toFind>::CheckAutoResizingGrow(void)
{
	if(enableAutoResizing==YSTRUE)
	{
		if(autoResizingMax==0 && autoResizingMin==0)  // Unlimited mode
		{
			if(HashSize*4<nElem)
			{
				Resize(nElem);
			}
		}
		else if(HashSize<autoResizingMax && HashSize*4<nElem)
		{
			Resize(YsSmaller <YSSIZE_T> (HashSize*2,autoResizingMax));
		}
	}
	return YSOK;
}

template <class toFind>
YSRESULT YsHashTable <toFind>::CheckAutoResizingShrink(void)
{
	if(enableAutoResizing==YSTRUE)
	{
		if(autoResizingMax==0 && autoResizingMin==0)  // Unlimited mode
		{
			if(nElem<HashSize/4)
			{
				Resize(YsGreater <YSSIZE_T> (1,nElem));
			}
		}
		else if(autoResizingMin<HashSize && nElem<HashSize)
		{
			Resize(YsGreater <YSSIZE_T> (HashSize/2,autoResizingMin));
		}
	}
	return YSOK;
}


////////////////////////////////////////////////////////////

// Sub Sub Group : Group of elements that have same key

template <class toFind,const int minKeyBufSize,const int minItemBufSize>
class YsHashSameKeyGroup
{
public:
	YsArray <unsigned,minKeyBufSize> key;
	YsArray <toFind,minItemBufSize> dat;
	YsList <YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> > *thisInTheList;
};


extern int YsHashSameKeyGroupCreateDeleteCounter;

// Sub Group : Group of sub groups that have same keysum

template <class toFind,const int minKeyBufSize,const int minItemBufSize>
class YsHashSameKeySumGroup
{
public:
	YsHashSameKeySumGroup();
	~YsHashSameKeySumGroup();
	YsList <YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> > *elmList;
};

template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize>::YsHashSameKeySumGroup()
{
	YsHashSameKeyGroupCreateDeleteCounter++;
	elmList=NULL;
}

template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize>::~YsHashSameKeySumGroup()
{
	YsHashSameKeyGroupCreateDeleteCounter--;
	elmList->DeleteList();
}



//

template <class toFind,const int minKeyBufSize,const int minItemBufSize>
class YsMultiKeyHashElementEnumHandle
{
public:
	YsHashElementEnumHandle outerHandle;
	YsList <YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> > *currentSubSubGroup;
};



// Table main body

template <class toFind,const int minKeyBufSize=1,const int minItemBufSize=1>
class YsMultiKeyHash  : protected YsHashTable <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>
{
protected:
// Complying with a stupid change made in g++ 3.4
using YsHashTable <YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *>::HashSize;
using YsHashTable <YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *>::table;
// Complying with a stupid change made in g++ 3.4

public:
	YsMultiKeyHash(YSSIZE_T hashSize=16);
	~YsMultiKeyHash();

	YSRESULT PrepareTable(void);
	YSRESULT PrepareTable(YSSIZE_T hashSize);

	YSRESULT AddElement(int nKey,const unsigned key[],toFind element);

	YSRESULT DeleteElement(int nKey,const unsigned key[],toFind element);

	YSRESULT DeleteKey(int nKey,const unsigned key[]);

	const YsArray <toFind,minItemBufSize> *FindElement(int nKey,const unsigned key[]) const;

	YSRESULT RewindElementEnumHandle
	    (YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const;
	YSRESULT FindNextElement
	    (YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const;
	const YsArray <toFind,minItemBufSize> *GetElement
	    (YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const;
	const YsArray <toFind,minItemBufSize> *GetSeachKey
	    (YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const;

protected:
	unsigned GetKeySum(YSSIZE_T nKey,const unsigned key[]) const;
	YSBOOL CheckSameKey(YSSIZE_T nKey1,const unsigned key1[],YSSIZE_T nKey2,const unsigned key2[]) const;
	YSRESULT FindAddSameKeyGroup
	   (unsigned &keySum,
		YsArray <unsigned,minKeyBufSize> &key,
	    YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeySumGroup,
	    YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeyGroup);
	YSRESULT FindSameKeyGroup
	   (unsigned &keySum,
		YsArray <unsigned,minKeyBufSize> &key,
	    YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeySumGroup,
	    YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeyGroup) const;
};


template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	YsMultiKeyHash(YSSIZE_T hashSize) : YsHashTable <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>(hashSize)
{
}

template <class toFind,const int minKeyBufSize,const int minItemBufSize>
	YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::~YsMultiKeyHash()
{
	int i,j;
	for(i=0; i<HashSize; i++)
	{
		for(j=0; j<table[i].GetN(); j++)
		{
			delete table[i][j].dat;
		}
	}
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::PrepareTable(void)
{
	int i,j;
	for(i=0; i<HashSize; i++)
	{
		for(j=0; j<table[i].GetN(); j++)
		{
			delete table[i][j].dat;
		}
	}

	return YsHashTable<YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::PrepareTable();
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::PrepareTable(YSSIZE_T hashSize)
{
	YSSIZE_T i,j;
	for(i=0; i<HashSize; i++)
	{
		for(j=0; j<table[i].GetN(); j++)
		{
			delete table[i][j].dat;
		}
	}
	return YsHashTable<YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::PrepareTable(hashSize);
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	AddElement(int nKey,const unsigned unsortedKey[],toFind element)
{
	unsigned keySum;
	YsArray <unsigned,minKeyBufSize> key;
	YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *sameKeySumGroup;
	YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> *sameKeyGroup;

	key.Set(nKey,unsortedKey);

	if(FindAddSameKeyGroup(keySum,key,sameKeySumGroup,sameKeyGroup)==YSOK)
	{
		sameKeyGroup->dat.AppendItem(element);
		return YSOK;
	}
	return YSERR;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	DeleteElement(int nKey,const unsigned unsortedKey[],toFind element)
{
	unsigned keySum;
	YsArray <unsigned,minKeyBufSize> key;
	YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *sameKeySumGroup;
	YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> *sameKeyGroup;

	key.Set(nKey,unsortedKey);  // key is sorted inside FindSameKeyGroup func

	if(FindSameKeyGroup(keySum,key,sameKeySumGroup,sameKeyGroup)==YSOK)
	{
		for(int i=0; i<sameKeyGroup->dat.GetN(); i++)
		{
			if(sameKeyGroup->dat[i]==element)
			{
				sameKeyGroup->dat.Delete(i);

				if(sameKeyGroup->dat.GetN()==0)
				{
					YsList <YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> > *seek;

					//YSBOOL found;
					//found=YSFALSE;
					//for(seek=sameKeySumGroup->elmList; seek!=NULL; seek=seek->Next())
					//{
					//	if(&seek->dat==sameKeyGroup)
					//	{
					//		sameKeySumGroup->elmList=seek->DeleteFromList();
					//		found=YSTRUE;
					//		break;
					//	}
					//}
					//if(found!=YSTRUE)
					//{
					//	YsErrOut("YsMultiKeyHash::DeleteElement()\n");
					//	YsErrOut("  Internal error (1)\n");
					//}

					// Above part is replaced with >>  2000/06/05
					seek=sameKeyGroup->thisInTheList;
					if(seek->SeekTop()==sameKeySumGroup->elmList)
					{
						sameKeySumGroup->elmList=seek->DeleteFromList();
					}
					else
					{
						YsErrOut("YsMultiKeyHash::DeleteElement()\n");
						YsErrOut("  Internal error (1)\n");
					}
					// <<


					if(sameKeySumGroup->elmList==NULL)
					{
						YsHashTable
						    <YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *>::
						        DeleteElement(keySum,sameKeySumGroup);
						delete sameKeySumGroup;
					}
				}

				return YSOK;
			}
		}
	}
	YsErrOut("YsMultiKeyHash::DeleteElement()\n");
	YsErrOut("  Tried to delete an element that is not listed.\n");
	return YSERR;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	DeleteKey(int nKey,const unsigned unsortedKey[])
{
	unsigned keySum;
	YsArray <unsigned,minKeyBufSize> key;
	YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *sameKeySumGroup;
	YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> *sameKeyGroup;

	key.Set(nKey,unsortedKey);  // key is sorted inside FindSameKeyGroup func

	if(FindSameKeyGroup(keySum,key,sameKeySumGroup,sameKeyGroup)==YSOK)
	{
		YsList <YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> > *seek;

		seek=sameKeyGroup->thisInTheList;
		if(seek->SeekTop()==sameKeySumGroup->elmList)
		{
			sameKeySumGroup->elmList=seek->DeleteFromList();
		}
		else
		{
			YsErrOut("YsMultiKeyHash::DeleteKey()\n");
			YsErrOut("  Internal error (1)\n");
		}

		if(sameKeySumGroup->elmList==NULL)
		{
			YsHashTable
			    <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::
			        DeleteElement(keySum,sameKeySumGroup);
			delete sameKeySumGroup;
		}

		return YSOK;
	}
	YsErrOut("YsMultiKeyHash::DeleteKey()\n");
	YsErrOut("  Tried To Delete a key that is not listed.\n");
	return YSERR;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
const YsArray <toFind,minItemBufSize> *YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	FindElement(int nKey,const unsigned unsortedKey[]) const
{
	unsigned keySum;
	YsArray <unsigned,minKeyBufSize> key;
	YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *sameKeySumGroup;
	YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> *sameKeyGroup;

	key.Set(nKey,unsortedKey);

	if(FindSameKeyGroup(keySum,key,sameKeySumGroup,sameKeyGroup)==YSOK)
	{
		return &sameKeyGroup->dat;
	}
	else
	{
		return NULL;
	}
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	RewindElementEnumHandle
    	(YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const
{
	YsHashTable <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::
	    RewindElementEnumHandle(handle.outerHandle);

	YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *subElem;

	handle.currentSubSubGroup=NULL;
	while(handle.currentSubSubGroup==NULL)
	{
		if(YsHashTable
		       <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::
		           GetElement(subElem,handle.outerHandle)==YSOK)
		{
			if(subElem->elmList!=NULL)
			{
				handle.currentSubSubGroup=subElem->elmList;
				return YSOK;
			}
		}
		else if(YsHashTable
		       <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::
		           FindNextElement(handle.outerHandle)!=YSOK)
		{
			return YSERR;
		}
	}
	return YSERR;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	FindNextElement
    	(YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const
{
	if(handle.currentSubSubGroup!=NULL)
	{
		handle.currentSubSubGroup=handle.currentSubSubGroup->Next();
		if(handle.currentSubSubGroup!=NULL)
		{
			return YSOK;
		}
	}

	// Falling into this point means either one of:
	//    1. Finding first element
	//    2. One SubGroup is over, and have to move to the next SubGroup
	while(handle.currentSubSubGroup==NULL)
	{
		YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *subGroup;

		if(FindNextElement(subGroup,handle.outerHandle)==YSOK)
		{
			handle.currentSubSubGroup=subGroup->elmList;
		}
		else
		{
			return YSERR;
		}
	}
	return YSOK;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
const YsArray <toFind,minItemBufSize> *
    YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	    GetElement(YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const
{
	if(handle.currentSubSubGroup!=NULL)
	{
		return &handle.currentSubSubGroup->dat;
	}
	return NULL;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
const YsArray <toFind,minItemBufSize> *
    YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	    GetSeachKey
	        (YsMultiKeyHashElementEnumHandle <toFind,minKeyBufSize,minItemBufSize> &handle) const
{
	if(handle.currentSubSubGroup!=NULL)
	{
		return &handle.currentSubSubGroup->key;
	}
	return NULL;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
unsigned
    YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	    GetKeySum(YSSIZE_T nKey,const unsigned key[]) const
{
	int i;
	unsigned sum;
	sum=0;
	for(i=0; i<nKey; i++)
	{
		sum+=key[i];
	}
	return sum;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSBOOL
    YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
	    CheckSameKey(YSSIZE_T nKey1,const unsigned key1[],YSSIZE_T nKey2,const unsigned key2[]) const
{
	if(nKey1==nKey2)
	{
		int i;
		for(i=0; i<nKey1; i++)
		{
			if(key1[i]!=key2[i])
			{
				return YSFALSE;
			}
		}
		return YSTRUE;
	}
	return YSFALSE;
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT
    YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
		FindAddSameKeyGroup
	       (unsigned &keySum,
			YsArray <unsigned,minKeyBufSize> &key,
	        YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeySumGroup,
	        YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeyGroup)
{
	if(FindSameKeyGroup(keySum,key,sameKeySumGroup,sameKeyGroup)==YSOK)
	{
		return YSOK;
	}
	else
	{
		if(sameKeySumGroup==NULL)
		{
			sameKeySumGroup=new YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize>;
			YsHashTable
		       <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::
		           AddElement(keySum,sameKeySumGroup);  // Pray for no error  :-)
		}

		YsList <YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> > *neo;

		neo=new YsList <YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> >;
		neo->dat.key=key;
		neo->dat.thisInTheList=neo;

		sameKeySumGroup->elmList=sameKeySumGroup->elmList->Append(neo);

		sameKeyGroup=&neo->dat;

		return YSOK;
	}
}



template <class toFind,const int minKeyBufSize,const int minItemBufSize>
YSRESULT
    YsMultiKeyHash <toFind,minKeyBufSize,minItemBufSize>::
		FindSameKeyGroup
	       (unsigned &keySum,
			YsArray <unsigned,minKeyBufSize> &key,
	        YsHashSameKeySumGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeySumGroup,
	        YsHashSameKeyGroup <toFind,minKeyBufSize,minItemBufSize> *&sameKeyGroup)
	            const
{
	sameKeySumGroup=NULL;
	sameKeyGroup=NULL;

	keySum=GetKeySum(key.GetNumItem(),key.GetArray());

	YsQuickSort <unsigned,void *> (key.GetNumItem(),key.GetEditableArray());

	if(YsHashTable
		  <YsHashSameKeySumGroup<toFind,minKeyBufSize,minItemBufSize> *>::
		      FindElement(sameKeySumGroup,keySum)==YSOK)
		          // sameKeySumGroup : Group of sub elements that have same keysum
	{
		YsList <YsHashSameKeyGroup<toFind,minKeyBufSize,minItemBufSize> > *seek;

		sameKeyGroup=NULL;
		for(seek=sameKeySumGroup->elmList; seek!=NULL; seek=seek->Next())
		{
			YSSIZE_T nKey1,nKey2;
			const unsigned *key1,*key2;

			nKey1=seek->dat.key.GetNumItem();
			key1=seek->dat.key.GetArray();
			nKey2=key.GetNumItem();
			key2=key.GetArray();

			if(CheckSameKey(nKey1,key1,nKey2,key2)==YSTRUE)
			{
				sameKeyGroup=&seek->dat;
				return YSOK;
			}
		}
	}
	return YSERR;
}



////////////////////////////////////////////////////////////

template <int nKeyLng>
class YsFixedLengthHashBase
{
protected:
	inline YSRESULT OrderKey(unsigned orderedKey[],const unsigned unorderedKey[]) const;
	inline unsigned int KeySum(const unsigned orderedKey[]) const;
	inline YSBOOL SameKey(const unsigned int orderedKey1[],const unsigned int orderedKey2[]) const;
	inline void CopyKey(unsigned int to[],const unsigned int from[]) const;
};

// 2013/12/22 Confirmed partial specialization works on VS2012 and g++ 4.6.
template <>
inline YSRESULT YsFixedLengthHashBase <1>::OrderKey(unsigned orderedKey[],const unsigned unorderedKey[]) const
{
	orderedKey[0]=unorderedKey[0];
	return YSOK;
}

template <>
inline YSRESULT YsFixedLengthHashBase <2>::OrderKey(unsigned orderedKey[],const unsigned unorderedKey[]) const
{
	if(unorderedKey[0]<unorderedKey[1])
	{
		orderedKey[0]=unorderedKey[0];
		orderedKey[1]=unorderedKey[1];
	}
	else
	{
		orderedKey[0]=unorderedKey[1];
		orderedKey[1]=unorderedKey[0];
	}
	return YSOK;
}

template <>
inline YSRESULT YsFixedLengthHashBase <3>::OrderKey(unsigned orderedKey[],const unsigned unorderedKey[]) const
{
	if(unorderedKey[0]<unorderedKey[1] && unorderedKey[0]<unorderedKey[2])
	{
		orderedKey[0]=unorderedKey[0];
		if(unorderedKey[1]<unorderedKey[2])
		{
			orderedKey[1]=unorderedKey[1];
			orderedKey[2]=unorderedKey[2];
		}
		else
		{
			orderedKey[1]=unorderedKey[2];
			orderedKey[2]=unorderedKey[1];
		}
	}
	else if(unorderedKey[1]<unorderedKey[2])
	{
		orderedKey[0]=unorderedKey[1];
		if(unorderedKey[0]<unorderedKey[2])
		{
			orderedKey[1]=unorderedKey[0];
			orderedKey[2]=unorderedKey[2];
		}
		else
		{
			orderedKey[1]=unorderedKey[2];
			orderedKey[2]=unorderedKey[0];
		}
	}
	else
	{
		orderedKey[0]=unorderedKey[2];
		if(unorderedKey[0]<unorderedKey[1])
		{
			orderedKey[1]=unorderedKey[0];
			orderedKey[2]=unorderedKey[1];
		}
		else
		{
			orderedKey[1]=unorderedKey[1];
			orderedKey[2]=unorderedKey[0];
		}
	}
	return YSOK;
}

template <int nKeyLng>
inline YSRESULT YsFixedLengthHashBase <nKeyLng>::OrderKey(unsigned orderedKey[],const unsigned unorderedKey[]) const
{
	for(int i=0; i<nKeyLng; i++)
	{
		orderedKey[i]=unorderedKey[i];
	}
	return YsQuickSort <unsigned,unsigned> (nKeyLng,orderedKey,NULL);
}

template <>
inline unsigned int YsFixedLengthHashBase <1>::KeySum(const unsigned int key[]) const
{
	return key[0];
}

template <>
inline unsigned int YsFixedLengthHashBase <2>::KeySum(const unsigned int key[]) const
{
	return key[0]+key[1];
}

template <>
inline unsigned int YsFixedLengthHashBase <3>::KeySum(const unsigned int key[]) const
{
	return key[0]+key[1]+key[2];
}

template <int nKeyLng>
inline unsigned int YsFixedLengthHashBase <nKeyLng>::KeySum(const unsigned int key[]) const
{
	unsigned int sum=0;
	for(int i=0; i<nKeyLng; ++i)
	{
		sum+=key[i];
	}
	return sum;
}

template <>
inline YSBOOL YsFixedLengthHashBase <1>::SameKey(const unsigned int orderedKey1[],const unsigned int orderedKey2[]) const
{
	if(orderedKey2[0]==orderedKey1[0])
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <>
inline YSBOOL YsFixedLengthHashBase <2>::SameKey(const unsigned int orderedKey1[],const unsigned int orderedKey2[]) const
{
	if(orderedKey2[0]==orderedKey1[0] &&
	   orderedKey2[1]==orderedKey1[1])
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <>
inline YSBOOL YsFixedLengthHashBase <3>::SameKey(const unsigned int orderedKey1[],const unsigned int orderedKey2[]) const
{
	if(orderedKey2[0]==orderedKey1[0] &&
	   orderedKey2[1]==orderedKey1[1] &&
	   orderedKey2[2]==orderedKey1[2])
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <int nKeyLng>
inline YSBOOL YsFixedLengthHashBase <nKeyLng>::SameKey(const unsigned int orderedKey1[],const unsigned int orderedKey2[]) const
{
	for(int i=0; i<nKeyLng; ++i)
	{
		if(orderedKey1[i]!=orderedKey2[i])
		{
			return YSFALSE;
		}
	}
	return YSTRUE;
}

template <int nKeyLng>
inline void YsFixedLengthHashBase <nKeyLng>::CopyKey(unsigned int to[],const unsigned int from[]) const
{
	for(int i=0; i<nKeyLng; ++i)
	{
		to[i]=from[i];
	}
}

////////////////////////////////////////////////////////////

template <class toFind,int nKeyLng>
class YsFixedLengthHashTable : public YsFixedLengthHashBase <nKeyLng>
{
private:
	class ElemEnumHandle
	{
	public:
		YSSIZE_T hashIdx,arrayIdx;
	};

	class HashElement
	{
	public:
		YSHASHKEY key[nKeyLng];
		toFind objective;
	};

	YSBOOL enableAutoResizing;
	YSSIZE_T autoResizingMin,autoResizingMax;

	YSSIZE_T nElem;
	YsArray <YsArray <HashElement> > keyList;

	// Support for STL-like iterator >>
public:
	class iterator
	{
	friend class YsFixedLengthHashTable <toFind,nKeyLng>;

	private:
		YsFixedLengthHashTable <toFind,nKeyLng> *table;
		ElemEnumHandle hd;
		YSBOOL ended;
	public:
		inline iterator &operator++()
		{
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return *this;
		}
		inline iterator operator++(int)
		{
			iterator copy=*this;
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return copy;
		}
		inline bool operator==(const iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return true;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hashIdx==from.hashIdx && this->arrayIdx==from.arrayIdx)
			{
				return true;
			}
			return false;
		}
		inline bool operator!=(const iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return false;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hd.hashIdx==from.hd.hashIdx && this->hd.arrayIdx==from.hd.arrayIdx)
			{
				return false;
			}
			return true;
		}
		inline toFind &operator*()
		{
			return table->GetElement(hd);
		}
	};
	class const_iterator
	{
	friend class YsFixedLengthHashTable <toFind,nKeyLng>;

	private:
		const YsFixedLengthHashTable <toFind,nKeyLng> *table;
		ElemEnumHandle hd;
		YSBOOL ended;
	public:
		inline const_iterator &operator++()
		{
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return *this;
		}
		inline const_iterator operator++(int)
		{
			const_iterator copy=*this;
			if(YSTRUE!=ended)
			{
				if(YSOK!=table->FindNextElement(hd))
				{
					ended=YSTRUE;
				}
			}
			return copy;
		}
		inline bool operator==(const const_iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return true;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hd.hashIdx==from.hd.hashIdx && this->hd.arrayIdx==from.hd.arrayIdx)
			{
				return true;
			}
			return false;
		}
		inline bool operator!=(const const_iterator &from)
		{
			if(this->ended==YSTRUE && from.ended==YSTRUE)
			{
				return false;
			}
			else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
			        this->hashIdx==from.hashIdx && this->arrayIdx==from.arrayIdx)
			{
				return false;
			}
			return true;
		}
		inline const toFind &operator*()
		{
			return table->GetElement(hd);
		}
	};
	iterator begin()
	{
		iterator iter;
		iter.table=this;
		iter.ended=YSFALSE;
		if(YSOK!=RewindElementEnumHandle(iter.hd))
		{
			iter.ended=YSTRUE;
		}
		return iter;
	}
	iterator end()
	{
		iterator iter;
		iter.table=this;
		iter.ended=YSTRUE;
		return iter;
	}
	const_iterator begin() const
	{
		const_iterator iter;
		iter.table=this;
		iter.ended=YSFALSE;
		if(YSOK!=RewindElementEnumHandle(iter.hd))
		{
			iter.ended=YSTRUE;
		}
		return iter;
	}
	const_iterator end() const
	{
		const_iterator iter;
		iter.table=this;
		iter.ended=YSTRUE;
		return iter;
	}
	// Support for STL-like iterator <<

public:
	inline YsFixedLengthHashTable(YSSIZE_T hSize=16);
	inline void EnableAutoResizing(void);
	inline void DisableAutoResizing(void);
	inline void CleanUpThin(void);
	inline YSRESULT PrepareTable(YSSIZE_T hashSize);
	inline YSRESULT Resize(YSSIZE_T hashSize);
	inline YSRESULT CollectGarbage(void);
	inline YSRESULT AddElement(int nKey,const YSHASHKEY unorderedKey[],const toFind &objective);
	inline YSRESULT UpdateElement(int nKey,const YSHASHKEY unorderedKey[],const toFind &objective);
	inline YSRESULT DeleteElement(int nKey,const YSHASHKEY unorderedKey[]);
	inline YSRESULT DeleteKey(int nKey,const YSHASHKEY unorderedKey[]);

	inline YSSIZE_T GetHashSize(void) const;

	YSBOOL IsHandleValid(ElemEnumHandle hd) const;
	ElemEnumHandle FindHandle(int nKey,const YSHASHKEY unorderedKey[]) const;
	inline const toFind *FindElement(int nKey,const unsigned unorderedKey[]) const;
	inline YSRESULT FindElement(toFind &elem,int nKey,const unsigned unorderedKey[]) const;

	inline YSBOOL CheckKeyExist(int nKey,const unsigned unorderedKey[]) const;

	/*! Returns a null handle.  You can iterate through keys by the following loop.
	    for(auto handle=hash.NullHandle(); YSOK==FindNextKey(handle); )
	    {
	        // Do something useful.
	    }
	*/
	ElemEnumHandle NullHandle(void) const;
	YSRESULT RewindElementEnumHandle(ElemEnumHandle &elHd) const;
	YSRESULT FindNextElement(ElemEnumHandle &elHd) const;
	YSRESULT FindNextKey(ElemEnumHandle &elHd) const;
	toFind &GetElement(ElemEnumHandle &elHd);
	const toFind &GetElement(ElemEnumHandle &elHd) const;
	const YSHASHKEY *GetKey(ElemEnumHandle &elHd) const;

	inline YSRESULT CheckAutoResizingGrow(void);
	inline YSRESULT CheckAutoResizingShrink(void);
};

////////////////////////////////////////////////////////////
// Support for STL-like iterator >>
template <class toFind,int nKeyLng>
inline typename YsFixedLengthHashTable <toFind,nKeyLng>::iterator begin(YsFixedLengthHashTable <toFind,nKeyLng> &table)
{
	return table.begin();
}

template <class toFind,int nKeyLng>
inline typename YsFixedLengthHashTable <toFind,nKeyLng>::iterator end(YsFixedLengthHashTable <toFind,nKeyLng> &table)
{
	return table.end();
}

template <class toFind,int nKeyLng>
inline typename YsFixedLengthHashTable <toFind,nKeyLng>::const_iterator begin(const YsFixedLengthHashTable <toFind,nKeyLng> &table)
{
	return table.begin();
}

template <class toFind,int nKeyLng>
inline typename YsFixedLengthHashTable <toFind,nKeyLng>::const_iterator end(const YsFixedLengthHashTable <toFind,nKeyLng> &table)
{
	return table.end();
}
// Support for STL-like iterator <<
////////////////////////////////////////////////////////////

template <class toFind,int nKeyLng>
YsFixedLengthHashTable <toFind,nKeyLng>::YsFixedLengthHashTable(YSSIZE_T hSize)
{
	enableAutoResizing=YSTRUE;
	autoResizingMin=0;
	autoResizingMax=0;

	PrepareTable(hSize);
}

template <class toFind,int nKeyLng>
void YsFixedLengthHashTable <toFind,nKeyLng>::EnableAutoResizing(void)
{
	enableAutoResizing=YSTRUE;
}

template <class toFind,int nKeyLng>
void YsFixedLengthHashTable <toFind,nKeyLng>::DisableAutoResizing(void)
{
	enableAutoResizing=YSFALSE;
}

template <class toFind,int nKeyLng>
void YsFixedLengthHashTable <toFind,nKeyLng>::CleanUpThin(void)
{
	for(YSSIZE_T i=0; i<keyList.GetN(); ++i)
	{
		keyList[i].Clear();
	}
	nElem=0;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::PrepareTable(YSSIZE_T hashSize)
{
	if(0>=hashSize)
	{
		hashSize=1;
	}

	keyList.Set(hashSize,NULL);
	CleanUpThin();

	return YSOK;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::Resize(YSSIZE_T hashSize)
{
	if(0>=hashSize)
	{
		hashSize=1;
	}

	YsArray <HashElement> allElem;
	for(YSSIZE_T i=0; i<keyList.GetN(); ++i)
	{
		allElem.Append(keyList[i]);
	}

	PrepareTable(hashSize);

	for(YSSIZE_T i=0; i<allElem.GetN(); ++i)
	{
		AddElement(nKeyLng,allElem[i].key,allElem[i].objective);
	}

	return YSOK;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::CollectGarbage(void)
{
	for(YSSIZE_T i=0; i<keyList.GetN(); ++i)
	{
		if(0==keyList[i].GetN())
		{
			keyList[i].ClearDeep();
		}
	}
	return YSOK;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::AddElement(int nKey,const YSHASHKEY unorderedKey[],const toFind &objective)
{
	YSHASHKEY hashEntry=this->KeySum(unorderedKey)%keyList.GetN();
	YSHASHKEY orderedKey[nKeyLng];

	this->OrderKey(orderedKey,unorderedKey);

	for(int i=0; i<keyList[hashEntry].GetN(); ++i)
	{
		if(YSTRUE==this->SameKey(orderedKey,keyList[hashEntry][i].key))
		{
			return YSERR;
		}
	}

	++nElem;
	keyList[hashEntry].Increment();
	this->CopyKey(keyList[hashEntry].GetEnd().key,orderedKey);
	keyList[hashEntry].GetEnd().objective=objective;

	CheckAutoResizingGrow();

	return YSOK;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::UpdateElement(int nKey,const YSHASHKEY unorderedKey[],const toFind &objective)
{
	YSHASHKEY hashEntry=this->KeySum(unorderedKey)%keyList.GetN();
	YSHASHKEY orderedKey[nKeyLng];

	this->OrderKey(orderedKey,unorderedKey);

	for(YSSIZE_T i=0; i<keyList[hashEntry].GetN(); ++i)
	{
		if(YSTRUE==this->SameKey(orderedKey,keyList[hashEntry][i].key))
		{
			keyList[hashEntry][i].objective=objective;
			return YSOK;
		}
	}

	++nElem;
	keyList[hashEntry].Increment();
	this->CopyKey(keyList[hashEntry].GetEnd().key,orderedKey);
	keyList[hashEntry].GetEnd().objective=objective;

	CheckAutoResizingGrow();

	return YSOK;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::DeleteElement(int nKey,const YSHASHKEY unorderedKey[])
{
	YSHASHKEY hashEntry=this->KeySum(unorderedKey)%keyList.GetN();
	YSHASHKEY orderedKey[nKeyLng];

	this->OrderKey(orderedKey,unorderedKey);

	for(int i=0; i<keyList[hashEntry].GetN(); ++i)
	{
		if(YSTRUE==this->SameKey(orderedKey,keyList[hashEntry][i].key))
		{
			keyList[hashEntry].DeleteBySwapping(i);
			--nElem;

			CheckAutoResizingShrink();
			return YSOK;
		}
	}

	return YSERR;
}

template <class toFind,int nKeyLng>
inline YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::DeleteKey(int nKey,const YSHASHKEY unorderedKey[])
{
	return DeleteElement(nKey,unorderedKey);
}

template <class toFind,int nKeyLng>
YSSIZE_T YsFixedLengthHashTable <toFind,nKeyLng>::GetHashSize(void) const
{
	return keyList.GetN();
}

template <class toFind,int nKeyLng>
YSBOOL YsFixedLengthHashTable <toFind,nKeyLng>::IsHandleValid(typename YsFixedLengthHashTable <toFind,nKeyLng>::ElemEnumHandle hd) const
{
	if(YSTRUE==keyList.IsInRange(hd.hashIdx) && YSTRUE==keyList[hd.hashIdx].IsInRange(hd.arrayIdx))
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <class toFind,int nKeyLng>
typename YsFixedLengthHashTable <toFind,nKeyLng>::ElemEnumHandle YsFixedLengthHashTable <toFind,nKeyLng>::FindHandle(int nKey,const YSHASHKEY unorderedKey[]) const
{
	ElemEnumHandle hd;
	hd.idx1=-1;
	hd.idx2=-1;
	if(nKey==nKeyLng)
	{
		YSHASHKEY orderedKey[nKeyLng];
		this->OrderKey(orderedKey,unorderedKey);

		YSSIZE_T idx1,idx2;
		if(YSOK==FindEntryIndex(idx1,idx2,nKey,orderedKey))
		{
			hd.idx1=idx1;
			hd.idx2=idx2;
		}
	}
	return hd;
}

template <class toFind,int nKeyLng>
const toFind *YsFixedLengthHashTable <toFind,nKeyLng>::FindElement(int nKey,const unsigned unorderedKey[]) const
{
	YSHASHKEY hashEntry=this->KeySum(unorderedKey)%keyList.GetN();
	YSHASHKEY orderedKey[nKeyLng];

	this->OrderKey(orderedKey,unorderedKey);

	for(int i=0; i<keyList[hashEntry].GetN(); ++i)
	{
		if(YSTRUE==this->SameKey(orderedKey,keyList[hashEntry][i].key))
		{
			return &keyList[hashEntry][i].objective;
		}
	}
	return NULL;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::FindElement(toFind &elem,int nKey,const unsigned unorderedKey[]) const
{
	const toFind *found=FindElement(nKey,unorderedKey);
	if(NULL!=found)
	{
		elem=*found;
		return YSOK;
	}
	return YSERR;
}

template <class toFind,int nKeyLng>
YSBOOL YsFixedLengthHashTable <toFind,nKeyLng>::CheckKeyExist(int nKey,const unsigned unorderedKey[]) const
{
	if(NULL!=FindElement(nKey,unorderedKey))
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <class toFind,int nKeyLng>
typename YsFixedLengthHashTable <toFind,nKeyLng>::ElemEnumHandle YsFixedLengthHashTable <toFind,nKeyLng>::NullHandle(void) const
{
	ElemEnumHandle handle;
	handle.hashIdx=-1;
	handle.arrayIdx=-1;
	return handle;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::RewindElementEnumHandle(ElemEnumHandle &handle) const
{
	for(YSSIZE_T i=0; i<keyList.GetN(); i++)
	{
		if(0<keyList[i].GetN())
		{
			handle.hashIdx=i;
			handle.arrayIdx=0;
			return YSOK;
		}
	}
	handle.hashIdx=-1;
	handle.arrayIdx=-1;
	return YSERR;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::FindNextElement(ElemEnumHandle &handle) const
{
	if(YSTRUE!=IsHandleValid(handle))
	{
		return RewindElementEnumHandle(handle);
	}

	YSSIZE_T hashIdx=handle.hashIdx;
	YSSIZE_T arrayIdx=handle.arrayIdx;

	++arrayIdx;
	if(arrayIdx<keyList[hashIdx].GetN())
	{
		handle.hashIdx=hashIdx;
		handle.arrayIdx=arrayIdx;
		return YSOK;
	}

	++hashIdx;
	arrayIdx=0;

	while(hashIdx<keyList.GetN())
	{
		if(0<keyList[hashIdx].GetN())
		{
			handle.hashIdx=hashIdx;
			handle.arrayIdx=0;
			return YSOK;
		}
		++hashIdx;
	}

	handle.hashIdx=-1;
	handle.arrayIdx=-1;

	return YSERR;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::FindNextKey(ElemEnumHandle &elHd) const
{
	return FindNextElement(elHd);
}

template <class toFind,int nKeyLng>
const toFind &YsFixedLengthHashTable <toFind,nKeyLng>::GetElement(ElemEnumHandle &elHd) const
{
	return keyList[elHd.hashIdx][elHd.arrayIdx].objective;
}

template <class toFind,int nKeyLng>
toFind &YsFixedLengthHashTable <toFind,nKeyLng>::GetElement(ElemEnumHandle &elHd)
{
	return keyList[elHd.hashIdx][elHd.arrayIdx].objective;
}

template <class toFind,int nKeyLng>
const YSHASHKEY *YsFixedLengthHashTable <toFind,nKeyLng>::GetKey(ElemEnumHandle &elHd) const
{
	return keyList[elHd.hashIdx][elHd.arrayIdx].key;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::CheckAutoResizingGrow(void)
{
	const YSSIZE_T hashSize=GetHashSize();
	if(enableAutoResizing==YSTRUE)
	{
		if(autoResizingMax==0)
		{
			if(hashSize*4<nElem)
			{
				Resize(nElem);
			}
		}
		else if(hashSize<autoResizingMax && (hashSize<<2)<nElem)
		{
			Resize(YsSmaller <YSSIZE_T> (hashSize*2,autoResizingMax));
		}
	}
	return YSOK;
}

template <class toFind,int nKeyLng>
YSRESULT YsFixedLengthHashTable <toFind,nKeyLng>::CheckAutoResizingShrink(void)
{
	const YSSIZE_T hashSize=GetHashSize();
	if(enableAutoResizing==YSTRUE)
	{
		if(autoResizingMin==0)
		{
			if(nElem<hashSize/4)
			{
				Resize(YsGreater <YSSIZE_T> (nElem,1));
			}
		}
		else if(autoResizingMin<hashSize && nElem<hashSize)
		{
			Resize(YsGreater <YSSIZE_T> (hashSize/2,autoResizingMin));
		}
	}
	return YSOK;
}

////////////////////////////////////////////////////////////

class YsFixedLengthToMultiHashElemEnumHandle
{
public:
	YSSIZE_T idx1,idx2,objectiveIdx;
};

template <class toFind,int nKeyLng,int nDefObjLng>
class YsFixedLengthToMultiHashTable : public YsFixedLengthHashBase <nKeyLng>
{
private:
	class HashElement
	{
	public:
		YSHASHKEY key[nKeyLng];
		YsArray <toFind,nDefObjLng> objective;
	};

public:
	YsFixedLengthToMultiHashTable(YSSIZE_T hSize=16);
	void CleanUp(void);
	void CleanUpThin(void); // Thin clean up.  Same as PrepareTable.
	YSRESULT PrepareTable(void);
	YSRESULT PrepareTable(YSSIZE_T hashSize);
	YSRESULT Resize(YSSIZE_T hashSize);
	YSRESULT CollectGarbage(void);
	YSRESULT AddElement(int nKey,const YSHASHKEY unorderedKey[],const toFind &objective);
	YSRESULT DeleteElement(int nKey,const YSHASHKEY unorderedKey[],const toFind &objective);
	YSRESULT DeleteKey(int nKey,const YSHASHKEY unorderedKey[]);
	const YsArray <toFind,nDefObjLng> *FindElement(int nKey,const YSHASHKEY unorderedKey[]) const;

	template <class SizeType>
	YSRESULT FindElement(SizeType &nFind,const toFind *&find,int nKey,const YSHASHKEY unorderedKey[]) const;

	YsArray <toFind,nDefObjLng> *FindElement(int nKey,const YSHASHKEY unorderedKey[]);
	YSRESULT FindElement(int &nFind,toFind *&find,int nKey,const YSHASHKEY unorderedKey[]);
	YSBOOL CheckKeyExist(int nKey,const YSHASHKEY unorderedKey[]) const;

	/*! Returns a null handle.  You can iterate through keys by the following loop.
	    for(auto handle=hash.NullHandle(); YSOK==FindNextKey(handle); )
	    {
	        // Do something useful.
	    }
	*/
	YsFixedLengthToMultiHashElemEnumHandle NullHandle(void) const;
	YSBOOL IsHandleValid(const YsFixedLengthToMultiHashElemEnumHandle &elHd) const;
	YSRESULT RewindElementEnumHandle(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;
	YSRESULT FindNextKey(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;
	YSRESULT FindNextElement(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;
	const toFind &GetElement(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;
	const YSHASHKEY *GetKey(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;

protected:
	YSRESULT FindEntryIndex(YSSIZE_T &idx1,YSSIZE_T &idx2,int nKey,const YSHASHKEY orderedKey[]) const;
	const HashElement *FindEntry(int nKey,const YSHASHKEY orderedKey[]) const;
	HashElement *FindEntry(int nKey,const YSHASHKEY orderedKey[]);
	HashElement *CreateEntry(int nKey,const YSHASHKEY orderedKey[]);

public:
	/*! \deprecated This function has been deprecated.  To be consistent with YsHashTable
	    and YsFixedLengthHashTable, use RewindElementEnumHandle, which has the same functionality. */
	YSRESULT RewindElemEnumHandle(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;

	/*! \deprecated This function has been deprecated.  To be consistent with YsHashTable
	    and YsFixedLengthHashTable, use FindNextElement, which has the same functionality. */
	YSRESULT FindNextElem(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;

	/*! \deprecated This function has been deprecated.  To be consistent with YsHashTable
	    and YsFixedLengthHashTable, use GetElement, which has the same functionality. */
	const toFind &GetElem(YsFixedLengthToMultiHashElemEnumHandle &elHd) const;

	void EnableAutoResizing(YSSIZE_T minSize,YSSIZE_T maxSize);
	void EnableAutoResizing(void);

	int GetNumUnused(void) const;

	const char *tableName;

private:
	YSRESULT CheckAutoResizingGrow(void);
	YSRESULT CheckAutoResizingShrink(void);

	YSBOOL enableAutoResizing;
	YSSIZE_T autoResizingMin,autoResizingMax;

	YSSIZE_T hashSize;
	YSSIZE_T nElem;
	YsArray <YsArray <HashElement> > keyList;
};


template <class toFind,int nKeyLng,int nDefObjLng>
YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::YsFixedLengthToMultiHashTable(YSSIZE_T hSize)
{
	keyList.Set(hSize,NULL);
	hashSize=hSize;
	nElem=0;
	enableAutoResizing=YSFALSE;
	autoResizingMin=0;
	autoResizingMax=0;

	tableName="";

	EnableAutoResizing();
}

template <class toFind,int nKeyLng,int nDefObjLng>
void YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CleanUp(void)
{
	PrepareTable();
}

template <class toFind,int nKeyLng,int nDefObjLng>
void YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CleanUpThin(void) // Thin clean up.  Same as PrepareTable.
{
	for(YSSIZE_T i=0; i<hashSize; i++)
	{
		keyList[i].Set(0,NULL);
	}
	nElem=0;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::PrepareTable(void)
{
	for(YSSIZE_T i=0; i<hashSize; i++)
	{
		keyList[i].Set(0,NULL);
	}
	nElem=0;
	return YSOK;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::PrepareTable(YSSIZE_T hSize)
{
	YSSIZE_T i;
	keyList.Set(0,NULL);  // <- This will invoke Shrink. 2005/05/20
	keyList.Set(hSize,NULL);
	hashSize=hSize;
	for(i=0; i<hashSize; i++)
	{
		keyList[i].Set(0,NULL);
	}
	nElem=0;
	return YSOK;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::Resize(YSSIZE_T newHashSize)
{
	YSSIZE_T i,j,idx1,keySum;
	YsArray <HashElement> elemList;
	for(i=0; i<hashSize; i++)
	{
		elemList.Append(keyList[i].GetN(),keyList[i]);
	}
	PrepareTable(newHashSize);  // <- This will reset nElem
	for(i=0; i<elemList.GetN(); i++)
	{
		keySum=0;
		for(j=0; j<nKeyLng; j++)
		{
			keySum+=elemList[i].key[j];;
		}
		idx1=keySum%hashSize;
		keyList[idx1].Append(elemList[i]);
		nElem+=elemList[i].objective.GetN();
	}
	return YSOK;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CollectGarbage(void)
{
	Resize(hashSize);
	return YSOK;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::AddElement(
    int nKey,const YSHASHKEY unorderedKey[],const toFind &objective)
{
	YSHASHKEY orderedKey[nKeyLng];
	HashElement *entry;

	if(nKey==nKeyLng)
	{
		this->OrderKey(orderedKey,unorderedKey);
		entry=FindEntry(nKey,orderedKey);
		if(entry!=NULL)
		{
			entry->objective.Append(objective);
			nElem++;
			CheckAutoResizingGrow();
			return YSOK;
		}
		else
		{
			entry=CreateEntry(nKey,orderedKey);
			if(entry!=NULL)
			{
				entry->objective.Append(objective);
				nElem++;
				CheckAutoResizingGrow();
				return YSOK;
			}
			else
			{
				YsErrOut("YsFixedLengthToMultiHashTable::AddElement()\n");
				YsErrOut("  Out of memory warning.\n");
				return YSERR;
			}
		}
	}
	else
	{
		YsErrOut("YsFixedLengthToMultiHashTable::AddElement()\n");
		YsErrOut("  Key length does not match.\n");
		return YSERR;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::DeleteElement(
    int nKey,const YSHASHKEY unorderedKey[],const toFind &objective)
{
	YSHASHKEY orderedKey[nKeyLng];

	if(nKey==nKeyLng)
	{
		YSSIZE_T idx1,idx2;
		this->OrderKey(orderedKey,unorderedKey);
		if(FindEntryIndex(idx1,idx2,nKey,orderedKey)==YSOK)
		{
			int i;
			for(i=0; i<keyList[idx1][idx2].objective.GetN(); i++)
			{
				if(keyList[idx1][idx2].objective[i]==objective)
				{
					keyList[idx1][idx2].objective.DeleteBySwapping(i);
					if(keyList[idx1][idx2].objective.GetN()==0)
					{
						keyList[idx1].DeleteBySwapping(idx2);
					}
					nElem--;
					CheckAutoResizingShrink();
					return YSOK;
				}
			}
		}
		YsErrOut("YsFixedLengthToMultiHashTable::DeleteElement()\n");
		YsErrOut("  Objective not found.\n");
		return YSERR;
	}
	else
	{
		YsErrOut("YsFixedLengthToMultiHashTable::DeleteElement()\n");
		YsErrOut("  Key length does not match.\n");
		return YSERR;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::DeleteKey(int nKey,const YSHASHKEY unorderedKey[])
{
	YSHASHKEY orderedKey[nKeyLng];

	if(nKey==nKeyLng)
	{
		YSSIZE_T idx1,idx2;
		this->OrderKey(orderedKey,unorderedKey);
		if(FindEntryIndex(idx1,idx2,nKey,orderedKey)==YSOK)
		{
			nElem-=keyList[idx1][idx2].objective.GetN();
			keyList[idx1].DeleteBySwapping(idx2);
			CheckAutoResizingShrink();
			return YSOK;
		}
		return YSERR;
	}
	else
	{
		YsErrOut("YsFixedLengthToMultiHashTable::DeleteKey()\n");
		YsErrOut("  Key length does not match.\n");
		return YSERR;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
const YsArray <toFind,nDefObjLng> *YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindElement(int nKey,const YSHASHKEY unorderedKey[]) const
{
	const HashElement *entry;
	YSHASHKEY orderedKey[nKeyLng];

	if(nKey==nKeyLng)
	{
		this->OrderKey(orderedKey,unorderedKey);
		entry=FindEntry(nKey,orderedKey);
		if(entry!=NULL)
		{
			return &entry->objective;
		}
		return NULL;
	}
	else
	{
		YsErrOut("YsFixedLengthToMultiHashTable::FindElement()\n");
		YsErrOut("  Key length does not match.\n");
		return NULL;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
template <class SizeType>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindElement(SizeType &nFind,const toFind *&find,int nKey,const YSHASHKEY unorderedKey[]) const
{
	const YsArray <toFind,nDefObjLng> *foundArray;
	foundArray=FindElement(nKey,unorderedKey);
	if(foundArray!=NULL && foundArray->GetN()>0)
	{
		nFind=(SizeType)foundArray->GetN();
		find=foundArray->GetArray();
		return YSOK;
	}
	else
	{
		nFind=0;
		find=NULL;
		return YSERR;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
YsArray <toFind,nDefObjLng> *YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindElement(int nKey,const YSHASHKEY unorderedKey[])
{
	HashElement *entry;
	YSHASHKEY orderedKey[nKeyLng];

	if(nKey==nKeyLng)
	{
		this->OrderKey(orderedKey,unorderedKey);
		entry=FindEntry(nKey,orderedKey);
		if(entry!=NULL)
		{
			return &entry->objective;
		}
		return NULL;
	}
	else
	{
		YsErrOut("YsFixedLengthToMultiHashTable::FindElement()\n");
		YsErrOut("  Key length does not match.\n");
		return NULL;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindElement
   (int &nFind,toFind *&find,int nKey,const YSHASHKEY unorderedKey[])
{
	YsArray <toFind,nDefObjLng> *foundArray;
	foundArray=FindElement(nKey,unorderedKey);
	if(foundArray!=NULL && foundArray->GetN()>0)
	{
		nFind=foundArray->GetN();
		find=foundArray->GetEditableArray();
		return YSOK;
	}
	else
	{
		nFind=0;
		find=NULL;
		return YSERR;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSBOOL YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CheckKeyExist
   (int nKey,const YSHASHKEY unorderedKey[]) const
{
	const YsArray <toFind,nDefObjLng> *entry;
	entry=FindElement(nKey,unorderedKey);
	if(entry!=NULL && entry->GetN()>0)
	{
		return YSTRUE;
	}
	else
	{
		return YSFALSE;
	}
}

template <class toFind,int nKeyLng,int nDefObjLng>
YsFixedLengthToMultiHashElemEnumHandle YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::NullHandle(void) const
{
	YsFixedLengthToMultiHashElemEnumHandle elHd;
	elHd.idx1=-1;
	elHd.idx2=-1;
	elHd.objectiveIdx=-1;
	return elHd;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSBOOL YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::IsHandleValid(const YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	if(0<=elHd.idx1 && elHd.idx1<hashSize &&
	   YSTRUE==keyList[elHd.idx1].IsInRange(elHd.idx2) &&
	   YSTRUE==keyList[elHd.idx1][elHd.idx2].objective.IsInRange(elHd.objectiveIdx))
	{
		return YSTRUE;
	}
	return YSFALSE;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::RewindElementEnumHandle
   (YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	for(YSSIZE_T i=0; i<hashSize; i++)
	{
		for(YSSIZE_T j=0; j<keyList[i].GetN(); j++)
		{
			if(keyList[i][j].objective.GetN()>0)
			{
				elHd.idx1=i;
				elHd.idx2=j;
				elHd.objectiveIdx=0;
				return YSOK;
			}
		}
	}
	return YSERR;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindNextElement
   (YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	if(YSTRUE!=IsHandleValid(elHd))
	{
		return RewindElementEnumHandle(elHd);
	}

	elHd.objectiveIdx++;
	if(elHd.objectiveIdx<keyList[elHd.idx1][elHd.idx2].objective.GetN())
	{
		return YSOK;
	}
	elHd.idx2++;
	elHd.objectiveIdx=0;
	if(elHd.idx2<keyList[elHd.idx1].GetN())
	{
		return YSOK;
	}

	int i;
	for(i=elHd.idx1+1; i<hashSize; i++)
	{
		int j;
		for(j=0; j<keyList[i].GetN(); j++)
		{
			if(keyList[i][j].objective.GetN()>0)
			{
				elHd.idx1=i;
				elHd.idx2=j;
				elHd.objectiveIdx=0;
				return YSOK;
			}
		}
	}
	return YSERR;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindNextKey
   (YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	if(YSTRUE!=IsHandleValid(elHd))
	{
		return RewindElementEnumHandle(elHd);
	}

	elHd.idx2++;
	elHd.objectiveIdx=0;
	if(elHd.idx2<keyList[elHd.idx1].GetN())
	{
		return YSOK;
	}

	for(YSSIZE_T i=elHd.idx1+1; i<hashSize; i++)
	{
		for(YSSIZE_T j=0; j<keyList[i].GetN(); j++)
		{
			if(keyList[i][j].objective.GetN()>0)
			{
				elHd.idx1=i;
				elHd.idx2=j;
				elHd.objectiveIdx=0;
				return YSOK;
			}
		}
	}
	return YSERR;
}

template <class toFind,int nKeyLng,int nDefObjLng>
const toFind &YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::GetElement
   (YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	return keyList[elHd.idx1][elHd.idx2].objective[elHd.objectiveIdx];
}

template <class toFind,int nKeyLng,int nDefObjLng>
const YSHASHKEY *YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::GetKey
   (YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	return keyList[elHd.idx1][elHd.idx2].key;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindEntryIndex
   (YSSIZE_T &idx1,YSSIZE_T &idx2,int /*nKey*/,const YSHASHKEY orderedKey[]) const
{
	const YSHASHKEY keySum=this->KeySum(orderedKey);

	idx1=keySum%hashSize;
	for(idx2=0; idx2<keyList[idx1].GetN(); idx2++)
	{
		if(YSTRUE==this->SameKey(keyList[idx1][idx2].key,orderedKey))
		{
			return YSOK;
		}
	}

	idx1=-1;
	idx2=-1;
	return YSERR;
}

template <class toFind,int nKeyLng,int nDefObjLng>
const typename YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::HashElement *
    YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindEntry(int nKey,const YSHASHKEY orderedKey[]) const
{
	YSSIZE_T idx1,idx2;
	if(FindEntryIndex(idx1,idx2,nKey,orderedKey)==YSOK)
	{
		return &keyList[idx1][idx2];
	}
	return NULL;
}

template <class toFind,int nKeyLng,int nDefObjLng>
typename YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::HashElement *
    YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindEntry(int nKey,const YSHASHKEY orderedKey[])
{
	YSSIZE_T idx1,idx2;
	if(FindEntryIndex(idx1,idx2,nKey,orderedKey)==YSOK)
	{
		return &keyList[idx1][idx2];
	}
	return NULL;
}

template <class toFind,int nKeyLng,int nDefObjLng>
typename YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::HashElement *
    YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CreateEntry(int nKey,const YSHASHKEY orderedKey[])
{
	YSHASHKEY keySum;
	int i;
	keySum=0;
	for(i=0; i<nKey; i++)
	{
		keySum+=orderedKey[i];
	}

	YSSIZE_T idx1,idx2;
	idx1=keySum%hashSize;
	idx2=keyList[idx1].GetN();

	HashElement newElem;
	for(i=0; i<nKey; i++)
	{
		newElem.key[i]=orderedKey[i];
	}
	keyList[idx1].Append(newElem);
	return &keyList[idx1][idx2];
}

template <class toFind,int nKeyLng,int nDefObjLng>
void YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::EnableAutoResizing(YSSIZE_T minSize,YSSIZE_T maxSize)
{
	enableAutoResizing=YSTRUE;
	autoResizingMin=minSize;
	autoResizingMax=maxSize;
}

template <class toFind,int nKeyLng,int nDefObjLng>
void YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::EnableAutoResizing(void)
{
	enableAutoResizing=YSTRUE;
	autoResizingMin=0;
	autoResizingMax=0;
}

template <class toFind,int nKeyLng,int nDefObjLng>
int YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::GetNumUnused(void) const
{
	YSSIZE_T i,n;
	YsArray <HashElement> elemList;
	n=0;
	for(i=0; i<hashSize; i++)
	{
		n+=keyList[i].GetNUnused();
//		int j;
//		for(j=0; j<keyList[j].GetN(); j++)
//		{
//			n+=keyList[i][j].objective.GetNUnused();
//		}
	}
	return (int)n;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CheckAutoResizingGrow(void)
{
	if(enableAutoResizing==YSTRUE)
	{
		if(autoResizingMax==0)
		{
			if(hashSize*4<nElem)
			{
				Resize(nElem);
			}
		}
		else if(hashSize<autoResizingMax && (hashSize<<2)<nElem)
		{
			Resize(YsSmaller <YSSIZE_T> (hashSize*2,autoResizingMax));
		}
	}
	return YSOK;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::CheckAutoResizingShrink(void)
{
	if(enableAutoResizing==YSTRUE)
	{
		if(autoResizingMin==0)
		{
			if(nElem<hashSize/4)
			{
				Resize(YsGreater <YSSIZE_T> (nElem,1));
			}
		}
		else if(autoResizingMin<hashSize && nElem<hashSize)
		{
			Resize(YsGreater <YSSIZE_T> (hashSize/2,autoResizingMin));
		}
	}
	return YSOK;
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::RewindElemEnumHandle(YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	return RewindElementEnumHandle(elHd);
}

template <class toFind,int nKeyLng,int nDefObjLng>
YSRESULT YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::FindNextElem(YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	return FindNextElement(elHd);
}

template <class toFind,int nKeyLng,int nDefObjLng>
const toFind &YsFixedLengthToMultiHashTable <toFind,nKeyLng,nDefObjLng>::GetElem(YsFixedLengthToMultiHashElemEnumHandle &elHd) const
{
	return GetElement(elHd);
}

////////////////////////////////////////////////////////////

class YsKeyStore
{
private:
	YSBOOL autoResizing;
	YSSIZE_T autoResizeMin,autoResizeMax;
	YSSIZE_T nTab,nKey;
	YsArray <YSHASHKEY> **keyArray;

public:
	class KeyPointer
	{
	public:
		YSSIZE_T row,column;
	};

	/*! Support for STL-like iterator. */
	class const_iterator
	{
	friend class YsKeyStore;

	private:
		const YsKeyStore *table;
		KeyPointer hd;
		YSBOOL ended;
	public:
		inline const_iterator &operator++();
		inline const_iterator operator++(int);
		inline bool operator==(const const_iterator &from);
		inline bool operator!=(const const_iterator &from);
		inline const YSHASHKEY operator*();
	};

	/*! Support for STL-like iterator */
	inline const_iterator begin() const;

	/*! Support for STL-like iterator */
	inline const_iterator end() const;



	YsKeyStore(YSSIZE_T nTable=16);
	YsKeyStore(const YsKeyStore &incoming);
	YsKeyStore(YsKeyStore &&incoming);
	~YsKeyStore();
	void CleanUp(void);
	void CleanUpThin(void);

	/*! This function clean up all keys and tables. */
	void CleanUpDeep(void);

	/*! This function copies incoming two-key store 'incoming' into this two-key store. */
	void CopyFrom(const YsKeyStore &incoming);

	/*! Copy operator. */
	YsKeyStore &operator=(const YsKeyStore &incoming);

	/*! This function moves incoming two-key store 'incoming' into this two-key store. */
	void MoveFrom(YsKeyStore &incoming);

	/*! Move operator. */
	YsKeyStore &operator=(YsKeyStore &&incoming);


	void PrepareTable(void);
	void EnableAutoResizing(YSSIZE_T minSize,YSSIZE_T maxSize);
	void EnableAutoResizing(void);
	void DisableAutoResizing(void);
	void Resize(YSSIZE_T nTable);

	/*! Returns the number of keys stored in this YsKeyStore. */
	YSSIZE_T GetN(void) const;

	/*! Deprecated.  Use GetN() instead. */
	int GetNumKey(void) const;

	int GetNumTable(void) const;

	YSRESULT AddKey(YSHASHKEY key);
	YSRESULT DeleteKey(YSHASHKEY key);
	YSBOOL IsIncluded(YSHASHKEY key) const;

	void NullifyPointer(KeyPointer &ptr) const;
	YSRESULT FindNextKey(KeyPointer &ptr) const;
	YSBOOL IsPointerValid(const KeyPointer &ptr) const;
	YSHASHKEY GetKey(const KeyPointer &ptr) const;

protected:
	void CheckResizeGrow(void);
	void CheckResizeShrink(void);
};

inline YsKeyStore::const_iterator YsKeyStore::begin() const
{
	const_iterator iter;
	iter.table=this;
	iter.ended=YSFALSE;
	NullifyPointer(iter.hd);
	if(YSOK!=FindNextKey(iter.hd))
	{
		iter.ended=YSTRUE;
	}
	return iter;
}

inline YsKeyStore::const_iterator YsKeyStore::end() const
{
	const_iterator iter;
	iter.table=this;
	iter.ended=YSTRUE;
	return iter;
}

inline YsKeyStore::const_iterator &YsKeyStore::const_iterator::operator++()
{
	if(YSTRUE!=ended)
	{
		if(YSOK!=table->FindNextKey(hd))
		{
			ended=YSTRUE;
		}
	}
	return *this;
}

inline YsKeyStore::const_iterator YsKeyStore::const_iterator::operator++(int)
{
	const_iterator copy=*this;
	if(YSTRUE!=ended)
	{
		if(YSOK!=table->FindNextKey(hd))
		{
			ended=YSTRUE;
		}
	}
	return copy;
}


inline bool YsKeyStore::const_iterator::operator==(const YsKeyStore::const_iterator &from)
{
	if(this->ended==YSTRUE && from.ended==YSTRUE)
	{
		return true;
	}
	else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
	        this->hd.row==from.hd.row && this->hd.column==from.hd.column)
	{
		return true;
	}
	return false;
}


inline bool YsKeyStore::const_iterator::operator!=(const YsKeyStore::const_iterator &from)
{
	if(this->ended==YSTRUE && from.ended==YSTRUE)
	{
		return false;
	}
	else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
	        this->hd.row==from.hd.row && this->hd.column==from.hd.column)
	{
		return false;
	}
	return true;
}


inline const YSHASHKEY YsKeyStore::const_iterator::operator*()
{
	return table->GetKey(hd);
}

inline YsKeyStore::const_iterator begin(const YsKeyStore &keystore)
{
	return keystore.begin();
}

inline YsKeyStore::const_iterator end(const YsKeyStore &keystore)
{
	return keystore.end();
}

////////////////////////////////////////////////////////////

class YsTwoKeyStore
{
protected:
	void CheckResizeGrow(void);
	void CheckResizeShrink(void);

	YSBOOL autoResizing;
	YSSIZE_T autoResizeMin,autoResizeMax;
	YSSIZE_T nTab,nKey;
	YsArray <YSHASHKEY> **keyArray;
public:
	class KeyPointer
	{
	public:
		YSSIZE_T row,column;
	};

	/*! Support for STL-like iterator. */
	class const_iterator
	{
	friend class YsTwoKeyStore;

	private:
		const YsTwoKeyStore *table;
		KeyPointer hd;
		YSBOOL ended;
	public:
		inline const_iterator &operator++();
		inline const_iterator operator++(int);
		inline bool operator==(const const_iterator &from);
		inline bool operator!=(const const_iterator &from);
		inline const YsStaticArray <YSHASHKEY,2> operator*();
	};

	/*! Support for STL-like iterator */
	inline const_iterator begin() const;

	/*! Support for STL-like iterator */
	inline const_iterator end() const;



	YsTwoKeyStore(int nTable=16);
	YsTwoKeyStore(const YsTwoKeyStore &incoming);
	YsTwoKeyStore(YsTwoKeyStore &&incoming);
	~YsTwoKeyStore();

	/*! This function clean up all keys, but keep the table size the same. */
	void CleanUp(void);

	/*! This function clean up all keys and tables. */
	void CleanUpDeep(void);

	/*! This function copies incoming two-key store 'incoming' into this two-key store. */
	void CopyFrom(const YsTwoKeyStore &incoming);

	/*! Copy operator. */
	YsTwoKeyStore &operator=(const YsTwoKeyStore &incoming);

	/*! This function moves incoming two-key store 'incoming' into this two-key store. */
	void MoveFrom(YsTwoKeyStore &incoming);

	/*! Move operator. */
	YsTwoKeyStore &operator=(YsTwoKeyStore &&incoming);


	void PrepareTable(void);
	void EnableAutoResizing(YSSIZE_T minSize,YSSIZE_T maxSize);
	void EnableAutoResizing(void);
	void DisableAutoResizing(void);
	void Resize(YSSIZE_T nTable);

	/*! Returns the number of keys stored in this YsKeyStore. */
	YSSIZE_T GetN(void) const;

	/*! Deprecated.  Use GetN() instead. */
	int GetNumKey(void) const;

	int GetNumTable(void) const;

	YSRESULT AddKey(YSHASHKEY key1,YSHASHKEY key2);
	YSRESULT DeleteKey(YSHASHKEY key1,YSHASHKEY key2);
	YSBOOL IsIncluded(YSHASHKEY key1,YSHASHKEY key2) const;

	void NullifyPointer(KeyPointer &ptr) const;
	YSRESULT FindNextKey(KeyPointer &ptr) const;
	YSBOOL IsPointerValid(const KeyPointer &ptr) const;
	YsStaticArray <YSHASHKEY,2> GetKey(const KeyPointer &ptr) const;
};

inline YsTwoKeyStore::const_iterator YsTwoKeyStore::begin() const
{
	const_iterator iter;
	iter.table=this;
	iter.ended=YSFALSE;
	NullifyPointer(iter.hd);
	if(YSOK!=FindNextKey(iter.hd))
	{
		iter.ended=YSTRUE;
	}
	return iter;
}

inline YsTwoKeyStore::const_iterator YsTwoKeyStore::end() const
{
	const_iterator iter;
	iter.table=this;
	iter.ended=YSTRUE;
	return iter;
}

inline YsTwoKeyStore::const_iterator &YsTwoKeyStore::const_iterator::operator++()
{
	if(YSTRUE!=ended)
	{
		if(YSOK!=table->FindNextKey(hd))
		{
			ended=YSTRUE;
		}
	}
	return *this;
}

inline YsTwoKeyStore::const_iterator YsTwoKeyStore::const_iterator::operator++(int)
{
	const_iterator copy=*this;
	if(YSTRUE!=ended)
	{
		if(YSOK!=table->FindNextKey(hd))
		{
			ended=YSTRUE;
		}
	}
	return copy;
}


inline bool YsTwoKeyStore::const_iterator::operator==(const YsTwoKeyStore::const_iterator &from)
{
	if(this->ended==YSTRUE && from.ended==YSTRUE)
	{
		return true;
	}
	else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
	        this->hd.row==from.hd.row && this->hd.column==from.hd.column)
	{
		return true;
	}
	return false;
}


inline bool YsTwoKeyStore::const_iterator::operator!=(const YsTwoKeyStore::const_iterator &from)
{
	if(this->ended==YSTRUE && from.ended==YSTRUE)
	{
		return false;
	}
	else if(this->ended!=YSTRUE && from.ended!=YSTRUE &&
	        this->hd.row==from.hd.row && this->hd.column==from.hd.column)
	{
		return false;
	}
	return true;
}


inline const YsStaticArray <YSHASHKEY,2> YsTwoKeyStore::const_iterator::operator*()
{
	return table->GetKey(hd);
}

inline YsTwoKeyStore::const_iterator begin(const YsTwoKeyStore &keystore)
{
	return keystore.begin();
}

inline YsTwoKeyStore::const_iterator end(const YsTwoKeyStore &keystore)
{
	return keystore.end();
}



/* } */
#endif




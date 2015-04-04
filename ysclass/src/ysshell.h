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

File Name: ysshell.h
//////////////////////////////////////////////////////////// */

#ifndef YSSHELL_IS_INCLUDED
#define YSSHELL_IS_INCLUDED
/* { */

#include "ysdef.h"
#include "yslist2.h"
#include "ysproperty.h"
#include "ysshelltype.h"
#include "yshash.h"
#include "ysstring.h"
#include "ysgeometry.h"

#ifdef YS_COMPILING_YSSHELL_CPP
class YsShell;
static YSRESULT YsBlendShell1to2
    (YsShell &newShell,
     const YsShell &sh1,const YsShell &sh2,int f,YSBOOL elimOverlap);
#endif


// YsBaseClass
//   YsShell

// Declaration /////////////////////////////////////////////

class YsShellSpecialAttribute
	// If an application wants to have special attributes :
	//     Inherit YsShell class and override functions
	//             YsShell::AttachSpecialAttributeToNewVertex(),
	//             YsShell::AttachSpecialAttributeToNewPolygon(),
	//             YsShell::DetachSpecialAttributeBeforeDeletingVertex() and
	//             YsShell::AttachSpecialAttributeBeforeDeletingPolygon().
	//      In this case, special attribute must be created
	//      and attached inside AttachSpecialAttributeToNew?????()
	//      and must be detach & deleted inside DetachSpecialAttributeBeforeDeleting?????().
	//      To detach, program must call function YsShellItem::SpecialAttributeDetached().
	//      This will guarantee that all vertices and polygons have
	//      necessary attributes.
{
};

class YsShellItem
{
public:
	YsShellItem();
	~YsShellItem();
	void Initialize(void);
	unsigned GetSearchKey(void) const;

	void CopyAttribute(const YsShellItem &from);

	int freeAttribute0; // Use free for any purpose
	int freeAttribute1; // Use free for any purpose
	int freeAttribute2; // Use free for any purpose
	mutable int freeAttribute3; // Use free for any purpose
	mutable int freeAttribute4;

	unsigned searchKey; // Key for hash search

	// const class YsShell *belongTo;  // belongTo is set only in CreateVertex
	// 2006/02/07 Use vtHd/plHd->GetContainer() instead of belongTo

	YSRESULT SetSpecialAttribute(YsShellSpecialAttribute *attrib);
	YsShellSpecialAttribute *GetSpecialAttribute(void);
	const YsShellSpecialAttribute *GetSpecialAttribute(void) const;
	void SpecialAttributeDetached(void);

protected:
	// YsShellItem will initialize 'special' to NULL.
	// But never delete it.
	YsShellSpecialAttribute *special;
};

// Declaration /////////////////////////////////////////////
class YsShellVertex : public YsShellItem
{
public:
	YsShellVertex();
	void Initialize(void);

	YSRESULT IncrementReferenceCount(void);
	YSRESULT DecrementReferenceCount(void);
	YSRESULT SetPosition(const YsVec3 &pos);
	YSRESULT SetNormal(const YsVec3 &nom);
	const YsVec3 &GetPosition(void) const;
	const YsVec3 &GetNormal(void) const;
	int GetReferenceCount(void) const;

	YSRESULT SetReferenceCount(int n);  // This function must be eliminated soon

	YsShellVertexHandle Mapper(const class YsShell &owner) const;
	void SetMapper(class YsShell &owner,YsShellVertexHandle toVtHd);
	union
	{
		const YsListItem <class YsShell2dVertex> *v2HdMap;  // 2005/01/21 For co-operation with YsShell2d
	};

protected:
	YsVec3 pos;
	int refCount;
#ifndef YSLOWMEM
	YsVec3 nom;
#endif
};

// Declaration /////////////////////////////////////////////
class YsShellPolygon : public YsShellItem
{
public:
	YsShellPolygon();
	void Initialize(void);

	YSRESULT SetNormal(const YsVec3 &n);
	const YsVec3 &GetNormal(void) const;
	YSRESULT SetColor(const YsColor &c);
	const YsColor &GetColor(void) const;
	YSRESULT SetVertexList(const class YsShell &shl,YSSIZE_T nVtId,const int vtId[]);
	YSRESULT SetVertexList(YSSIZE_T nVtx,const YsShellVertexHandle vtx[]);
	int GetNumVertex(void) const;
	const YsShellVertexHandle *GetVertexArray(void) const;
	int GetVertexId(YSSIZE_T i)const;
	YsShellVertexHandle GetVertexHandle(YSSIZE_T i) const;
	void Invert(void);

protected:
	YsVec3 nom;
	YsArray <YsShellVertexHandle,4> idx;
#ifndef YSLOWMEM
	YsColor col;
#endif
};

class YsShellReadSrfVariable
{
public:
	YsArray <YsString,16> args;
	int mode;
	YsShellPolygonHandle plHd;
	YSBOOL noShadingPolygon;
	YsArray <YsShellVertexHandle,16> plVtHdList;
	YsArray <YsShellVertexHandle> allVtHdList;
	YsArray <YsShellPolygonHandle> allPlHdList;
	YsColor color;
	YsVec3 nom;

	int errorCount;

	YsShellReadSrfVariable();
	void PrintErrorMessage(const char msg[]);
};


////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////

class YsShellRepairFlipDirectionInfo
{
public:
	YsShellPolygonHandle plHd;
	YsShellVertexHandle vtHd1,vtHd2;
};

class YsShellVertexHandleKeyPair
{
public:
	YsShellVertexHandle vtHd;
	unsigned searchKey;
};

class YsShellPolygonHandlehKeyPair
{
public:
	YsShellVertexHandle plHd;
	unsigned searchKey;
};

typedef enum 
{
	YSSHELL_NOELEM,
	YSSHELL_VERTEX,
	YSSHELL_EDGE,
	YSSHELL_POLYGON
} YSSHELLELEMTYPE;

class YsShellElemType
{
public:
	YSSHELLELEMTYPE elemType;
	union
	{
		YsShellVertexHandle vtHd;
		YsShellVertexHandle edVtHd[2];
		YsShellPolygonHandle plHd;
	};
};

// Declaration /////////////////////////////////////////////
class YsShellSearchTable;


class YsShell
{
// CAUTION!
// Whenever add an item, do not forget to modify operator=
private:
	mutable YsListAllocator <YsShellVertex> vertexStore;
	mutable YsListAllocator <YsShellPolygon> polygonStore;

	// Removing mapper from YsShellVertex
	YsHashTable <YsShellVertexHandle> vtxMapping;

protected:
// Members to copy in operator= {
	YSBOOL matrixIsSet;
	YsMatrix4x4 mat;
	YsListContainer <YsShellPolygon> plg;
	YsListContainer <YsShellVertex> vtx;
// }

	// In order to help implement Undo/Redo
	YsListContainer <YsShellPolygon> frozenPlg;
	YsListContainer <YsShellVertex> frozenVtx;


public:
	/*! Support for STL-like iterator */
	class VertexEnumerator
	{
	public:
		const YsShell *shl;

		class iterator
		{
		public:
			const YsShell *shl;
			YsShellVertexHandle prev,vtHd,next;
			int dir;
		private:
			inline void Forward(void)
			{
				if(0<=dir)
				{
					prev=vtHd;
					vtHd=next;
					next=shl->FindNextVertex(vtHd);
				}
				else
				{
					next=vtHd;
					vtHd=prev;
					prev=shl->FindPrevVertex(vtHd);
				}
			}
			inline void Backward(void)
			{
				if(0<=dir)
				{
					next=vtHd;
					vtHd=prev;
					prev=shl->FindPrevVertex(vtHd);
				}
				else
				{
					prev=vtHd;
					vtHd=next;
					next=shl->FindNextVertex(vtHd);
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
				return (shl==from.shl && vtHd==from.vtHd);
			}
			inline bool operator!=(const iterator &from)
			{
				return (shl!=from.shl || vtHd!=from.vtHd);
			}
			inline YsShellVertexHandle &operator*()
			{
				return vtHd;
			}
		};

		/*! Support for STL-like iterator */
		inline iterator begin() const
		{
			iterator iter;
			iter.shl=shl;
			iter.prev=NULL;
			iter.vtHd=shl->FindNextVertex(NULL);
			iter.next=shl->FindNextVertex(iter.vtHd);
			iter.dir=1;
			return iter;
		}

		/*! Support for STL-like iterator */
		inline iterator end() const
		{
			iterator iter;
			iter.shl=shl;
			iter.prev=NULL;
			iter.vtHd=NULL;
			iter.next=NULL;
			iter.dir=1;
			return iter;
		}

		/*! Support for STL-like iterator */
		inline iterator rbegin() const
		{
			iterator iter;
			iter.shl=shl;
			iter.next=NULL;
			iter.vtHd=shl->FindPrevVertex(NULL);
			iter.prev=shl->FindPrevVertex(iter.vtHd);
			iter.dir=-1;
			return iter;
		}

		/*! Support for STL-like iterator */
		inline iterator rend() const
		{
			iterator iter;
			iter.shl=shl;
			iter.prev=NULL;
			iter.vtHd=NULL;
			iter.next=NULL;
			iter.dir=-1;
			return iter;
		}

		YsArray <YsShellVertexHandle> Array(void) const
		{
			YsArray <YsShellVertexHandle> hdArray(shl->GetNumVertex(),NULL);
			YSSIZE_T idx=0;
			for(auto hd=begin(); hd!=end(); ++hd)
			{
				hdArray[idx]=*hd;
				++idx;
			}
			return hdArray;
		}
	};

	/*! Support for STL-like iterator 
	    Vertices can be iterated by: 
	    YsShell shl; or const YsShell shl;
	    for(auto vtIter=shl.AllVertex().begin(); vtIter!=shl.AllVertex().end(); ++vtIter)
	    {
			(*vtIter) is a YsShellVertexHandle in the loop.
	    }

		Or, for range-based for:
		for(auto vtHd : shl.AllVertex())
		{
			vtHd is a YsShellVertexHandle in the loop.
		}

		Since the iterator caches the next vertex handle in itself, it is safe to delete a vertex inside the loop.
	    */
	inline const VertexEnumerator AllVertex(void) const
	{
		VertexEnumerator allVertex;
		allVertex.shl=this;
		return allVertex;
	}



	/*! Support for STL-like iterator */
	class PolygonEnumerator
	{
	public:
		const YsShell *shl;

		class iterator
		{
		public:
			const YsShell *shl;
			YsShellPolygonHandle prev,plHd,next;
			int dir;
		private:
			inline void Forward(void)
			{
				if(0<=dir)
				{
					prev=plHd;
					plHd=next;
					next=shl->FindNextPolygon(plHd);
				}
				else
				{
					next=plHd;
					plHd=prev;
					prev=shl->FindPrevPolygon(plHd);
				}
			}
			inline void Backward(void)
			{
				if(0<=dir)
				{
					next=plHd;
					plHd=prev;
					prev=shl->FindPrevPolygon(plHd);
				}
				else
				{
					prev=plHd;
					plHd=next;
					next=shl->FindNextPolygon(plHd);
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
				return (shl==from.shl && plHd==from.plHd);
			}
			inline bool operator!=(const iterator &from)
			{
				return (shl!=from.shl || plHd!=from.plHd);
			}
			inline YsShellPolygonHandle &operator*()
			{
				return plHd;
			}
		};

		/*! Support for STL-like iterator */
		inline iterator begin() const
		{
			iterator iter;
			iter.shl=shl;
			iter.prev=NULL;
			iter.plHd=shl->FindNextPolygon(NULL);
			iter.next=shl->FindNextPolygon(iter.plHd);
			iter.dir=1;
			return iter;
		}

		/*! Support for STL-like iterator */
		inline iterator end() const
		{
			iterator iter;
			iter.shl=shl;
			iter.prev=NULL;
			iter.plHd=NULL;
			iter.next=NULL;
			iter.dir=1;
			return iter;
		}

		/*! Support for STL-like iterator */
		inline iterator rbegin() const
		{
			iterator iter;
			iter.shl=shl;
			iter.next=NULL;
			iter.plHd=shl->FindPrevPolygon(NULL);
			iter.prev=shl->FindPrevPolygon(iter.plHd);
			iter.dir=-1;
			return iter;
		}

		/*! Support for STL-like iterator */
		inline iterator rend() const
		{
			iterator iter;
			iter.shl=shl;
			iter.prev=NULL;
			iter.plHd=NULL;
			iter.next=NULL;
			iter.dir=-1;
			return iter;
		}

		YsArray <YsShellPolygonHandle> Array(void) const
		{
			YsArray <YsShellPolygonHandle> hdArray(shl->GetNumPolygon(),NULL);
			YSSIZE_T idx=0;
			for(auto hd=begin(); hd!=end(); ++hd)
			{
				hdArray[idx]=*hd;
				++idx;
			}
			return hdArray;
		}
	};

	/*! Support for STL-like iterator 
	    Polygons can be iterated by: 
	    YsShell shl; or const YsShell shl;
	    for(auto plIter=shl.AllPolygon().begin(); plIter!=shl.AllPolygon().end(); ++plIter)
	    {
			(*plIter) is a YsShellPolygonHandle in the loop.
	    }

		Or, with range-based for loop:
	    for(auto plHd : shl.AllPolygon())
	    {
			plHd is a YsShellPolygonHandle in the loop.
	    }

		Since the iterator caches the next Polygon handle in itself, it is safe to delete a Polygon inside the loop.
	    */
	inline const PolygonEnumerator AllPolygon(void) const
	{
		PolygonEnumerator allPolygon;
		allPolygon.shl=this;
		return allPolygon;
	}



public:
	YsShell();
	YsShell(const YsShell &from);
	~YsShell();

private:
	void CommonInitialization(void);

public:

	YSRESULT AttachSearchTable(YsShellSearchTable *search) const;
	  // NOTE:YsShell will not automatically release memory of search table
	  //      YsShell user is responsible for releasing memory block assigned to the search table
	YSRESULT DetachSearchTable(void) const;
	const YsShellSearchTable *GetSearchTable(void) const;


// Modifiers
public:
	const YsShell &operator=(const YsShell &from);
	void CleanUp(void);
	int AddVertex(const YsVec3 &vec);
	// int AddPolygon(int nv,const int v[]);   2005/01/19 Deleted
	int AddPolygonVertex(YSSIZE_T nv,const YsVec3 v[]);
	YsShellVertexHandle AddVertexH(const YsVec3 &vec);
	YsShellPolygonHandle AddPolygonH(YSSIZE_T nv,const int v[]);
	YsShellPolygonHandle AddPolygonH(YSSIZE_T nv,const YsShellVertexHandle v[]);
	YsShellPolygonHandle AddPolygonVertexH(YSSIZE_T nv,const YsVec3 v[]);
	YSRESULT ModifyVertexPosition(int vtId,const YsVec3 &neoPos);
	YSRESULT ModifyVertexPosition(YsShellVertexHandle vtHd,const YsVec3 &neoPos);
	YSRESULT ModifyPolygon(int plId,YSSIZE_T nv,const int v[]);
	YSRESULT ModifyPolygon(YsShellPolygonHandle plHd,YSSIZE_T nv,const YsShellVertexHandle v[]);
	template <const int N>
	YSRESULT ModifyPolygon(YsShellPolygonHandle plHd,const YsArray <YsShellVertexHandle,N> &plVtHd);
	YSRESULT DeletePolygon(int plId);
	YSRESULT DeletePolygon(YsShellPolygonHandle plHd);
	YSRESULT DeleteVertex(int vtId);
	YSRESULT DeleteVertex(YsShellVertexHandle vtHd);
	YSRESULT DeleteVertexAtTheSamePosition(int nLatX=32,int nLatY=32,int nLatZ=32);
	YSRESULT DeleteVertexAtTheSamePosition(int &nDeleted,int nLatX=32,int nLatY=32,int nLatZ=32);
	YSRESULT MakeVertexMappingToDeleteVertexAtTheSamePositionByKdTree(const double &tol);
	YSRESULT DeleteVertexAtTheSamePositionByKdTree(int &nDeleted,const double &tol=YsTolerance);
	YSRESULT RepairPolygonOrientationBasedOnNormal(void);  // search table required
	YSRESULT RepairPolygonOrientationAssumingClosedShell(void);  // search table required
	YSRESULT RepairPolygonOrientationBasedOnNormal(YsArray <YsShellPolygonHandle> &invertedPlHd);
	YSRESULT RepairPolygonOrientationAssumingClosedShell
	    (YsArray <YsShellPolygonHandle> &invertedPlHdList);  // search table required
	YSBOOL IsOverlappingPolygon(YsShellPolygonHandle plHd);  // search table required
	YSRESULT OmitPolygonAfterBlend(void);
	void SetMatrix(const YsMatrix4x4 &mat);
	YSRESULT Triangulate(YsArray <YsShellPolygonHandle> &newPlHd,YsShellPolygonHandle plHd,YSBOOL killOriginalPolygon);

	void SetTrustPolygonNormal(YSBOOL trust);
	YSBOOL TrustPolygonNormal(void) const;

	YSRESULT FreezeVertex(YsShellVertexHandle vtHd);
	YSRESULT FreezePolygon(YsShellPolygonHandle plHd);
	YSRESULT MeltVertex(YsShellVertexHandle vtHd);
	YSRESULT MeltPolygon(YsShellPolygonHandle plHd);
	YSRESULT DeleteFrozenVertex(YsShellVertexHandle vtHd);
	YSRESULT DeleteFrozenPolygon(YsShellPolygonHandle plHd);
	YSRESULT DisposeFrozenPolygon(void);
	YSRESULT DisposeFrozenVertex(void);

public:
	YsShellSpecialAttribute *GetSpecialAttributeVertex(YsShellVertexHandle vtHd);
	const YsShellSpecialAttribute *GetSpecialAttributeVertex(YsShellVertexHandle vtHd) const;
	YsShellSpecialAttribute *GetSpecialAttributePolygon(YsShellPolygonHandle plHd);
	const YsShellSpecialAttribute *GetSpecialAttributePolygon(YsShellPolygonHandle plHd) const;

	YSRESULT SetSpecialAttributeVertex(YsShellVertexHandle vtHd,YsShellSpecialAttribute *attr);
	YSRESULT SetSpecialAttributePolygon(YsShellPolygonHandle plHd,YsShellSpecialAttribute *attr);
protected:
	virtual YSRESULT AttachSpecialAttributeToNewVertex(YsShellVertex &vtx,YsShellVertexHandle vtHd);
	virtual YSRESULT AttachSpecialAttributeToNewPolygon(YsShellPolygon &plg,YsShellPolygonHandle plHd);
	virtual YSRESULT DetachSpecialAttributeBeforeDeletingVertex(YsShellVertex &vtx);
	virtual YSRESULT DetachSpecialAttributeBeforeDeletingPolygon(YsShellPolygon &plg);

protected:
	YsShellPolygonHandle AddPolygonVertex(YSSIZE_T nv,const YsVec3 v[],int fa3,const int fa4[]);

	mutable YsShellSearchTable *searchTable;

	YSBOOL FindRepairFlipDirectionOfOneReliablePolygon
	    (YsArray <YsShellPolygonHandle> &invertedPlHdList,YsShellPolygonHandle plHd);
	YSRESULT RecursivelyRepairFlipDirection
	    (int &nDone,YsArray <YsShellPolygonHandle> &invertedPlHdList,YsShellPolygonHandle plHd);
	YSRESULT RepairFlipDirectionOfOnePolygon
	    (YsArray <YsShellPolygonHandle> &invertedPlHdList,YsShellRepairFlipDirectionInfo &info);
	YSRESULT AddNeighborToList
	    (YsArray <YsShellRepairFlipDirectionInfo,4096> &todo,YsShellPolygonHandle plHd);

public:
	YSRESULT SaveFreeAttribute4OfPolygon(YsArray <int> &attrib4) const;
	YSRESULT RestoreFreeAttribute4OfPolygon(YsArray <int> &attrib4) const;
	YSRESULT ClearFreeAttribute4OfPolygon(int attrib4);
	YSRESULT SaveFreeAttribute4OfVertex(YsArray <int> &attrib4) const;
	YSRESULT RestoreFreeAttribute4OfVertex(YsArray <int> &attrib4) const;
	YSRESULT ClearFreeAttribute4OfVertex(int attrib4);

protected:
	YSRESULT SaveFreeAttribute4OfPolygonAndNeighbor(
	    YsArray <YsPair <YsShellPolygonHandle,int>,64> &savePlAtt4,
	    int nPl,const YsShellPolygonHandle plHdList[],int freeAtt4Value) const;
	YSRESULT RestoreFreeAttribute4OfPolygon(int nSavePlAtt4,const YsPair <YsShellPolygonHandle,int> savePlAtt4[]) const;
	YSRESULT SaveFreeAttribute4OfVertexAndNeighbor(
	    YsArray <YsPair <YsShellVertexHandle,int>,64> &saveVtAtt4,
	    int nVt,const YsShellVertexHandle vtHdList[],int freeAtt4Value) const;
	YSRESULT RestoreFreeAttribute4OfVertex(int nSaveVtAtt4,const YsPair <YsShellVertexHandle,int> saveVtAtt4[]) const;

// Non modifiers
public:
	void Encache(void) const;
	void Decache(void) const;

	void RewindVtxPtr(void) const;
	YsShellVertexHandle StepVtxPtr(void) const;
	void RewindPlgPtr(void) const;
	YsShellPolygonHandle StepPlgPtr(void) const;

	YsShellVertexHandle FindNextVertex(YsShellVertexHandle current) const;
	YsShellVertexHandle FindPrevVertex(YsShellVertexHandle current) const;
	YsShellPolygonHandle FindNextPolygon(YsShellPolygonHandle current) const;
	YsShellPolygonHandle FindPrevPolygon(YsShellPolygonHandle current) const;
	int GetVertexIdFromHandle(YsShellVertexHandle vtx) const;
	int GetPolygonIdFromHandle(YsShellPolygonHandle plg) const;
	YsShellVertexHandle GetVertexHandleFromId(int vtId) const;
	YsShellPolygonHandle GetPolygonHandleFromId(int plId) const;


	/*! Returns normal vector of the polygon plHd. */
	YsVec3 GetNormal(YsShellPolygonHandle plHd) const;

	/*! Returns normal vector of the polygon in nom.  If plHd does not belong to this YsShell, this
	    function will return YSERR.  */
	YSRESULT GetNormal(YsVec3 &nom,YsShellPolygonHandle plHd) const;

	/*! Returns color of the polygon plHd. */
	YsColor GetColor(YsShellPolygonHandle plHd) const;

	/*! Returns color of the polygon in nom.  If plHd does not belong to this YsShell, this
	    function will return YSERR.  */
	YSRESULT GetColor(YsColor &col,YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetNormal instead.  Left for backward compatibility. */
	YSRESULT GetNormalOfPolygon(YsVec3 &nom,int plId) const;

	/*! Deprecated.  Use GetColor instead.  Left for backward compatibility. */
	YSRESULT GetColorOfPolygon(YsColor &col,int plId) const;

	/*! Deprecated.  Use GetNormal instead.  Left for backward compatibility. */
	YSRESULT GetNormalOfPolygon(YsVec3 &nom,YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetColor instead.  Left for backward compatibility. */
	YSRESULT GetColorOfPolygon(YsColor &col,YsShellPolygonHandle plHd) const;

	YSRESULT ComputeNormalOfPolygonFromGeometry(YsVec3 &nom,YsShellPolygonHandle plHd) const;
	YSRESULT MakePolygonKeyNormalPairFromGeometry(YsPair <unsigned int,YsVec3> &plKeyNomPair,YsShellPolygonHandle plHd) const;
	YSRESULT MakePolygonKeyNormalPairListFromGeometry(YsArray <YsPair <unsigned int,YsVec3> > &plKeyNomPair,int nPl,const YsShellPolygonHandle plHdList[]) const;
	double ComputeDihedralAngle(YsShellPolygonHandle plHd1,YsShellPolygonHandle plHd2) const;
	double ComputeEdgeLength(YsShellVertexHandle vtHd1,YsShellVertexHandle vtHd2) const;
	double ComputeEdgeLength(int nVt,const YsShellVertexHandle vtHd[]) const;
	double ComputeAngleCos(const YsShellVertexHandle vtHd[3]) const;  // Straight -> 0.0
	double ComputeMinimumAngleCos(YsShellPolygonHandle plHd) const;
	double ComputeMinimumAngleCos(int nPlVt,const YsShellVertexHandle plVtHd[]) const;
	double ComputeVolume(void) const;


	/*! Returns center (non-weight average) of vertex positions. */
	const YsVec3 GetCenter(YSSIZE_T nVt,const YsShellVertexHandle vtHd[]) const;

	/*! Returns center (non-weight average) of vertex positions. */
	template <const int N>
	inline const YsVec3 GetCenter(const YsArray <YsShellVertexHandle,N> &vtHdArray) const;

	/*! Returns center (non-weight average) of the vertices of the polygon. */
	const YsVec3 GetCenter(YsShellPolygonHandle plHd) const;

	/*! Returns center of the edge. */
	const YsVec3 GetCenter(YsShellVertexHandle edVtHd0,YsShellVertexHandle edVtHd1) const;

	/*! Returns center of the edge. */
	const YsVec3 GetCenter(const YsShellVertexHandle edVtHd[2]) const;


	const YsVec3 &GetCenterOfPolygon(YsVec3 &cen,YsShellPolygonHandle plHd) const;
	const YsVec3 &GetCenterOfEdge(YsVec3 &mid,YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2) const;
	const YsVec3 &GetCenterOfVertexGroup(YsVec3 &cen,YSSIZE_T  nVt,const YsShellVertexHandle vtHdList[]) const;
	const YsVec3 &GetCenterOfTriangle(YsVec3 &cen,YsShellVertexHandle trVtHd1,YsShellVertexHandle trVtHd2,YsShellVertexHandle trVtHd3) const;
	const YsVec3 &GetArbitraryInsidePointOfPolygon(YsVec3 &isp,YsShellPolygonHandle plHd) const;
	YSRESULT GetArbitraryInsidePointOfPolygonOnPlane(YsVec3 &isp,YsShellPolygonHandle plHd,const YsPlane &pln) const;

	const double GetEdgeSquareLength(YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2) const;
	const double GetEdgeSquareLength(YsShellVertexHandle edVtHd[2]) const;

	const double GetPointToVertexDistance(const YsVec3 &pos,YsShellVertexHandle vtHd) const;
	const double GetVertexToPointDistance(YsShellVertexHandle vtHd,const YsVec3 &pos) const;
	const double GetPointToVertexSquareDistance(const YsVec3 &pos,YsShellVertexHandle vtHd) const;
	const double GetVertexToPointSquareDistance(YsShellVertexHandle vtHd,const YsVec3 &pos) const;

	const double GetEdgeLength(YsShellPolygonHandle plHd,YSSIZE_T edIdx) const;
	const double GetEdgeLength(YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2) const;
	const double GetEdgeLength(YsShellVertexHandle edVtHd[2]) const;
	const double GetEdgeLength(YSSIZE_T  nVt,const YsShellVertexHandle edVtHd[]) const;
	const double GetLoopLength(YSSIZE_T  nVt,const YsShellVertexHandle edVtHd[]) const;

	YSRESULT SetNormalOfPolygon(int plId,const YsVec3 &nom);
	YSRESULT SetColorOfPolygon(int plId,const YsColor &col);
	YSRESULT SetNormalOfPolygon(YsShellPolygonHandle plHd,const YsVec3 &nom);
	YSRESULT SetColorOfPolygon(YsShellPolygonHandle plHd,const YsColor &col);

	YSRESULT ValidateVtId(YSSIZE_T nVtId,const int vtId[]) const;
	YSRESULT ValidatePlId(YSSIZE_T nPlId,const int plId[]) const;

	YSRESULT CheckPotentialPolygon(YSSIZE_T nVt,const YsShellVertexHandle vt[],const double &cosThr);

	int GetNumPolygon(void) const;
	int GetNumVertex(void) const;
	int GetMaxNumVertexOfPolygon(void) const;

	/*! Returns the vertex position in pos. */
	YSRESULT GetVertexPosition(YsVec3 &pos,int vtId) const;

	/*! Returns the vertex position in pos. */
	inline YSRESULT GetVertexPosition(YsVec3 &pos,YsShellVertexHandle vtHd) const;

	/*! Returns the vertex position. */
	inline YsVec3 GetVertexPosition(YsShellVertexHandle vtHd) const;

	/*! Alias for GetVertexPosition. */
	inline YSRESULT GetPosition(YsVec3 &pos,YsShellVertexHandle vtHd) const;

	/*! Alias for GetVertexPosition. */
	inline YsVec3 GetPosition(YsShellVertexHandle vtHd) const;

	template <const int N>
	inline YSRESULT GetVertexPosition(YsArray <YsVec3,N> &vtPos,YSSIZE_T np,const YsShellVertexHandle vtHd[]) const;

	template <const int N,const int M>
	inline YSRESULT GetVertexPosition(YsArray <YsVec3,N> &vtPos,const YsArray <YsShellVertexHandle,M> &vtHd) const;

	int GetVertexReferenceCount(int vtId) const;
	int GetVertexReferenceCount(YsShellVertexHandle vtHd) const;

	// 10/22/2001
	YSRESULT GetAverageNormalAtVertex(YsVec3 &nom,YsShellVertexHandle vtHd) const;

	unsigned GetSearchKey(YsShellVertexHandle vtHd) const;
	unsigned GetSearchKey(YsShellPolygonHandle plHd) const;

	/*! This function returns a search key of the vertex pointed by vtHd even if the vertex is frozen. 
	    GetSearchKey returns a search key only when vtHd is not frozen. */
	unsigned GetSearchKeyMaybeFrozen(YsShellVertexHandle vtHd) const;

	/*! This function returns a search key of the polygon pointed by plHd even if the vertex is frozen. 
	    GetSearchKey returns a search key only when vtHd is not frozen. */
	unsigned GetSearchKeyMaybeFrozen(YsShellPolygonHandle plHd) const;

	void GetBoundingBox(YsVec3 &min,YsVec3 &max) const;

	void GetBoundingBox(YsVec3 bbx[2]) const;

	YsVec3 GetBoundingBoxCenter(void) const;

	/*! Returns the number of vertices of polygon plHd. */
	const int GetPolygonNumVertex(YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetPolygonNumVertex instead. */
	int GetNumVertexOfPolygon(int plId) const;

	/*! Deprecated.  Use GetPolygonNumVertex instead. */
	int GetNumVertexOfPolygon(YsShellPolygonHandle plHd) const;

	/*! Returns the list of vertices used by the polygon plHd. */
	YSRESULT GetPolygon(int &nPlVt,const YsShellVertexHandle *&plVtHd,YsShellPolygonHandle plHd) const;

	/*! Returns the list of vertices used by the polygon plHd. 
	    The polygon can be frozen. */
	YSRESULT GetPolygonMaybeFrozen(int &nPlVt,const YsShellVertexHandle *&plVtHd,YsShellPolygonHandle plHd) const;

	/*! Returns the list of vertices used by the polygon plHd. */
	template <int N>
	inline YSRESULT GetPolygon(YsArray <YsShellVertexHandle,N> &vtHd,YsShellPolygonHandle plHd) const;

	/*! Alias for GetPolygon.  Left for backward compatibility. */
	template <int N>
	inline YSRESULT GetPolygon(YsArray <YsVec3,N> &vtx,YsShellPolygonHandle plHd) const;

	/*! Returns an YsArray of the polygon 'plHd'. */
	inline YsArray <YsShellVertexHandle,4> GetPolygonVertex(YsShellPolygonHandle plHd) const;

	/*! Alias for GetPolygon.  Left for backward compatibility. */
	YSRESULT GetVertexListOfPolygon(int &nPlVt,const YsShellVertexHandle *&plVtHd,YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetPolygon instead of this function.  Left for backward compatibility. */
	YSRESULT GetVertexListOfPolygon(int idx[],YSSIZE_T maxcount,int plId) const;

	/*! Deprecated.  Use GetPolygon instead of this function.  Left for backward compatibility. */
	YSRESULT GetVertexListOfPolygon(YsVec3 vtx[],YSSIZE_T maxcount,int plId) const;

	/*! Deprecated.  Use GetPolygon instead of this function.  Left for backward compatibility. */
	const YsShellVertexHandle *GetVertexListOfPolygon(YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetPolygon instead of this function.  Left for backward compatibility. */
	YSRESULT GetVertexListOfPolygon(YsShellVertexHandle vtHd[],YSSIZE_T maxcount,YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetPolygon instead of this function.  Left for backward compatibility. */
	YSRESULT GetVertexListOfPolygon(YsVec3 vtx[],YSSIZE_T maxcount,YsShellPolygonHandle plHd) const;

	/*! Alias for GetPolygon.  Left for backward compatibility. */
	template <int minSize>
	inline YSRESULT GetVertexListOfPolygon(YsArray <YsShellVertexHandle,minSize> &vtHd,YsShellPolygonHandle plHd) const;

	/*! Alias for GetPolygon.  Left for backward compatibility. */
	template <int minSize>
	YSRESULT GetVertexListOfPolygon(YsArray <YsVec3,minSize> &vtx,YsShellPolygonHandle plHd) const;

	template <int minSize>
	YSRESULT GetVertexListFromPolygonList(YsArray <YsShellVertexHandle,minSize> &vtHdList,YSSIZE_T nPl,const YsShellPolygonHandle plHdList[]) const;

	/*! Returns the area of polygon plHd. */
	const double GetPolygonArea(YsShellPolygonHandle plHd) const;

	/*! Deprecated.  Use GetPolygonArea instead. */
	double GetAreaOfPolygon(YsShellPolygonHandle plHd) const;

	double GetHeightOfPolygon(YsShellPolygonHandle plHd,YsShellVertexHandle baseEdVtHd[2]) const;
	double GetPolygonEdgeLength(YsShellPolygonHandle plHd,int edId) const;

	/*! Populate edVtHd[0] and edVtHd[1] with the Nth (zero-based) edge of polygon plHd.
	    N can be negative or greater than the number of polygon vertex, in which case the
	    function will take the polygon vertices as a cycle. */
	inline YSRESULT GetPolygonEdgeVertex(YsShellVertexHandle edVtHd[2],YsShellPolygonHandle plHd,YSSIZE_T edId) const;

	/*! Populate edVtHd0 and edVtHd1 with the Nth (zero-based) edge of polygon plHd.
	    N can be negative or greater than the number of polygon vertex, in which case the
	    function will take the polygon vertices as a cycle. */
	inline YSRESULT GetPolygonEdgeVertex(YsShellVertexHandle &edVtHd1,YsShellVertexHandle &edVtHd2,YsShellPolygonHandle plHd,YSSIZE_T edId) const;

	inline YsShellVertexHandle PlVt(YsShellPolygonHandle plHd,int idx) const;
	inline const YsVec3 &PlVtPos(YsShellPolygonHandle plHd,int idx) const;
	inline YsShellVertexHandle PlVtLp(YsShellPolygonHandle plHd,int idx) const;
	inline const YsVec3 &PlVtPosLp(YsShellPolygonHandle plHd,int idx) const;


	YSRESULT GetPolygonGroupByNormalSimilarity
	   (YsArray <YsShellPolygonHandle,16> &plHdList,YsShellPolygonHandle seedPlHd,
	    YsKeyStore *excludePlKey=NULL,const double &cosThr=1.0-YsTolerance) const;
	YSRESULT GetCircumferentialAngleOfPolygonGroup
	   (double &minAngle,YsShellVertexHandle &minVtHd,double &maxAngle,YsShellVertexHandle &maxVtHd,
	    const YsVec3 &org,const YsVec3 &axs,int nPl,const YsShellPolygonHandle *plHdList) const;

	void GetMatrix(YsMatrix4x4 &mat) const;

	YsShellPolygon *GetPolygon(int id);
	YsShellVertex *GetVertex(int id);
	const YsShellPolygon *GetPolygon(int id) const;
	const YsShellVertex *GetVertex(int id) const;
	inline YsShellPolygon *GetPolygon(YsShellPolygonHandle plg);
	inline YsShellVertex *GetVertex(YsShellVertexHandle vtx);
	inline const YsShellPolygon *GetPolygon(YsShellPolygonHandle plHd) const;
	inline const YsShellVertex *GetVertex(YsShellVertexHandle vtHd) const;

	YSBOOL IsFrozen(YsShellPolygonHandle plHd) const;
	YSBOOL IsFrozen(YsShellVertexHandle vtHd) const;

	YSSIDE CountRayIntersection
	    (int &CrossCounter,const YsVec3 &org,const YsVec3 &vec,int plIdSkip=-1,YSBOOL watch=YSFALSE) const;
	YSSIDE CountRayIntersection
	    (int &CrossCounter,const YsVec3 &org,const YsVec3 &vec,YsShellPolygonHandle plHdSkip,YSBOOL watch=YSFALSE) const;
	YSSIDE CountRayIntersection
	    (int &CrossCounter,const YsVec3 &org,const YsVec3 &vec,const class YsShellPolygonStore *limitToThesePlg,const class YsShellPolygonStore *excludePlg,YSBOOL watch=YSFALSE) const;

	YSSIDE CheckInsideSolid(const YsVec3 &pnt,const YsShellPolygonStore *limitToThesePlg=NULL,const YsShellPolygonStore *excludePlg=NULL,YSBOOL watch=YSFALSE) const;

	YSRESULT InvertPolygon(int plId);
	YSRESULT InvertPolygon(YsShellPolygonHandle plHd);


	YSRESULT ShootFiniteRay
	    (YsArray <YsVec3> &is,YsArray <YsShellPolygonHandle> &plHdLst,
	     const YsVec3 &p1,const YsVec3 &p2,const class YsShellLattice *ltc=NULL) const;
	YSRESULT ShootInfiniteRay
	    (YsArray <YsVec3> &is,YsArray <YsShellPolygonHandle> &plHdLst,
	     const YsVec3 &org,const YsVec3 &vec,const class YsShellLattice *ltc=NULL) const;




	int ShootRay(YsVec3 &intersect,const YsVec3 &p1,const YsVec3 &vec) const;
	YsShellPolygonHandle ShootRayH(YsVec3 &intersect,const YsVec3 &p1,const YsVec3 &vec) const;
	    // Return PlId or PlHd
	void ShootRay(YsArray <YsVec3> &intersect,const YsVec3 &p1,const YsVec3 &vec) const;


	YSRESULT BuildOctree(class YsShellOctree &oct,int maxDepth,double bbxMargine=1.02) const;

	YSBOOL IsStarShape(const YsVec3 &withRespectTo) const;

	YSRESULT AutoComputeNormal(YsShellPolygonHandle plHd,YSBOOL replaceCurrentNormal,YSBOOL ignoreCurrentNormal);
	YSRESULT AutoComputeNormalAll(YSBOOL replaceCurrentNormal,YSBOOL ignoreCurrentNormal);

	YSRESULT AutoComputeVertexNormalAll(YSBOOL weighted);

protected:
	YSRESULT BlendPolygonVtId
	    (int &nvNew,YsShellVertexHandle vtIdNew[],int nBuf,
	     int nvOrg,const YsShellVertexHandle vtIdOrg[],int nv,const YsShellVertexHandle vtId[]);
	YSBOOL EliminateHangnail(int &nv,YsShellVertexHandle v[]);


	unsigned uniqueSearchKey;
	unsigned GetUniqueSearchKey(void);
	void RewindSearchKey(void);

	YSBOOL trustPolygonNormal;   // Added 2000/11/10

// File IO
public:
	YsShellReadSrfVariable *readSrfVar;

	YSRESULT BeginReadSrf(void);
	YSRESULT BeginMergeSrf(void);
	YSRESULT ReadSrfOneLine
	 (const char str[],
	  YsArray <YsShellPolygonHandle> *noShadingPolygon=NULL,YsArray <YsShellVertexHandle> *roundVtx=NULL);
	YSRESULT EndReadSrf(void);

	YSRESULT LoadSrf(const char fn[],
	    YsArray <YsShellPolygonHandle> *noShadingPolygon=NULL,YsArray <YsShellVertexHandle> *roundVtx=NULL);
	YSRESULT LoadSrf(FILE *fp,
	    YsArray <YsShellPolygonHandle> *noShadingPolygon=NULL,YsArray <YsShellVertexHandle> *roundVtx=NULL);
	YSRESULT MergeSrf(const char fn[],
	    YsArray <YsShellPolygonHandle> *noShadingPolygon=NULL,YsArray <YsShellVertexHandle> *roundVtx=NULL);
	YSRESULT MergeSrf(FILE *fp,
	    YsArray <YsShellPolygonHandle> *noShadingPolygon=NULL,YsArray <YsShellVertexHandle> *roundVtx=NULL);
	YSRESULT LoadTri(const char fn[]);
	YSRESULT LoadMsh(const char fn[]);
	YSRESULT MergeMsh(const char fn[]);
	YSRESULT LoadMs3(const char fn[]);
	YSRESULT MergeMs3(const char fn[]);
	YSRESULT LoadDat(const char fn[]);
	YSRESULT MergeDat(const char fn[]);
	YSRESULT SaveSrf(
	    const char fn[],
	    YSSIZE_T nNoShadingPolygon=0,const YsShellPolygonHandle noShadingPolygon[]=NULL,
	    YSSIZE_T nRoundVtx=0,const YsShellVertexHandle roundVtx[]=NULL) const;
	YSRESULT SaveSrf(
	    class YsTextOutputStream &textOut,
	    YSSIZE_T nNoShadingPolygon=0,const YsShellPolygonHandle noShadingPolygon[]=NULL,
	    YSSIZE_T nRoundVtx=0,const YsShellVertexHandle roundVtx[]=NULL) const;
	YSRESULT SaveMsh(int &nIgnored,const char fn[]) const;
	YSRESULT SaveMs3(int &nIgnored,const char fn[]) const;

	YSRESULT LoadStl(const char fn[]);
	YSRESULT MergeStl(const char fn[]);
	YSRESULT SaveStl(int &nIgnored,const char fn[],const char title[]=NULL) const;  // <- This function ignores non-triangular faces.
	YSRESULT SaveStl(int &nIgnored,FILE *fp,const char title[]=NULL) const;  // <- This function ignores non-triangular faces.
	YSRESULT SaveStl(const char fn[],const char title[]=NULL) const;  // <- This function ignores non-triangular faces.
	YSRESULT SaveStl(FILE *fp,const char title[]=NULL) const;  // <- This function ignores non-triangular faces.
	YSRESULT SaveBinStl(int &nIgnored,const char fn[],const char title[]=NULL) const;
	YSRESULT SaveBinStl(int &nIgnored,FILE *fp,const char title[]=NULL) const;
	YSRESULT SaveBinStl(const char fn[],const char title[]=NULL) const;
	YSRESULT SaveBinStl(FILE *fp,const char title[]=NULL) const;

	friend YSRESULT YsBlendShell1to2(YsShell &,const YsShell &,const YsShell &,int,YSBOOL);



public:
	/*! This function is left for backward compatibility.  The vertices and polygons are not globally memory-pooled any more, and
	    therefore this function does nothing.  Use CollectGarbage for cleaning up per-instance memory pool. */
	static void CleanUpVertexPolygonStore(void);

	/*! This function removeds unused memory pools as much as possible. */
	void CollectGarbage(void) const;
protected:
	YsListItem <YsShellPolygon> *CreatePolygon(void);
	YsListItem <YsShellVertex> *CreateVertex(void);

	// Bounding box becomes valid when
	//    GetBoundingBox function computes bounding box.
	// becomes invalid when
	//    a vertex position is modified
	//    a matrix is set
	//    a vertex is deleted
	//    the shell is cleaned up
	// automatically updated when
	//    a vertex is added outside the bounding box
	mutable YSBOOL bbxIsValid;
	mutable YsVec3 bbx1,bbx2;

public:
	void ResetVertexMapping(void);
	YsShellVertexHandle FindVertexMapping(const YsShellVertex *fromVtx) const;
	YsShellVertexHandle FindVertexMapping(YsShellVertexHandle vtHd) const;
	YSRESULT AddVertexMapping(YsShellVertexHandle fromVtHd,YsShellVertexHandle toVtHd); // fromVtHd needs to be the vertex of this shell.
	YSRESULT AddVertexMapping(const YsShellVertex *fromVtx,YsShellVertexHandle toVtHd); // fromVtHd needs to be the vertex of this shell.
};

template <const int N>
inline const YsVec3 YsShell::GetCenter(const YsArray <YsShellVertexHandle,N> &vtHdArray) const
{
	return GetCenter(vtHdArray.GetN(),vtHdArray);
}

template <const int N>
YSRESULT YsShell::ModifyPolygon(YsShellPolygonHandle plHd,const YsArray <YsShellVertexHandle,N> &plVtHd)
{
	return ModifyPolygon(plHd,plVtHd.GetN(),plVtHd);
}


////////////////////////////////////////////////////////////

/*! Support for STL-like iterator */
inline YsShell::VertexEnumerator::iterator begin(const YsShell::VertexEnumerator &allVertex)
{
	return allVertex.begin();
}

/*! Support for STL-like iterator */
inline YsShell::VertexEnumerator::iterator end(const YsShell::VertexEnumerator &allVertex)
{
	return allVertex.end();
}

/*! Support for STL-like iterator */
inline YsShell::VertexEnumerator::iterator rbegin(const YsShell::VertexEnumerator &allVertex)
{
	return allVertex.rbegin();
}

/*! Support for STL-like iterator */
inline YsShell::VertexEnumerator::iterator rend(const YsShell::VertexEnumerator &allVertex)
{
	return allVertex.rend();
}

/*! Support for STL-like iterator */
inline YsShell::PolygonEnumerator::iterator begin(const YsShell::PolygonEnumerator &allPolygon)
{
	return allPolygon.begin();
}

/*! Support for STL-like iterator */
inline YsShell::PolygonEnumerator::iterator end(const YsShell::PolygonEnumerator &allPolygon)
{
	return allPolygon.end();
}

/*! Support for STL-like iterator */
inline YsShell::PolygonEnumerator::iterator rbegin(const YsShell::PolygonEnumerator &allPolygon)
{
	return allPolygon.rbegin();
}

/*! Support for STL-like iterator */
inline YsShell::PolygonEnumerator::iterator rend(const YsShell::PolygonEnumerator &allPolygon)
{
	return allPolygon.rend();
}

////////////////////////////////////////////////////////////

YsShellPolygon *YsShell::GetPolygon(YsShellPolygonHandle plHd)
{
	if(plHd!=NULL)
	{
		if(plHd->GetContainer()==&plg) // 2006/02/07 plHd->dat.belongTo==this)
		{
			return (YsShellPolygon *)(&plHd->dat);
		}
		else
		{
			YsErrOut("YsShell::GetPolygon()\n");
			YsErrOut("  Trying to access to a polygon by a handle for another shell!!\n");
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

inline YsShellVertex *YsShell::GetVertex(YsShellVertexHandle vtHd)
{
	if(vtHd!=NULL)
	{
		if(vtHd->GetContainer()==&vtx) // 2006/02/07 dat.belongTo==this)
		{
			return (YsShellVertex *)(&vtHd->dat);
		}
		else
		{
			YsErrOut("YsShell::GetVertex()\n");
			YsErrOut("  Trying to access to a vertex by a handle for another shell!!\n");
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

inline const YsShellPolygon *YsShell::GetPolygon(YsShellPolygonHandle plHd) const
{
	if(plHd!=NULL)
	{
		if(plHd->GetContainer()==&plg) // 2006/02/07 dat.belongTo==this)
		{
			return &plHd->dat;
		}
		else
		{
			YsErrOut("YsShell::GetPolygon() const\n");
			YsErrOut("  Trying to access to a polygon by a handle for another shell!!\n");
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

inline const YsShellVertex *YsShell::GetVertex(YsShellVertexHandle vtHd) const
{
	if(vtHd!=NULL)
	{
		if(vtHd->GetContainer()==&vtx) // 2006/02/07 dat.belongTo==this)
		{
			return &vtHd->dat;
		}
		else
		{
			YsErrOut("YsShell::GetVertex() const\n");
			YsErrOut("  Trying to access to a vertex by a handle for another shell!!\n");
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

inline YSRESULT YsShell::GetVertexPosition(YsVec3 &pos,YsShellVertexHandle vtHd) const
{
	const YsShellVertex *vtx=GetVertex(vtHd);
	if(NULL!=vtx)
	{
		if(YSTRUE==matrixIsSet)
		{
			pos=mat*vtx->GetPosition();
		}
		else
		{
			pos=vtx->GetPosition();
		}
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

inline YsVec3 YsShell::GetVertexPosition(YsShellVertexHandle vtHd) const
{
	YsVec3 pos;
	GetVertexPosition(pos,vtHd);
	return pos;
}

inline YSRESULT YsShell::GetPosition(YsVec3 &pos,YsShellVertexHandle vtHd) const
{
	return GetVertexPosition(pos,vtHd);
}

inline YsVec3 YsShell::GetPosition(YsShellVertexHandle vtHd) const
{
	return GetVertexPosition(vtHd);
}

YSRESULT YsShell::GetPolygonEdgeVertex(YsShellVertexHandle edVtHd[2],YsShellPolygonHandle plHd,YSSIZE_T N) const
{
	return GetPolygonEdgeVertex(edVtHd[0],edVtHd[1],plHd,N);
}

YSRESULT YsShell::GetPolygonEdgeVertex(YsShellVertexHandle &edVtHd0,YsShellVertexHandle &edVtHd1,YsShellPolygonHandle plHd,YSSIZE_T N) const
{
	int nPlVt;
	const YsShellVertexHandle *plVtHd;
	if(YSOK==GetVertexListOfPolygon(nPlVt,plVtHd,plHd))
	{
		if(0>N)
		{
			YSSIZE_T mod=N%nPlVt;
			N=mod+nPlVt;
		}
		else if(nPlVt<=N)
		{
			N%=nPlVt;
		}
		edVtHd0=plVtHd[N];
		edVtHd1=plVtHd[(N+1)%nPlVt];
		return YSOK;
	}
	return YSERR;
}

inline YsShellVertexHandle YsShell::PlVt(YsShellPolygonHandle plHd,int idx) const
{
	return plHd->dat.GetVertexHandle(idx);
}

inline const YsVec3 &YsShell::PlVtPos(YsShellPolygonHandle plHd,int idx) const
{
	return plHd->dat.GetVertexHandle(idx)->dat.GetPosition();
}

inline YsShellVertexHandle YsShell::PlVtLp(YsShellPolygonHandle plHd,int idx) const
{
	return plHd->dat.GetVertexHandle(idx%plHd->dat.GetNumVertex());
}

inline const YsVec3 &YsShell::PlVtPosLp(YsShellPolygonHandle plHd,int idx) const
{
	return plHd->dat.GetVertexHandle(idx%plHd->dat.GetNumVertex())->dat.GetPosition();
}

template <int minSize>
inline YSRESULT YsShell::GetVertexListOfPolygon(YsArray <YsShellVertexHandle,minSize> &vtHd,YsShellPolygonHandle plHd) const
{
	int nVtxPlg;
	nVtxPlg=GetNumVertexOfPolygon(plHd);
	if(nVtxPlg>0)
	{
		vtHd.Set(nVtxPlg,GetVertexListOfPolygon(plHd));
		return YSOK;
	}
	return YSERR;
}

template <int N>
inline YSRESULT YsShell::GetPolygon(YsArray <YsShellVertexHandle,N> &vtHd,YsShellPolygonHandle plHd) const
{
	int nPlVt;
	const YsShellVertexHandle *plVtHd;
	if(YSOK==GetPolygon(nPlVt,plVtHd,plHd))
	{
		vtHd.Set(nPlVt,plVtHd);
		return YSOK;
	}
	return YSERR;
}

template <int N>
inline YSRESULT YsShell::GetPolygon(YsArray <YsVec3,N> &vtx,YsShellPolygonHandle plHd) const
{
	int nPlVt;
	const YsShellVertexHandle *plVtHd;
	if(YSOK==GetPolygon(nPlVt,plVtHd,plHd))
	{
		vtx.Set(nPlVt,NULL);
		for(YSSIZE_T idx=0; idx<nPlVt; ++idx)
		{
			GetVertexPosition(vtx[idx],plVtHd[idx]);
		}
		return YSOK;
	}
	return YSERR;
}

inline YsArray <YsShellVertexHandle,4> YsShell::GetPolygonVertex(YsShellPolygonHandle plHd) const
{
	YsArray <YsShellVertexHandle,4> plVtHd;
	GetPolygon(plVtHd,plHd);
	return plVtHd;
}

template <int minSize>
inline YSRESULT YsShell::GetVertexListOfPolygon(YsArray <YsVec3,minSize> &vtx,YsShellPolygonHandle plHd) const
{
	int nVtxPlg;
	nVtxPlg=GetNumVertexOfPolygon(plHd);
	if(nVtxPlg>0)
	{
		int i;
		const YsShellVertexHandle *vtHd;

		vtx.Set(nVtxPlg,NULL);
		vtHd=GetVertexListOfPolygon(plHd);
		for(i=0; i<nVtxPlg; i++)
		{
			YsVec3 vtxPos;
			GetVertexPosition(vtxPos,vtHd[i]);
			vtx.SetItem(i,vtxPos);
		}
		return YSOK;

	}
	return YSERR;
}

template <int minSize>
inline YSRESULT YsShell::GetVertexListFromPolygonList(YsArray <YsShellVertexHandle,minSize> &vtHdList,YSSIZE_T nPl,const YsShellPolygonHandle plHdList[]) const
{
	int i,nVtxMax,nVtx;
	int nPlVt;
	const YsShellVertexHandle *plVtHd;

	nVtxMax=0;
	for(i=0; i<nPl; i++)
	{
		GetVertexListOfPolygon(nPlVt,plVtHd,plHdList[i]);
		nVtxMax+=nPlVt;
	}

	vtHdList.Set(nVtxMax,NULL);
	nVtx=0;
	for(i=0; i<nPl; i++)
	{
		GetVertexListOfPolygon(nPlVt,plVtHd,plHdList[i]);

		int j;
		for(j=0; j<nPlVt; j++)
		{
			vtHdList[nVtx++]=plVtHd[j];;
		}
	}

	YsRemoveDuplicateInUnorderedArray(vtHdList);

	return YSOK;
}

template <const int N>
inline YSRESULT YsShell::GetVertexPosition(YsArray <YsVec3,N> &vtPos,YSSIZE_T np,const YsShellVertexHandle vtHd[]) const
{
	int i;
	vtPos.Set(np,NULL);
	for(i=0; i<np; i++)
	{
		GetVertexPosition(vtPos[i],vtHd[i]);
	}
	return YSOK;
}

template <const int N,const int M>
inline YSRESULT YsShell::GetVertexPosition(YsArray <YsVec3,N> &vtPos,const YsArray <YsShellVertexHandle,M> &vtHd) const
{
	return GetVertexPosition(vtPos,vtHd.GetN(),(const YsShellVertexHandle *)vtHd);
}


////////////////////////////////////////////////////////////


// Declaration /////////////////////////////////////////////

class YsShellEdge
{
public:
	int edId;
	YsShellVertexHandle edVt1,edVt2;
	YsArray <YsShellPolygonHandle> usedBy;
};

class YsShellEdgeList
{
	enum
	{
		HashTableSize=16384,
	};

public:
	YsShellEdgeList();
	YsShellEdgeList(const YsShellEdgeList &from);
	~YsShellEdgeList();

	int GetNumEdge(void) const;
	YSRESULT CleanUp(void);
	YSRESULT Build(const YsShell &shl);
	YSRESULT GetEdge(YsShellVertexHandle &edVt1,YsShellVertexHandle &edVt2,int edId) const;
	YSRESULT LookUpEdge(int &edId,YsShellVertexHandle edVt1,YsShellVertexHandle edVt2,YSBOOL takeReverse=YSTRUE) const;
	YSRESULT LookUpEdge(YsShellEdge *&edge,YsShellVertexHandle edVt1,YsShellVertexHandle edVt2,YSBOOL takeReverse=YSTRUE) const;

	const YsShellEdge *GetEdge(int edId) const;

protected:
	YSRESULT AddEdge(YsShellVertexHandle vtHd1,YsShellVertexHandle vtHd2,YsShellPolygonHandle plId,const YsShell &shl);

	int nEdge;
	YsArray <YsShellEdge *> edgeHash[HashTableSize]; // Hash by (edVtKey1+edVtKey2)%HashTableSize
	YsList <YsShellEdge> *edgeList;
	const YsShell *shell;
};


////////////////////////////////////////////////////////////

class YsShellVertexStore : public YsKeyStore
{
friend class const_iterator;

public:
	/*! In case it needs to iterate through polygon keys, you can write like:
		YsShellPolygonStore plStore;
			for(auto iter=plStore.AllKey().begin(); iter!=plStore.AllKey().end(); ++iter)
			{
				// Do something useful
			}
		or,
			for(auto key : plStore.AllKey()
			{
				// Do something useful
			}
		using range-based for.
	*/
	const YsKeyStore &AllKey(void) const;

	/*! STL-like iterator support. */
	class const_iterator
	{
	friend class YsShellVertexStore;

	private:
		const YsShellVertexStore *table;
		YsKeyStore::const_iterator iter;
	public:
		inline const_iterator &operator++();
		inline const_iterator operator++(int);
		inline bool operator==(const const_iterator &from);
		inline bool operator!=(const const_iterator &from);
		inline YsShellVertexHandle operator*();  // Due to dependency, it is written in ysshellsearch.h
	};

	/*! Support for STL-like iterator */
	inline const_iterator begin() const;

	/*! Support for STL-like iterator */
	inline const_iterator end() const;



	YsShellVertexStore();
	YsShellVertexStore(const YsShell &shl);
	YsShellVertexStore(const YsShell &shl,YSSIZE_T nVtx,const YsShellVertexHandle vtHdArray[]);

	template <const int N>
	YsShellVertexStore(const YsShell &shl,const YsArray <YsShellVertexHandle,N> &vtHdArray);

	void CleanUp(void);
	void SetShell(const YsShell &shl);
	const YsShell &GetShell(void) const;
	YSRESULT AddVertex(YsShellVertexHandle vtHd);
	YSRESULT AddVertex(YSSIZE_T nVt,const YsShellVertexHandle vtHd[]);

	template <const int N>
	YSRESULT AddVertex(const YsArray <YsShellVertexHandle,N> &vtHdArray);

	YSRESULT DeleteVertex(YsShellVertexHandle vtHd);
	YSBOOL IsIncluded(YsShellVertexHandle vtHd) const;

	/*! Returns an array that contains all vertex handles stored in this object. */
	YsArray <YsShellVertexHandle> GetArray(void) const;

protected:
	const YsShell *shl;
};

inline YsShellVertexStore::const_iterator YsShellVertexStore::begin() const
{
	const_iterator iter;
	iter.table=this;
	iter.iter=YsKeyStore::begin();
	return iter;
}

inline YsShellVertexStore::const_iterator YsShellVertexStore::end() const
{
	const_iterator iter;
	iter.table=this;
	iter.iter=YsKeyStore::end();
	return iter;
}

inline YsShellVertexStore::const_iterator &YsShellVertexStore::const_iterator::operator++()
{
	++iter;
	return *this;
}

inline YsShellVertexStore::const_iterator YsShellVertexStore::const_iterator::operator++(int)
{
	const_iterator copy=*this;
	++iter;
	return copy;
}


inline bool YsShellVertexStore::const_iterator::operator==(const YsShellVertexStore::const_iterator &from)
{
	return this->iter==from.iter;
}


inline bool YsShellVertexStore::const_iterator::operator!=(const YsShellVertexStore::const_iterator &from)
{
	return this->iter!=from.iter;
}


inline YsShellVertexStore::const_iterator begin(const YsShellVertexStore &store)
{
	return store.begin();
}

inline YsShellVertexStore::const_iterator end(const YsShellVertexStore &store)
{
	return store.end();
}



template <const int N>
YsShellVertexStore::YsShellVertexStore(const YsShell &shl,const YsArray <YsShellVertexHandle,N> &vtHdArray)
{
	this->shl=&shl;
	EnableAutoResizing();
	AddVertex(vtHdArray.GetN(),vtHdArray);
}

template <const int N>
YSRESULT YsShellVertexStore::AddVertex(const YsArray <YsShellVertexHandle,N> &vtHdArray)
{
	return AddVertex(vtHdArray.GetN(),vtHdArray);
}


////////////////////////////////////////////////////////////


class YsShellPolygonStore : public YsKeyStore
{
friend class const_iterator;

public:
	YsShellPolygonStore();
	YsShellPolygonStore(const YsShell &shl);
	YsShellPolygonStore(const YsShell &shl,int nPlHd,const YsShellPolygonHandle plHdArray[]);

	template <const int N>
	YsShellPolygonStore(const YsShell &shl,const YsArray <YsShellPolygonHandle,N> &plHdArray);

	/*! In case it needs to iterate through polygon keys, you can write like:
		YsShellPolygonStore plStore;
			for(auto iter=plStore.AllKey().begin(); iter!=plStore.AllKey().end(); ++iter)
			{
				// Do something useful
			}
		or,
			for(auto key : plStore.AllKey()
			{
				// Do something useful
			}
		using range-based for.
	*/
	const YsKeyStore &AllKey(void) const;

	/*! Support for STL-like iterator. */
	class const_iterator
	{
	friend class YsShellPolygonStore;

	private:
		const YsShellPolygonStore *table;
		YsKeyStore::const_iterator iter;
	public:
		inline const_iterator &operator++();
		inline const_iterator operator++(int);
		inline bool operator==(const const_iterator &from);
		inline bool operator!=(const const_iterator &from);
		inline YsShellPolygonHandle operator*();  // Due to dependency, it is written in ysshellsearch.h
	};

	/*! Support for STL-like iterator */
	inline const_iterator begin() const;

	/*! Support for STL-like iterator */
	inline const_iterator end() const;



	void CleanUp(void);
	void SetShell(const YsShell &shl);
	const YsShell &GetShell(void) const;
	YSRESULT AddPolygon(YsShellPolygonHandle plHd);
	YSRESULT AddPolygon(int nPl,const YsShellPolygonHandle plHd[]);

	/*! Returns an array that contains all polygon handles stored in this object. */
	YsArray <YsShellPolygonHandle> GetArray(void) const;

	template <const int N>
	YSRESULT AddPolygon(const YsArray <YsShellPolygonHandle,N> &plHdArray);

	YSRESULT DeletePolygon(YsShellPolygonHandle plHd);
	YSBOOL IsIncluded(YsShellPolygonHandle plHd) const;
protected:
	const YsShell *shl;
};

inline YsShellPolygonStore::const_iterator YsShellPolygonStore::begin() const
{
	const_iterator iter;
	iter.table=this;
	iter.iter=YsKeyStore::begin();
	return iter;
}

inline YsShellPolygonStore::const_iterator YsShellPolygonStore::end() const
{
	const_iterator iter;
	iter.table=this;
	iter.iter=YsKeyStore::end();
	return iter;
}

inline YsShellPolygonStore::const_iterator &YsShellPolygonStore::const_iterator::operator++()
{
	++iter;
	return *this;
}

inline YsShellPolygonStore::const_iterator YsShellPolygonStore::const_iterator::operator++(int)
{
	const_iterator copy=*this;
	++iter;
	return copy;
}


inline bool YsShellPolygonStore::const_iterator::operator==(const YsShellPolygonStore::const_iterator &from)
{
	return this->iter==from.iter;
}


inline bool YsShellPolygonStore::const_iterator::operator!=(const YsShellPolygonStore::const_iterator &from)
{
	return this->iter!=from.iter;
}


inline YsShellPolygonStore::const_iterator begin(const YsShellPolygonStore &store)
{
	return store.begin();
}

inline YsShellPolygonStore::const_iterator end(const YsShellPolygonStore &store)
{
	return store.end();
}



template <const int N>
YsShellPolygonStore::YsShellPolygonStore(const YsShell &shl,const YsArray <YsShellPolygonHandle,N> &plHdArray)
{
	this->shl=&shl;
	EnableAutoResizing();
	AddPolygon(plHdArray.GetN(),plHdArray);
}

template <const int N>
YSRESULT YsShellPolygonStore::AddPolygon(const YsArray <YsShellPolygonHandle,N> &plHdArray)
{
	return AddPolygon(plHdArray.GetN(),plHdArray);
}


////////////////////////////////////////////////////////////


class YsShellEdgeStore : public YsTwoKeyStore
{
friend class const_iterator;

protected:
	const YsShell *shl;

public:
	YsShellEdgeStore();
	YsShellEdgeStore(const YsShell &shl);

	/*! In case it needs to iterate through polygon keys, you can write like:
		YsShellPolygonStore plStore;
			for(auto iter=plStore.AllKey().begin(); iter!=plStore.AllKey().end(); ++iter)
			{
				// Do something useful
			}
		or,
			for(auto key : plStore.AllKey()
			{
				// Do something useful
			}
		using range-based for.
	*/
	const YsTwoKeyStore &AllKey(void) const;

	/*! Support for STL-like iterator. */
	class const_iterator
	{
	friend class YsShellEdgeStore;

	private:
		const YsShellEdgeStore *table;
		YsTwoKeyStore::const_iterator iter;
	public:
		inline const_iterator &operator++();
		inline const_iterator operator++(int);
		inline bool operator==(const const_iterator &from);
		inline bool operator!=(const const_iterator &from);
		inline const YsStaticArray <YsShellVertexHandle,2> operator*();  // Due to dependency, it is written in ysshellsearch.h
	};

	/*! Support for STL-like iterator */
	inline const_iterator begin() const;

	/*! Support for STL-like iterator */
	inline const_iterator end() const;



	void CleanUp(void);
	void SetShell(const YsShell &shl);
	const YsShell &GetShell(void) const;
	YSRESULT AddEdge(YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2);
	YSRESULT AddEdge(const YsShellVertexHandle edVtHd[2]);
	YSRESULT DeleteEdge(YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2);
	YSRESULT DeleteEdge(const YsShellVertexHandle edVtHd[2]);
	YSBOOL IsIncluded(YsShellVertexHandle edVtHd1,YsShellVertexHandle edVtHd2) const;
	YSBOOL IsIncluded(const YsShellVertexHandle edVtHd[2]) const;
};



inline YsShellEdgeStore::const_iterator YsShellEdgeStore::begin() const
{
	const_iterator iter;
	iter.table=this;
	iter.iter=YsTwoKeyStore::begin();
	return iter;
}

inline YsShellEdgeStore::const_iterator YsShellEdgeStore::end() const
{
	const_iterator iter;
	iter.table=this;
	iter.iter=YsTwoKeyStore::end();
	return iter;
}

inline YsShellEdgeStore::const_iterator &YsShellEdgeStore::const_iterator::operator++()
{
	++iter;
	return *this;
}

inline YsShellEdgeStore::const_iterator YsShellEdgeStore::const_iterator::operator++(int)
{
	const_iterator copy=*this;
	++iter;
	return copy;
}


inline bool YsShellEdgeStore::const_iterator::operator==(const YsShellEdgeStore::const_iterator &from)
{
	return this->iter==from.iter;
}


inline bool YsShellEdgeStore::const_iterator::operator!=(const YsShellEdgeStore::const_iterator &from)
{
	return this->iter!=from.iter;
}


inline YsShellEdgeStore::const_iterator begin(const YsShellEdgeStore &store)
{
	return store.begin();
}

inline YsShellEdgeStore::const_iterator end(const YsShellEdgeStore &store)
{
	return store.end();
}



////////////////////////////////////////////////////////////


/*! This function is deprecated.  Don't use.  Use YsCheckShellCollisionEx */
YSBOOL YsCheckShellCollision(YsShell &sh1,YsShell &sh2);
	// Note:This function will use freeAttribute3 of YsShellPolygon

YSBOOL YsCheckShellCollisionEx(
    YsVec3 &firstFound,YsShellPolygonHandle &foundPlHd1,YsShellPolygonHandle &foundPlHd2,
    const YsShell &shl1,const YsShell &shl2);

YSRESULT YsBlendShell
    (YsShell &newShell,
     const YsShell &sh1,
     const YsShell &sh2,
     YSBOOLEANOPERATION boolop=YSBOOLBLEND);
    // Note:This function will use freeAttribute2 and 3.

YSRESULT YsSeparateShell
    (YsShell &sh1,YsShell &sh2,const YsShell &org,const YsShell &ref);
    // Note:This function will use freeAttribute2,3 and 4.
    // sh1=org-ref
    // sh2=org&ref

YSRESULT YsReplaceShellEdge
    (YsShell &shl,YsShellVertexHandle vtHd1,YsShellVertexHandle vtHd2,YSSIZE_T nNewEdVt,const YsShellVertexHandle newEdVt[]);
YSRESULT YsAddShell(YsShell &shl1,const YsShell &shl2);

/*! Applies boolean operation to shl0 and shl1 and returns the result in newShell. 
    This function internally calls YsBlendShell3.  
    However, YSBOOLSEPARATE is not supported because only one output shell can be given to this function.
    This function is deprecated and left for backward compatibility. */
YSRESULT YsBlendShell2(
    YsShell &newShell,const YsShell &shl0,const YsShell &shl1,YSBOOLEANOPERATION boolop,
    int ltcX,int ltcY,int ltcZ,int ltcHashSize);

/*! Works same as YsBlendShell2 except that it supports YSBOOLSEPARATE.
    If YSBOOLSEPARATE is given as YSBOOLEANTYPE, subtraction shl0 minus shl1 is returned in newShell, and intersection of shl0 and shl1 is returned in newShell2.  
    newShell2 will not be used if boolop is not YSBOOLSEPARATE.  A dummy YsShell should be given as newShell2 in that
    case. */
YSRESULT YsBlendShell3(
    YsShell &newShell,YsShell &newShell2,const YsShell &shl0,const YsShell &shl1,YSBOOLEANOPERATION boolop,
    int nLtcCell);

YSRESULT YsShellRemoveFreeIntersection(YsShellVertexStore &itscVtx,YsShell &newShell,const YsShell &shl0,int nLtcCell);

YSRESULT YsBlendShellPolygon
    (YsArray <YsShellPolygonHandle> &newPlHd,YsShell &shl,const YsShellPolygonHandle plHd[2],YsShellLattice *ltc);

/* } */
#endif

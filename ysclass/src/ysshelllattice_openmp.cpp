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

File Name: ysshelllattice_openmp.cpp
//////////////////////////////////////////////////////////// */

#include "ysclass.h"
#include "ysshell.h"


#include <omp.h>


void YsShellLatticeElem::Lock(void)
{
	omp_set_lock((omp_lock_t *)ompLock);
}

void YsShellLatticeElem::Unlock(void)
{
	omp_unset_lock((omp_lock_t *)ompLock);
}

////////////////////////////////////////////////////////////

void YsShellLattice::InitLock(void)
{
	if(NULL!=plgToCellTable_ompLock)
	{
		DestroyLock();
	}

	omp_lock_t *ompLock;
	ompLock=new omp_lock_t;
	omp_init_lock(ompLock);

	plgToCellTable_ompLock=ompLock;
}

void YsShellLattice::DestroyLock(void)
{
	if(NULL!=plgToCellTable_ompLock)
	{
		omp_destroy_lock((omp_lock_t *)plgToCellTable_ompLock);
	}
	plgToCellTable_ompLock=NULL;
}

void YsShellLattice::LockPlgToCellTable(void)
{
	omp_set_lock((omp_lock_t *)plgToCellTable_ompLock);
}

void YsShellLattice::UnlockPlgToCellTable(void)
{
	omp_unset_lock((omp_lock_t *)plgToCellTable_ompLock);
}

void YsShellLattice::AssignElementLock(void)
{
	DestroyElementLock();
	int i;
	n_elem_ompLock=(nBlkX+1)*(nBlkY+1)*(nBlkZ+1);

	omp_lock_t *ompLock;
	ompLock=new omp_lock_t [n_elem_ompLock];

	elem_ompLock=ompLock;
	for(i=0; i<n_elem_ompLock; i++)
	{
		omp_init_lock(&ompLock[i]);
		blk[i].ompLock=&ompLock[i];
	}
}

void YsShellLattice::DestroyElementLock(void)
{
	if(NULL!=elem_ompLock)
	{
		omp_lock_t *ompLock;
		ompLock=(omp_lock_t *)elem_ompLock;

		int i;
		for(i=0; i<n_elem_ompLock; i++)
		{
			omp_destroy_lock(&ompLock[i]);
		}

		delete [] ompLock;
		elem_ompLock=NULL;
	}
}

////////////////////////////////////////////////////////////

YSRESULT YsShellLattice::SetDomain_Polygon(const class YsShell &,YSSIZE_T nPl,const YsShellPolygonHandle plHdList[])
{
	int i;
	YsShellPolygonHandle plHd;
	YsLoopCounter ctr;
	YSRESULT res;

	plHd=NULL;
	res=YSOK;

	ctr.BeginCounter(nPl);
	#pragma omp parallel for
	for(i=0; i<nPl; i++)
	{
		#pragma omp critical
		{
			ctr.Increment();
		}

		if(YSOK==res)
		{
			if(AddPolygon(plHdList[i])!=YSOK)
			{
				#pragma omp critical
				{
					YsErrOut("YsShellLattice::SetDomain()\n");
					YsErrOut("  Cannot add polygon\n");
				}
				res=YSERR;
			}
		}
	}
	ctr.EndCounter();


	// Anti-non-determinism. May not be the perfect solution.  But, it should reduce the effect. >>
	const int nCell=(GetNumBlockX()+1)*(GetNumBlockY()+1)*(GetNumBlockZ()+1);

	#pragma omp parallel for
	for(i=0; i<nCell; i++)
	{
		int j;
		YsShellLatticeElem *elem=&blk[i];
		YsArray <unsigned int> keyArray;

		keyArray.Set(elem->vtxList.GetN(),NULL);
		for(j=0; j<elem->vtxList.GetN(); j++)
		{
			keyArray[j]=shl->GetSearchKey(elem->vtxList[j]);
		}
		YsQuickSort <unsigned int,YsShellVertexHandle> (elem->vtxList.GetN(),keyArray,elem->vtxList);

		keyArray.Set(elem->plgList.GetN(),NULL);
		for(j=0; j<elem->plgList.GetN(); j++)
		{
			keyArray[j]=shl->GetSearchKey(elem->plgList[j]);
		}
		YsQuickSort <unsigned int,YsShellPolygonHandle> (elem->plgList.GetN(),keyArray,elem->plgList);

		keyArray.Set(elem->edgList.GetN(),NULL);
		for(j=0; j<elem->edgList.GetN(); j++)
		{
			keyArray[j]=shl->GetSearchKey(elem->edgList[j].edVtHd[0]);
		}
		YsQuickSort <unsigned int,YsShellLatticeEdgePiece> (elem->edgList.GetN(),keyArray,elem->edgList);

		int jStart=0;
		for(j=0; j<elem->edgList.GetN(); j++)
		{
			keyArray[j]=shl->GetSearchKey(elem->edgList[j].edVtHd[1]);
			if(elem->edgList[j].edVtHd[jStart]!=elem->edgList[j].edVtHd[j])
			{
				if(j-jStart>1)
				{
					YsQuickSort <unsigned int,YsShellLatticeEdgePiece> (j-jStart,keyArray.GetEditableArray()+jStart,elem->edgList.GetEditableArray()+jStart);
				}
				jStart=j;
			}
		}
		if(j-jStart>1)
		{
			YsQuickSort <unsigned int,YsShellLatticeEdgePiece> (j-jStart,keyArray.GetEditableArray()+jStart,elem->edgList.GetEditableArray()+jStart);
		}
	}
	// Anti-non-determinism. May not be the perfect solution.  But, it should reduce the effect. <<


	if(YSTRUE==enablePlgToCellTable)
	{
		#pragma omp parallel for
		for(i=0; i<nPl; i++)
		{
			YsArray <YsFixedLengthArray <int,4>,4> *cellList;

			unsigned plgKey[1];

			plgKey[0]=shl->GetSearchKey(plHdList[i]);
			cellList=plgToCellTable.FindElement(1,plgKey);
			if(cellList!=NULL && cellList->GetNumItem()>0)
			{
				int j;
				for(j=0; j<cellList->GetN(); j++)
				{
					const int bx=(*cellList)[j].dat[0];
					const int by=(*cellList)[j].dat[1];
					const int bz=(*cellList)[j].dat[2];
					YSBOOL found=YSFALSE;

					YsShellLatticeElem *elm=GetEditableBlock(bx,by,bz);
					if(NULL!=elm)
					{
						YSSIZE_T k;
						for(k=elm->plgList.GetN()-1; k>=0; k--)
						{
							if(elm->plgList[k]==plHdList[i])
							{
								(*cellList)[j].dat[3]=(int)k;
								found=YSTRUE;
								break;
							}
						}
					}

					if(YSTRUE!=found)
					{
						YsPrintf("Polygon -> Lattice-Cell matching table may be broken.\n");
					}
				}
			}
		}
	}

	return res;
}


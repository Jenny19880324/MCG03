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

File Name: yswizard.cpp
//////////////////////////////////////////////////////////// */

#include "ysclass.h"

// Implementation //////////////////////////////////////////


const char *YsWizard::ClassName="YsWizard";



// Implementation //////////////////////////////////////////
const char *YsCollisionOfPolygon::ClassName="YsCollisionOfPolygon";

YsCollisionOfPolygon::YsCollisionOfPolygon()
{
	np1=0;
	p1=NULL;  // <- External pointer (this class doesn't allocate)
	np2=0;
	p2=NULL;  // <- External pointer (this class doesn't allocate)
}

int YsCollisionOfPolygon::GetNumVertexOfPolygon1(void) const
{
	return (int)np1;
}

const YsVec3 *YsCollisionOfPolygon::GetVertexListOfPolygon1(void) const
{
	return p1;
}

int YsCollisionOfPolygon::GetNumVertexOfPolygon2(void) const
{
	return (int)np2;
}

const YsVec3 *YsCollisionOfPolygon::GetVertexListOfPolygon2(void) const
{
	return p2;
}

void YsCollisionOfPolygon::SetPolygon1(YSSIZE_T nv1,const YsVec3 v1[])
{
	YsVec3 nom;
	YsGetAverageNormalVector(nom,nv1,v1);
	SetPolygon1(nv1,v1,nom);
}

void YsCollisionOfPolygon::SetPolygon1(YSSIZE_T nv1,const YsVec3 v1[],const YsVec3 &nom1)
{
	if(nv1>0)
	{
		np1=nv1;
		p1=v1;

		YsBoundingBoxMaker3 bbx;
		bbx.Make(nv1,v1);
		bbx.Get(p1min,p1max);

		YsVec3 nom;
		nom=nom1;
		if(nom.IsNormalized()!=YSTRUE)
		{
			nom.Normalize();
		}
		pln1.Set(v1[0],nom);

		p1Prj.Set(0,NULL);
	}
}

void YsCollisionOfPolygon::SetPolygon2(YSSIZE_T nv2,const YsVec3 v2[])
{
	YsVec3 nom;
	YsGetAverageNormalVector(nom,nv2,v2);
	SetPolygon2(nv2,v2,nom);
	p2Prj.Set(0,NULL);
}

YSRESULT YsCollisionOfPolygon::PrecomputeProjectionOfPolygon1(void)
{
	if(np1>0 && p1!=NULL)
	{
		if(pln1.GetNormal()!=YsOrigin())
		{
			YsAtt3 att;
			YsVec3 prj;
			YsBoundingBoxMaker2 makeBbx;
			int i;

			att.SetForwardVector(pln1.GetNormal());
			att.GetMatrix4x4(p1PrjMat);
			p1PrjMat.Invert();
			p1PrjMat.Translate(-pln1.GetOrigin());

			p1Prj.Set(np1,NULL);
			makeBbx.Begin();
			for(i=0; i<np1; i++)
			{
				prj=p1PrjMat*p1[i];
				p1Prj[i].GetXY(prj);
				makeBbx.Add(p1Prj[i]);
			}
			makeBbx.Get(p1PrjMin,p1PrjMax);
			return YSOK;
		}
		else
		{
			YsErrOut("YsCollisionOfPolygon::PrecomputeProjectionOfPolygon1()\n");
			YsErrOut("  Normal of polygon1 is not set.\n");
			return YSERR;
		}
	}
	else
	{
		YsErrOut("YsCollisionOfPolygon::PrecomputeProjectionOfPolygon1()\n");
		YsErrOut("  This function must be called after SetPolygon1()\n");
		return YSERR;
	}
}

YSRESULT YsCollisionOfPolygon::PrecomputeProjectionOfPolygon2(void)
{
	if(np2>0 && p2!=NULL)
	{
		if(pln2.GetNormal()!=YsOrigin())
		{
			YsAtt3 att;
			YsVec3 prj;
			YsBoundingBoxMaker2 makeBbx;
			int i;

			att.SetForwardVector(pln2.GetNormal());
			att.GetMatrix4x4(p2PrjMat);
			p2PrjMat.Invert();
			p2PrjMat.Translate(-pln2.GetOrigin());

			p2Prj.Set(np2,NULL);
			makeBbx.Begin();
			for(i=0; i<np2; i++)
			{
				prj=p2PrjMat*p2[i];
				p2Prj[i].GetXY(prj);
				makeBbx.Add(p2Prj[i]);
			}
			makeBbx.Get(p2PrjMin,p2PrjMax);
			return YSOK;
		}
		else
		{
			YsErrOut("YsCollisionOfPolygon::PrecomputeProjectionOfPolygon2()\n");
			YsErrOut("  Normal of polygon2 is not set.\n");
			return YSERR;
		}
	}
	else
	{
		YsErrOut("YsCollisionOfPolygon::PrecomputeProjectionOfPolygon2()\n");
		YsErrOut("  This function must be called after SetPolygon2()\n");
		return YSERR;
	}
}

void YsCollisionOfPolygon::SetPolygon2(YSSIZE_T nv2,const YsVec3 v2[],const YsVec3 &nom2)
{
	if(nv2>0)
	{
		np2=nv2;
		p2=v2;

		YsBoundingBoxMaker3 bbx;
		bbx.Make(nv2,v2);
		bbx.Get(p2min,p2max);

		YsVec3 nom;
		nom=nom2;
		if(nom.IsNormalized()!=YSTRUE)
		{
			nom.Normalize();
		}
		pln2.Set(v2[0],nom);
	}
}

YSBOOL YsCollisionOfPolygon::CheckOnTheSamePlane(void) const
{
	YsVec3 n1,n2,o1;
	pln1.GetNormal(n1);
	pln2.GetNormal(n2);

	if((n1^n2).GetSquareLength()<=YsSqr(YsTolerance))  // Modified 2000/11/10
	{
		pln1.GetOrigin(o1);
		return pln2.CheckOnPlane(o1);
	}
	return YSFALSE;
}

YSBOOL YsCollisionOfPolygon::CheckCollision(void) const
{
	YsVec3 firstFound;
	return CheckCollision(firstFound);
}

YSBOOL YsCollisionOfPolygon::CheckCollision(YsVec3 &firstFound) const
{
	int i,j;
	const YsVec3 *v1,*v2,*u1,*u2;
	YSSIDE is;
	YsVec3 crs;

	if(YsCheckBoundingBoxCollision3(p1min,p1max,p2min,p2max)==YSTRUE)
 	{
		if(np1>=2 || np2>=3) // Polygon/LineSeg1 vs Plane2
		{
			for(i=0; i<np1; i++)
			{
				v1=&p1[i];
				v2=&p1[(i+1)%np1];
				if(pln2.GetPenetration(crs,*v1,*v2)==YSOK)
				{
					is=CheckInsideOfPolygon2(crs);
					// is=YsCheckInsidePolygon3(crs,np2,p2);
					if(is==YSINSIDE || is==YSBOUNDARY)
					{
						firstFound=crs;
						return YSTRUE;
					}
				}
				if(pln2.CheckOnPlane(*v1)==YSTRUE)
				{
					is=CheckInsideOfPolygon2(*v1);
					// is=YsCheckInsidePolygon3(*v1,np2,p2);
					if(is==YSINSIDE || is==YSBOUNDARY)
					{
						firstFound=*v1;
						return YSTRUE;
					}
				}
			}
		}
		if(np1>=3 || np2>=2) // Polygon/LineSeg2 vs Plane1
		{
			for(i=0; i<np2; i++)
			{
				v1=&p2[i];
				v2=&p2[(i+1)%np2];
				if(pln1.GetPenetration(crs,*v1,*v2)==YSOK)
				{
					is=CheckInsideOfPolygon1(crs);
					// is=YsCheckInsidePolygon3(crs,np1,p1);
					if(is==YSINSIDE || is==YSBOUNDARY)
					{
						firstFound=crs;
						return YSTRUE;
					}
				}
				if(pln1.CheckOnPlane(*v1)==YSTRUE)
				{
					is=CheckInsideOfPolygon1(*v1);
					// is=YsCheckInsidePolygon3(*v1,np1,p1);
					if(is==YSINSIDE || is==YSBOUNDARY)
					{
						firstFound=*v1;
						return YSTRUE;
					}
				}
			}
		}

		// If two edges are close to each other...
		// Threshold is 1% of longer edge.
		for(i=0; i<np1; i++)
		{
			v1=&p1[i];
			v2=&p1[(i+1)%np1];

			for(j=0; j<np2; j++)
			{
				u1=&p2[j];
				u2=&p2[(j+1)%np2];

				YsVec3 n1,n2;
				if(YsGetNearestPointOfTwoLine(n1,n2,*v1,*v2,*u1,*u2)==YSOK &&
				   YsCheckInBetween3(n1,*v1,*v2)==YSTRUE &&
				   YsCheckInBetween3(n2,*u1,*u2)==YSTRUE)
				{
					if((n1-n2).GetSquareLength()<YsToleranceSqr)
					{
						firstFound=n1;
						return YSTRUE;
					}
				}
			}
		}
	}
	return YSFALSE;
}

YSINTERSECTION YsCollisionOfPolygon::CheckPenetration(void) const
{
	if(YsCheckBoundingBoxCollision3(p1min,p1max,p2min,p2max)==YSTRUE)
 	{
		return YsGetPolygonPenetration(np1,p1,np2,p2);
	}
	return YSAPART;
}

YSBOOL YsCollisionOfPolygon::OneEdgeLiesOnTheOtherPlane(void) const
{
	int i;
	for(i=0; i<np1; i++)
	{
		if(pln2.CheckOnPlane(p1[i])==YSTRUE &&
		   pln2.CheckOnPlane(p1[(i+1)%np1])==YSTRUE)
		{
			return YSTRUE;
		}
	}

	for(i=0; i<np2; i++)
	{
		if(pln1.CheckOnPlane(p2[i])==YSTRUE &&
		   pln1.CheckOnPlane(p2[(i+1)%np1])==YSTRUE)
		{
			return YSTRUE;
		}
	}

	return YSFALSE;
}

YSBOOL YsCollisionOfPolygon::CheckIntersectionBetweenPolygon1AndBlock(const YsVec3 &corner1,const YsVec3 &corner2) const
{
	int i;
	YsVec3 is,cube[8];
//	YsVec3 face[4];
//	int cubeFaceIdx[6][4]=
//	{
//		{0,1,2,3},
//		{4,5,6,7},
//		{0,1,5,4},
//		{1,2,6,5},
//		{2,3,7,6},
//		{3,0,4,7}
//	};
	static const int cubeEdgeIdx[12][2]=
	{
		{0,1},{1,2},{2,3},{3,0},
		{4,5},{5,6},{6,7},{7,4},
		{0,4},{1,5},{2,6},{3,7}
	};

	int np;
	const YsVec3 *p;
	YsBoundingBoxMaker3 bbx;
	YsVec3 min,max;

	np=GetNumVertexOfPolygon1();
	p=GetVertexListOfPolygon1();

	// Constrain min and max
	bbx.Begin(corner1);
	bbx.Add(corner2);
	bbx.Get(min,max);
	if(YsCheckBoundingBoxCollision3(p1min,p1max,min,max)!=YSTRUE)
	{
		return YSFALSE;
	}
	cube[0].Set(min.x(),min.y(),min.z());
	cube[1].Set(max.x(),min.y(),min.z());
	cube[2].Set(max.x(),max.y(),min.z());
	cube[3].Set(min.x(),max.y(),min.z());
	cube[4].Set(min.x(),min.y(),max.z());
	cube[5].Set(max.x(),min.y(),max.z());
	cube[6].Set(max.x(),max.y(),max.z());
	cube[7].Set(min.x(),max.y(),max.z());

	// If all corners are one side of the plane, no intersection
	int nSide[3];
	nSide[0]=0;
	nSide[1]=0;
	nSide[2]=0;
	for(i=0; i<8; i++)
	{
		// Old Implementation (modified 10/24/2001)
		// nSide[pln1.GetSideOfPlane(cube[i])+1]++;

		// Modified 10/24/2001;
		int side;
		side=pln1.GetSideOfPlane(cube[i]);
		nSide[side+1]++;
		if(side==0)
		{
			YSSIDE sideOfPolygon;
			sideOfPolygon=CheckInsideOfPolygon1(cube[i]);
			// sideOfPolygon=YsCheckInsidePolygon3(cube[i],np1,p1);
			if(sideOfPolygon==YSINSIDE || sideOfPolygon==YSBOUNDARY)
			{
				return YSTRUE;
			}
		}
	}
	if((nSide[0]==0 && nSide[1]==0 && nSide[2]>0) ||
	   (nSide[0]>0 && nSide[1]==0 && nSide[2]==0))
	{
		return YSFALSE;
	}


	// One of the point is contained inside the region
	for(i=0; i<np; i++)
	{
		if(YsCheckInsideBoundingBox3(p[i],min,max)==YSTRUE)
		{
			return YSTRUE;
		}
	}

	// One of the edges of the block intersect with the polygon
	for(i=0; i<12; i++)
	{
		if(CheckIntersectionBetweenPolygon1AndLineSegment(cube[cubeEdgeIdx[i][0]],cube[cubeEdgeIdx[i][1]])==YSTRUE)
		{
			return YSTRUE;
		}
	}

	// One of the face of the box intersect with the polygon (kind of corner cutting)
	const int xComp=0,yComp=1,zComp=2;
	static const int componentTable[3][3]=
	{
		{xComp,yComp,zComp},   // Left,Right face : Normal in xComp direction, region in yComp,zComp plane
		{yComp,xComp,zComp},   // Top,Bottom face : Normal in yComp direction, region in xComp,zComp plane
		{zComp,xComp,yComp}    // Front,Back face : Normal in zComp direction, region in xComp,yComp plane
	};
	static const double dirTable[3][3]=
	{
		{1.0,0.0,0.0},   // Normal for Left/Right face
		{0.0,1.0,0.0},   // Normal for Top/Bottom face
		{0.0,0.0,1.0}    // Normal for Front/Back face
	};

	for(i=0; i<3; i++)
	{
		int j,k;
		YsVec2 plnRegion1,plnRegion2;
		int nomComp,plnComp1,plnComp2;
		YsVec3 nom;

		nomComp=componentTable[i][0];
		plnComp1=componentTable[i][1];
		plnComp2=componentTable[i][2];

		plnRegion1.Set(min.GetValue()[plnComp1]-YsTolerance,min.GetValue()[plnComp2]-YsTolerance);
		plnRegion2.Set(max.GetValue()[plnComp1]+YsTolerance,max.GetValue()[plnComp2]+YsTolerance);
		nom.Set(dirTable[nomComp][0],dirTable[nomComp][1],dirTable[nomComp][2]);

		for(j=0; j<2; j++)
		{
			YsPlane pln;
			YsVec3 org;

			org=(j==0 ? min : max);
			pln.Set(org,nom);

			for(k=0; k<np1; k++)
			{
				YsVec3 x1,x2;
				YsVec2 ref;
				x1=p1[k];
				x2=p1[(k+1)%np1];
				if(pln.GetPenetration(is,x1,x2)==YSOK)
				{
					ref.Set(is.GetValue()[plnComp1],is.GetValue()[plnComp2]);
					if(YsCheckInsideBoundingBox2(ref,plnRegion1,plnRegion2)==YSTRUE)
					{
						return YSTRUE;
					}
				}
			}
		}
	}

	return YSFALSE;
}

YSBOOL YsCollisionOfPolygon::CheckIntersectionBetweenPolygon1AndLineSegment(const YsVec3 &x1,const YsVec3 &x2) const
{
	YsVec3 crs;
	return CheckIntersectionBetweenPolygon1AndLineSegment(crs,x1,x2);
}

YSBOOL YsCollisionOfPolygon::CheckIntersectionBetweenPolygon1AndLineSegment(YsVec3 &crs,const YsVec3 &x1,const YsVec3 &x2) const
{
	YsVec3 bbx1,bbx2;
	YsBoundingBoxMaker3 bbx;
	YSSIDE is;

	bbx.Begin(x1);
	bbx.Add(x2);
	bbx.Get(bbx1,bbx2);

	if(YsCheckBoundingBoxCollision3(p1min,p1max,bbx1,bbx2)==YSTRUE)
	{
		if(pln1.GetPenetration(crs,x1,x2)==YSOK)
		{
			is=CheckInsideOfPolygon1(crs);
			// is=YsCheckInsidePolygon3(crs,np1,p1);
			if(is==YSINSIDE || is==YSBOUNDARY)
			{
				return YSTRUE;
			}
		}
	}
	return YSFALSE;
}

YSSIDE YsCollisionOfPolygon::CheckInsideOfPolygon1(const YsVec3 &tst) const
{
	if(p1Prj.GetN()>0)
	{
		YsVec3 prj;
		YsVec2 xy;
		prj=p1PrjMat*tst;
		xy.GetXY(prj);
		return YsCheckInsidePolygon2(xy,np1,p1Prj);
	}
	else
	{
		return YsCheckInsidePolygon3(tst,np1,p1);
	}
}

YSSIDE YsCollisionOfPolygon::CheckInsideOfPolygon2(const YsVec3 &tst) const
{
	if(p2Prj.GetN()>0)
	{
		YsVec3 prj;
		YsVec2 xy;
		prj=p2PrjMat*tst;
		xy.GetXY(prj);
		return YsCheckInsidePolygon2(xy,np2,p2Prj);
	}
	else
	{
		return YsCheckInsidePolygon3(tst,np2,p2);
	}
}

// Implementation //////////////////////////////////////////
const char *YsBoundingBoxMaker2::ClassName="YsBoundingBoxMaker2";

YsBoundingBoxMaker2::YsBoundingBoxMaker2()
{
	first=YSTRUE;
}

void YsBoundingBoxMaker2::Begin(void)
{
	min.Set(0.0,0.0);
	max.Set(0.0,0.0);
	first=YSTRUE;
}

void YsBoundingBoxMaker2::Begin(const YsVec2 &vec)
{
	min=vec;
	max=vec;
	first=YSFALSE;
}

void YsBoundingBoxMaker2::Add(const YsVec2 &vec)
{
	if(first!=YSTRUE)
	{
		min.Set
		   (YsSmaller(min.x(),vec.x()),
		    YsSmaller(min.y(),vec.y()));
		max.Set
		   (YsGreater(max.x(),vec.x()),
		    YsGreater(max.y(),vec.y()));
	}
	else
	{
		Begin(vec);
	}
}

void YsBoundingBoxMaker2::Make(YSSIZE_T n,const YsVec2 v[])
{
	if(n>0)
	{
		int i;
		Begin(v[0]);
		for(i=1; i<n; i++)
		{
			Add(v[i]);
		}
	}
}

void YsBoundingBoxMaker2::Make(YsList <YsVec2> *lst)
{
	if(lst!=NULL)
	{
		Begin(lst->dat);
	}
	while(lst!=NULL)
	{
		Add(lst->dat);
		lst=lst->Next();
	}
}

void YsBoundingBoxMaker2::Make(const YsArray <YsVec2> &ary)
{
	if(ary.GetNumItem()>0)
	{
		Begin(ary.GetItem(0));
	}
	int i;
	for(i=0; i<ary.GetNumItem(); i++)
	{
		Add(ary.GetItem(i));
	}
}

void YsBoundingBoxMaker2::Get(YsVec2 &n,YsVec2 &x) const
{
	n=min;
	x=max;
}

const YsVec2 &YsBoundingBoxMaker2::GetCenter(YsVec2 &cen) const
{
	cen=(min+max)/2.0;
	return cen;
}

YSBOOL YsBoundingBoxMaker2::IsInside(const YsVec2 &pos) const
{
	return YsCheckInsideBoundingBox2(pos,min,max);
}

YSBOOL YsBoundingBoxMaker2::IsInside(const YsVec3 &pos) const
{
	YsVec2 ref;
	ref.Set(pos.x(),pos.y());
	return YsCheckInsideBoundingBox2(ref,min,max);
}

YsVec2 YsBoundingBoxMaker2::Center(void) const
{
	return (min+max)/2.0;
}


// Implementation //////////////////////////////////////////
const char *YsBoundingBoxMaker3::ClassName="YsBoundingBoxMaker3";

YsBoundingBoxMaker3::YsBoundingBoxMaker3()
{
	first=YSTRUE;
}

void YsBoundingBoxMaker3::Begin(void)
{
	min=YsOrigin();
	max=YsOrigin();
	first=YSTRUE;
}

void YsBoundingBoxMaker3::Begin(const YsVec3 &vec)
{
	min=vec;
	max=vec;
	first=YSFALSE;
}

void YsBoundingBoxMaker3::Add(const YsVec3 &vec)
{
	if(first!=YSTRUE)
	{
		min.Set
		   (YsSmaller(min.x(),vec.x()),
		    YsSmaller(min.y(),vec.y()),
		    YsSmaller(min.z(),vec.z()));
		max.Set
		   (YsGreater(max.x(),vec.x()),
		    YsGreater(max.y(),vec.y()),
		    YsGreater(max.z(),vec.z()));
	}
	else
	{
		Begin(vec);
	}
}

void YsBoundingBoxMaker3::Add(YSSIZE_T nVec,const YsVec3 vec[])
{
	for(YSSIZE_T idx=0; idx<nVec; ++idx)
	{
		Add(vec[idx]);
	}
}

void YsBoundingBoxMaker3::Make(YSSIZE_T n,const YsVec3 v[])
{
	if(n>0)
	{
		int i;
		Begin(v[0]);
		for(i=1; i<n; i++)
		{
			Add(v[i]);
		}
	}
}

void YsBoundingBoxMaker3::Make(const YsList <YsVec3> *lst)
{
	if(lst!=NULL)
	{
		Begin(lst->dat);
	}
	while(lst!=NULL)
	{
		Add(lst->dat);
		lst=lst->Next();
	}
}

void YsBoundingBoxMaker3::Make(const YsShell &shl,YSSIZE_T nVt,const YsShellVertexHandle vtHd[])
{
	int i;
	YsVec3 pos;
	Begin();
	for(i=0; i<nVt; i++)
	{
		shl.GetVertexPosition(pos,vtHd[i]);
		Add(pos);
	}
}

void YsBoundingBoxMaker3::Get(YsVec3 &n,YsVec3 &x) const
{
	n=min;
	x=max;
}

void YsBoundingBoxMaker3::Get(YsVec3 minMax[2]) const
{
	return Get(minMax[0],minMax[1]);
}

const YsVec3 &YsBoundingBoxMaker3::GetCenter(YsVec3 &cen) const
{
	cen=(min+max)/2.0;
	return cen;
}

YSBOOL YsBoundingBoxMaker3::IsInside(const YsVec3 &pos) const
{
	return YsCheckInsideBoundingBox3(pos,min,max);
}

YSBOOL YsBoundingBoxMaker3::IsInside(const YsVec2 &pos) const
{
	YsVec3 ref;
	ref.Set(pos.x(),pos.y(),0.0);
	return YsCheckInsideBoundingBox3(ref,min,max);
}

YSBOOL YsBoundingBoxMaker3::IsInsideXY(const YsVec3 &pos) const
{
	if(min.x()-YsTolerance<=pos.x() && pos.x()<=max.x()+YsTolerance &&
	   min.y()-YsTolerance<=pos.y() && pos.y()<=max.y()+YsTolerance)
	{
		return YSTRUE;
	}
	return YSFALSE;
}

YSBOOL YsBoundingBoxMaker3::IsInsideXY(const YsVec2 &pos) const
{
	if(min.x()-YsTolerance<=pos.x() && pos.x()<=max.x()+YsTolerance &&
	   min.y()-YsTolerance<=pos.y() && pos.y()<=max.y()+YsTolerance)
	{
		return YSTRUE;
	}
	return YSFALSE;
}

YsVec3 YsBoundingBoxMaker3::Center(void) const
{
	return (min+max)/2.0;
}

const double YsBoundingBoxMaker3::GetDiagonalLength(void) const
{
	return (max-min).GetLength();
}


////////////////////////////////////////////////////////////

YSRESULT YsTraceLineSegment::ResetLineSegment(void)
{
	if(lSeg.GetN()>0)
	{
		int i;
		double sl;

		totalLength=0.0;
		segLength.Set(0,NULL);
		segLengthAddUp.Set(0,NULL);
		segLengthAddUp.Append(0.0);
		for(i=0; i<lSeg.GetN()-1; i++)
		{
			sl=(lSeg[i+1]-lSeg[i]).GetLength();
			totalLength+=sl;
			segLength.Append(sl);
			segLengthAddUp.Append(totalLength);
		}
		if(isLoop==YSTRUE)
		{
			sl=(lSeg[0]-lSeg.GetEndItem()).GetLength();
			totalLength+=sl;
			segLength.Append(sl);
			segLengthAddUp.Append(totalLength);
		}

		curPos.pos=lSeg[0];
		curPos.segParam=0.0;
		curPos.seg=0;

		if(totalLength<YsTolerance)
		{
			lSeg.Set(0,NULL);
			return YSERR;
		}

		return YSOK;
	}
	return YSERR;
}

YSRESULT YsTraceLineSegment::SetLineSegment(YSSIZE_T np,const YsVec3 p[],YSBOOL islp)
{
	lSeg.Set(np,p);
	isLoop=islp;
	if(islp==YSTRUE)
	{
		lSeg.Append(lSeg[0]);
	}
	return ResetLineSegment();
}

YSRESULT YsTraceLineSegment::SetLineSegment(YsArray <YsVec3> &p,YSBOOL islp)
{
	lSeg=p;
	isLoop=islp;
	if(islp==YSTRUE)
	{
		lSeg.Append(lSeg[0]);
	}
	return ResetLineSegment();
}

int YsTraceLineSegment::GetCurrentSegment(void) const
{
	return curPos.seg;
}

const YsVec3 &YsTraceLineSegment::GetCurrentPosition(void) const
{
	return curPos.pos;
}

const YsVec3 YsTraceLineSegment::GetCurrentTangent(void) const
{
	YsVec3 t;
	if(curPos.seg<lSeg.GetN()-1)
	{
		t=lSeg[curPos.seg+1]-lSeg[curPos.seg];
		t.Normalize();
		return t;
	}
	else if(2<=lSeg.GetN())
	{
		t=lSeg[lSeg.GetN()-1]-lSeg[lSeg.GetN()-2];
		t.Normalize();
		return t;
	}
	return YsOrigin();
}

const double &YsTraceLineSegment::GetTotalLength(void) const
{
	return totalLength;
}

YSRESULT YsTraceLineSegment::MoveByDistance(const double &dist)
{
	return MoveByDistance(curPos,dist);
}

YSRESULT YsTraceLineSegment::SetPositionByParameter(const double &tt)
{
	return SetPositionByParameter(curPos,tt);
}

YSRESULT YsTraceLineSegment::MoveByDistance(YsLineSegmentTracer &tracer,const double &dist)
{
	if(dist>0.0)
	{
		double distRemain;
		distRemain=dist;
		while(distRemain>0.0)
		{
			double restOfCurSeg;
			restOfCurSeg=(1.0-tracer.segParam)*segLength[tracer.seg];
			if(distRemain<restOfCurSeg)
			{
				tracer.segParam+=distRemain/segLength[tracer.seg];
				tracer.segParam=YsBound(tracer.segParam,0.0,1.0);
				tracer.pos=lSeg[tracer.seg]*(1.0-tracer.segParam)+lSeg[tracer.seg+1]*tracer.segParam;
				distRemain=0.0;
			}
			else
			{
				distRemain-=restOfCurSeg;
				if(tracer.seg<lSeg.GetN()-2)
				{
					tracer.seg++;
					tracer.pos=lSeg[tracer.seg];
					tracer.segParam=0.0;
				}
				else if(isLoop!=YSTRUE)
				{
					tracer.pos=lSeg[tracer.seg+1];
					tracer.segParam=1.0;
					distRemain=0.0;
				}
				else
				{
					tracer.seg=0;
					tracer.pos=lSeg[0];
					tracer.segParam=0.0;
				}
			}
		}
		return YSOK;
	}
	else if(dist<0.0)
	{
		double distRemain;
		distRemain=-dist;
		while(distRemain>0.0)
		{
			double restOfCurSeg;
			restOfCurSeg=tracer.segParam*segLength[tracer.seg];
			if(distRemain<restOfCurSeg)
			{
				tracer.segParam-=distRemain/segLength[tracer.seg];
				tracer.segParam=YsBound(tracer.segParam,0.0,1.0);
				tracer.pos=lSeg[tracer.seg]*(1.0-tracer.segParam)+lSeg[tracer.seg+1]*tracer.segParam;
				distRemain=0.0;
			}
			else
			{
				distRemain-=restOfCurSeg;
				if(tracer.seg>0)
				{
					tracer.seg--;
					tracer.pos=lSeg[tracer.seg+1];
					tracer.segParam=1.0;
				}
				else if(isLoop!=YSTRUE)
				{
					tracer.pos=lSeg[0];
					tracer.segParam=0.0;
					distRemain=0.0;
				}
				else
				{
					tracer.seg=(int)lSeg.GetN()-2;
					tracer.pos=lSeg.GetEndItem();
					tracer.segParam=1.0;
				}
			}
		}
		return YSOK;
	}
	else
	{
		return YSOK;
	}
}

YSRESULT YsTraceLineSegment::SetPositionByParameter(YsLineSegmentTracer &tracer,const double &tt)
{
	double t;
	t=tt;
	if(isLoop==YSTRUE)
	{
		if(t>1.0)
		{
			t=fmod(t,1.0);
		}
		else if(t<0.0)
		{
			t=1.0-fmod(-t,1.0);
		}
	}
	else
	{
		t=YsBound(t,0.0,1.0);
	}

	double tDist;
	tDist=totalLength*t;

	YSSIZE_T n1,n2;
	n1=0;
	n2=segLengthAddUp.GetN()-1;
	while(n2-n1>1)
	{
		YSSIZE_T mid;
		mid=(n1+n2)/2;
		if(segLengthAddUp[mid]<tDist)
		{
			n1=mid;
		}
		else
		{
			n2=mid;
		}
	}

	double distRemain;
	distRemain=tDist-segLengthAddUp[n1];

	tracer.segParam=distRemain/segLength[n1];
	tracer.segParam=YsBound(tracer.segParam,0.0,1.0);
	tracer.pos=lSeg[n1]*(1.0-tracer.segParam)+lSeg[n1+1]*tracer.segParam;
	tracer.seg=(int)n1;

	return YSOK;
}

YSRESULT YsTraceLineSegment::SetPositionByVertex(YsLineSegmentTracer &tracer,int idx)
{
	if(0<=idx && idx<lSeg.GetN()-1)
	{
		tracer.pos=lSeg[idx];
		tracer.seg=idx;
		tracer.segParam=0.0;
		return YSOK;
	}
	else if(idx==lSeg.GetN()-1)
	{
		tracer.pos=lSeg[idx];
		tracer.seg=idx-1;
		tracer.segParam=1.0;
		return YSOK;
	}
	return YSERR;
}

double YsTraceLineSegment::GetParameter(const YsLineSegmentTracer &tracer) const
{
	if((0<=tracer.seg && tracer.seg<lSeg.GetN()-1) ||
	  (tracer.seg==lSeg.GetN()-1 && isLoop==YSTRUE))
	{
		double l;
		l=segLengthAddUp[tracer.seg]+segLength[tracer.seg]*tracer.segParam;
		return l/totalLength;
	}
	return -1.0;
}


//
//  Ship.cpp
//  MCG03
//
//  Created by Jing Li on 12/2/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef M_PI
#define M_PI YsPi
#endif

#include "Ship.h"

Ship::Ship()
{
  readStl();
  //position=YsVec3(10.0,0.0,0.0);
  velocity=YsVec3(0.0,10,0.0);
  YsVec3 axis=-YsXVec();
  double radians=0.0;//YsPi/2.0;
  orientation=Quaternion(axis,radians);

  for(int i=0;i<triangles.size();i++)
    {
      triangles[i].normal=orientation*triangles[i].normal;
      for(int j=0;j<3;j++)
	{
	  triangles[i].vertices[j]=orientation*triangles[i].vertices[j]+YsVec3(0.0,0.0,20.0);
	}
    }

}

Ship::~Ship()
{
}

void Ship::readStl()
{
  shl.LoadStl("/afs/andrew.cmu.edu/usr1/jingli2/private/particleSimulation/svn/trunk/data/waterfall.stl");
  for(auto plHd:shl.AllPolygon())
    {
      Triangle triangle;
      YsVec3 norm;
      shl.GetNormal(norm,plHd);
      triangle.normal=norm;

      YsArray<YsVec3,4>plVtPos;
      shl.GetPolygon(plVtPos,plHd);

      int i=0;
      for(auto &pos:plVtPos)
	{
	  triangle.vertices[i]=pos;
          i++;
	}
      triangles.push_back(triangle);
    }

}

void Ship::move()
{
    for (int i=0; i<triangles.size(); i++) {
      triangles[i].vertices[0]+=orientation*velocity*0.01;
       triangles[i].vertices[1]+=orientation*velocity*0.01;
       triangles[i].vertices[2]+=orientation*velocity*0.01;
    }
}

/*virtual*/ void Ship::collide(Particle &particle) const
{

  GLdouble red[3]={1.0,0.0,0.0};
    for (int i=0; i<triangles.size(); i++) {
      if( collideWithOneTriangle(particle,triangles[i])){
            particle.setColor(red);
	}
    }
}

bool Ship::collideWithOneTriangle( Particle &particle,const Triangle &triangle)const
{
    // TODO detect collision. If there is one, update velocity
    YsVec3 a=particle.getPosition()-triangle.vertices[0];
    YsVec3 n=-triangle.normal;

    //particle is not travelling towards triangle
    YsVec3 v=particle.getVelocity();
    if(v*n>0)
        return false;

    double d=a*n;
    YsVec3 p=particle.getPosition()-d*n;

    if(hit_face(p,particle,triangle)||hit_edge(p,particle,triangle)){
        YsVec3 u=v-v*n*n*2;
        //particle.setPosition(p);
	    particle.setVelocity(u);
        return true;
    }
    return false;
}

bool Ship::hit_face(YsVec3 &p,Particle &particle,const Triangle &triangle)const
{
    YsVec3 a=triangle.vertices[0];
    YsVec3 b=triangle.vertices[1];
    YsVec3 c=triangle.vertices[2];

    YsVec3 n=(b-a)^(c-a);
    double n2=n*n;
    YsVec3 na=(c-b)^(p-b);
    YsVec3 nb=(a-c)^(p-c);
    YsVec3 nc=(b-a)^(p-a);

    double alpha=n*na/n2;
    if(alpha<0 || alpha>1) return false;

    double beta=n*nb/n2;
    if(beta<0 || beta>1) return false;

    double gama=n*nc/n2;
    if(gama<0 || gama>1) return false;

    double distance=(p-particle.getPosition()).GetLength();

    return distance<1.0;
}

bool Ship::hit_edge(YsVec3 &p,Particle &particle,const Triangle &triangle)const
{
    YsVec3 a=triangle.vertices[0];
    YsVec3 b=triangle.vertices[1];
    YsVec3 c=triangle.vertices[2];

    /* p_ is p projected onto an edge */
    YsVec3 p_=(p-a)*(b-a)*(b-a)/((b-a)*(b-a))+a;
    YsVec3 d=particle.getPosition()-p_;
    double dis=d.GetLength();
    if(dis<1.0)
        return true;

    p_=(p-b)*(c-b)*(c-b)/((c-b)*(c-b))+b;
    d=particle.getPosition()-p_;
    dis=d.GetLength();
    if(dis<1.0)
        return true;

    p_=(p-c)*(a-c)*(a-c)/((a-c)*(a-c))+c;
    d=particle.getPosition()-p_;
    dis=d.GetLength();
    if(dis<1.0)
        return true;

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*virtual*/ void Ship::draw() const
{
    glColor3ub(255,255,255);
    for (int i=0; i<triangles.size(); i++) {
     glBegin(GL_LINE_LOOP);
       glNormal3d(triangles[i].normal.x(), triangles[i].normal.y(), triangles[i].normal.z());
        glVertex3d(triangles[i].vertices[0].x(), triangles[i].vertices[0].y(), triangles[i].vertices[0].z());
        glVertex3d(triangles[i].vertices[1].x(), triangles[i].vertices[1].y(), triangles[i].vertices[1].z());
        glVertex3d(triangles[i].vertices[2].x(), triangles[i].vertices[2].y(), triangles[i].vertices[2].z());
     glEnd();
  }
 }


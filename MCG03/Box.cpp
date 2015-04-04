#include "Box.h"


#ifndef M_PI
#define M_PI YsPi
#endif


Box::Box()
{
	min=-YsXYZ();
	max= YsXYZ();
        ifmove=false;
}
Box::~Box()
{
}

void Box::move()
{
  ifmove=true;
  static int forward=1;
  YsVec3 center=(fixedmin+max)/2;

  if(forward && min.x()<center.x()){
    min.AddX(10*0.01);
  }
  else if(forward && min.x()>=center.x()){
    forward=0;
  }
  else if(!forward && min.x()>fixedmin.x()){
    min.SubX(10*0.01);
  }
  else if(!forward && min.x()<=fixedmin.x()){
    forward=1;
  }
}

/*virtual*/ void Box::collide(Particle &particle) const
{
    YsVec3 r=particle.getPosition();
    YsVec3 v=particle.getVelocity();

    if(ifmove && r.x()<min.x() && v.x()<10)
      {
	r.SetX(min.x()+10*0.01);
        v.SetX(-v.x()+20);
      }
    if(!ifmove && r.x()<min.x() && v.x()<0.0)
      {
	r.SetX(min.x());
        v.MulX(-1.0);
      }
	if(max.x()<r.x() && 0.0<v.x())
	{
		r.SetX(YsBound(r.x(),min.x(),max.x()));
		v.MulX(-1.0);
	}
	if((r.y()<min.y() && v.y()<0.0) || (max.y()<r.y() && 0.0<v.y()))
	{
		r.SetY(YsBound(r.y(),min.y(),max.y()));
		v.MulY(-1.0);
	}
	if((r.z()<min.z() && v.z()<0.0) || (max.z()<r.z() && 0.0<v.z()))
	{
		r.SetZ(YsBound(r.z(),min.z(),max.z()));
		v.MulZ(-1.0);
	}

	particle.setPosition(r);
	particle.setVelocity(v);

}

/*virtual*/ void Box::draw() const
{
  GLdouble vertices[]=
  {
    min.x(),min.y(),min.z(),
    max.x(),min.y(),min.z(),
    max.x(),min.y(),max.z(),
    min.x(),min.y(),max.z(),

    min.x(),max.y(),min.z(),
    max.x(),max.y(),min.z(),
    max.x(),max.y(),max.z(),
    min.x(),max.y(),max.z()
  };

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_DOUBLE,0,vertices);

  static GLubyte allIndices[]=
  {
	0,1, 1,2, 2,3, 3,0,
	4,5, 5,6, 6,7, 7,4,
	0,4,
	1,5,
	2,6,
	3,7
   /* 4,5,6,7,
   1,2,6,5,
   0,1,5,4,
   0,3,2,1,
   0,4,7,3,
   2,3,7,6 */
  };
  glColor3ub(255,255,255);
  glDrawElements(GL_LINES,24,GL_UNSIGNED_BYTE,allIndices);
}

void Box::setMinMax(const YsVec3 &min,const YsVec3 &max)
{
	this->min=min;
	this->max=max;
    this->fixedmin=min;
}
void Box::setMinMax(const YsVec3 minmax[2])
{
	this->min=minmax[0];
	this->max=minmax[1];
}


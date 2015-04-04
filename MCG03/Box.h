#ifndef BOX_H_IS_INCLUDED
#define BOX_H_IS_INCLUDED

#include <stdio.h>


#ifdef _WIN32
#include <windows.h>
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


#ifdef __APPLE__
   #include "../ysclass/src/ysclass.h"
#else
   #include <ysclass.h>
#endif
#include "Obstacle.h"


class Box:public Obstacle
{
private:
  YsVec3 min,max,fixedmin;
  bool ifmove;
    
public:
    Box();
    ~Box();

    virtual void move();
    virtual void draw() const;
    virtual void collide(Particle &particle) const;

	void setMinMax(const YsVec3 &min,const YsVec3 &max);
	void setMinMax(const YsVec3 minmax[2]);
};

#endif


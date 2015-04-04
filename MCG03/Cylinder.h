//
//  Cylinder.h
//  MCG03
//
//  Created by Jing Li on 10/17/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__Cylinder__
#define __MCG03__Cylinder__

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


class Cylinder:public Obstacle
{
private:
    YsVec3 origin;  //the coordinate of the origin at the bottom of the cylinder
    double radius;  //the radius of the cylinder
    double top;  //the top coordinate
    double bottom;  //the bottom coordinate
    
public:
    Cylinder();
    ~Cylinder();

    virtual void draw() const;
    virtual void collide(Particle &particle) const;
    
    double bottom_fun(YsVec3 r)const;
    double side_fun(YsVec3 r)const;

	void setRadius(const double r);
};

#endif /* defined(__MCG03__Cylinder__) */

//
//  Ship.h
//  MCG03
//
//  Created by Jing Li on 12/2/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__Ship__
#define __MCG03__Ship__

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

#include <vector>
#ifdef __APPLE__
#include "../ysclass/src/ysclass.h"
#else
#include <ysclass.h>
#endif
#include "Obstacle.h"
#include "Quaternion.h"

using namespace std;

#define eps 0.001

class Ship:public Obstacle
{
    typedef struct {
        YsVec3 vertices[3];
        YsVec3 normal;
    } Triangle;

private:
    YsShell shl;
    Quaternion orientation;
    YsVec3 velocity;
    //YsVec3 position;
    vector<Triangle> triangles;

public:
    Ship();
    ~Ship();

    void readStl();
    virtual void draw() const;
    virtual void collide(Particle &particle) const;
    bool collideWithOneTriangle( Particle &particle,const Triangle &triangle)const;
    bool hit_face(YsVec3 &p,Particle &particle,const Triangle &triangle)const;
    bool hit_edge(YsVec3 &p,Particle &particle,const Triangle &triangle)const;

    void move();

};

#endif /* defined(__MCG03__Ship__) */

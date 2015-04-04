//
//  Cylinder.cpp
//  MCG03
//
//  Created by Jing Li on 10/17/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#include "Cylinder.h"


#ifndef M_PI
#define M_PI YsPi
#endif


Cylinder::Cylinder()
{
    radius=20.0;
    top=10.0;
    bottom=0.0;
    origin=YsVec3(0.0, 0.0, 0.0);
}

Cylinder::~Cylinder()
{

}

void Cylinder::draw()const
{
}

void Cylinder::collide(Particle &particle)const
{
    YsVec3 r=particle.getPosition();
    YsVec3 v=particle.getVelocity();
    YsVec3 n;
    //particle penetrates neither bottom nor radius
    if (bottom_fun(r)<0 && side_fun(r)<0) {
        return;
    }
    if (bottom_fun(r)>0) {
        n=YsYVec();
        if (n*v<0) {
            v=v-2*(v*n)*n; //bounce velocity
            particle.setVelocity(v);
        }
    }
    if (side_fun(r)>0) {
        n=origin-r;
        n.SetY(0.0);
        n.Normalize();
        if (n*v<0) {
            v=v-2*(v*n)*n; //bounce velocity
            particle.setVelocity(v);
        }
    }
}

double  Cylinder::side_fun(YsVec3 r)const
{
    return pow((r.x()-origin.x()),2.0)+pow((r.z()-origin.z()), 2.0)-pow(radius, 2.0);
}

double Cylinder::bottom_fun(YsVec3 r)const
{
    return bottom-r.y();
}



void Cylinder::setRadius(const double r)
{
	radius=r;
}

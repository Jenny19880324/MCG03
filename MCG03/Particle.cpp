//
//  Particle.cpp
//  MCG03
//
//  Created by Jing Li on 9/18/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#include "Particle.h"


Particle::Particle()
{
	mass=1.0;           //mass of a particle
	density=1.0;        //density of a particle
	pressure=0.0;          //pressure of a particle
    position=YsOrigin();        //position of a particle
    velocity=YsVec3(10.0,-10.0,0.0);        //velocity of a particle
    pressureForce=YsOrigin();   //force of pressure
    viscosityForce=YsOrigin();   //force of viscosity
    surfaceTension=YsOrigin();    //force of surface
    color[0]=1.0;
    color[1]=1.0;
    color[2]=1.0;
    pointSize=1.0;
}

void Particle::draw()
{
    glPointSize(pointSize);
    glColor3dv(color);
    glBegin(GL_POINTS);
    glVertex3f(position.x(), position.y(),position.z());
    glEnd();
}

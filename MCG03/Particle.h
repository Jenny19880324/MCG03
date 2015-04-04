//
//  Particle.h
//  MCG03
//
//  Created by Jing Li on 9/18/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__Particle__
#define __MCG03__Particle__

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
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

#define g (9.8)

class Particle
{
private:
    double mass;    //mass of a particle
    double density;        //density of a particle
    double pressure;          //pressure of a particle
    YsVec3 position;        //position of a particle
    YsVec3 velocity;        //velocity of a particle
    YsVec3 pressureForce;   //force of pressure
    YsVec3 viscosityForce;   //force of viscosity
    YsVec3 surfaceTension;    //force of surface
    YsVec3 gravity;           //force of gravity
    YsVec3 normal;
    double colorField;
    double curvature;

    GLdouble color[3];          //color for test
    GLfloat pointSize;

    int numNeighbors;
    double weightSum;               //sigma wij
    YsVec3 weightedPosition;      //used for surface reconstruction
    YsVec3 updatedKernelCenter;
    YsMatrix3x3 covarianceMatrix;
    YsMatrix3x3 G;                  //used for surface reconstruction
    YsMatrix3x3 R;                     //used for surface reconstruction
    double det;                     // determinant of G


public:
    Particle();
    const double getMass()const {return mass;}
    const double getDensity()const {return density;}
    const double getPressure()const {return pressure;}
    const YsVec3 getPosition()const {return position;}
    const YsVec3 getVelocity()const {return velocity;}
    const YsVec3 getNormal()const {return normal;}
    const double getColorField()const {return colorField;}
    const double getCurvature()const {return curvature;}
    const YsVec3 getPressureForce()const {return pressureForce;}
    const YsVec3 getViscosityForce()const {return viscosityForce;}
    const YsVec3 getSurfaceTension()const {return surfaceTension;}
    const YsVec3 getGravity()const {return gravity;}
    const GLdouble *getColor()const {return color;};
    const GLfloat getPointSize()const {return pointSize;}
    const YsVec3 getWeightedPosition()const {return weightedPosition;}
    const YsMatrix3x3 getG()const {return G;}
    const double getDet()const {return det;}
    const double getWeightSum()const {return weightSum;}
    const YsMatrix3x3 getCovarianceMatrix()const {return covarianceMatrix;}
    const int getNumNeighbors()const {return numNeighbors;}
    const YsVec3 getUpdatedKernelCenter()const {return updatedKernelCenter;}

    void setColor(const GLdouble c[3]){color[0]=c[0],color[1]=c[1],color[2]=c[2];}
    void setMass(const double m) {mass=m;}
    void setDensity(const double d) {density=d;}
    void setPressure(const double p) {pressure=p;}
    void setPosition(const YsVec3 p) {position=p;}
    void setVelocity(const YsVec3 v) {velocity=v;}
    void setNormal(const YsVec3 n){normal=n;}
    void setColorField(const double c) {colorField=c;}
    void setCurvature(const double c) {curvature=c;}
    void setPressureForce(const YsVec3 p) {pressureForce=p;}
    void setViscosityForce(const YsVec3 v) {viscosityForce=v;}
    void setSurfaceTension(const YsVec3 surfaceTension)
    {this->surfaceTension=surfaceTension;}
    void setGravity() {gravity=mass*g*YsVec3(0.0,-1.0,0.0);}
    void setPointSize(const double p){pointSize=p;}
    void setWeightedPosition(const YsVec3 weightedPosition){this->weightedPosition=weightedPosition;}
    void setG(const YsMatrix3x3 G){this->G=G;}
    void setDet(const double det){this->det=det;}
    void setWeightSum(const double weightSum){this->weightSum=weightSum;}
    void setCovarianceMatrix(const YsMatrix3x3 covarianceMatrix){this->covarianceMatrix=covarianceMatrix;}
    void setNumNeighbors(const int numNeighbors)
    {this->numNeighbors=numNeighbors;}
    void setUpdatedKernelCenter(const YsVec3 updatedKernelCenter)
    {this->updatedKernelCenter=updatedKernelCenter;}
    void draw();
};

#endif /* defined(__MCG03__Particle__) */

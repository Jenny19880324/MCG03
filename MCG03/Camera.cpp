//
//  Camera.cpp
//  MCG03
//
//  Created by Jing Li on 9/25/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#include <GLUT/glut.h>
#include "Camera.h"

extern GLdouble width, height;

Camera::Camera()
{
    Initialize();
}

void Camera::Initialize(void)
{
    x=0;
    y=0;
    z=0;
    h=0;
    p=0;
    b=0;
    
    fov=M_PI/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}

void Camera::transformation(void)
{
    glRotated(-b*180.0/M_PI,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/M_PI,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-h*180.0/M_PI,0.0,1.0,0.0);  // Rotation about Y axis
    glTranslated(-x,-y,-z);
}

void Camera::GetForwardVector(double &vx,double &vy,double &vz)
{
    vx=-cos(p)*sin(h);
    vy= sin(p);
    vz=-cos(p)*cos(h);
}

void Camera::GetUpVector(double &vx,double &vy,double &vz)
{
    const double ux0=0.0;
    const double uy0=1.0;
    const double uz0=0.0;
    
    const double ux1=cos(b)*ux0-sin(b)*uy0;
    const double uy1=sin(b)*ux0+cos(b)*uy0;
    const double uz1=uz0;
    
    const double ux2=ux1;
    const double uy2=cos(p)*uy1-sin(p)*uz1;
    const double uz2=sin(p)*uy1+cos(p)*uz1;
    
    vx=cos(-h)*ux2-sin(-h)*uz2;
    vy=uy2;
    vz=sin(-h)*ux2+cos(-h)*uz2;
}

void Camera::GetRightVector(double &vx,double &vy,double &vz)
{
    const double ux0=1.0;
    const double uy0=0.0;
    const double uz0=0.0;
    
    const double ux1=cos(b)*ux0-sin(b)*uy0;
    const double uy1=sin(b)*ux0+cos(b)*uy0;
    const double uz1=uz0;
    
    const double ux2=ux1;
    const double uy2=cos(p)*uy1-sin(p)*uz1;
    const double uz2=sin(p)*uy1+cos(p)*uz1;
    
    vx=cos(-h)*ux2-sin(-h)*uz2;
    vy=uy2;
    vz=sin(-h)*ux2+cos(-h)*uz2;
}
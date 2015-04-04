//
//  Camera.h
//  MCG03
//
//  Created by Jing Li on 9/25/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__Camera__
#define __MCG03__Camera__

#include <stdio.h>
#include <math.h>

class Camera
{
public:
    double x,y,z;
    double h,p,b;
    
    double fov,nearZ,farZ;
    
    Camera();
    void Initialize(void);
    void transformation(void);
    
    void GetForwardVector(double &vx,double &vy,double &vz);
    void GetUpVector(double &vx,double &vy,double &vz);
    void GetRightVector(double &vx,double &vy,double &vz);
};

#endif /* defined(__MCG03__Camera__) */

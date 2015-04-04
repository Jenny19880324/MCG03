//
//  Obstacle.h
//  MCG03
//
//  Created by Jing Li on 9/18/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__Obstacle__
#define __MCG03__Obstacle__

#include <iostream>
#include "Particle.h"

class Obstacle
{
public:
    virtual void draw()const =0;
    virtual void collide(Particle &particle) const =0;
    virtual void move(){};
};



#endif /* defined(__MCG03__Obstacle__) */

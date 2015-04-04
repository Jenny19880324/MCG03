//
//  hose.h
//  MCG03
//
//  Created by Jing Li on 9/18/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__Hose__
#define __MCG03__Hose__

#include "Quaternion.h"
class ParticleSimulation;
class Hose
{
 private:
  YsVec3 position;   //world position of origin
  Quaternion orientation;
  YsVec3 velocity; //world velocity
  size_t nx;  // number of particles in local x direction
  size_t nz;   // number of particles in local y direction
  size_t ny; //number of particles has been injected in local z direction

  double stepx;
  double stepy;
  double stepz;


 public:
    double dt;//time interval
    double timer;
    double start;  //time to start injecting
    double stop;   //time to stop injecting

  Hose();
  void setPosition(const YsVec3 &position){this->position=position;}
  void setOrientation(YsVec3 &axis,double radians);
  void setVelocity(const YsVec3 &velocity){this->velocity=velocity;}
  void setNx(const size_t nx){this->nx=nx;}
  void setNy(const size_t ny){this->ny=ny;}
  void setInterval(const double interval);


  const YsVec3& getPosition()const {return position;}
  const Quaternion& getOrientation()const {return orientation;}
  const YsVec3& getVelocity()const {return velocity;}
  const size_t getNx()const {return nx;}
  const size_t getNy()const {return ny;}

  void inject(ParticleSimulation *sim);
};

#endif /* defined(__MCG03__Hose__) */

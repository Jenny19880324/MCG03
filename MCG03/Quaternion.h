#ifndef __MCG03__Quaternion__
#define __MCG03__Quaternion__

#include <math.h>

#ifdef __APPLE__
#include "../ysclass/src/ysclass.h"
#else
#include <ysclass.h>
#endif

class Quaternion
{
public:
    double w,x,y,z;
    /**
     * Default constructor. Leaves values uninitialized.
     */
    Quaternion() {}

    /**
     * Construct a quaternion with the given values.
     */
    Quaternion( double w,double x, double y, double z )
    : w( w ), x( x ), y( y ), z( z ) { }

  /*
   * Constructs a quaternion representing a rotation about the given axis
   * by the given angle
   */
  Quaternion(YsVec3& axis,double radians);
  Quaternion operator*(const Quaternion& rhs)const;
    
  /*
   * Rotate a vector by this quaternion
   */
  YsVec3 operator*(const YsVec3& rhs)const;
};

inline double norm(const Quaternion& q){
  return q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w;
}

Quaternion normalize(const Quaternion&q);


#endif /* defined(__MCG03__Quaternon__) */

#include "Quaternion.h"

static void make_unit(Quaternion& q)
{
  double maginv=1.0/sqrt(norm(q));
  q.x*=maginv;
  q.y*=maginv;
  q.z*=maginv;
  q.w*=maginv;
}

Quaternion::Quaternion(YsVec3& axis,double radians)
{
  radians*=0.5;
  axis.Normalize();
  double sine=sin(radians);
 
  w=cos(radians);
  x=sine*axis.x();
  y=sine*axis.y();
  z=sine*axis.z();
  
  make_unit(*this);
}

Quaternion Quaternion::operator*( const Quaternion& rhs ) const
{
    return Quaternion(
       w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
       w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
       w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
       w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x
    );
}

YsVec3 Quaternion::operator*( const YsVec3& v ) const
{
    // nVidia SDK implementation
    YsVec3 qvec( x, y, z );
    YsVec3 uv = qvec^v;
    YsVec3 uuv = qvec^uv;
    uv *= ( 2.0 * w );
    uuv *= 2.0;

    return v + uv + uuv;
}

Quaternion normalize( const Quaternion& q )
{
    Quaternion rv( q );
    make_unit( rv );
    return rv;
}

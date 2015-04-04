#include "Hose.h"
#include "ParticleSimulation.h"

Hose::Hose()
{
  position=YsVec3(0.0,25.0,0.0);
  YsVec3 axis=YsZVec();
  double radians=YsPi/4.0;
  orientation=Quaternion(axis,radians);
  velocity=YsVec3(0.0,-20.0,0.0);
  velocity=orientation*velocity;

  nx=5;
  nz=5;
  ny=0;

  setInterval(0.9);
  timer=0.0;
}

void Hose::setOrientation(YsVec3 &axis,double radians)
{
    orientation=Quaternion(axis,radians);
}

void Hose:: setInterval(const double interval)
{
    const double sqrt3=1.7320508;
    const double sin72=0.9519565;

    stepx=interval;
    stepz=interval*sqrt3/2.0;
}

void Hose::inject(ParticleSimulation *sim)
{
printf("%s\n",__func__);
  ny++;
  const double dx=stepx*(double)(nx-1);
  const double dz=stepz*(double)(nz-1);

  const YsVec3 o(-dx/2.0,0.0,-dz/2.0);
  const double xShift=(double)(ny%2)*stepx/2.0;
  const double zShift=(double)(ny%2)*stepx/3.0;


  for(size_t zz=0;zz<nz;++zz)
  {
    for(size_t xx=0;xx<nx;++xx)
    {
      const double xOffset=stepx*(double)xx+xShift;
      const double zOffset=stepz*(double)zz+zShift;

      YsVec3 localIniPos=o+YsVec3(xOffset,0.0,zOffset);
      YsVec3 worldIniPos=orientation*localIniPos+position;

      Particle particle;
      particle.setPosition(worldIniPos);
      particle.setVelocity(velocity);

      sim->particles.push_back(particle);
    }
  }
  sim->numParticles=sim->particles.size();
}

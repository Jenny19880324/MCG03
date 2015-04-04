#include <vector>
#include <ysclass.h>

void CreateUniformInitialParticleLocation(std::vector <YsVec3> &iniPos,int nx,int ny,int nz,const YsVec3 center,const double interval)
{
	const double sqrt3=1.7320508;
	const double sin72=0.9510565;

	const double stepx=interval;
	const double stepy=interval*sin72*sqrt3/2.0;
	const double stepz=interval*sqrt3/2.0;

	const double dx=stepx*(double)(nx-1);
	const double dy=stepy*(double)(ny-1);
	const double dz=stepz*(double)(nz-1);

	const YsVec3 o(-dx/2.0,-dy/2.0,-dz/2.0);

	for(int yy=0; yy<ny; ++yy)
	{
		const double xShift=(double)(yy%2)*interval/2.0;
		const double zShift=(double)(yy%2)*interval/3.0;

		for(int zz=0; zz<nz; ++zz)
		{
			for(int xx=0; xx<nx; ++xx)
			{
				const double xOffset=stepx*(double)xx+xShift;
				const double yOffset=stepy*(double)yy;
				const double zOffset=stepz*(double)zz+zShift;

				iniPos.push_back(o+YsVec3(xOffset,yOffset,zOffset));
			}
		}
	}
}

int main(void)
{
	std::vector <YsVec3> iniPos;
	CreateUniformInitialParticleLocation(iniPos,5,5,5,YsVec3(0.0,10.0,0.0),0.9);

	for(auto pos : iniPos)
	{
		printf("V %lf %lf %lf\n",pos.x(),pos.y(),pos.z());
	}
	return 0;
}

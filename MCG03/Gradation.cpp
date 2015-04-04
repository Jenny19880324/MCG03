#include "Gradation.h"
void GetColorGradation(double col[3],double t)
{
	// 0      0.333   0.666     1.0
	// Blue   Green   Yellow    Red

	if(0.0>t)
	{
		col[0]=0.0;
		col[1]=0.0;
		col[2]=1.0;
	}
	else if(t<1.0/3.0)
	{
		const double T=t/(1.0/3.0);
		col[0]=0.0;
		col[1]=T;
		col[2]=1.0-T;
	}
	else if(t<2.0/3.0)
	{
		const double T=(t-(1.0/3.0))/(1.0/3.0);
		col[0]=T;
		col[1]=1.0;
		col[2]=0.0;
	}
	else if(t<1.0)
	{
		const double T=(t-(2.0/3.0))/(1.0/3.0);
		col[0]=1.0;
		col[1]=1.0-T;
		col[2]=0.0;
	}
	else
	{
		col[0]=1.0;
		col[1]=0.0;
		col[2]=0.0;
	}
}

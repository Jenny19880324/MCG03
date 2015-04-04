#ifndef YSINTERPOLATION_IS_INCLUDED
#define YSINTERPOLATION_IS_INCLUDED
/* { */

/*! Returns a function value of y(t)=-2t^3+3t^2, which satisfies y(0)=0, y(1)=1, y'(0)=0, y'(1)=0. */
inline const double YsG1Continuous(const double t)
{
	// See also smooth_interpolation.docx
	const double tt=t*t;
	return -2.0*tt*t+3.0*tt;
}

/*! Returns a functin value of y(t)=6*t^5-15*t^4+10*t^3, which satisfies y(0)=0, y(1)=1, y'(0)=y''(0)=y'(1)==y''(1)=0. */
inline const double YsG2Continuous(const double t)
{
	// See also smooth_interpolation.docx
	const double tt=t*t;
	const double ttt=tt*t;
	return 6.0*ttt*tt-15.0*t*ttt+10.0*ttt;
}

/* } */
#endif

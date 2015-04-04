//
//  SmoothingKernel.h
//  MCG03
//
//  Created by Jing Li on 11/6/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__SmoothingKernel__
#define __MCG03__SmoothingKernel__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#ifdef __APPLE__
#include "../ysclass/src/ysclass.h"
#else
#include <ysclass.h>
#endif

#ifndef M_PI
#define M_PI 3.1415927
#endif

static const double threshold = 1.0e-2;
static const double sigmaWpoly6=315.0/(64.0*M_PI);
static const double inline coeffWpoly6(double h){ return 315.0/(64.0*M_PI*pow(h,9.0));}
static const double inline coeffWspiky(double h) { return 15.0/(M_PI*pow(h, 6.0));}
static const double inline coeffWviscosity(double h) { return 15/(2*M_PI*pow(h, 3.0));}

/*smoothing kernels*/
/*smoothing kernel used except pressure or viscosity computation*/
double Wpoly6(YsVec3 ri,YsVec3 rj,double h);
/*anisotropic kernel used in surface reconstruction */
double aWpoly6(YsVec3 ri,YsVec3 rj,YsMatrix3x3 G);
/*gradiant of Wpoly6 used for normal computation from color field*/
YsVec3 gradientWpoly6(YsVec3 ri,YsVec3 rj,double h);
/*laplacian of Wpoly6 used for curvation computation from color field*/
double laplacianWpoly6(YsVec3 ri,YsVec3 rj,double h);

/*smoothing kernel used for pressure computation*/
double Wspiky(YsVec3 ri,YsVec3 rj,double h);
/*gradiant of Wspiky used for pressure computation*/
YsVec3 gradientWspiky(YsVec3 ri,YsVec3 rj,double h);

/*smoothing kernel used for viscosity computation*/
double Wviscosity(YsVec3 ri,YsVec3 rj,double h);
/*laplacian of Wviscosity used for viscosity computation*/
double laplacianWviscosity(YsVec3 ri,YsVec3 rj,double h);

#endif /* defined(__MCG03__SmoothingKernel__) */

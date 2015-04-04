//
//  SmoothingKernel.cpp
//  MCG03
//
//  Created by Jing Li on 11/6/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#include "SmoothingKernel.h"

double Wpoly6(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri - rj;
    double r = rij.GetLength();

    double w = 0.0;
    if (r >= 0 && r <= h) {
        w = pow((pow(h,2.0)-pow(r, 2.0)),3.0);
    }
    return coeffWpoly6(h)*w;
}

double aWpoly6(YsVec3 ri,YsVec3 rj,YsMatrix3x3 G)
{
    auto Gr = G*(ri - rj);
    YsVec3 r=ri-rj;
    double det;
    G.GetDeterminant(det);
    double r_length=r.GetLength();
    double Gr_length = Gr.GetLength();
    double w=0.0;
    if(Gr_length>=0 && Gr_length<=1.0){
      w=pow((1-Gr_length*Gr_length),3.0);
    }
    return sigmaWpoly6*det*w;
}

YsVec3 gradientWpoly6(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri - rj;
    double r = rij.GetLength();
    double w = 0.0;
    if (r >=0 && r<= h) {
        w = -6*pow(r, 5.0)+12*pow(h, 2.0)*pow(r, 3.0)-6*pow(h, 4.0)*r;
    }
    return coeffWpoly6(h)*w*rij/r;
}

double laplacianWpoly6(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri -rj;
    double r=rij.GetLength();
    double w=0.0;
    if(r >=0 && r<=h ){
        w = -30*pow(r, 4.0)+36*pow(h, 2)*pow(r, 2)-6*pow(h, 4.0);
    }
    return coeffWpoly6(h)*w;
}

double Wspiky(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri - rj;
    double r = rij.GetLength();

    double w = 0.0;
    if(r >= 0 && r <= h) {
        w   = pow((h-r), 3.0);
    }
    return coeffWspiky(h)*w;
}

YsVec3 gradientWspiky(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri - rj;
    double  r = rij.GetLength();
    double  w = 0.0;

    if (r >=0 && r <=h ) {
        w = -3*pow(r,2)+6*h*r-3*pow(h, 2);
    }

    if (threshold<r) {
        return coeffWspiky(h)*w*rij/r;
    }
    else{
        return YsOrigin();
    }
}

double Wviscosity(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri - rj;
    double r  = rij.GetLength();

    double w = 0.0;
    if (r >=0 && r <= h) {
        double term0 = -pow(r, 3.0) / (2.0*pow(h, 3.0));
        double term1 = pow(r, 2.0) / pow(h, 2.0);
        double term2 = h / (2.0*r);
        w = term0+term1+term2-1;
    }
    return coeffWviscosity(h)*w;
}

double laplacianWviscosity(YsVec3 ri,YsVec3 rj,double h)
{
    auto rij = ri - rj;
    double  r = rij.GetLength();

    double w = 0.0;
    if (r >=0 && r<= h) {
        if(r<0.1*h) r=0.1*h;
        double term0 = -3*r/pow(h, 3.0);
        double term1 = 2/pow(h, 2.0);
        double term2 = h/pow(r, 3.0);
        w = term0 + term1 + term2;
    }
    return coeffWviscosity(h)*w;
}

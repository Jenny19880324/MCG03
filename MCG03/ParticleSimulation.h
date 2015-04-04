//
//  Sep10.h
//  MCG03
//
//  Created by Jing Li on 9/11/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#ifndef __MCG03__ParticleSimulation__
#define __MCG03__ParticleSimulation__

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <cassert>
#ifdef __APPLE__
	#include "../ysclass/src/ysclass.h"
#else
	#include <ysclass.h>
#endif
#include "Particle.h"
#include "Cylinder.h"
#include "Box.h"
#include "SmoothingKernel.h"
#include "Hose.h"
#include "Ship.h"

#define hWpoly6  1.0 //(sqrt(315.0/64.0/M_PI*(0.6-(1.0/7))))
#define hWspiky  1.0 //(sqrt(15.0/4/M_PI))
#define hWviscosity 1.0 //(1.0e-5)
#define hWeightedPosition 1.0

#define rambda 0.95
#define kr 4
#define ks 1400
#define kn 0.5
#define N 25


using namespace std;

typedef vector< Particle * > Block;
typedef vector< Block > Lattice;

typedef struct {
    YsVec3 p[3];
} Triangle;

typedef struct {
    YsVec3 p[8];
    double val[8];
} GridCell;


/*function used in surface reconstruction */
YsMatrix3x3 GramSchmidtOrthonormalize(YsMatrix3x3 eigenVector);
void sortSingularValues(YsMatrix3x3 &eigenValue,YsMatrix3x3 &eigenVector);
void swap(YsMatrix3x3 &eigenValue,YsMatrix3x3 &eigenVector,int i,int j);
double w(YsVec3 ri,YsVec3 rj,double h);
YsMatrix3x3 convert(const YsMatrix &mat);
YsMatrix convert(const YsMatrix3x3 &mat3x3);

void filename(char *filename,size_t len);


class ParticleSimulation
{
    typedef YsVec3 (ParticleSimulation::*YsVec3PP)(const Particle *a, const Particle *b)const;
    typedef double (ParticleSimulation::*doublePP)(const Particle *a, const Particle *b)const;
    typedef int (ParticleSimulation::*intPP)(const Particle *a, const Particle *b)const;
    typedef YsMatrix3x3 (ParticleSimulation::*YsMatrix3x3PP)(const Particle *a, const Particle *b)const;


    typedef double (ParticleSimulation::*doubleVP)(const YsVec3 &pa,const Particle *b) const;
    typedef YsVec3 (ParticleSimulation::*YsVec3VP)(const YsVec3 &pa, const Particle *b)const;

private:
    /*member variables*/
    /*static member variables*/
    static const double iniDensity;
    static const double dt;            //time interval
    static const double hPressure; // h of pressure
    static const double hPressureForce; // h of pressure force
    static const double hViscosity; // h of viscosity

    /* k is used for computing pressure from density
        * k is a gas constant that depends on the temperature
        * the value of 1000.0 is recommended by author of MCG03
        */
    double k;
    /*viscosity of the fluid
        * high viscosity recommended by author of MCG03
        */
    double mu;
    double sigma;

    /*volatile member variables*/
    double t;
    double mass;   //initial mass of every particle
    size_t numParticles;    //number of particles

    std::vector <Particle> particles;

	bool isotropic;


    /*member functions*/
    /*member function to find surface tension*/
    YsVec3 normal(const Particle *a,const Particle *b)const;
    double curvature(const Particle *a,const Particle *b)const;
    double colorField(const Particle *a,const Particle *b)const;
    double anisotropicColorField(const YsVec3 &pa,const Particle *b)const;
    double density(const Particle *a,const Particle *b) const;
    YsVec3 pressureForce(const Particle *a,const Particle *b) const;
    YsVec3 viscosityForce(const Particle *a,const Particle *b)const;
    YsVec3 positionSum(const Particle *i,const Particle *j)const;
    double weightSum(const Particle *i,const Particle *j)const;
    int numNeighbors(const Particle *i,const Particle *j)const;
    YsMatrix3x3 covarianceMatrixSum(const Particle *i,const Particle *j)const;


    void calculateColorField(int i0,int i1);
    void calculateDensity(int i0,int i1);

	class ThreadInfo
	{
	public:
		int i0,i1;
		ParticleSimulation *sim;
	};
	static void calculateDensityThread(ThreadInfo *info);
    static void calculateForceThread(ThreadInfo *info);
    static void calculateGThread(ThreadInfo *info);

	class MeshThreadInfo : public ThreadInfo
	{
	public:
	    vector<Triangle> triangles;
	};
    static void meshThread(class MeshThreadInfo *info);

    void calculatePressure(int i0,int i1);
	void calculateNormalAndCurvature(int i0,int i1);
    void calculateForce(int i0,int i1);
    void calculateNumNeighbors(int i0,int i1);
    void calculateWeightSum(int i0,int i1);
    void calculateWeightedPosition(int i0,int i1);
    void calculateCovarianceMatrix(int i0,int i1);
    void calculateG(int i0,int i1);
    void calculateUpdatedKernelCenter(int i0,int i1);

    void collision();

    void createLattice(double maxH);
    YsVec3 particleLatticeLoop(const Particle *a,const YsVec3PP fun)const;
    double particleLatticeLoop(const Particle *a,const doublePP fun)const;
    int particleLatticeLoop(const Particle *a,const intPP fun)const;
    YsMatrix3x3 particleLatticeLoop(const Particle *a,const YsMatrix3x3PP fun)const;


    static const double hDensity; // h of density
    static const double hColorField;   // h of color field

    Lattice lattice;
    double hx,hy,hz; //size of each block in x,y,z direction
    int numBlocksX; //number of blocks in x direction
    int numBlocksY; //number of blocks in y direction
    int numBlocksZ; //number of blocks in z direction
    int numBlocks; // total number of blocks
    YsVec3 bbx[2];        //position of the bounding box corners

    double cx,cy,cz; //size of each cube in x,y,z direction
    double iso;
    static const int edgeTable[256];
    static const int triTable[256][16];

    int numCubesX; //number of cubes in x direction
    int numCubesY; //number of cubes in y direction
    int numCubesZ; //number of cubes in z direction

    vector<Triangle> triangles;

    double density(const YsVec3 &pa,const Particle *b) const;
    double colorField(const YsVec3 &pa,const Particle *b)const;
    YsVec3 normal(const YsVec3 &pa,const Particle *b)const;
    double vectorLatticeLoop(const YsVec3 &pos,doubleVP fun) const;


public:
    int particleOfInterest;
    Obstacle *obstacle;
    Hose hose;

    /* public member variables */

    bool hoseOn;
    bool wallMove;
    bool waterFall;

    enum MODE
    {
        MODE_NORMAL,
        MODE_DENSITY_TEST,
        MODE_PRESSURE_FORCE_TEST,
        MODE_VISCOSITY_FORCE_TEST,
        MODE_SURFACE_TENSION_TEST,
        MODE_RESET,
    };
    MODE mode;
    bool stepByStep;
    bool colorHighDensity;
    bool output;
    bool drawParticle;

    YsShell shl;


    /*constructor and destructor*/
    ParticleSimulation();
    ParticleSimulation(Obstacle &ob,std::vector<YsVec3> iniPos,double interval);
    ~ParticleSimulation();

    /*initialize velocity, location of particles*/
    void init(std::vector<YsVec3> &iniPos);
    void setObstacle(Obstacle &obstacle);
    void initParticles(std::vector <YsVec3> &iniPos);
    void placeParticles(std::vector <YsVec3> &iniPos);
    void placeParticles(std::vector <YsVec3> &iniPos,const YsVec3 &velosity);
    size_t getNumParticles()const {return numParticles;}
    Particle getParticle(size_t i)const {return particles[i];}

    void destroy();
    void update();
    void drawParticles();
    void drawObstacle();
    void drawParticleOfInterest(void);
    void drawMesh();
    void drawColorFieldGrid();

    void read(const char *infile);
    void write(const char *outfile);
    void setK(double k){this->k=k;}
    void setMu(double mu){this->mu=mu;}
    void setSigma(double sigma){this->sigma=sigma;}
    void setIso(double iso){this->iso=iso;}

	bool getIsotropicMode(void) const;
	void setIsotropicMode(bool iso);

	double getMarchingCubesSize(void) const;
	void setMarchingCubesSize(double c);

    /* surface reconstruction */
    /*we extract an iso- surface from a redefined scalar field*/

    double evaluateColorFieldAtPoint(const YsVec3 &pos)const;
    void triangulate(vector<Triangle> &triangles,GridCell grid,double isolevel);
    YsVec3 vertexInterp(double isolevel,YsVec3 p1,YsVec3 p2,double valp1,double valp2);

    void mesh(vector<Triangle> &triangles,int i0,int i1);





    friend void Hose::inject(ParticleSimulation *sim);
};

#endif /* defined(__MCG03__Sep10__) */

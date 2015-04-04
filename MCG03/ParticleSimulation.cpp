//
//  ParticleSimulation.cpp
//  MCG03
//
//  Created by Jing Li on 9/11/14.
//  Copyright (c) 2014 Jing Li. All rights reserved.
//

#include "ParticleSimulation.h"
#include "Gradation.h"
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <thread>

#ifdef _WIN32
#ifndef M_PI
const double M_PI=3.1415927;
#endif
#endif


void createFilename(char *outfile,size_t len)
{
    static const char *PREFFIX="state";
    static const size_t PRELEN=strlen(PREFFIX);
    static const char *SUFFIX=".txt";
    static const size_t SUFLEN=strlen(SUFFIX);

    // generate a name based on the time stamp
    time_t timer;
    time( &timer);

    assert(PRELEN+SUFLEN+12<len);
    strcpy(outfile,PREFFIX);
    strftime(outfile+PRELEN,len-PRELEN-1,"%y%m%d%H%M%S",localtime(&timer));
    if(strlen(outfile)+SUFLEN<len)
        strcat(outfile,SUFFIX);
}

/* function used in surface reconstruction */

YsMatrix3x3 GramSchmidtOrthonormalize(YsMatrix3x3 eigenVector)
{
    YsVec3 v1=YsVec3(eigenVector.v11(),eigenVector.v21(),eigenVector.v31());
    YsVec3 v2=YsVec3(eigenVector.v12(),eigenVector.v22(),eigenVector.v32());
    YsVec3 v3=YsVec3(eigenVector.v13(),eigenVector.v23(),eigenVector.v33());

    v1.Normalize();

    v2=v2-v1*v2*v1;
    v2.Normalize();

    v3=v3-v1*v3*v1-v2*v3*v2;
    v3.Normalize();

    YsMatrix3x3 mat;
    mat.Set11(v1.x());
    mat.Set21(v1.y());
    mat.Set31(v1.z());

    mat.Set12(v2.x());
    mat.Set22(v2.y());
    mat.Set32(v2.z());

    mat.Set13(v3.x());
    mat.Set23(v3.y());
    mat.Set33(v3.z());

    return mat;
}

void sortSingularValues(YsMatrix3x3 &eigenValue,YsMatrix3x3 &eigenVector)
{
    if(eigenValue.v11()<eigenValue.v22()){
        swap(eigenValue,eigenVector,1,2);
    }
    if(eigenValue.v11()<eigenValue.v33()){
        swap(eigenValue,eigenVector,1,3);
    }
    if(eigenValue.v22()<eigenValue.v33()){
        swap(eigenValue,eigenVector,2,3);
    }
}

void swap(YsMatrix3x3 &eigenValue,YsMatrix3x3 &eigenVector,int i,int j)
{
    double temp=eigenValue.v(i,i);
    eigenValue.Set(i,i,eigenValue.v(j,j));
    eigenValue.Set(j,j,temp);

    temp=eigenVector.v(1, i);
    eigenVector.Set(1, i, eigenVector.v(1, j));
    eigenVector.Set(1, j, temp);

    temp=eigenVector.v(2, i);
    eigenVector.Set(2, i, eigenVector.v(2, j));
    eigenVector.Set(2, j, temp);

    temp=eigenVector.v(3, i);
    eigenVector.Set(3, i, eigenVector.v(3, j));
    eigenVector.Set(3, j, temp);
}


double w(YsVec3 xi,YsVec3 xj,double ri)
{
    auto xij=xi-xj;
    double x=xij.GetLength();
    double w=0.0;
    if (x<ri) {
        w=1-pow((x/ri),3.0);
    }
    return w;
}

/* class ParticleSimulation */
const double ParticleSimulation::dt(0.01);
const double ParticleSimulation::iniDensity = 1.0;
const double ParticleSimulation::hDensity =hWpoly6;
const double ParticleSimulation::hPressure = hWpoly6;
const double ParticleSimulation::hPressureForce=hWspiky;
const double ParticleSimulation::hViscosity = hWviscosity;
const double ParticleSimulation::hColorField   = hWpoly6;

ParticleSimulation::ParticleSimulation(){
    stepByStep=true;
    colorHighDensity=false;
    particleOfInterest=0;
    t=0.0;
    numParticles=0;
    drawParticle=true;
    k=500;
    mu=0.2*10;  //viscosity
    sigma=0.078*10;  //surface tension
    //iso=0.04;//moveWall
    //iso=0.15;//hoseOn
    iso=0.02;
    hoseOn=false;
    wallMove=false;
    waterFall=false;
	isotropic=true;

	cx=0.2;
	cy=0.2;
	cz=0.2;
}

ParticleSimulation::~ParticleSimulation()
{
    destroy();
}

void ParticleSimulation::write(const char *outfile)
{
    FILE *out;
    if(outfile==NULL){
        char defaultfile[256];
        createFilename(defaultfile,256);
        out=fopen(defaultfile,"w");
    }
    else{
        out=fopen(outfile,"w");
    }

    if(out==NULL){
        printf("Cannot open output file.\n");
        return;
    }

    for(int i=0;i<numParticles;i++)
    {
        fprintf(out,"mass %lf\n",particles[i].getMass());
        fprintf(out,"density %lf\n",particles[i].getDensity());
        fprintf(out,"pressure %lf\n",particles[i].getPressure());
        fprintf(out,"position %s\n",particles[i].getPosition().Txt());
        fprintf(out,"velocity %s\n",particles[i].getVelocity().Txt());
    }
    fclose(out);
}

void ParticleSimulation::read(const char *infile)
{
    assert(strlen(infile)<256);
    FILE *in=fopen(infile,"r");
    if(in==NULL){
        printf("Cannot open input file.\n");
        return;
    }
    YsString str;
    while(NULL!=str.Fgets(in))
    {
        YsArray <YsString> args;
        str.Arguments(args);

        if(args.GetN())
        {
            Particle particle;

            if(0==strcmp("mass",args[0]) && 2<=args.GetN())
            {
                particle.setMass(atof(args[1]));
                particles.push_back(particle);
            }
            else if(0==strcmp("density",args[0]) && 2<=args.GetN())
            {
                particles.back().setDensity(atof(args[1]));
            }
            else if(0==strcmp("pressure",args[0]) && 2<=args.GetN())
            {
                particles.back().setPressure(atof(args[1]));
            }
            else if(0==strcmp("position",args[0]) && 4<=args.GetN())
            {
                particles.back().setPosition(YsVec3(atof(args[1]),atof(args[2]),atof(args[3])));
            }
            else if(0==strcmp("velocity",args[0]) && 4<=args.GetN())
            {
                particles.back().setVelocity(YsVec3(atof(args[1]),atof(args[2]),atof(args[3])));
            }
        }
    }
    numParticles=particles.size();
    createLattice(1.0);
    calculateDensity(0,numParticles);
    calculatePressure(0,numParticles);
    calculateColorField(0,numParticles);

printf("particles.size()=%d\n",particles.size());
    fclose(in);
}

bool ParticleSimulation::getIsotropicMode(void) const
{
	return isotropic;
}
void ParticleSimulation::setIsotropicMode(bool iso)
{
	isotropic=iso;
}

double ParticleSimulation::getMarchingCubesSize(void) const
{
	return cx;
}
void ParticleSimulation::setMarchingCubesSize(double c)
{
	cx=c;
	cy=c;
	cz=c;
}

void ParticleSimulation::init(std::vector<YsVec3> &iniPos)
{
}

void ParticleSimulation::setObstacle(Obstacle &obstacle)
{
    this->obstacle=&obstacle;
}

void ParticleSimulation::initParticles(std::vector <YsVec3> &iniPos)
{
    numParticles=iniPos.size();
    particles.resize(numParticles);
printf("numParticles=%d\n",numParticles);
    for (size_t i=0; i<numParticles; i++) {
        particles[i].setPosition(iniPos[i]);
        particles[i].setVelocity(YsOrigin());
    }

    //create an empty lattice
    createLattice(1.0);
    calculateDensity(0,numParticles);
    calculatePressure(0,numParticles);
    calculateColorField(0,numParticles);
}


void ParticleSimulation::placeParticles(std::vector <YsVec3> &iniPos)
{
    size_t preNumParticles=numParticles;
printf("preNumParticles=%d\n",preNumParticles);
    numParticles+=iniPos.size();
    particles.resize(numParticles);

    /*for (size_t i=preNumParticles; i<numParticles; i++) {
        particles[i].setPosition(iniPos[i-preNumParticles]);
        particles[i].setVelocity(YsOrigin());
    }

    //create an empty lattice
    createLattice(1.0);
    calculateDensity(0,numParticles);
    calculatePressure(0,numParticles);
    calculateColorField(0,numParticles);*/
}

/*void ParticleSimulation::placeParticles(std::vector <YsVec3> &iniPos,const YsVec3 &velosity)
{
    size_t preNumParticles=numParticles;
    numParticles+=iniPos.size();
    particles.resize(numParticles);

    for (size_t i=preNumParticles; i<numParticles; i++)
    {
        particles[i].setPosition(iniPos[i-preNumParticles]);
        particles[i].setVelocity(velosity);
    }

    //create an empty lattice
    createLattice(1.0);
    calculateDensity(0,numParticles);
    calculatePressure(0,numParticles);
    calculateColorField(0,numParticles);
}*/

void ParticleSimulation::createLattice(double maxH)
{
    YsBoundingBoxMaker3 mkBbx;
    for(int idx=0; idx<numParticles; ++idx)
    {
        particles[idx].getPosition();
        mkBbx.Add(particles[idx].getPosition());
    }
    mkBbx.Get(bbx);

    bbx[0]-=YsXYZ()*YsTolerance;
    bbx[1]+=YsXYZ()*YsTolerance;
    YsInflateBoundingBox(bbx[0],bbx[1],bbx[0],bbx[1],1.1);
    const YsVec3 d=bbx[1]-bbx[0];

    maxH=YsGreater(maxH,hDensity);
    maxH=YsGreater(maxH,hPressure);
    maxH=YsGreater(maxH,hPressureForce);
    maxH=YsGreater(maxH,hViscosity);
    maxH=YsGreater(maxH,hColorField);

    numBlocksX=YsGreater(1,(int)(d.x()/maxH));
    numBlocksY=YsGreater(1,(int)(d.y()/maxH));
    numBlocksZ=YsGreater(1,(int)(d.z()/maxH));

    hx=d.x()/(double)numBlocksX;
    hy=d.y()/(double)numBlocksY;
    hz=d.z()/(double)numBlocksZ;

    numBlocks=numBlocksX*numBlocksY*numBlocksZ;
    lattice.resize(numBlocks);
    for(auto &block : lattice)
    {
        block.clear();
    }

    for(int i=0; i<numParticles; ++i)
    {
        //block index in x,y,z direction
        int bx=(particles[i].getPosition().x()-bbx[0].x())/hx;
        int by=(particles[i].getPosition().y()-bbx[0].y())/hy;
        int bz=(particles[i].getPosition().z()-bbx[0].z())/hz;

        const int blockIdx=bz*numBlocksX*numBlocksY+by*numBlocksX+bx;

        if(blockIdx<0 || numBlocks<=blockIdx)
        {
            printf("Boom!  Something went wrong!\n");
            exit(1);
        }
        lattice[blockIdx].push_back(&particles[i]);
    }
}


//loop through all the particles in the neighbour block
YsVec3 ParticleSimulation::particleLatticeLoop(const Particle *a,YsVec3PP fun)const
{
    YsVec3 res=YsOrigin();

    int bx=(a->getPosition().x()-bbx[0].x())/hx;
    int by=(a->getPosition().y()-bbx[0].y())/hy;
    int bz=(a->getPosition().z()-bbx[0].z())/hz;

    for (int i=bx-1; i<=bx+1; i++) {
        for (int j=by-1; j<=by+1; j++) {
            for (int k=bz-1; k<=bz+1; k++) {
                if (i>=0 && i<numBlocksX &&
                        j>=0 && j<numBlocksY &&
                        k>=0 && k<numBlocksZ) {
                    int neibBlockIdx=k*numBlocksX*numBlocksY+j*numBlocksX+i;
                    Block block=lattice[neibBlockIdx];

                    for (int n=0; n<block.size();n++) {
                        Particle *b=block[n];
                        if(a!=b)
                        {
                            res+=(this->*fun)(a,b);
                        }
                    }
                }
            }
        }
    }
    return res;
}

double ParticleSimulation::particleLatticeLoop(const Particle *a,doublePP fun)const
{
    double res=0.0;

    int bx=(a->getPosition().x()-bbx[0].x())/hx;
    int by=(a->getPosition().y()-bbx[0].y())/hy;
    int bz=(a->getPosition().z()-bbx[0].z())/hz;

    for (int i=bx-1; i<=bx+1; i++)
    {
        for (int j=by-1; j<=by+1; j++)
        {
            for (int k=bz-1; k<=bz+1; k++)
            {
                // block boundary check
                if (i>=0 && i<numBlocksX &&j>=0 && j<numBlocksY && k>=0 && k<numBlocksZ)
                {
                    int neibBlockIdx=k*numBlocksX*numBlocksY+j*numBlocksX+i;
                    Block block=lattice[neibBlockIdx];
                    for (int n=0; n<block.size();n++)
                    {
                        Particle *b=block[n];
                        res+=(this->*fun)(a,b);
                    }
                }
            }
        }
    }
    return res;
}

int ParticleSimulation::particleLatticeLoop(const Particle *a,intPP fun)const
{
    int res=0;

    int bx=(a->getPosition().x()-bbx[0].x())/hx;
    int by=(a->getPosition().y()-bbx[0].y())/hy;
    int bz=(a->getPosition().z()-bbx[0].z())/hz;

    for (int i=bx-1; i<=bx+1; i++)
    {
        for (int j=by-1; j<=by+1; j++)
        {
            for (int k=bz-1; k<=bz+1; k++)
            {
                // block boundary check
                if (i>=0 && i<numBlocksX &&j>=0 && j<numBlocksY && k>=0 && k<numBlocksZ)
                {
                    int neibBlockIdx=k*numBlocksX*numBlocksY+j*numBlocksX+i;
                    Block block=lattice[neibBlockIdx];
                    for (int n=0; n<block.size();n++)
                    {
                        Particle *b=block[n];
                        res+=(this->*fun)(a,b);
                    }
                }
            }
        }
    }
    return res;
}

YsMatrix3x3 ParticleSimulation::particleLatticeLoop(const Particle *a,YsMatrix3x3PP fun)const
{
    YsMatrix3x3 res;
    res.Set(1,1,0);
    res.Set(1,2,0);
    res.Set(1,3,0);
    res.Set(2,1,0);
    res.Set(2,2,0);
    res.Set(2,3,0);
    res.Set(3,1,0);
    res.Set(3,2,0);
    res.Set(3,3,0);


    int bx=(a->getPosition().x()-bbx[0].x())/hx;
    int by=(a->getPosition().y()-bbx[0].y())/hy;
    int bz=(a->getPosition().z()-bbx[0].z())/hz;

    for (int i=bx-1; i<=bx+1; i++)
    {
        for (int j=by-1; j<=by+1; j++)
        {
            for (int k=bz-1; k<=bz+1; k++)
            {
                // block boundary check
                if (i>=0 && i<numBlocksX &&j>=0 && j<numBlocksY && k>=0 && k<numBlocksZ)
                {
                    int neibBlockIdx=k*numBlocksX*numBlocksY+j*numBlocksX+i;
                    Block block=lattice[neibBlockIdx];
                    for (int n=0; n<block.size();n++)
                    {
                        Particle *b=block[n];
                        res+=(this->*fun)(a,b);
                    }
                }
            }
        }
    }
    return res;
}




double ParticleSimulation::density(const Particle *a,const Particle *b)const
{
    YsVec3 ra=a->getPosition();
    YsVec3 rb=b->getPosition();
    double mb=b->getMass();
    double W=Wpoly6(ra,rb,hDensity);
    return mb*W;
}

void ParticleSimulation::calculateDensity(int i0,int i1)
{
    doublePP densityFun=&ParticleSimulation::density;
    for (int i=i0; i<i1; i++) {
        double density=particleLatticeLoop(&particles[i], densityFun);
        particles[i].setDensity(density);
    }
}

/* static */ void ParticleSimulation::calculateDensityThread(ThreadInfo *info)
{
    info->sim->calculateDensity(info->i0,info->i1);
    info->sim->calculatePressure(info->i0,info->i1);
    info->sim->calculateColorField(info->i0,info->i1);
    info->sim->calculateNormalAndCurvature(info->i0
                                           ,info->i1);
}

/* static */ void ParticleSimulation::calculateForceThread(ThreadInfo *info)
{
    info->sim->calculateForce(info->i0,info->i1);
}


double ParticleSimulation::colorField(const Particle *i,const Particle *j)const
{
    YsVec3 ri=i->getPosition();
    YsVec3 rj=j->getPosition();
    double mj=j->getMass();
    double rhoj=j->getDensity();
    double W=Wpoly6(ri, rj, hColorField);
//printf("mj=%lf,rhoj=%lf,laplacian_W=%lf\n",mj,rhoj,laplacian_W);
    return mj/rhoj*W;
}

void ParticleSimulation::calculateColorField(int i0,int i1)
{
    doublePP colorFieldFun=&ParticleSimulation::colorField;
    for (int i=i0; i<i1; i++) {
        double colorField=particleLatticeLoop(&particles[i], colorFieldFun);
        particles[i].setColorField(colorField);
    }

}

void ParticleSimulation::calculatePressure(int i0,int i1)
{
    for (int i=i0; i<i1; i++) {
        double density=particles[i].getDensity();
        double pressure=k*(density-iniDensity);
        particles[i].setPressure(pressure);
    }
}

void ParticleSimulation::calculateNormalAndCurvature(int i0,int i1)
{
    YsVec3PP normalFun=&ParticleSimulation::normal;
    doublePP curvatureFun=&ParticleSimulation::curvature;

    for(int i=i0; i<i1; ++i)
    {
        const YsVec3 n=particleLatticeLoop(&particles[i],normalFun);
        const double cur=particleLatticeLoop(&particles[i],curvatureFun);
        particles[i].setCurvature(cur);
        particles[i].setNormal(n);
    }
}

void ParticleSimulation::calculateForce(int i0,int i1)
{
    YsVec3PP pressureForceFun=&ParticleSimulation::pressureForce;
    YsVec3PP viscosityForceFun=&ParticleSimulation::viscosityForce;

    for(int i=i0; i<i1; ++i)
    {
        YsVec3 pressureForce=-particleLatticeLoop(&particles[i],pressureForceFun);
        YsVec3 viscosityForce=particleLatticeLoop(&particles[i], viscosityForceFun);
        particles[i].setPressureForce(pressureForce);
        particles[i].setViscosityForce(viscosityForce);
        particles[i].setSurfaceTension(
                sigma*particles[i].getNormal()*particles[i].getCurvature());
        particles[i].setGravity();
    }
}

YsVec3 ParticleSimulation::normal(const Particle *a,const Particle *b)const
{
    YsVec3 pa=a->getPosition();
    YsVec3 pb=b->getPosition();
    double mb=b->getMass();
    double db=b->getDensity();
    YsVec3 gradiant_W_cs=gradientWpoly6(pa,pb,hColorField);
    YsVec3 n=mb/db*gradiant_W_cs;
    return n;
}

double ParticleSimulation::curvature(const Particle *a,const Particle *b)const
{
    YsVec3 pa=a->getPosition();
    YsVec3 pb=b->getPosition();
    double mb=b->getMass();
    double db=b->getDensity();
    double laplacian_W_cs=laplacianWpoly6(pa, pb, hColorField);
    return -mb/db*laplacian_W_cs;
}

//return presFromBonA
YsVec3 ParticleSimulation::pressureForce(const Particle *a,const Particle *b)const
{
    double presa=a->getPressure();
    YsVec3 pa=a->getPosition();
    YsVec3 pb=b->getPosition();
    double mb=b->getMass();
    double presb=b->getPressure();
    double db=b->getDensity();
    YsVec3 gradient_W=gradientWspiky(pa,pb,hPressure);
    YsVec3 pres= mb*(presa+presb)/(2*db)*gradient_W;
    return pres;
}

//return viscFromBonA
YsVec3 ParticleSimulation::viscosityForce(const Particle *a,const Particle *b)const
{
    YsVec3 va=a->getVelocity();
    YsVec3 pa=a->getPosition();
    YsVec3 pb=b->getPosition();
    double mb=b->getMass();
    YsVec3 vb=b->getVelocity();
    double db=b->getDensity();
    double laplacian_W=laplacianWviscosity(pa,pb,hViscosity);
    return mu*mb*(vb-va)/db*laplacian_W;
}

void ParticleSimulation::collision()
{
    /*when a particle collides with an object
     *simply push the particle out of the object
     *and reflect the velocity component that is
     *perpendicular to the object's surface
     */
    for (int i=0; i<numParticles; i++) {
        obstacle->collide(particles[i]);
    }
}

void ParticleSimulation::update()
{

    t+=dt;
    createLattice(1.0);

    if(hoseOn)
    {
        hose.timer-=dt;
        if(hose.timer<0.0)
        {
           hose.timer+=0.05;
           hose.inject(this);
        }
    }

    if(wallMove){
      obstacle->move();
    }

    createLattice(1.0);

    bool singleThreaded=false;
    if(singleThreaded)
    {
        calculateDensity(0,numParticles);
        calculatePressure(0,numParticles);
        calculateColorField(0,numParticles);
        calculateNormalAndCurvature(0,numParticles);
        calculateForce(0,numParticles);
    }
    else
    {
        const int nThread=6;
        ThreadInfo info[nThread];
        for(int i=0; i<nThread; ++i)
        {
            info[i].sim=this;
            info[i].i0=i*numParticles/nThread;
            info[i].i1=(i+1)*numParticles/nThread;
        }

        std::thread thr[nThread];
        for(int i=0; i<nThread; ++i)
        {
            std::thread t(calculateDensityThread,&info[i]);
            thr[i].swap(t);
        }
        for(int i=0; i<nThread; ++i)
        {
            thr[i].join();
        }

        std::thread forceThread[nThread];
        for(int i=0;i<nThread;++i)
        {
            std::thread t(calculateForceThread,&info[i]);
            forceThread[i].swap(t);
        }
        for(int i=0;i<nThread;++i)
        {
            forceThread[i].join();
        }
        //calculatePressure(0,numParticles);
        //calculateColorField(0,numParticles);
        //calculateNormalAndCurvature(0,numParticles);
        //calculateForce(0,numParticles);
    }

    collision();

    for(int n=0; n<numParticles; ++n)
    {
        YsVec3 pressureForce=particles[n].getPressureForce();
        YsVec3 viscosityForce=particles[n].getViscosityForce();
        YsVec3 surfaceTension=particles[n].getSurfaceTension();
        YsVec3 gravity=particles[n].getGravity();

        double density=particles[n].getDensity();
        double pressure=particles[n].getPressure();
        double colorField=particles[n].getColorField();
        double curvature=particles[n].getCurvature();
        YsVec3 normal=particles[n].getNormal();

        YsVec3 force,acceleration,velocity,position;
        GLdouble white[3]={1.0,1.0,1.0};
        //particles[n].setColor(white);

        switch (mode) {
            case MODE_PRESSURE_FORCE_TEST:
                force=pressureForce;
                acceleration=force/density;
                velocity=particles[n].getVelocity()+acceleration*dt;
                position=particles[n].getPosition()+velocity*dt;
                particles[n].setVelocity(velocity);
                particles[n].setPosition(position);
                break;

            case MODE_VISCOSITY_FORCE_TEST:
                force=viscosityForce;
                acceleration=force/density;
                velocity=particles[n].getVelocity()+acceleration*dt;
                position=particles[n].getPosition()+velocity*dt;
                particles[n].setVelocity(velocity);
                particles[n].setPosition(position);
                break;

            case MODE_SURFACE_TENSION_TEST:
                force=surfaceTension;
                acceleration=force/density;
                velocity=particles[n].getVelocity()+acceleration*dt;
                position=particles[n].getPosition()+velocity*dt;
                particles[n].setVelocity(velocity);
                particles[n].setPosition(position);
                break;

            case MODE_NORMAL:
                viscosityForce=particles[n].getViscosityForce();
                surfaceTension=particles[n].getSurfaceTension();
                gravity=particles[n].getGravity();

                force=pressureForce+viscosityForce+surfaceTension+gravity;
                acceleration=force/density;
                velocity=particles[n].getVelocity()+acceleration*dt;
                position=particles[n].getPosition()+velocity*dt;
                particles[n].setVelocity(velocity);
                particles[n].setPosition(position);
                break;

            default:
                break;
        }
    }
}

void ParticleSimulation::destroy()
{
}



const int ParticleSimulation::edgeTable[256]={
    0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
    0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
    0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
    0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
    0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
    0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
    0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
    0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
    0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
    0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
    0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
    0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
    0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
    0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
    0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
    0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
    0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
    0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
    0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
    0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
    0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
    0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
    0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
    0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
    0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
    0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
    0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
    0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
    0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
    0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
    0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
    0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };

const int ParticleSimulation::triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
    {8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
    {4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
    {3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
    {1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
    {4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
    {4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
    {5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
    {2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
    {9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
    {0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
    {2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
    {10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
    {5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
    {5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
    {9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
    {1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
    {10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
    {8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
    {2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
    {7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
    {2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
    {11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
    {5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
    {11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
    {11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
    {9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
    {2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
    {6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
    {3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
    {6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
    {10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
    {6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
    {8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
    {7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
    {3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
    {5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
    {0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
    {9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
    {8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
    {5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
    {0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
    {6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
    {10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
    {10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
    {8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
    {1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
    {0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
    {10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
    {3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
    {6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
    {9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
    {8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
    {3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
    {6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
    {0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
    {10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
    {10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
    {2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
    {7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
    {7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
    {2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
    {1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
    {11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
    {8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
    {0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
    {7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
    {10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
    {2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
    {6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
    {7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
    {2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
    {1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
    {10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
    {10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
    {0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
    {7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
    {6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
    {8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
    {9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
    {6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
    {4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
    {10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
    {8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
    {0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
    {1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
    {8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
    {10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
    {4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
    {10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
    {5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
    {11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
    {9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
    {6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
    {7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
    {3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
    {7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
    {3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
    {6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
    {9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
    {1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
    {4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
    {7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
    {6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
    {3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
    {0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
    {6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
    {0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
    {11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
    {6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
    {5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
    {9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
    {1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
    {1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
    {10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
    {0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
    {5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
    {10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
    {11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
    {9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
    {7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
    {2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
    {8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
    {9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
    {9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
    {1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
    {9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
    {9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
    {5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
    {0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
    {10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
    {2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
    {0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
    {0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
    {9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
    {5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
    {3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
    {5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
    {8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
    {0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
    {9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
    {1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
    {3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
    {4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
    {9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
    {11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
    {11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
    {2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
    {9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
    {3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
    {1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
    {4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
    {4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
    {0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
    {3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
    {3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
    {0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
    {9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
    {1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};


double ParticleSimulation::density(const YsVec3 &pa,const Particle *b) const
{
    YsVec3 pb=b->getPosition();
    double mb=b->getMass();
    double W=Wpoly6(pa,pb,hDensity);
    return mb*W;
}

double ParticleSimulation::colorField(const YsVec3 &r,const Particle *j)const
{
    YsVec3 rj=j->getPosition();
    double mj=j->getMass();
    double rhoj=j->getDensity();
    double W=Wpoly6(r, rj, hColorField);
//printf("mj=%lf,rhoj=%lf,laplacian_W=%lf\n",mj,rhoj,laplacian_W);
    return mj/rhoj*W;
}

YsVec3 ParticleSimulation::normal(const YsVec3 &pa,const Particle *b)const
{
    YsVec3 pb=b->getPosition();
    double mb=b->getMass();
    double db=b->getDensity();
    YsVec3 gradiant_W_cs=gradientWpoly6(pa,pb,hColorField);
    YsVec3 n=mb/db*gradiant_W_cs;
    return n;
}


void ParticleSimulation::drawParticles()
{

    //calculateWeightSum();
    //calculateWeightedPosition();
    //calculateUpdatedKernelCenter();
    glBegin(GL_POINTS);
    for (int i=0; i<numParticles; i++) {
        glPointSize(particles[i].getPointSize());
        glColor3dv(particles[i].getColor());
        glVertex3f(particles[i].getPosition().x(),
                   particles[i].getPosition().y(),
                   particles[i].getPosition().z());
    }
    glEnd();

}

void ParticleSimulation::drawObstacle()
{
    obstacle->draw();
}

void ParticleSimulation::drawParticleOfInterest(void)
{
    if(0<=particleOfInterest && particleOfInterest<numParticles)
    {
        glColor3d(0.0,0.0,1.0);
        particles[particleOfInterest].setPointSize(10.0);
        particles[particleOfInterest].draw();
        particles[particleOfInterest].setPointSize(1.0);

		glBegin(GL_LINES);

		glColor3d(1.0,0.0,0.0);
		auto pos=particles[particleOfInterest].getPosition();
		auto t=YsUnitVector(particles[particleOfInterest].getSurfaceTension());
		glVertex3dv(pos);
		glVertex3dv(pos+t);

		glColor3d(0.0,0.0,1.0);
		auto nom=YsUnitVector(particles[particleOfInterest].getNormal());
		glVertex3dv(pos);
		glVertex3dv(pos+nom);

		glEnd();
    }
}

void ParticleSimulation::drawColorFieldGrid()
{
	glPointSize(3);

	glBegin(GL_POINTS);
	for(int x=-15; x<=15; ++x)
	{
		for(int y=0; y<=10; ++y)
		{
			for(int z=-15; z<=15; ++z)
			{
				const YsVec3 pos(x,y,z);
				const double cf=evaluateColorFieldAtPoint(pos);

				const double t=cf/4.0;
                if(1.0>t){

				double col[3];
				GetColorGradation(col,t);

				glColor3dv(col);
				glVertex3dv(pos);
                }
			}
		}
	}
	glEnd();

	glPointSize(1);
}

double ParticleSimulation::weightSum(const Particle *i,const Particle *j)const
{
    YsVec3 xi=i->getPosition();
    YsVec3 xj=j->getPosition();
    double wij=w(xi, xj, 2*hWeightedPosition);
    return wij;
}

YsVec3 ParticleSimulation::positionSum(const Particle *i,const Particle *j)const
{
    YsVec3 xi=i->getPosition();
    YsVec3 xj=j->getPosition();
    double wij=w(xi, xj, 2*hWeightedPosition);
    return wij*xj;
}

int ParticleSimulation::numNeighbors(const Particle *i,const Particle *j)const
{
    YsVec3 xi=i->getPosition();
    YsVec3 xj=j->getPosition();
    YsVec3 xij=xi-xj;
    if(xij.GetLength()<2*hWeightedPosition){
        return 1;
    }
    return 0;
}

YsMatrix3x3 ParticleSimulation::covarianceMatrixSum(const Particle *i,const Particle *j)const
{

    YsVec3 xi=i->getPosition();
    YsVec3 xj=j->getPosition();
    YsVec3 xiw=i->getWeightedPosition();

    double wij=w(xi, xj, 2*hWeightedPosition);
    YsVec3 x=xj-xiw;

    YsMatrix3x3 mat;
    mat.Set(1,1,x.x()*x.x());
    mat.Set(1,2,x.x()*x.y());
    mat.Set(1,3,x.x()*x.z());

    mat.Set(2,1,x.y()*x.x());
    mat.Set(2,2,x.y()*x.y());
    mat.Set(2,3,x.y()*x.z());

    mat.Set(3,1,x.z()*x.x());
    mat.Set(3,2,x.z()*x.y());
    mat.Set(3,3,x.z()*x.z());

    return wij*mat;
}

void ParticleSimulation::calculateNumNeighbors(int i0,int i1)
{
    intPP numNeighborsFun=&ParticleSimulation::numNeighbors;
    for(int i=i0;i<i1;i++)
    {
        int numNeighbors=particleLatticeLoop(&particles[i],numNeighborsFun);
        particles[i].setNumNeighbors(numNeighbors);
    }
}
void ParticleSimulation::calculateWeightSum(int i0,int i1)
{
    doublePP weightSumFun=&ParticleSimulation::weightSum;
    for(int i=i0;i<i1;i++)
    {
        double weightSum=particleLatticeLoop(&particles[i], weightSumFun);
        particles[i].setWeightSum(weightSum);
    }
}

void ParticleSimulation::calculateWeightedPosition(int i0,int i1)
{
    YsVec3PP positionSumFun=&ParticleSimulation::positionSum;
    for(int i=i0;i<i1;i++)
    {
        YsVec3 positionSum=particleLatticeLoop(&particles[i], positionSumFun);
        YsVec3 weightedPosition=positionSum/particles[i].getWeightSum();
        particles[i].setWeightedPosition(weightedPosition);
    }

}

void ParticleSimulation::calculateUpdatedKernelCenter(int i0,int i1)
{
    for(int i=i0;i<i1;i++)
    {
        YsVec3 xi=particles[i].getPosition();
        YsVec3 xiw=particles[i].getWeightedPosition();
        YsVec3 xi_=(1-rambda)*xi+rambda*xiw;
        particles[i].setUpdatedKernelCenter(xi_);
    }
}


void ParticleSimulation::calculateCovarianceMatrix(int i0,int i1)
{
    YsMatrix3x3PP covarianceMatrixSumFun=&ParticleSimulation::covarianceMatrixSum;

    for (int i=i0; i<i1; i++) {
          YsMatrix3x3 covarianceMatrixSum=particleLatticeLoop(&particles[i],covarianceMatrixSumFun);
        YsMatrix3x3 covarianceMatrix=covarianceMatrixSum/particles[i].getWeightSum();
        particles[i].setCovarianceMatrix(covarianceMatrix);

    }
}

void ParticleSimulation::calculateG(int i0,int i1)
{
    for (int i=i0; i<i1; i++) {
        YsMatrix3x3 C3x3=particles[i].getCovarianceMatrix();

        YsMatrix C=convert(C3x3);
        YsMatrix eigenValue;
        eigenValue.Create(3,3);
        YsMatrix eigenVector;
        eigenVector.Create(3,3);

        YsFindEigenValueEigenVectorByJacobiMethod(eigenValue, eigenVector, C);

        /* convert eigenValue and eigenVector to YsMatrix3x3 */
        YsMatrix3x3 eigenValue3x3=convert(eigenValue);
        YsMatrix3x3 eigenVector3x3=convert(eigenVector);

        YsMatrix3x3 RT=eigenVector3x3;
        RT.Transpose();

        YsMatrix3x3 Sigma=5*eigenValue3x3;

        /*if(particles[i].getNumNeighbors()<=N){
            Sigma=kn*Sigma;
        }
        else{
            double sigma1=eigenValue3x3.v(1,1);
            for (int k=1; k<=3; k++) {
                double sigmak=max(eigenValue3x3.v(k,k),sigma1/kr);
                Sigma.Set(k, k,eigenValue3x3.v(k, k));
            }
        }*/

        Sigma.Invert();
        YsMatrix3x3 G=1/hWeightedPosition*RT*Sigma*eigenVector3x3;
       particles[i].setG(G);
    }
}

YsMatrix3x3 convert(const YsMatrix &mat)
{
    YsMatrix3x3 mat3x3;
    for(int r=1; r<=3; ++r)
    {
        for(int c=1; c<=3; ++c)
        {
            mat3x3.Set(r,c,mat.v(r,c));
        }
    }
    return mat3x3;
}

YsMatrix convert(const YsMatrix3x3 &mat3x3)
{
    YsMatrix mat;
    mat.Create(3,3);
    for(int r=1; r<=3; ++r)
    {
        for(int c=1; c<=3; ++c)
        {
            mat.Set(r,c,mat3x3.v(r,c));
        }
    }
    return mat;
}


double ParticleSimulation::evaluateColorFieldAtPoint(const YsVec3 &pos)const
{
    doubleVP colorFieldFun;
    if(true==isotropic)
    {
    	colorFieldFun=&ParticleSimulation::colorField;
	}
	else
	{
    	colorFieldFun=&ParticleSimulation::anisotropicColorField;
	}
    return vectorLatticeLoop(pos, colorFieldFun);
}

double ParticleSimulation::anisotropicColorField(const YsVec3 &x,const Particle *j)const
{
    double mj=j->getMass();
    double rhoj=j->getDensity();
    YsVec3 xj_=j->getUpdatedKernelCenter();
    YsMatrix3x3 Gj=j->getG();
    double W=aWpoly6(x, xj_,Gj);
    return mj/rhoj*W;
}

double ParticleSimulation::vectorLatticeLoop(const YsVec3 &pos,doubleVP fun) const
{
    double res=0.0;

    int bx=(pos.x()-bbx[0].x())/hx;
    int by=(pos.y()-bbx[0].y())/hy;
    int bz=(pos.z()-bbx[0].z())/hz;

    for (int i=bx-1; i<=bx+1; i++)
    {
        for (int j=by-1; j<=by+1; j++)
        {
            for (int k=bz-1; k<=bz+1; k++)
            {
                if (i>=0 && i<numBlocksX &&j>=0 && j<numBlocksY && k>=0 && k<numBlocksZ)
                   {
                    int neibBlockIdx=k*numBlocksX*numBlocksY+j*numBlocksX+i;
                    Block block=lattice[neibBlockIdx];
                    for (int n=0; n<block.size();n++)
                    {
                        Particle *b=block[n];
                        res+=(this->*fun)(pos,b);
                    }
                }
            }
        }
    }
    return res;
}

void ParticleSimulation::triangulate(vector<Triangle> &triangles,GridCell grid,double iso)
{
    int cubeindex;
    YsVec3 vertlist[12];
    /*
     Determine the index into the edge table which
     tells us which vertices are inside of the surface
     */
    cubeindex = 0;
    if (grid.val[0] < iso) cubeindex |= 1;
    if (grid.val[1] < iso) cubeindex |= 2;
    if (grid.val[2] < iso) cubeindex |= 4;
    if (grid.val[3] < iso) cubeindex |= 8;
    if (grid.val[4] < iso) cubeindex |= 16;
    if (grid.val[5] < iso) cubeindex |= 32;
    if (grid.val[6] < iso) cubeindex |= 64;
    if (grid.val[7] < iso) cubeindex |= 128;

    /* Cube is entirely in/out of the surface */
    if (edgeTable[cubeindex] == 0)
        return;

    /* Find the vertices where the surface intersects the cube */
    if (edgeTable[cubeindex] & 1)
        vertlist[0] =
        vertexInterp(iso,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
    if (edgeTable[cubeindex] & 2)
        vertlist[1] =
        vertexInterp(iso,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
    if (edgeTable[cubeindex] & 4)
        vertlist[2] =
        vertexInterp(iso,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
    if (edgeTable[cubeindex] & 8)
        vertlist[3] =
        vertexInterp(iso,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
    if (edgeTable[cubeindex] & 16)
        vertlist[4] =
        vertexInterp(iso,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
    if (edgeTable[cubeindex] & 32)
        vertlist[5] =
        vertexInterp(iso,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
    if (edgeTable[cubeindex] & 64)
        vertlist[6] =
        vertexInterp(iso,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
    if (edgeTable[cubeindex] & 128)
        vertlist[7] =
        vertexInterp(iso,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
    if (edgeTable[cubeindex] & 256)
        vertlist[8] =
        vertexInterp(iso,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
    if (edgeTable[cubeindex] & 512)
        vertlist[9] =
        vertexInterp(iso,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
    if (edgeTable[cubeindex] & 1024)
        vertlist[10] =
        vertexInterp(iso,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
    if (edgeTable[cubeindex] & 2048)
        vertlist[11] =
        vertexInterp(iso,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

    /* Create the triangle */
    for (int i=0;triTable[cubeindex][i]!=-1;i+=3) {
        Triangle triangle;
        triangle.p[0] = vertlist[triTable[cubeindex][i  ]];
        triangle.p[1] = vertlist[triTable[cubeindex][i+1]];
        triangle.p[2] = vertlist[triTable[cubeindex][i+2]];
        triangles.push_back(triangle);
    }
}

/*
 Linearly interpolate the position where an isosurface cuts
 an edge between two vertices, each with their own scalar value
 */
YsVec3 ParticleSimulation::vertexInterp(double iso,YsVec3 p1,YsVec3 p2,double valp1,double valp2)
{
    double mu;
    YsVec3 p;

    if (fabs(iso-valp1) < 0.00001)
        return(p1);
    if (fabs(iso-valp2) < 0.00001)
        return(p2);
    if (fabs(valp1-valp2) < 0.00001)
        return(p1);
    mu = (iso - valp1) / (valp2 - valp1);
    p.SetX( p1.x() + mu * (p2.x() - p1.x())) ;
    p.SetY(p1.y() + mu * (p2.y() - p1.y()));
    p.SetZ(p1.z() + mu * (p2.z() - p1.z()));

    return(p);
}




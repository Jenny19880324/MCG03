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

/* static */void ParticleSimulation::calculateGThread(ThreadInfo *info)
{
    info->sim->calculateWeightSum(info->i0,info->i1);
    info->sim->calculateWeightedPosition(info->i0,info->i1);
    info->sim->calculateUpdatedKernelCenter(info->i0,info->i1);
    info->sim->calculateNumNeighbors(info->i0,info->i1);
    info->sim->calculateCovarianceMatrix(info->i0,info->i1);
    info->sim->calculateG(info->i0,info->i1);
}

/* static */ void ParticleSimulation::meshThread(MeshThreadInfo *info)
{
    info->sim->mesh(info->triangles,info->i0,info->i1);
}

void ParticleSimulation::mesh(vector<Triangle> &triangles,int i0,int i1)
{
    size_t preNumTriangles=triangles.size();
    std::vector <double> cs;

	const int numCubesX=i1-i0;

    cs.resize((numCubesX+1)*(numCubesY+1)*(numCubesZ+1));
    for (int i=0; i<=numCubesX; i++) {
        for (int j=0; j<=numCubesY; j++) {
            for (int k=0; k<=numCubesZ; k++) {
                auto pos=bbx[0]+(i0+i)*cx*YsXVec()+j*cy*YsYVec()+k*cz*YsZVec();
                auto idx=k*(numCubesX+1)*(numCubesY+1)+j*(numCubesX+1)+i;
                cs[idx]=evaluateColorFieldAtPoint(pos);
                //printf("cs[%d]=%lf\n",idx,cs[idx]);
            }
        }
    }

    for (int i=i0; i<i1; i++) {
        for (int j=0; j<numCubesY; j++) {
            for (int k=0; k<numCubesZ; k++) {

                GridCell cell;
                cell.p[0]=bbx[0]+i*cx*YsXVec()+j*cy*YsYVec()+k*cz*YsZVec();  // (i  ,j  ,k)
                cell.p[1]=cell.p[0]+cx*YsXVec();                             // (i+1,j  ,k)
                cell.p[2]=cell.p[0]+cx*YsXVec()+cz*YsZVec();                 // (i+1,j  ,k+1)
                cell.p[3]=cell.p[0]+cz*YsZVec();                             // (i  ,j  ,k+1)
                cell.p[4]=cell.p[0]+cy*YsYVec();                             // (i  ,j+1,k)
                cell.p[5]=cell.p[1]+cy*YsYVec();                             // (i+1,j+1,k)
                cell.p[6]=cell.p[2]+cy*YsYVec();                             // (i+1,j+1,k+1)
                cell.p[7]=cell.p[3]+cy*YsYVec();                             // (i,  j+1,k+1)

                cell.val[0]=cs[(i-i0  )+(j  )*(numCubesX+1)+(k  )*(numCubesX+1)*(numCubesY+1)];
                cell.val[1]=cs[(i+1-i0)+(j  )*(numCubesX+1)+(k  )*(numCubesX+1)*(numCubesY+1)];
                cell.val[2]=cs[(i+1-i0)+(j  )*(numCubesX+1)+(k+1)*(numCubesX+1)*(numCubesY+1)];
                cell.val[3]=cs[(i-i0  )+(j  )*(numCubesX+1)+(k+1)*(numCubesX+1)*(numCubesY+1)];
                cell.val[4]=cs[(i-i0  )+(j+1)*(numCubesX+1)+(k  )*(numCubesX+1)*(numCubesY+1)];
                cell.val[5]=cs[(i+1-i0)+(j+1)*(numCubesX+1)+(k  )*(numCubesX+1)*(numCubesY+1)];
                cell.val[6]=cs[(i+1-i0)+(j+1)*(numCubesX+1)+(k+1)*(numCubesX+1)*(numCubesY+1)];
                cell.val[7]=cs[(i-i0  )+(j+1)*(numCubesX+1)+(k+1)*(numCubesX+1)*(numCubesY+1)];
                triangulate(triangles,cell,iso);
            }
        }
    }
}

void ParticleSimulation::drawMesh()
{

	if(true==isotropic)
	{
	    createLattice(1.0);
	}
	else
	{
	    createLattice(24.0);
	}
    shl.CleanUp();
    triangles.resize(0);

    YsVec3 bbx[2]={this->bbx[0],this->bbx[1]};
    bbx[0]-=YsXYZ()*1.5;
    bbx[1]+=YsXYZ()*1.5;
    numCubesX=(bbx[1].x()-bbx[0].x())/cx;
    numCubesY=(bbx[1].y()-bbx[0].y())/cy;
    numCubesZ=(bbx[1].z()-bbx[0].z())/cz;

    const int nThread=6;
    std::thread thr[nThread];

	if(true!=isotropic)
	{
		ThreadInfo gInfo[nThread];
	    for(int i=0;i<nThread;++i)
	    {
	        gInfo[i].sim=this;
	        gInfo[i].i0=i*numParticles/nThread;
	        gInfo[i].i1=(i+1)*numParticles/nThread;
	    }

	   for(int i=0;i<nThread;++i)
	    {
	      std::thread t(calculateGThread,&gInfo[i]);
	      thr[i].swap(t);
	   }
	    for(int i=0;i<nThread;++i)
	    {
	        thr[i].join();
	    }
	}



    MeshThreadInfo mcInfo[nThread];
    for(int i=0;i<nThread;++i)
    {
        mcInfo[i].sim=this;
        mcInfo[i].i0=i*numCubesX/nThread;
        mcInfo[i].i1=(i+1)*numCubesX/nThread;
    }

    std::thread meshThr[nThread];
printf("%s:%d\n",__FUNCTION__,__LINE__);
    for(int i=0;i<nThread;i++)
    {
        std::thread t(meshThread,&mcInfo[i]);
        meshThr[i].swap(t);
    }
    for(int i=0;i<nThread;++i)
    {
        meshThr[i].join();
    }
printf("%s:%d\n",__FUNCTION__,__LINE__);


	this->triangles.resize(0);
	for(auto &i : mcInfo)
	{
		for(auto &t : i.triangles)
		{
			this->triangles.push_back(t);

	        YsShellVertexHandle triVtHd[3]=
	        {
	            shl.AddVertexH(t.p[0]),
	            shl.AddVertexH(t.p[1]),
	            shl.AddVertexH(t.p[2])
	        };
	        YsShellPolygonHandle plHd=shl.AddPolygonH(3,triVtHd);
		}
	}


    YsVec3 lightDir(0.5,1.0,8.0); //Vector toward the light source
    lightDir.Normalize();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    GLfloat light[4]={lightDir.xf(),lightDir.yf(),lightDir.zf(),0.0};
    glLightfv(GL_LIGHT0,GL_POSITION,light);
    glPopMatrix();

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);


    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
    for (int i=0; i<triangles.size(); i++) {
        YsVec3 n=-(triangles[i].p[0]-triangles[i].p[1])^(triangles[i].p[0]-triangles[i].p[2]);
        n=YsUnitVector(n);
        glNormal3d(n.x(),n.y(),n.z());
        glVertex3dv(triangles[i].p[0]);
        glVertex3dv(triangles[i].p[1]);
        glVertex3dv(triangles[i].p[2]);
    }
    glEnd();

    glDisable(GL_LIGHTING);

}


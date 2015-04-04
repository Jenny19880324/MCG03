#include <stdio.h>
#include <math.h>
#include <fssimplewindow.h>
#include <vector>
#include <ParticleSimulation.h>

#undef min
#undef max

typedef struct {
    //not allocated,pointed it to something static
    const char *infile;
    //not allocated,pointed it to something static
    const char *outfile;
}Options;

void CreateUniformInitialParticleLocation(std::vector <YsVec3> &iniPos,int nx,int ny,int nz,const YsVec3 center,const double interval)
{
	iniPos.resize(0);

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

                iniPos.push_back(center+o+YsVec3(xOffset,yOffset,zOffset));
            }
        }
    }
}


class CameraObject
{
public:
    double x,y,z;
    double h,p,b;

    double fov,nearZ,farZ;

    CameraObject();
    void Initialize(void);
    void SetUpCameraProjection(void);
    void SetUpCameraTransformation(void);

    void GetForwardVector(double &vx,double &vy,double &vz);
};

CameraObject::CameraObject()
{
    Initialize();
}

void CameraObject::Initialize(void)
{
    x=0;
    y=0;
    z=0;
    h=0;
    p=0;
    b=0;

    fov=YsPi/6.0;  // 30 degree
    nearZ=0.1;
    farZ=200.0;
}

void CameraObject::SetUpCameraProjection(void)
{
    int wid,hei;
    double aspect;

    FsGetWindowSize(wid,hei);
    aspect=(double)wid/(double)hei;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov*180.0/YsPi,aspect,nearZ,farZ);
}

void CameraObject::SetUpCameraTransformation(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(-b*180.0/YsPi,0.0,0.0,1.0);  // Rotation about Z axis
    glRotated(-p*180.0/YsPi,1.0,0.0,0.0);  // Rotation about X axis
    glRotated(-h*180.0/YsPi,0.0,1.0,0.0);  // Rotation about Y axis
    glTranslated(-x,-y,-z);
}

void CameraObject::GetForwardVector(double &vx,double &vy,double &vz)
{
    vx=-cos(p)*sin(h);
    vy= sin(p);
    vz=-cos(p)*cos(h);
}


class OrbitingViewer
{
public:
    double h,p;
    double dist;
    double focusX,focusY,focusZ;

    OrbitingViewer();
    void Initialize(void);
    void SetUpCamera(CameraObject &camera);
};

OrbitingViewer::OrbitingViewer()
{
    Initialize();
}

void OrbitingViewer::Initialize(void)
{
    h=0;
    p=0;
    dist=50.0;
    focusX=0.0;
    focusY=0.0;
    focusZ=0.0;
}

void OrbitingViewer::SetUpCamera(CameraObject &camera)
{
    camera.h=h;
    camera.p=p;
    camera.b=0.0;

    double vx,vy,vz;
    camera.GetForwardVector(vx,vy,vz);
    camera.x=focusX-vx*dist;
    camera.y=focusY-vy*dist;
    camera.z=focusZ-vz*dist;
}

/*
 * print_usage - Prints program usage
 */
static void print_usage(const char *programname)
{
    printf("Usage: %s -i input_filename -o output_filename\n",programname);
}
/*
 * Parse args into an Options struct.Return true on success,false on failure
 */
static bool parse_args(Options* opt,int argc,char *argv[])
{
    opt->infile=NULL;
    opt->outfile=NULL;
    for(int i=1;i<argc;i++)
    {
        switch(argv[i][1])
        {
            case 'i':
                if(i<argc-1)
                opt->infile=argv[++i];
                break;
            case 'o':
                if(i<argc-1)
                    opt->outfile=argv[++i];
                break;
        }
    }
    return true;
}


int main(int argc,char **argv)
{
    Options opt;
    if(!parse_args(&opt,argc,argv)){
        return 1;
    }
    char strk[256],strmu[256],strsigma[256],striso[256];
    int terminate=0;
    CameraObject camera;
    OrbitingViewer orbit;

	orbit.focusY=4.0;

    camera.z=10.0;

    FsOpenWindow(16,16,800,600,1);


    Cylinder cylinder;
    cylinder.setRadius(9.0);

    //YsVec3 min(-30.0,0.0,-15.0);
    //YsVec3 max(30.0,50.0,15.0);
    Box container;
    //container.setMinMax(min,max);

    Ship cliff;


    std::vector<YsVec3> iniPos;
	CreateUniformInitialParticleLocation(iniPos,60,10,30,YsVec3(0.0,4.0,0.0),0.9);

    ParticleSimulation sim;

    if(opt.infile){
        sim.read(opt.infile);
    }

	sim.mode=sim.MODE_NORMAL;

    YSBOOL obstacleSet=YSFALSE;
    YSBOOL pause=YSTRUE;
    YSBOOL cube=YSFALSE;
    YSBOOL savestl=YSFALSE;

    while(0==terminate)
    {
        FsPollDevice();

        int wid,hei;
        FsGetWindowSize(wid,hei);

        int key=FsInkey();
        switch(key)
        {
        case FSKEY_F1:  //live demo 1: hose
		    {
			container.setMinMax(YsVec3(-12.0,0.0,-12.0),YsVec3(5.0,40.0,5.0));
            sim.setObstacle(container);
	        obstacleSet=YSTRUE;
            sim.hose.setPosition(YsVec3(-10.0,15.0,0.0));

            break;
			}
        case FSKEY_F2: //live demo 2: wall moving
            {
    		container.setMinMax(YsVec3(-12.0,0.0,-12.0),YsVec3(5.0,40.0,5.0));
CreateUniformInitialParticleLocation(iniPos,10,20,10,YsVec3(0.0,10.0,0.0),0.9);
            sim.initParticles(iniPos);
            sim.setObstacle(container);
            obstacleSet=YSTRUE;
            break;
            }
        case FSKEY_F3: //live demo 3:water falling
        {
            sim.setObstacle(cliff);
            obstacleSet=YSTRUE;
            YsVec3 axis=YsZVec();
            sim.hose.setOrientation(axis,YsPi/4.0);
            sim.hose.setPosition(YsVec3(10.0,35.0,10.0));
               }
            break;
        case FSKEY_H:
            switch(sim.hoseOn)
            {
                case true:
                    sim.hoseOn=false;
                    break;
                case false:
                    sim.hoseOn=true;
                    break;
            }
            break;
       case FSKEY_V:
            switch(sim.wallMove)
            {
                case true:
                    sim.wallMove=false;
                    break;
                case false:
                    sim.wallMove=true;
                    break;
            }
            break;

		case FSKEY_O:
			switch(sim.getIsotropicMode())
			{
			case true:
				sim.setIsotropicMode(false);
				printf("Anisotropic mode.\n");
				break;
			case false:
				sim.setIsotropicMode(true);
				printf("Isotropic mode.\n");
				break;
			}
			break;
		case FSKEY_0:
			{
				double c=sim.getMarchingCubesSize();
				c+=0.2;
				if(2.0<c)
				{
					c=0.2;
				}
				printf("Block size=%lf\n",c);
				sim.setMarchingCubesSize(c);
			}
			break;

       case FSKEY_Q:
            if(cube){
                cube=YSFALSE;
            }
            else{
                cube=YSTRUE;
            }
            break;
        case FSKEY_L:
            if(savestl){
                savestl=YSFALSE;
            }
            else{
                savestl=YSTRUE;
            }
            break;
        case FSKEY_W:
                sim.write(NULL);
                break;
	case FSKEY_P:
	       	YsFlip(pause);
	       	break;
        case FSKEY_ESC:
            terminate=1;
            break;
        }


        if(FSKEY_SPACE==key || YSTRUE!=pause)
	{
	  sim.update();
	}
        if(0!=FsGetKeyState(FSKEY_LEFT))
        {
            orbit.h+=YsPi/180.0;
        }
        if(0!=FsGetKeyState(FSKEY_RIGHT))
        {
            orbit.h-=YsPi/180.0;
        }
        if(0!=FsGetKeyState(FSKEY_UP))
        {
            orbit.p+=YsPi/180.0;
        }
        if(0!=FsGetKeyState(FSKEY_DOWN))
        {
            orbit.p-=YsPi/180.0;
        }
        if(0!=FsGetKeyState(FSKEY_F) && orbit.dist>1.0)
        {
            orbit.dist/=1.05;
        }
        if(0!=FsGetKeyState(FSKEY_B) && orbit.dist<200.0)
        {
            orbit.dist*=1.05;
        }
        orbit.SetUpCamera(camera);

        glClearColor(0,0,0,0);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        glViewport(0,0,wid,hei);

        // Set up 3D drawing
        camera.SetUpCameraProjection();
        camera.SetUpCameraTransformation();

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1,1);

        // 3D drawing from here
	glPushMatrix();
	glPointSize(3);

        if(cube){
	       sim.drawMesh();
           if(savestl){
               sim.shl.SaveBinStl("test.stl");
               printf("stl saved\n");
               savestl=YSFALSE;
	       }
        }
        else{
	      sim.drawParticles();
        }

        if(obstacleSet){
            sim.drawObstacle();
        }
        //sim.drawColorFieldGrid();

        glPopMatrix();

        // Set up 2D drawing
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0,(float)wid-1,(float)hei-1,0,-1,1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);

        FsSwapBuffers();
        FsSleep(25);
    }

    return 0;
}



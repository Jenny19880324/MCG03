#include "../fssimplewindow.h"

#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>


static UIView *viewCache=NULL;

void FsIOSReportUIViewPointer(UIView *view)
{
	viewCache=view;
}

unsigned int FsIOSCreateDepthBuffer(int backingWidth,int backingHeight)
{
	GLuint depthRenderbuffer;
	
	glGenRenderbuffers(1,&depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER,depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return 0;
    }

	return depthRenderbuffer;
}

void FsIOSDeleteDepthBuffer(unsigned int depthBuffer)
{
	if (depthBuffer)
	{
		GLuint bufferIdArray[1]={(GLuint)depthBuffer};
		glDeleteRenderbuffers(1,bufferIdArray);
	}
}



void FsChangeToProgramDirC(void)
{
	NSString *path;
	path=[[NSBundle mainBundle] bundlePath];
	printf("BundlePath:%s\n",[path UTF8String]);
	
	[[NSFileManager defaultManager] changeCurrentDirectoryPath:path];
}

void FsSleepC(int ms)
{
	if(ms>0)
	{
		double sec;
		sec=(double)ms/1000.0;
		[NSThread sleepForTimeInterval:sec];
	}
}


int FsPassedTimeC(void)
{
	int ms;
	
#if !__has_feature(objc_arc)
 	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
#endif
	
	static NSTimeInterval last=0.0;
	NSTimeInterval now;
	
	now=[[NSDate date] timeIntervalSince1970];
	
	NSTimeInterval passed;
	passed=now-last;
	ms=(int)(1000.0*passed);
	
	if(ms<0)
	{
		ms=1;
	}
	last=now;
	
#if !__has_feature(objc_arc)
	[pool release];	
#endif
	
	return ms;
}

void FsSwapBuffersC(void)
{
	if(NULL!=viewCache)
	{
		[[viewCache view] presentFramebuffer];
	}
}

#ifndef FSSIMPLEWINDOW_IOS_H_IS_INCLUDED
#define FSSIMPLEWINDOW_IOS_H_IS_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

	/* For communication between XCode framework and FsSimpleWindow framework >> */
	void FsIOSReportWindowSize(int wid,int hei);
	unsigned int FsIOSCreateDepthBuffer(int backingWidth,int backingHeight);
	void FsIOSDeleteDepthBuffer(unsigned int depthBuffer);
	
	void FsIOSReportFirstTouchBegan(int x,int y);
	void FsIOSReportTouchMoved(int x,int y);
	void FsIOSReportLastTouchEnded(int x,int y);
	
	void FsIOSClearTouchCache(void);
	void FsIOSAddTouchCache(int x,int y);

	void FsIOSIntervalCallBack(void);
	/* For communication between XCode framework and FsSimpleWindow framework << */
	
	
	void FsChangeToProgramDirC(void);
	void FsSleepC(int ms);
	int FsPassedTimeC(void);
	void FsSwapBuffersC(void);
	const char *FsKeyCodeToString(int keyCode);
#ifdef __cplusplus
}
#endif


#ifdef __OBJC__
void FsIOSReportUIViewPointer(UIView *view);
#endif


#endif



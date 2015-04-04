#ifndef FSWINDOWINTERNAL_IS_INCLUDED
#define FSWINDOWINTERNAL_IS_INCLUDED
/* { */

class FsSimpleWindowInternal
{
public:
	HWND hWnd;
	HGLRC hRC;
	HPALETTE hPlt;
	HDC hDC;

	FsSimpleWindowInternal();
};

/* } */
#endif

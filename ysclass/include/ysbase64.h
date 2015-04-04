#ifndef YSBASE___IS_INCLUDED
#define YSBASE___IS_INCLUDED
/* { */

#include "ysarray.h"
#include "ysstring.h"

YSRESULT YsEncodeBase64(YsArray <YsString> &base64,YSSIZE_T length,const unsigned char byteData[]);

template <const int N>
YSRESULT YsEncodeBase64(YsArray <YsString> &base64,const YsArray <unsigned char,N> &byteData)
{
	return YsEncodeBase64(base64,byteData.GetN(),byteData);
}


class YsBase64Decoder
{
private:
	enum
	{
		BUFSIZE=4096,
		BITPERBYTE=8
	};

	int nBufUsed,nBitUsed;
	unsigned char bitBuf[BUFSIZE];
	unsigned char decodeTable[256];

public:
	YsBase64Decoder();
	void CleanUp(void);
private:
	void WriteBit(YsArray <unsigned char> &outBuf,unsigned int bit,int bitLength);
public:
	void DecodeString(YsArray <unsigned char> &outBuf,const char str[]);
	void DecodeString(YsArray <unsigned char> &outBuf,const YsString &str);
	void Flush(YsArray <unsigned char> &outBuf);
};


/* } */
#endif

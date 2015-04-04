#include "ysarray.h"
#include "ysstring.h"
#include "ysbase64.h"


/*
   012345|670123|456701|234567|012345|67
   aaaaaa bbbbbb cccccc dddddd eeeeee ff

or

   765432|107654|321076|543210|765432|10
   aaaaaa bbbbbb cccccc dddddd eeeeee ff 

or

   765432|107654|321076|543210|765432|10----
   aaaaaa bbbbbb cccccc dddddd eeeeee ffffff 
?
   */
YSRESULT YsEncodeBase64(YsArray <YsString> &base64,YSSIZE_T length,const unsigned char byteData[])
{
	const char *codeSrc="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	base64.CleanUp();

	YsString coding;
	unsigned int nBitFilled=0;
	unsigned int byteOut=0;

	const YSSIZE_T charPerLine=76;

	for(YSSIZE_T pos=0; pos<length; ++pos)
	{
		unsigned int nextByte=byteData[pos];
		unsigned int nBitRemain=8;

		while(0<nBitRemain)
		{
			unsigned int nBitTake=YsSmaller(nBitRemain,6-nBitFilled);
			unsigned int nBitShift=8-nBitTake;

			byteOut<<=nBitTake;
			byteOut|=(nextByte>>nBitShift);
			nextByte<<=nBitTake;
			nextByte&=255;

			nBitFilled+=nBitTake;
			if(6<=nBitFilled)
			{
				coding.Append(codeSrc[byteOut]);
				byteOut=0;
				nBitFilled=0;
				if(charPerLine<=coding.Strlen())
				{
					base64.Append(coding);
					coding.Set("");
				}
			}

			nBitRemain-=nBitTake;
		}
	}

printf("%d\n",nBitFilled);
	if(0<nBitFilled)
	{
		byteOut<<=(6-nBitFilled);
		coding.Append(codeSrc[byteOut]);
	}
	if(0<coding.Strlen())
	{
		base64.Append(coding);
	}

	return YSOK;
}





YsBase64Decoder::YsBase64Decoder()
{
	char *decodeSrc="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	for(int i=0; i<256; i++)
	{
		decodeTable[i]=255;
	}
	for(int i=0; i<64; i++)
	{
		decodeTable[decodeSrc[i]]=(unsigned char)i;
	}

	CleanUp();
}

void YsBase64Decoder::CleanUp(void)
{
	nBufUsed=0;
	nBitUsed=0;
	bitBuf[0]=0;
}

void YsBase64Decoder::WriteBit(YsArray <unsigned char> &outBuf,unsigned int bit,int bitLength)
{
	int restbit;

	restbit=BITPERBYTE-nBitUsed;
	if(bitLength<=restbit)
	{
		int topBit,bitShift;
		topBit=BITPERBYTE-1-nBitUsed;
		bitShift=topBit-(bitLength-1);
		bitBuf[nBufUsed]|=(bit<<bitShift);

		nBitUsed+=bitLength;
		if(nBitUsed>=BITPERBYTE)
		{
			nBitUsed=0;
			nBufUsed++;
			if(nBufUsed>=BUFSIZE)
			{
				outBuf.Append(BUFSIZE,bitBuf);
				nBufUsed=0;
			}
			bitBuf[nBufUsed]=0;
		}
	}
	else
	{
		WriteBit(outBuf,bit>>(bitLength-restbit),restbit);
		WriteBit(outBuf,bit,bitLength-restbit);
	}
}

void YsBase64Decoder::Flush(YsArray <unsigned char> &outBuf)
{
	int len=nBufUsed; /* Actually last hanging bits mean nothing.  Just can discard. +(nBitUsed+BITPERBYTE-1)/BITPERBYTE; */
	outBuf.Append(len,bitBuf);
}

void YsBase64Decoder::DecodeString(YsArray <unsigned char> &outBuf,const char str[])
{
	for(int i=0; 0!=str[i]; i++)
	{
		unsigned char decode=decodeTable[((unsigned char *)str)[i]];
		if(decode<64)
		{
			WriteBit(outBuf,decode,6);
		}
	}
}

void YsBase64Decoder::DecodeString(YsArray <unsigned char> &outBuf,const YsString &str)
{
	DecodeString(outBuf,str.Txt());
}


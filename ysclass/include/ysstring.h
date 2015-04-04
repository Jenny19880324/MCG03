/* ////////////////////////////////////////////////////////////

YS Class Library
Copyright (c) 2014 Soji Yamakawa.  All rights reserved.

YS Class Library is an open-source class-library project since 1999.
It has been used and battle-tested in commercial, research, and free programs.
Please visit the following URL for more details.

http://www.ysflight.com

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation 
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

File Name: ysstring.h
//////////////////////////////////////////////////////////// */

#ifndef YSSTRING_IS_INCLUDED
#define YSSTRING_IS_INCLUDED
/* { */

/*! \file */

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "ysdef.h"
#include "ysarray.h"

/*! Moves *ptr to the first character that is not space or tab.
    \param ptr [In/Out] String */
void YsStringHead(char **ptr);

/*! Deletes spaces, tabs, and new-line characters at the end of the string. 
    \param str [In/Out] String
    */
void YsStringTail(char *str);

/*! Turns small letters into capital letters. */
void YsCapitalize(char str[]);

/*! Turns capital letters into small letters. */
void YsUncapitalize(char str[]);

/*! Makes a string of double-precision floating point.  It drops all zeros after non-zero digit below decimal. */
const char *YsOptimizeDoubleString(char buf[],const double &value);

// For parsing CSV, blank="", comma=","
void YsArguments2(YsArray <class YsString,16> &args,const char str[],const char *blank=" \t",const char *comma=",");
void YsArguments2(YsArray <class YsString,16> &args,const char str[],YSSIZE_T l,const char *blank=" \t",const char *comma=",");

// Reference: http://en.wikipedia.org/wiki/UTF-8
/*! This function encodes UTF8 string from a wide-character (UNICODE) string.
    Wide-character type does not have to be wchar_t.  It can be any integral-type array
    that represents a sequence of Unicode characters followed by zero (terminator).

	\param lOut [In]  Length of the output buffer
	\param out  [Out] Output buffer
	\param in   [In]  Input wide-character string.
    */
template <class WCHAR_TYPE>
size_t YsWCharToUTF8(size_t lOut,char out[],WCHAR_TYPE in[])
{
	unsigned int iPtr=0,oPtr=0;
	unsigned char *uOut=(unsigned char *)out;

	while(oPtr<lOut-1 && 0!=in[iPtr])
	{
		if(in[iPtr]<=0x7f)
		{
			uOut[oPtr]=(unsigned char)in[iPtr];
			oPtr++;
		}
		else if(in[iPtr]<=0x7ff)
		{
			if(lOut-2<=oPtr)
			{
				// Buffer overrun.  Cannot put zero at the end.
				break;
			}
			unsigned int low6=in[iPtr]&0x3f;
			unsigned int high5=(in[iPtr]>>6)&0x1f;

			uOut[oPtr++]=(unsigned char)(0xc0|high5);
			uOut[oPtr++]=(unsigned char)(0x80|low6);
		}
		else if(in[iPtr]<=0xffff)
		{
			if(lOut-3<=oPtr)
			{
				// Buffer overrun.  Cannot put zero at the end.
				break;
			}

			unsigned int low6=in[iPtr]&0x3f;
			unsigned int mid6=(in[iPtr]>>6)&0x3f;
			unsigned int high4=(in[iPtr]>>12)&0x0f;

			uOut[oPtr++]=(unsigned char)(0xe0|high4);
			uOut[oPtr++]=(unsigned char)(0x80|mid6);
			uOut[oPtr++]=(unsigned char)(0x80|low6);
		}
		else // The following block will never be used unless wchar_t is greater than 16-bit.
		{
			unsigned long inChar=in[iPtr];
			unsigned int bit00to05=inChar&0x3f;
			unsigned int bit06to11=(inChar>>6)&0x3f;
			unsigned int bit12to17=(inChar>>12)&0x3f;
			unsigned int bit18to23=(inChar>>18)&0x3f;
			unsigned int bit24to29=(inChar>>24)&0x3f;
			unsigned int bit30=(inChar>>30)&0x01;

			if(in[iPtr]<=0x1fffff)
			{
				if(lOut-4<=oPtr)
				{
					// Buffer overrun.  Cannot put zero at the end.
					break;
				}
				uOut[oPtr++]=(unsigned char)(0xf0|(bit18to23&0x07));
				uOut[oPtr++]=(unsigned char)(0x80|bit12to17);
				uOut[oPtr++]=(unsigned char)(0x80|bit06to11);
				uOut[oPtr++]=(unsigned char)(0x80|bit00to05);
			}
			else if(in[iPtr]<=0x3ffffff)
			{
				if(lOut-5<=oPtr)
				{
					// Buffer overrun.  Cannot put zero at the end.
					break;
				}
				uOut[oPtr++]=(unsigned char)(0xf8|(bit24to29&0x03));
				uOut[oPtr++]=(unsigned char)(0x80|bit18to23);
				uOut[oPtr++]=(unsigned char)(0x80|bit12to17);
				uOut[oPtr++]=(unsigned char)(0x80|bit06to11);
				uOut[oPtr++]=(unsigned char)(0x80|bit00to05);
			}
			else if(in[iPtr]<=0x7fffffff)
			{
				if(lOut-6<=oPtr)
				{
					// Buffer overrun.  Cannot put zero at the end.
					break;
				}
				uOut[oPtr++]=(unsigned char)(0xfc|bit30);
				uOut[oPtr++]=(unsigned char)(0x80|bit24to29);
				uOut[oPtr++]=(unsigned char)(0x80|bit18to23);
				uOut[oPtr++]=(unsigned char)(0x80|bit12to17);
				uOut[oPtr++]=(unsigned char)(0x80|bit06to11);
				uOut[oPtr++]=(unsigned char)(0x80|bit00to05);
			}
		}
		iPtr++;
	}

	if(oPtr<lOut)
	{
		out[oPtr]=0;
		oPtr++;
	}

	return oPtr;
}

/*! This function decodes UTF8-character string into wide-character (UNICODE) string.
    Wide-character string does not have to be of wchar_t.  It can be any integral-type array.
    \tparam WCHAR_TYPE Type of the wide character.  It typically is wchar_t.
    \param lOut [In]  Length of the output buffer
    \param out  [Out] Output buffer
    \param in   [In]  Input UTF8 string
    */
template <class WCHAR_TYPE>
size_t YsUTF8ToWChar(size_t lOut,WCHAR_TYPE out[],const char in[])
{
	unsigned int iPtr=0,oPtr=0;
	unsigned char *uIn=(unsigned char *)in;

	while(oPtr<lOut-1 && 0!=uIn[iPtr])
	{
		if(0==(uIn[iPtr]&0x80))
		{
			out[oPtr]=uIn[iPtr];
			iPtr++;
		}
		else if(0xc0==(uIn[iPtr]&0xe0))
		{
			if(0==uIn[iPtr+1])
			{
				// Pre-mature termination.
				break;
			}
			unsigned int high5=uIn[iPtr]&0x1f;
			unsigned int low6=uIn[iPtr+1]&0x3f;
			out[oPtr]=(WCHAR_TYPE)((high5<<6)|low6);
			iPtr+=2;
		}
		else if(0xe0==(uIn[iPtr]&0xf0))
		{
			if(0==uIn[iPtr+1] || 0==uIn[iPtr+2])
			{
				// Pre-mature termination.
				break;
			}
			unsigned int high4=uIn[iPtr]&0x0f;
			unsigned int mid6=uIn[iPtr+1]&0x3f;
			unsigned int low6=uIn[iPtr+2]&0x3f;
			out[oPtr]=(WCHAR_TYPE)((high4<<12)|(mid6<<6)|low6);
			iPtr+=3;
		}
		else if(0xf0==(uIn[iPtr]&0xf8))
		{
			if(0==uIn[iPtr+1] || 0==uIn[iPtr+2] || 0==uIn[iPtr+3])
			{
				// Pre-mature termination.
				break;
			}
			unsigned int bit18to20=uIn[iPtr]&0x07;
			unsigned int bit12to17=uIn[iPtr+1]&0x3f;
			unsigned int bit06to11=uIn[iPtr+2]&0x3f;
			unsigned int bit00to05=uIn[iPtr+3]&0x3f;
			out[oPtr]=(WCHAR_TYPE)((bit18to20<<18)|(bit12to17<<12)|(bit06to11<<6)|bit00to05);
			iPtr+=4;
		}
		else if(0xf8==(uIn[iPtr]&0xfc))
		{
			if(0==uIn[iPtr+1] || 0==uIn[iPtr+2] || 0==uIn[iPtr+3] || 0==uIn[iPtr+4])
			{
				// Pre-mature termination.
				break;
			}
			unsigned int bit24to25=uIn[iPtr]&0x03;
			unsigned int bit18to23=uIn[iPtr+1]&0x3f;
			unsigned int bit12to17=uIn[iPtr+2]&0x3f;
			unsigned int bit06to11=uIn[iPtr+3]&0x3f;
			unsigned int bit00to05=uIn[iPtr+4]&0x3f;
			out[oPtr]=(WCHAR_TYPE)((bit24to25<<24)|(bit18to23<<18)|(bit12to17<<12)|(bit06to11<<6)|bit00to05);
			iPtr+=5;
		}
		else if(0xfc==(uIn[iPtr]&0xfe))
		{
			if(0==uIn[iPtr+1] || 0==uIn[iPtr+2] || 0==uIn[iPtr+3] || 0==uIn[iPtr+4] || 0==uIn[iPtr+5])
			{
				// Pre-mature termination.
				break;
			}
			unsigned int bit30=uIn[iPtr]&0x01;
			unsigned int bit24to29=uIn[iPtr+1]&0x3f;
			unsigned int bit18to23=uIn[iPtr+2]&0x3f;
			unsigned int bit12to17=uIn[iPtr+3]&0x3f;
			unsigned int bit06to11=uIn[iPtr+4]&0x3f;
			unsigned int bit00to05=uIn[iPtr+5]&0x3f;
			out[oPtr]=(WCHAR_TYPE)((bit30<<30)|(bit24to29<<24)|(bit18to23<<18)|(bit12to17<<12)|(bit06to11<<6)|bit00to05);
			iPtr+=6;
		}
		else
		{
			// Invalid utf8 string
			break;
		}
		oPtr++;
	}

	if(oPtr<lOut)
	{
		out[oPtr]=0;
		oPtr++;
	}

	return oPtr;
}


////////////////////////////////////////////////////////////

/*! Template class for a text string.  It is in particular used as a base class of
    \sa YsString and \sa YsWString

	\tparam CHARTYPE An integral type that represents a character.
    */
template <class CHARTYPE>
class YsGenericString :  protected YsArray <CHARTYPE,16>
{
protected:
	// >> Workaround for g++ v4.3.2.  These lines are not supposed to be necessary.
	using YsArray <CHARTYPE,16>::vv;
	using YsArray <CHARTYPE,16>::GetN;
	using YsArray <CHARTYPE,16>::Resize;
	using YsArray <CHARTYPE,16>::MakeUnitLength;
	// << Workaround for g++ v4.3.2.  These lines are not supposed to be necessary.

protected:
	enum
	{
		CRCODE=0x0d,
		LFCODE=0x0a
	};

public:

	/*! Measures the number of characters of the string.  It counts the index until it finds 0==str[index]. */
	static YSSIZE_T Strlen(const CHARTYPE str[]);

	/*! Returns 0 if str1 and str2 are equal, positive number if str1 is lexically later than str2, or negative number
	   if str1 is lexically earlier than str2. */
	static int Strcmp(const CHARTYPE str1[],const CHARTYPE str2[]);

	/*! Returns 0 if str1 and str2 are equal up to the given length or the length of the longer string of str1 and str2, 
	    positive number if str1 is lexically later than str2, or negative number if str1 is lexically earlier than str2. */
	static int Strncmp(const CHARTYPE str1[],const CHARTYPE str2[],YSSIZE_T n);

	/*! Copies the content of src to dst.  The caller must make sure dst has the sufficient length. */
	static CHARTYPE *Strcpy(CHARTYPE dst[],const CHARTYPE src[]);

	/*! Adds str2 to the end of str1 */
	static int Strcat(CHARTYPE str1[],const CHARTYPE str2[]);

	/*! Returns YSTRUE if c is printable, namely if c is greater than 0x20 (space).  TAB is not considered printable. */
	static YSBOOL Isprint(CHARTYPE c);

	/*! Returns YSTRUE if str includes c. */
	static YSBOOL CharIsOneOf(CHARTYPE c,const CHARTYPE str[]);

	/*! Default constructor.  Creates an empty string. */
	inline YsGenericString();

	/*! Default constructor.  Creates a copy of the string. 
	    \param from [In] The source string
	    */
	inline YsGenericString(const CHARTYPE from[]);

	/*! Default constructor.  Creates a copy of the string. 
	    \param from [In] The source string
	    */
	inline YsGenericString(const YsGenericString <CHARTYPE> &from);

	/*! Returns the number of characters (excluding the zero terminator) of the array.
	    This function is faster than strlen in C-standard library because this YsGenericString
	    class stores the number of characters.
	    */
	inline YSSIZE_T Strlen(void) const;

	/*! Makes a copy of a string.
	    \param from [In] The source string
	    */
	inline YSRESULT Set(const CHARTYPE from[]);

	/*! Makes a copy of a string up to n characters.  If the length of the incoming
	    string is shorter than n, the string is copied up to its length.
	    \param n    [In] Maximum number of characters to be copied.
	    \param from [In] The source string
	    */
	inline YSRESULT Set(YSSIZE_T n,const CHARTYPE from[]);

	/*! Replaces one letter of the string.  This function substitutes c to the nth character (n is zero based).
	    If c is zero, it changes the length of the string. 
	    \param n [In] Location in the string to be replaced.  Must be 0<=n && n<Strlen()
	    \param c [In] New character 
	    */
	inline YSRESULT Set(YSSIZE_T n,CHARTYPE c);

	/*! Gives constant access to the characters in the string. */
	inline const CHARTYPE &operator[](YSSIZE_T idx) const;

	/*! Gives constant access to the characters in the string. */
	inline const CHARTYPE &operator[](YSSIZE_T idx);

	/*! Cast operator. */
	inline operator const CHARTYPE *() const;

	/*! Cast operator. */
	inline operator const CHARTYPE *();

	/*! Returns the constant pointer to the array that stores the string. */
	inline const CHARTYPE *Ptr(void ) const;

	/*! Returns the constant pointer to the array that stores the string. */
	inline const CHARTYPE *Txt(void ) const; // 2009/03/27 To make it look like YsVec3/YsVec2

	/*! Returns the constant pointer to the array that stores the string. */
	inline const CHARTYPE *GetArray(void) const;

	/*! Returns the last character of the string. */
	inline CHARTYPE LastChar(void) const;

	/*! Appends the given string. 
	    \param str [In] incoming string.
	    */
	inline YSRESULT Append(const CHARTYPE str[]);

	/*! Appends a character.
	    \param c [In] The character to be added.
	    */
	inline YSRESULT Append(CHARTYPE c);

	/*! Appends the given string up to n characters.
	    */
	inline YSRESULT Append(YSSIZE_T n,const CHARTYPE str[]);

	/*! Shortens the string by one character by deleting the last character. */
	inline YSRESULT BackSpace(void);

	/*! Shortens the string by one character by deleting (*this)[n]. */
	inline YSRESULT Delete(int n);               // str[pos] is deleted.

	/*! Delets l characters starting from (*this)[n]. */
	inline YSRESULT DeleteRange(YSSIZE_T n,YSSIZE_T l);

	/*! Inserts a character in the string.  Characters after (*this)[n] will be shifted
	    to the right by one character, and (*this)[n] will become c. */
	inline YSRESULT Insert(YSSIZE_T n,const CHARTYPE c);  // str[pos] will be c.  Later string will be shifted.

	/*! Inserts a given string in this string.  Characters after (*this)[n] will be shifted
	    to the right by Strlen(str), and str[i] will be copied to (*this)[n+i]. */
	inline YSRESULT InsertString(YSSIZE_T n,const CHARTYPE str[]);

	/*! Deletes spaces and tabs at the begining of the string. */
	inline void DeleteHeadSpace(void);

	/*! Deletes spaces, tabs, and new-line codes at the end of the string. */
	inline void DeleteTailSpace(void);

	/*! Turns small letters ('a' to 'z') into capital letters ('A' to 'Z').
	    It does not capitalize full-size alphabets defined in UNICODE. */
	inline void Capitalize(void);

	/*! Turns capital letters ('A' to 'Z') into capital letters ('a' to 'z').
	    It does not uncapitalize full-size alphabets defined in UNICODE. */
	inline void Uncapitalize(void);

	/*! Changes the length of the string.  It can only shrink the string, but cannot
	    extend the string. */
	inline void SetLength(YSSIZE_T n);

	/*! This function assumes that this string is a file name and replaces the file extension.
	    \param ext [In] New extention.  It can but does not have to include '.'.  (It can be ".ext" or "ext") */
	inline void ReplaceExtension(const CHARTYPE ext[]);

	/*! This function assumes that this string is a file name and removes the file extension.*/
	inline void RemoveExtension(void);

	/*! This function assumes that this string is a file name and extracts the file extension.
	    \param ext [Out] Extention of the file name.
	    '.' will be included in the returned extension.  The ext will be like ".cpp" */
	inline void GetExtension(YsGenericString <CHARTYPE> &ext) const;

	/*! Makes a full-path file name from the directory path and file name.
	    \param path [In] Directory path
	    \param name [In] File name */
	const CHARTYPE *MakeFullPathName(const CHARTYPE path[],const CHARTYPE name[]);

	/*! Decomposes the string into multiple words. 
	    \tparam STRTYPE Type of the class that stores decomposed words.  It needs to be YsGenericString or its sub-class.
	    \param args [Out] Decomposed words
	    \param blank [In] An string of CHARTYPE that contains blank (separator) characters
	    \param comma [In] An string of CHARTYPE that contains comma (separator) characters
	    */
	template <class STRTYPE,const int N>
	YSRESULT Arguments(YsArray <STRTYPE,N> &args,const CHARTYPE *blank,const CHARTYPE *comma) const;

	/*! This function assumes that this string is a file name with directory path, and separate it into
	    directory part and file-name part.
	    \param path [Out] Returns directory part
	    \param name [Out] Returns file-name part
	    */
	inline YSRESULT SeparatePathFile(YsGenericString <CHARTYPE> &path,YsGenericString <CHARTYPE> &name) const;

	/*! Makes a UNICODE string by decoding UTF-8 encoded char string.
	    \param utf8 [In] A UTF-8 encoded string 
	    */
	inline YSRESULT SetUTF8String(const char utf8[]);

	/*! Makes a UTF-8 encoded string.
	    \param utf8 [Out] Returns a UTF-8 encoded string 
	    */
	inline YSRESULT GetUTF8String(class YsString &utf8) const;

	/*! Reads a line from a file using fgets function, and decodes UNICODE string assuming that the 
	    incoming line is UTF-8 encoded. */
	inline const CHARTYPE *FgetsUTF8(FILE *fp);

	/*! Returns YSTRUE if this string includes a keyword.  If firstIndex is not NULL, first position (index to the first 
	    letter of the first occurence of the keyword) will be returned to *firstIndex.

	    If this string does not include the keyword and firstIndex is not NULL, *firstIndex will be -1.

	    This function is case sensitive.  */
	inline YSBOOL FindWord(YSSIZE_T *firstIndex,const CHARTYPE keyword[]) const;

	/*! Returns YSTRUE if all words in the searchText is included in this string. */
	inline YSBOOL MatchSearchKeyWord(const CHARTYPE searchText[]) const;

	/*! Returns YSTRUE if all words in the searchText is included in this string. */
	inline YSBOOL MatchSearchKeyWord(const YsGenericString <CHARTYPE> &searchText) const;
};

template <class CHARTYPE>
inline YSSIZE_T YsGenericString <CHARTYPE>::Strlen(const CHARTYPE str[])
{
	YSSIZE_T i;
	for(i=0; 0!=str[i]; i+=4)
	{
		if(0==str[i+1])  // I don't know this classis loop-expansion technique does any performance improvement though.
		{
			return i+1;
		}
		else if(0==str[i+2])
		{
			return i+2;
		}
		else if(0==str[i+3])
		{
			return i+3;
		}
	}
	return i;
}

template <class CHARTYPE>
inline int YsGenericString <CHARTYPE>::Strcmp(const CHARTYPE str1[],const CHARTYPE str2[])
{
	YSSIZE_T i;
	for(i=0; 0!=str1[i] && 0!=str2[i] && str1[i]==str2[i]; i++)
	{
	}
	return (int)str1[i]-(int)str2[i];
}

template <class CHARTYPE>
inline int YsGenericString <CHARTYPE>::Strncmp(const CHARTYPE str1[],const CHARTYPE str2[],YSSIZE_T n)
{
	YSSIZE_T i;
	for(i=0; i<n && 0!=str1[i] && 0!=str2[i] && str1[i]==str2[i]; i++)
	{
	}
	return (i==n ? 0 : (int)str1[i]-(int)str2[i]);
}

template <class CHARTYPE>
inline CHARTYPE *YsGenericString <CHARTYPE>::Strcpy(CHARTYPE dst[],const CHARTYPE src[])
{
	int i;
	for(i=0; 0!=src[i]; i++)
	{
		dst[i]=src[i];
	}
	dst[i]=0;
	return dst;
}

template <class CHARTYPE>
int YsGenericString <CHARTYPE>::Strcat(CHARTYPE str1[],const CHARTYPE str2[])
{
	int l=0;
	for(l=0; 0!=str1[l]; ++l)
	{
	}

	if(NULL!=str2)
	{
		for(int i=0; 0!=str2[i]; ++i)
		{
			str1[l]=str2[i];
			++l;
		}
	}

	str1[l]=0;
	return l;
}

template <class CHARTYPE>
inline YSBOOL YsGenericString <CHARTYPE>::Isprint(CHARTYPE c)
{
	if(0x20<=c)
	{
		return YSTRUE;
	}
	else
	{
		return YSFALSE;
	}
}

template <class CHARTYPE>
inline YSBOOL YsGenericString <CHARTYPE>::CharIsOneOf(CHARTYPE c,const CHARTYPE str[])
{
	int i;
	for(i=0; str[i]!=0; i++)
	{
		if(str[i]==c)
		{
			return YSTRUE;
		}
	}
	return YSFALSE;
}

template <class CHARTYPE>
inline YsGenericString <CHARTYPE>::YsGenericString()
{
	YsArray <CHARTYPE,16>::MakeUnitLength(0);
}

template <class CHARTYPE>
inline YsGenericString <CHARTYPE>::YsGenericString(const CHARTYPE from[])
{
	Set(from);
}

template <class CHARTYPE>
inline YsGenericString <CHARTYPE>::YsGenericString(const YsGenericString <CHARTYPE> &from)
{
	YsArray <CHARTYPE,16>::Set(from.Strlen()+1,from.GetArray());
}

template <class CHARTYPE>
inline YSSIZE_T YsGenericString <CHARTYPE>::Strlen(void) const
{
	return YsArray <CHARTYPE,16>::GetN()-1;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Set(const CHARTYPE str[])
{
	if(NULL!=str)
	{
		return YsArray <CHARTYPE,16>::Set(Strlen(str)+1,str);
	}
	else
	{
		CHARTYPE empty[1]={0};
		return YsArray <CHARTYPE,16>::Set(1,empty);
	}
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Set(YSSIZE_T n,const CHARTYPE str[])
{
	if(NULL==str)
	{
		return Set(NULL);
	}

	YSSIZE_T l;
	l=(YSSIZE_T)Strlen(str);
	if(l<n)
	{
		n=l;
	}

	if(YsArray <CHARTYPE,16>::Set(n+1,str)==YSOK)
	{
		YsArray <CHARTYPE,16>::vv[n]=0;
		return YSOK;
	}
	else
	{
		return YSERR;
	}
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Set(YSSIZE_T n,CHARTYPE c)
{
	YsArray <CHARTYPE,16>::vv[n]=c;
	if(0==c && n<GetN()-1)
	{
		Resize(n+1);
	}
	return YSOK;
}

template <class CHARTYPE>
inline const CHARTYPE &YsGenericString <CHARTYPE>::operator[](YSSIZE_T idx) const
{
	return vv[idx];
}

template <class CHARTYPE>
inline const CHARTYPE &YsGenericString <CHARTYPE>::operator[](YSSIZE_T idx)
{
	return vv[idx];
}

template <class CHARTYPE>
inline YsGenericString <CHARTYPE>::operator const CHARTYPE *() const
{
	return vv;
}

template <class CHARTYPE>
inline YsGenericString <CHARTYPE>::operator const CHARTYPE *()
{
	return vv;
}

template <class CHARTYPE>
inline const CHARTYPE *YsGenericString <CHARTYPE>::Ptr(void) const
{
	return vv;
}

template <class CHARTYPE>
inline const CHARTYPE *YsGenericString <CHARTYPE>::Txt(void) const
{
	return vv;
}

template <class CHARTYPE>
inline const CHARTYPE *YsGenericString <CHARTYPE>::GetArray(void) const
{
	return vv;
}

template <class CHARTYPE>
inline CHARTYPE YsGenericString <CHARTYPE>::LastChar(void) const
{
	if(Strlen()>0)
	{
		return vv[Strlen()-1];
	}
	return 0;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Append(const CHARTYPE str[])
{
	Resize(GetN()-1);
	return YsArray <CHARTYPE,16>::Append((int)Strlen(str)+1,str);
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Append(CHARTYPE c)
{
	size_t n;
	n=Strlen();
	Resize(n+2);
	vv[n]=c;
	vv[n+1]=0;
	return YSOK;
}

template <class CHARTYPE>
YSRESULT YsGenericString <CHARTYPE>::Append(YSSIZE_T n,const CHARTYPE str[])
{
	if(NULL!=str)
	{
		YSSIZE_T  curLen=Strlen();
		YSSIZE_T  toAdd=YsSmaller <YSSIZE_T> (n,strlen(str));
		Resize(curLen+toAdd+1);
		for(YSSIZE_T i=0; i<toAdd; i++)
		{
			vv[curLen+i]=str[i];
		}
		vv[curLen+toAdd]=0;
	}
	return YSOK;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::BackSpace(void)
{
	if(Strlen()>0)
	{
		YSSIZE_T l=Strlen();
		vv[l-1]=0;
		Resize(l);
		return YSOK;
	}
	return YSERR;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Delete(int pos)               // str[pos] is deleted.
{
	if(0<=pos && pos<Strlen())
	{
		YSSIZE_T l=Strlen();

		for(YSSIZE_T i=pos; i<l; i++)
		{
			vv[i]=vv[i+1];
		}

		Resize(l);
		return YSOK;
	}
	return YSERR;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::DeleteRange(YSSIZE_T pos,YSSIZE_T lng)
{
	if(0<=pos && pos<Strlen() && 0<lng)
	{
		if(Strlen()<=pos+lng)
		{
			SetLength(pos);
		}
		else
		{
			for(YSSIZE_T i=0; i<lng; i++)
			{
				vv[pos+i]=vv[pos+i+lng];
			}
			SetLength(Strlen()-lng);
		}
		return YSOK;
	}
	return YSERR;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::Insert(YSSIZE_T pos,const CHARTYPE c)  // str[pos] will be c.  Later string will be shifted.
{
	if(Strlen()<=pos)
	{
		Append(c);
		return YSOK;
	}
	else if(0<=pos)
	{
		const int n=Strlen();

		Resize(n+2);

		for(YSSIZE_T i=n+1; i>pos; i--)
		{
			vv[i]=vv[i-1];
		}
		vv[pos]=c;

		return YSOK;
	}
	return YSERR;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::InsertString(YSSIZE_T pos,const CHARTYPE str[])
{
	if(0==Strlen())
	{
		return Set(str);
	}
	else
	{
		const YSSIZE_T lng=Strlen(str);
		if(0<lng)
		{
			if(0>pos)
			{
				pos=0;
			}
			if(Strlen()<pos)
			{
				pos=Strlen();
			}

			SetLength(Strlen()+lng);

			for(YSSIZE_T i=Strlen()-1; i>=(pos+lng); i--)
			{
				vv[i]=vv[i-lng];
			}

			for(YSSIZE_T i=0; i<lng; i++)
			{
				vv[pos+i]=str[i];
			}
		}
	}
	return YSOK;
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::DeleteHeadSpace(void)
{
	int i,nDel;
	nDel=0;
	for(i=0; vv[i]!=0; i++)
	{
		nDel=i;
		if(vv[i]!=' ' && vv[i]!='\t' && YSTRUE==Isprint(vv[i]))
		{
			break;
		}
	}

	if(nDel>0)
	{
		for(i=nDel; vv[i]!=0; i++)
		{
			vv[i-nDel]=vv[i];
		}
		vv[i-nDel]=0;
		Resize(GetN()-nDel);
	}
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::DeleteTailSpace(void)
{
	unsigned int chr;
	YSSIZE_T i,cut,len;

	len=Strlen();
	cut=0;
	for(i=0; i<len; i++)
	{
		chr=(unsigned int)(vv[i]);
		if(chr!=' ' && chr!='\t' && YSTRUE==Isprint(chr))
		{
			cut=i+1;
		}
	}

	if(cut<len)
	{
		vv[cut]=0;
		Resize(cut+1);
	}
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::Capitalize(void)
{
	int i;
	for(i=0; vv[i]!=0; i++)
	{
		if('a'<=vv[i] && vv[i]<='z')
		{
			vv[i]=vv[i]+'A'-'a';
		}
	}
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::Uncapitalize(void)
{
	int i;
	for(i=0; vv[i]!=0; i++)
	{
		if('A'<=vv[i] && vv[i]<='Z')
		{
			vv[i]=vv[i]+'a'-'A';
		}
	}
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::SetLength(YSSIZE_T n)
{
	if(0<=n)
	{
		YSSIZE_T k=Strlen();

		Resize(n+1);
		for(YSSIZE_T i=k; i<n; i++)
		{
			vv[i]=' ';
		}
		vv[n]=0;
	}
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::ReplaceExtension(const CHARTYPE ext[])
{
	YSSIZE_T i,insPoint;
	YSSIZE_T extLen;
	const CHARTYPE *extWithoutDot;

	if(ext==NULL || ext[0]==0)
	{
		RemoveExtension();
		return;
	}

	if(ext[0]=='.')
	{
		extLen=(int)Strlen(ext)-1;
		extWithoutDot=ext+1;
	}
	else
	{
		extLen=(int)Strlen(ext);
		extWithoutDot=ext;
	}

	YSSIZE_T l=Strlen();
	for(i=l-1; i>=0; i--)
	{
		if(vv[i]=='.' || vv[i]=='/' || vv[i]=='\\')
		{
			break;
		}
	}

	if(i<0 || vv[i]!='.') // Means there's no dot in the filename.
	{
		insPoint=Strlen();
		YsArray <CHARTYPE,16>::Resize(Strlen()+extLen+2);
	}
	else
	{
		insPoint=i;
		YsArray <CHARTYPE,16>::Resize(insPoint+extLen+2);
	}

	vv[insPoint]='.';
	Strcpy(vv+insPoint+1,extWithoutDot);
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::RemoveExtension(void)
{
	for(YSSIZE_T i=Strlen()-1; i>=0; i--)
	{
		if(vv[i]=='.')
		{
			vv[i]=0;
			YsArray <CHARTYPE,16>::Resize(i+1);
			break;
		}
		else if(vv[i]=='/' || vv[i]=='\\' || vv[i]==':')
		{
			break;
		}
	}
}

template <class CHARTYPE>
inline void YsGenericString <CHARTYPE>::GetExtension(YsGenericString <CHARTYPE> &ext) const
{
	for(YSSIZE_T i=Strlen()-1; i>=0; i--)
	{
		if('.'==vv[i])
		{
			ext.Set(vv+i);
			return;
		}
	}
	ext.SetLength(0);
}

template <class CHARTYPE>
inline const CHARTYPE *YsGenericString <CHARTYPE>::MakeFullPathName(const CHARTYPE path[],const CHARTYPE name[])
{
	Set(path);
	if(LastChar()=='/' || LastChar()=='\\')
	{
		Append(name);
	}
	else
	{
		const CHARTYPE add[2]={'/',0};
		Append(add);
		Append(name);
	}
	return Txt();
}

template <class CHARTYPE>
template <class STRTYPE,const int N>
YSRESULT YsGenericString <CHARTYPE>::Arguments(YsArray <STRTYPE,N> &args,const CHARTYPE *blank,const CHARTYPE *comma) const
{
	args.Set(0,NULL);

	const YSSIZE_T l=Strlen();

	YSSIZE_T i=0;
	while(YSTRUE==CharIsOneOf(vv[i],blank))
	{
		i++;
	}

	YSSIZE_T head=i;
	while(i<l)
	{
		if(vv[head]=='\"')
		{
			head++;
			i++;
			while(i<l && vv[i]!='\"')
			{
				if(YSTRUE!=Isprint(vv[i]))
				{
					break;
				}
				i++;
			}

			args.Increment();
			args.GetEnd().Set(i-head,vv+head);

			if(vv[i]=='\"')
			{
				i++;
			}
		}
		else
		{
			while(i<l && (CharIsOneOf(vv[i],blank)!=YSTRUE && CharIsOneOf(vv[i],comma)!=YSTRUE))
			{
				if(YSTRUE!=Isprint(vv[i]))
				{
					break;
				}
				i++;
			}

			if(head<i)  // <- This condition is added on 2005/03/03
			{
				args.Increment();
				args.GetEnd().Set(i-head,vv+head);
			}
			else if(head==i && CharIsOneOf(vv[i],comma)==YSTRUE) // < This condition is added (I thought there was, did I accidentally delete?) on 2012/01/26
			{
				const CHARTYPE empty[1]={0};
				args.Increment();
				args.GetEnd().Set(empty);
			}
		}

		while(i<l && YSTRUE!=Isprint(vv[i]))
		{
			i++;
		}
		while(i<l && CharIsOneOf(vv[i],blank)==YSTRUE)  // Skip blank separator
		{
			i++;
		}
		if(CharIsOneOf(vv[i],comma)==YSTRUE) // Skip one comma separator
		{
			i++;

			while(i<l && CharIsOneOf(vv[i],blank)==YSTRUE)  // Skip blank separator after a comma separator
			{
				i++;
			}

			if(i==l)
			{
				args.Increment();
				args.GetEnd().MakeUnitLength(0);
			}
		}
		head=i;
	}

	return YSOK;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::SeparatePathFile(YsGenericString <CHARTYPE> &pth,YsGenericString <CHARTYPE> &fil) const
{
	int cutPtr,seekPtr;

	cutPtr=0;

	/* Skip Drive Name */
	if(vv[0]!=0 && vv[1]==':')
	{
		cutPtr+=2;
	}

	/* scan until nul Stopper */
	for(seekPtr=cutPtr; vv[seekPtr]!=0; seekPtr++)
	{
		if(vv[seekPtr]=='\\' || vv[seekPtr]=='/' || vv[seekPtr]==':')
		{
			cutPtr=seekPtr+1;
		}
	}

	/* cutPtr points *tmp  or after ':' or after last '\\' */
	fil.Set(vv+cutPtr);

	pth.Resize(cutPtr+1);
	for(YSSIZE_T i=0; i<cutPtr; i++)
	{
		pth.vv[i]=vv[i];
	}
	pth.vv[cutPtr]=0;

	return YSOK;
}

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::SetUTF8String(const char in[])
{
	if(NULL!=in)
	{
		YSSIZE_T l=strlen(in);
		Resize(l+1);

		YSSIZE_T nWChar=YsUTF8ToWChar <CHARTYPE> (l+1,vv,in);  // nWChar includes zero terminator.
		Resize(nWChar);
	}
	else
	{
		CHARTYPE empty[1]={0};
		Set(empty);
	}

	return YSOK;
}

template <class CHARTYPE>
inline const CHARTYPE *YsGenericString <CHARTYPE>::FgetsUTF8(FILE *fp)
{
	const YSSIZE_T bufSize=256;

	char buf[bufSize];
	if(NULL==fgets(buf,bufSize-1,fp))
	{
		return NULL;
	}

	size_t l=strlen(buf);
	if('\n'==buf[l-1])
	{
		buf[l-1]=0;
		SetUTF8String(buf);
		return vv;
	}

	YsArray <char,256> utf8;
	utf8.Set(l,buf);
	while(fgets(buf,bufSize-1,fp)!=NULL)
	{
		size_t l=strlen(buf);
		utf8.Append(l,buf);
		if(buf[l-1]=='\n')
		{
			utf8[utf8.GetN()-1]=0;
			SetUTF8String(utf8);
			return vv;
		}
	}

	if(utf8.GetN()==0)
	{
		MakeUnitLength(0);
		return NULL;
	}

	utf8.Append(0);
	SetUTF8String(utf8);
	return vv;
}

template <class CHARTYPE>
inline YSBOOL YsGenericString <CHARTYPE>::FindWord(YSSIZE_T *firstIndex,const CHARTYPE keyword[]) const
{
	YSSIZE_T keywordL=Strlen(keyword);
	for(YSSIZE_T i=0; i<=Strlen()-keywordL; i++)
	{
		if(vv[i]==keyword[0])
		{
			for(YSSIZE_T j=0; 0!=keyword[j]; j++)
			{
				if(vv[i+j]!=keyword[j])
				{
					goto NEXTI;
				}
			}
			if(NULL!=firstIndex)
			{
				*firstIndex=i;
			}
			return YSTRUE;
		}
	NEXTI:
		;
	}

	if(NULL!=firstIndex)
	{
		*firstIndex=-1;
	}
	return YSFALSE;
}

template <class CHARTYPE>
inline YSBOOL YsGenericString <CHARTYPE>::MatchSearchKeyWord(const CHARTYPE searchText[]) const
{
	YsGenericString <CHARTYPE> txt(searchText);
	return MatchSearchKeyWord(txt);
}

template <class CHARTYPE>
inline YSBOOL YsGenericString <CHARTYPE>::MatchSearchKeyWord(const YsGenericString <CHARTYPE> &searchText) const
{
	YsArray <YsGenericString <CHARTYPE> > args;
	const CHARTYPE blank[3]={' ','\t',0};
	const CHARTYPE comma[3]={',',';',0};

	searchText.Arguments(args,blank,comma);

	for(YSSIZE_T argIdx=0; argIdx<args.GetN(); ++argIdx)
	{
		if(YSTRUE!=FindWord(NULL,args[argIdx]))
		{
			return YSFALSE;
		}
	}
	return YSTRUE;
}




/*! Class for storing and manipulating C string. */
class YsString : public YsGenericString <char>
{
public:
	/*! Default constructor.  Constructs an empty string. */
	inline YsString();

	/*! Creates a copy of the incoming string. 
	    \param from [In] Incoming (source) string
	    */
	inline YsString(const char from[]);

	/*! Creates a copy of the incoming string. 
	    \param from [In] Incoming (source) string
	    */
	inline YsString(const YsString &from);

	/*! Copy operator. */
	inline YsString &operator=(const YsString &from);

	/*! Move constructor. */
	inline YsString(YsString &&from);

	/*! Move operator. */
	inline YsString &operator=(YsString &&str);

	/*! Moves "incoming" into this string. */
	inline YsString &MoveFrom(YsString &incoming);

	/*! Decompose the string into words separated by the blank and comma characters.
	    \param args [Out] Decomposed words
	    \param blank [In] Blank (separator) characters
	    \param comma [In] Comma (separator) characters
	    */
	template <const int N>
	inline YSRESULT Arguments(YsArray <YsString,N> &args,const char *blank=" \t",const char *comma=",") const;

	/*! This function assumes that this string is a file name with directory path, and separate it into
	    directory part and file-name part.
	    \param path [Out] Returns directory part
	    \param name [Out] Returns file-name part
	    */
	inline YSRESULT SeparatePathFile(YsString &path,YsString &name) const;

	/*! Deletes spaces, tabs, and new-line characters at the end of the string. */
	inline void DeleteTailSpace(void);

	/*! Turns small letters ('a' to 'z') into capital letters ('A' to 'Z'). */
	inline void Capitalize(void);

	/*! Turns capital letters ('A' to 'Z') into small letters ('a' to 'z'). */
	inline void Uncapitalize(void);

	/*! Encodes incoming zero-terminated UNICODE string to UTF8 string.
	    \param wStr [In] Incoming UNICODE string
	    */
	template <class WCHARTYPE>
	inline void EncodeUTF8(const WCHARTYPE wStr[]);

	/*! Read a line from a file. */
	inline const char *Fgets(FILE *fp);

	/*! Makes a printf-formatted string.  It re-sizes itself according to
	    the output and is a good replacement for sprintf. */
	int Printf(const char *fom,...);
};

template <class CHARTYPE>
inline YSRESULT YsGenericString <CHARTYPE>::GetUTF8String(class YsString &outStr) const
{
	outStr.EncodeUTF8 <CHARTYPE> (vv);
	return YSOK;
}

inline YsString::YsString()
{
}

inline YsString::YsString(const char from[])  // Isn't it automatic?
{
	Set(from);
}

inline YsString::YsString(const YsString &from)
{
	YsArray <char,16>::Set(from.Strlen()+1,from.GetArray());
}

inline YsString &YsString::operator=(const YsString &from)
{
	YsGenericString <char>::Set(from);
	return *this;
}

inline YsString::YsString(YsString &&from)
{
	MoveFrom(from);
}

inline YsString &YsString::operator=(YsString &&str)
{
	MoveFrom(str);
	return *this;
}

inline YsString &YsString::MoveFrom(YsString &incoming)
{
	YsGenericString <char>::MoveFrom(incoming); // Is supposed to fall back to YsArray <char,16>::MoveFrom
	return *this;
}

template <const int N>
inline YSRESULT YsString::Arguments(YsArray <YsString,N> &args,const char *blank,const char *comma) const
{
	YsGenericString <char>::Arguments <YsString,N> (args,blank,comma);
	return YSOK;
}

inline YSRESULT YsString::SeparatePathFile(YsString &pth,YsString &fil) const
{
	int cutPtr,seekPtr;

	cutPtr=0;

	/* Skip Drive Name */
	if(vv[0]!=0 && vv[1]==':')
	{
		cutPtr+=2;
	}

	/* scan until nul Stopper */
	for(seekPtr=cutPtr; vv[seekPtr]!=0; seekPtr++)
	{
		if((vv[seekPtr]&0x80)!=0 && vv[seekPtr+1]!=0)  // 2 byte char
		{
			seekPtr++;
		}
		else if(vv[seekPtr]=='\\' || vv[seekPtr]=='/' || vv[seekPtr]==':')
		{
			cutPtr=seekPtr+1;
		}
	}

	/* cutPtr points *tmp  or after ':' or after last '\\' */
	fil.Set(vv+cutPtr);

	int i;
	pth.Resize(cutPtr+1);
	for(i=0; i<cutPtr; i++)
	{
		pth.vv[i]=vv[i];
	}
	pth.vv[cutPtr]=0;

	return YSOK;
}

inline void YsString::DeleteTailSpace(void)
{
	unsigned int chr;
	YSSIZE_T i,cut,len;

	len=Strlen();
	cut=0;
	for(i=0; i<len; i++)
	{
		chr=((unsigned char *)vv)[i];

		if((0x81<=chr && chr<=0x9f) || 0xe0<=chr)
		{
			cut=i+2;
			i++;
		}
		else if(isprint(chr) && chr!=' ' && chr!='\t')
		{
			cut=i+1;
		}
	}

	if(cut<len)
	{
		vv[cut]=0;
		Resize(cut+1);
	}
}

inline void YsString::Capitalize(void)
{
	for(YSSIZE_T i=0; vv[i]!=0; i++)
	{
		if((0x81<=((unsigned char*)vv)[i] && ((unsigned char*)vv)[i]<=0x9f) || 0xe0<=((unsigned char*)vv)[i])
		{
			i++;
			if(vv[i]==0)
			{
				break;
			}
		}
		else if('a'<=vv[i] && vv[i]<='z')
		{
			vv[i]=vv[i]+'A'-'a';
		}
	}
}

inline void YsString::Uncapitalize(void)
{
	for(YSSIZE_T i=0; vv[i]!=0; i++)
	{
		if((0x81<=((unsigned char*)vv)[i] && ((unsigned char*)vv)[i]<=0x9f) || 0xe0<=((unsigned char*)vv)[i])
		{
			i++;
			if(vv[i]==0)
			{
				break;
			}
		}
		else if('A'<=vv[i] && vv[i]<='Z')
		{
			vv[i]=vv[i]+'a'-'A';
		}
	}
}

template <class WCHARTYPE>
inline void YsString::EncodeUTF8(const WCHARTYPE wStr[])
{
	if(NULL!=wStr)
	{
		YSSIZE_T l=0;
		for(l=0; wStr[l]!=0; l++)
		{
		}

		size_t lOut=l*6+1;  // Worst case six-byte encoding times N wchars plus terminator.
		YsArray <char,16>::Resize(lOut);

		size_t nByte=YsWCharToUTF8(lOut,vv,wStr); // nByte includes zero terminator.
		YsArray <char,16>::Resize(nByte);
	}
	else
	{
		Set("");
	}
}

inline int operator==(const YsString &str1,const YsString &str2)
{
	return (strcmp(str1,str2)==0);
}

inline int operator!=(const YsString &str1,const YsString &str2)
{
	return (strcmp(str1,str2)!=0);
}

inline int operator<(const YsString &str1,const YsString &str2)
{
	// str1<str2   <=>  str1-str2<0
	return (strcmp(str1,str2)<0);
}

inline int operator>(const YsString &str1,const YsString &str2)
{
	// str1>str2   <=>  str1-str2>0
	return (strcmp(str1,str2)>0);
}

inline int operator<=(const YsString &str1,const YsString &str2)
{
	return (strcmp(str1,str2)<=0);
}

inline int operator>=(const YsString &str1,const YsString &str2)
{
	return (strcmp(str1,str2)>=0);
}

inline const char *YsString::Fgets(FILE *fp)
{
	char buf[256];
	YsArray <char,16>::Set(0,NULL);
	while(fgets(buf,255,fp)!=NULL)
	{
		YSBOOL eol=YSFALSE;

		int len=(int)strlen(buf);
		while(0<len && (0x0d==buf[len-1] || 0x0a==buf[len-1]))
		{
			eol=YSTRUE;
			buf[len-1]=0;
			--len;
		}

		if(YSTRUE==eol)
		{
			YsArray <char,16>::Append(len+1,buf);
			break;
		}
		else
		{
			YsArray <char,16>::Append(len,buf);
		}
	}

	if(GetN()==0)
	{
		Set("");
		return NULL;
	}

	if(vv[GetN()-1]!=0)  // In case /n is not at the end of the file 2006/10/26
	{
		YsArray <char,16>::Append(0);
	}

	return vv;
}

////////////////////////////////////////////////////////////

/*! Class for storing and manipulating wide-character (UNICODE) string. */
class YsWString : public YsGenericString <wchar_t>
{
public:
	/*! Default constructor.  Creates an empty string. */
	inline YsWString();

	/*! Creates a copy of the incoming string.
	    \param from [In] Incoming string.
	    */
	inline YsWString(const wchar_t from[]);

	/*! Creates a copy of the incoming string.
	    \param from [In] Incoming string.
	    */
	inline YsWString(const YsWString &from);

	/*! Copy operator. */
	inline YsWString &operator=(const YsWString &from);

	/*! Move constructor.
	    Question is if the move constructor of YsArray <wchar_t> is automatically used if I do not make a specific move constructor here.  I need more study. 
	    Experiment with VS2012 implies the move constructor/operator of the base class will not be automatically used for a sub-class, even though the sub-class has no specific data members.  So, probably I need this function. */
	inline YsWString(YsWString &&from);

	/*! Move operator.  Same question as move constructor remains. */
	inline YsWString &operator=(YsWString &&from);

	/*! Moves a string from incoming string. */
	inline YsWString &MoveFrom(YsWString &from);

	/*! Decompose the string into words separated by the blank and comma characters.
	    \param args [Out] Decomposed words
	    \param blank [In] Blank (separator) characters
	    \param comma [In] Comma (separator) characters
	    */
	template <const int N>
	inline YSRESULT Arguments(YsArray <YsWString,N> &args,const wchar_t *blank=L" \t",const wchar_t *comma=L",") const;
};

inline YsWString::YsWString()
{
}

inline YsWString::YsWString(const wchar_t from[])
{
	Set(from);
}

inline YsWString::YsWString(const YsWString &from)
{
	Set(from);
}

inline YsWString &YsWString::operator=(const YsWString &from)
{
	YsGenericString <wchar_t>::Set(from);
	return *this;
}

inline YsWString::YsWString(YsWString &&from)
{
	MoveFrom(from);
}

inline YsWString &YsWString::operator=(YsWString &&from)
{
	MoveFrom(from);
	return *this;
}

inline YsWString &YsWString::MoveFrom(YsWString &from)
{
	YsGenericString <wchar_t>::MoveFrom(from); // Is supposed to fall back to YsArray <wchar_t,16>::MoveFrom
	return *this;
}

template <const int N>
inline YSRESULT YsWString::Arguments(YsArray <YsWString,N> &args,const wchar_t *blank,const wchar_t *comma) const
{
	return YsGenericString <wchar_t>::Arguments <YsWString,N> (args,blank,comma);
}

////////////////////////////////////////////////////////////

template <class strType,class assocType>
YSRESULT YsQuickSortString(YSSIZE_T n,strType const *keyValue[],assocType assocValue[],YSBOOL watch=YSFALSE)
{
	if(n<=1)
	{
		return YSOK;
	}

	if(watch==YSTRUE)
	{
		YsPrintf("YsQuickSortString (YsString *) %d\n",n);
	}

	YSSIZE_T i,dividePoint;
	int dif;
	const strType *divider,*nonDivider;
	YSBOOL allEqual;
	divider=keyValue[n/2];  // Can use random number.
	nonDivider=NULL;

TRYAGAIN:
	dividePoint=0;
	allEqual=YSTRUE;
	for(i=0; i<n; i++)
	{
		dif=strType::Strcmp(*keyValue[i],*divider);
		if(dif<0)
		{
			if(i!=dividePoint)
			{
				YsSwapSomething <const strType *> (keyValue[dividePoint],keyValue[i]);
				if(assocValue!=NULL)
				{
					YsSwapSomething <assocType> (assocValue[dividePoint],assocValue[i]);
				}
			}
			dividePoint++;
		}
		if(dif!=0)
		{
			nonDivider=keyValue[i];
			allEqual=YSFALSE;
		}
	}

	if(allEqual==YSTRUE)
	{
		return YSOK;
	}

	if(dividePoint==0 && nonDivider!=NULL)
	{
		divider=nonDivider;
		goto TRYAGAIN;
	}

	if(assocValue!=NULL)
	{
		YsQuickSortString <strType,assocType> (dividePoint,keyValue,assocValue);
		YsQuickSortString <strType,assocType> ((n-dividePoint),keyValue+dividePoint,assocValue+dividePoint);
	}
	else
	{
		YsQuickSortString <strType,assocType> (dividePoint,keyValue,NULL);
		YsQuickSortString <strType,assocType> ((n-dividePoint),keyValue+dividePoint,NULL);
	}

	return YSOK;
}

template <class assocType>
YSRESULT YsQuickSortString(YSSIZE_T n,YsString const *keyValue[],assocType assocValue[],YSBOOL watch=YSFALSE)
{
	if(n<=1)
	{
		return YSOK;
	}

	if(watch==YSTRUE)
	{
		YsPrintf("YsQuickSortString (YsString *) %d\n",n);
	}

	YSSIZE_T i,dividePoint;
	int dif;
	const YsString *divider,*nonDivider;
	YSBOOL allEqual;
	divider=keyValue[n/2];  // Can use random number.
	nonDivider=NULL;

TRYAGAIN:
	dividePoint=0;
	allEqual=YSTRUE;
	for(i=0; i<n; i++)
	{
		dif=YsString::Strcmp(*keyValue[i],*divider);
		if(dif<0)
		{
			if(i!=dividePoint)
			{
				YsSwapSomething <const YsString *> (keyValue[dividePoint],keyValue[i]);
				if(assocValue!=NULL)
				{
					YsSwapSomething <assocType> (assocValue[dividePoint],assocValue[i]);
				}
			}
			dividePoint++;
		}
		if(dif!=0)
		{
			nonDivider=keyValue[i];
			allEqual=YSFALSE;
		}
	}

	if(allEqual==YSTRUE)
	{
		return YSOK;
	}

	if(dividePoint==0 && nonDivider!=NULL)
	{
		divider=nonDivider;
		goto TRYAGAIN;
	}

	if(assocValue!=NULL)
	{
		YsQuickSortString <assocType> (dividePoint,keyValue,assocValue);
		YsQuickSortString <assocType> ((n-dividePoint),keyValue+dividePoint,assocValue+dividePoint);
	}
	else
	{
		YsQuickSortString <assocType> (dividePoint,keyValue,NULL);
		YsQuickSortString <assocType> ((n-dividePoint),keyValue+dividePoint,NULL);
	}

	return YSOK;
}

template <class assocType>
YSRESULT YsQuickSortString(YSSIZE_T n,char const *keyValue[],assocType assocValue[],YSBOOL watch=YSFALSE)
{
	if(n<=1)
	{
		return YSOK;
	}

	YSSIZE_T i,dividePoint;
	int dif;
	const char *divider,*nonDivider;
	YSBOOL allEqual;
	divider=keyValue[n/2];  // Can use random number.
	nonDivider=NULL;

	if(watch==YSTRUE)
	{
		YsPrintf("YsQuickSortString (const char *) %d\n",(int)n);
	}

TRYAGAIN:
	dividePoint=0;
	allEqual=YSTRUE;
	for(i=0; i<n; i++)
	{
		dif=strcmp(keyValue[i],divider);
		if(dif<0)
		{
			if(i!=dividePoint)
			{
				YsSwapSomething <const char *> (keyValue[dividePoint],keyValue[i]);
				if(assocValue!=NULL)
				{
					YsSwapSomething <assocType> (assocValue[dividePoint],assocValue[i]);
				}
			}
			dividePoint++;
		}
		if(dif!=0)
		{
			nonDivider=keyValue[i];
			allEqual=YSFALSE;
		}
	}

	if(allEqual==YSTRUE)
	{
		return YSOK;
	}

	if(dividePoint==0 && nonDivider!=NULL)
	{
		if(watch==YSTRUE)
		{
			YsPrintf("YsQuickSortString: Trying again...\n");
		}
		divider=nonDivider;
		goto TRYAGAIN;
	}

	if(assocValue!=NULL)
	{
		YsQuickSortString <assocType> (dividePoint,keyValue,assocValue,watch);
		YsQuickSortString <assocType> ((n-dividePoint),keyValue+dividePoint,assocValue+dividePoint,watch);
	}
	else
	{
		YsQuickSortString <assocType> (dividePoint,keyValue,NULL,watch);
		YsQuickSortString <assocType> ((n-dividePoint),keyValue+dividePoint,NULL,watch);
	}

	return YSOK;
}




/* } */
#endif

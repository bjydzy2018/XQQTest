/******************************************************************************
 Copyright:      1999-2013, iFLYTEK Co., Ltd.
 File name:      VAString.h
 Description:    U8 和 U16字符串
 Author:         pffang
 Version:        1.0
 Date:           2013-4-8
 History:        2013-10-10  创建
 *******************************************************************************/
#pragma once
#ifndef VASTRING_H_
#define VASTRING_H_
#include "VACommon.h"

namespace VA {

#define ITEMSOF( array ) (sizeof(array)/sizeof(array[0]))
//template<int n>
//struct char_array_wrapper {
//  char result[n];
//};
//template<typename T, int s>
//char_array_wrapper<s> the_type_of_the_variable_is_not_an_array(
//    const T (&array)[s]) {
//}
//#define ITEMSOF(v) sizeof(the_type_of_the_variable_is_not_an_array(v).result)
//template<class T, int n>
//int ITEMSOF(T (&p)[n]) {
//  return n;
//}
#if VA_VC
typedef unsigned long U32Char; /* at least 32 bits */
typedef wchar_t U16Char; /* at least 16 bits */
#elif VA_GNUC
typedef wchar_t U32Char; /* at least 32 bits */
typedef unsigned short U16Char; /* at least 16 bits */
#endif

typedef char U8Char; /* typically 8 bits */
typedef unsigned short GBKChar;

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (unsigned long)0x0000FFFD
#define UNI_MAX_BMP (unsigned long)0x0000FFFF
#define UNI_MAX_UTF16 (unsigned long)0x0010FFFF
#define UNI_MAX_UTF32 (unsigned long)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (unsigned long)0x0010FFFF

#define UNI_MAX_UTF8_BYTES_PER_CODE_POINT 4

#define UNI_UTF16_BYTE_ORDER_MARK_NATIVE  (unsigned short)0xFEFF
#define UNI_UTF16_BYTE_ORDER_MARK_SWAPPED (unsigned short)0xFFFE

typedef enum {
    conversionOK, /* conversion successful */
    sourceExhausted, /* partial character in source, but hit end */
    targetExhausted, /* insuff. room in target for conversion */
    sourceIllegal /* source sequence is illegal/malformed */
} ConversionResult;

typedef enum {
    strictConversion = 0,
    lenientConversion
} ConversionFlags;

ConversionResult ConvertUTF8toUTF16(const U8Char* sourceStart,
                                    const U8Char* sourceEnd,
                                    U16Char* targetStart, U16Char* targetEnd,
                                    ConversionFlags flags);

ConversionResult ConvertUTF8toUTF32(const U8Char* sourceStart,
                                    const U8Char* sourceEnd,
                                    U32Char* targetStart, U32Char* targetEnd,
                                    ConversionFlags flags);

ConversionResult ConvertUTF16toUTF8(const U16Char* sourceStart,
                                    const U16Char* sourceEnd,
                                    U8Char* targetStart, U8Char* targetEnd,
                                    ConversionFlags flags);

ConversionResult ConvertUTF32toUTF8(const U32Char* sourceStart,
                                    const U32Char* sourceEnd,
                                    U8Char* targetStart, U8Char* targetEnd,
                                    ConversionFlags flags);

ConversionResult ConvertUTF16toUTF32(const U16Char* sourceStart,
                                     const U16Char* sourceEnd,
                                     U32Char* targetStart, U32Char* targetEnd,
                                     ConversionFlags flags);

ConversionResult ConvertUTF32toUTF16(const U32Char* sourceStart,
                                     const U32Char* sourceEnd,
                                     U16Char* targetStart, U16Char* targetEnd,
                                     ConversionFlags flags);

bool isLegalUTF8Sequence(const U8Char *source, const U8Char *sourceEnd);

bool isLegalUTF8String(const U8Char **source, const U8Char *sourceEnd);

unsigned getNumBytesForUTF8(U8Char firstByte);

template<typename Type>
inline size_t ustrlen(const Type* str) {
    const Type* eos = str;
    while (*eos++)
        ;
    return (size_t)(eos - str - 1);
}
template<typename Type>
inline Type* ustrncpy(Type* destStr, const Type* srcStr, size_t nCount) {
    Type* start = destStr;
    while (nCount && (*destStr++ = *srcStr++))
        nCount--;

    if (nCount)
        while (--nCount)
            *destStr++ = 0;
    return start;
}

void stringFormatV(std::string& str, const char *sformat, va_list args_list);
void stringFormat(std::string& str, const char *sformat, ...);
void stringFormatAppendV(std::string& str, const char *sformat,
                         va_list args_list);
void stringFormatAppend(std::string& str, const char *sformat, ...);

void wstringFormatV(std::wstring& wstr, const wchar_t *wsformat,
                    va_list args_list);
void wstringFormat(std::wstring& wstr, const wchar_t *wsformat, ...);
void wstringFormatAppendV(std::wstring& wstr, const wchar_t *wsformat,
                          va_list args_list);
void wstringFormatAppend(std::wstring& wstr, const wchar_t *wsformat, ...);

std::string TrimLeft(const std::string& str);
std::string TrimRight(const std::string& str);
std::string Trim(const std::string& str);

size_t strlen_utf8(const U8Char *s);

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

size_t U16CharToU8Char(const U16Char utf16char, U8Char* putf8char);
size_t U8CharToU16Char(const U8Char *byte, U16Char& utf16char);
size_t U32CharToU8Char(const U32Char utf32char, U8Char* putf8char);
size_t U8CharToU32Char(const U8Char* putf8char, U32Char& utf32char);
size_t U32CharToU16Char(const U32Char utf32char, U16Char* putf16char);
size_t U16CharToU32Char(const U16Char* putf16char, U32Char& utf32char);

int IsGBKChar(unsigned short code);
int IsUTF16Char(U16Char code);
size_t U16CharToGBKChar(const U16Char utf16char, char* pgbkchar);
size_t GBKCharToU16Char(const char* byte, U16Char& utf16char);

class U16StrToU8Str {
    U8Char* m_s;
 public:
    U16StrToU8Str(const U16Char* s);
    ~U16StrToU8Str();
    operator const U8Char*() const;
};
class U8StrToU16Str {
    U16Char* m_s;
 public:
    U8StrToU16Str(const U8Char* s);
    ~U8StrToU16Str();
    operator const U16Char*() const;
};
class U32StrToU8Str {
    U8Char* m_s;
 public:
    U32StrToU8Str(const U32Char* s);
    ~U32StrToU8Str();
    operator const U8Char*() const;
};
class U8StrToU32Str {
    U32Char* m_s;
 public:
    U8StrToU32Str(const U8Char* s);
    ~U8StrToU32Str();
    operator const U32Char*() const;
};
class U32StrToU16Str {
    U16Char* m_s;
 public:
    U32StrToU16Str(const U32Char* s);
    ~U32StrToU16Str();
    operator const U16Char*() const;
};
class U16StrToU32Str {
    U32Char* m_s;
 public:
    U16StrToU32Str(const U16Char* s);
    ~U16StrToU32Str();
    operator const U32Char*() const;
};

class GBKStrToU8Str {
    U8Char* m_s;
 public:
    GBKStrToU8Str(const char* s);
    ~GBKStrToU8Str();
    operator const U8Char*() const;
};

class GBKStrToU16Str {
    U16Char* m_s;
 public:
    GBKStrToU16Str(const char* s);
    ~GBKStrToU16Str();
    operator const U16Char*() const;
};

class U8StrToGBKStr {
    char* m_s;
 public:
    U8StrToGBKStr(const U8Char* s);
    ~U8StrToGBKStr();
    operator const char*() const;
};

class U16StrToGBKStr {
    char* m_s;
 public:
    U16StrToGBKStr(const U16Char* s);
    ~U16StrToGBKStr();
    operator const char*() const;
};

class U8String;
class U16String;

class U8String {
    U8Char* m_s;
    size_t m_l;

    U8Char* __StrRStr(const U8Char* string, const U8Char* lpszSub) const;
    U8Char __Reverse(U8Char ch);
    //protected:
    void __ConcatCopy(const U8Char* str1, size_t nLen1, const U8Char* str2,
                      size_t nLen2);

 public:
    U8String(const U8String& stringSrc);/*拷贝构造*/
    U8String(U8Char ch, unsigned int nRepeat = 1);
    U8String(const U8Char* lpsz = NULL, unsigned int nLength = 0);
    U8String(const std::string& stringSrc);

    U8String(const U16String& u16stringSrc);
    U8String(const U16Char* lpsz);

    void fromU16String(const U16String& u16stringSrc);
    void fromU16String(const U16Char* lpsz);

    const U8String& operator=(const U8String& stringSrc);
    const U8String& operator=(U8Char ch);
    const U8String& operator=(const U8Char* lpsz);
    const U8String& operator=(const U16String& u16stringSrc);
    const U8String& operator=(const U16Char* lpsz);
    const U8String& operator=(const std::string& stringSrc);

    operator const U8Char*() const;
    operator U8Char*() const;
    operator const unsigned char*() const;
    operator unsigned char*() const;

    const U8String& operator+=(const U8String& string);
    const U8String& operator+=(U8Char ch);
    const U8String& operator+=(const U8Char* lpsz);

    U8Char operator [](unsigned int nIndex) const;
    U8Char& operator [](unsigned int nIndex);

    ~U8String();

//Return: 0 ==; <0 String<lpsz; >0 String>lpsz;
    int Compare(const U8Char *lpsz) const;
    int CompareNoCase(const U8Char *lpsz) const;
    size_t Delete(size_t iIndex, size_t nCount = 1);
    void Empty();

    int Find(U8Char ch) const;
    int Find(const U8Char* lpszSub) const;
    int Find(U8Char ch, int nStart) const;
    int Find(const U8Char* lpszSub, int nStart) const;
    int FindOneOf(const U8Char* lpCharsSet) const;
    int FindOneOf(const U8Char* lpCharsSet, int nStart) const;

    void Format(const U8Char* lpszFormat, ...);
    void FormatV(const U8Char* lpszFormat, va_list argList);

    U8Char GetAt(int nIndex) const;
    const U8Char* GetBuffer() const;
    size_t GetLength() const;

    size_t Insert(size_t iIndex, U8Char ch);
    size_t Insert(size_t iIndex, const U8Char* lpsz);
    bool IsEmpty() const;
    bool IsNull() const;
    operator bool() const;
    U8String Left(int nCount) const;
    void MakeLower();
    void MakeReverse();
    void MakeUpper();
    U8String Mid(size_t nFirst) const;
    U8String Mid(size_t nFirst, size_t nCount) const;
    size_t Remove(U8Char ch);
    size_t Replace(U8Char chOld, U8Char chNew);
    size_t Replace(const U8Char* lpszOld, const U8Char* lpszNew);

    int ReverseFind(U8Char ch) const;
    int ReverseFind(const U8Char* lpszSub) const;
    int ReverseFind(U8Char ch, size_t nStart) const;
    int ReverseFind(const U8Char* lpszSub, size_t nStart) const;

    U8String Right(int nCount) const;
    void SetAt(unsigned int nIndex, U8Char ch) const;
    U8String SpanExcluding(const U8Char lpszCharSet) const;
    U8String SpanIncluding(const U8Char lpszCharSet) const;

    void TrimLeft();
    void TrimLeft(U8Char chTarget);
    void TrimLeft(const U8Char* lpszTargets);
    void TrimRight();
    void TrimRight(U8Char chTarget);
    void TrimRight(const U8Char* lpszTargets);

    friend bool operator==(const U8String& s1, const U8String& s2);
    friend bool operator==(const U8String& s1, const U8Char* s2);
    friend bool operator==(const U8Char* s1, const U8String& s2);
    friend bool operator !=(const U8String& s1, const U8String& s2);
    friend bool operator !=(const U8String& s1, const U8Char* s2);
    friend bool operator !=(const U8Char* s1, const U8String& s2);
    friend bool operator <(const U8String& s1, const U8String& s2);
    friend bool operator <(const U8String& s1, const U8Char* s2);
    friend bool operator <(const U8Char* s1, const U8String& s2);
    friend bool operator >(const U8String& s1, const U8String& s2);
    friend bool operator >(const U8String& s1, const U8Char* s2);
    friend bool operator >(const U8Char* s1, const U8String& s2);
    friend bool operator <=(const U8String& s1, const U8String& s2);
    friend bool operator <=(const U8String& s1, const U8Char* s2);
    friend bool operator <=(const U8Char* s1, const U8String& s2);
    friend bool operator >=(const U8String& s1, const U8String& s2);
    friend bool operator >=(const U8String& s1, const U8Char* s2);
    friend bool operator >=(const U8Char* s1, const U8String& s2);

    friend U8String operator +(const U8String& string1,
                               const U8String& string2);
    friend U8String operator +(const U8String& string, U8Char ch);
    friend U8String operator +(U8Char ch, const U8String& string);
    friend U8String operator +(const U8String& string, const U8Char* lpsz);
    friend U8String operator +(const U8Char* lpsz, const U8String& string);
};

class U16String {
    U16Char* m_s;
    size_t m_l;

    /*C-Style can override by C library functions to increase speed*/
    size_t __StrLen(const U16Char* str) const;
    U16Char* __StrNCpy(U16Char* destStr, const U16Char* srcStr,
                       size_t nCount) const;
    int __StrCmp(const U16Char* src, const U16Char* dst) const;
    int __StrNoCaseCmp(const U16Char* src, const U16Char* dst) const;
    const U16Char* __StrChr(const U16Char* string, U16Char ch) const;
    U16Char* __StrRChr(U16Char* string, U16Char ch) const;
    const U16Char* __StrStr(const U16Char* string,
                            const U16Char* lpszSub) const;
    const U16Char* __StrRStr(const U16Char* string,
                             const U16Char* lpszSub) const;
    U16Char* __StrPbrk(U16Char* string, const U16Char* CharsSet) const;
    U16Char __ToLower(U16Char ch);
    U16Char __ToUpper(U16Char ch);
    U16Char __Reverse(U16Char ch);
    void __ConcatCopy(const U16Char* str1, size_t nLen1, const U16Char* str2,
                      size_t nLen2);
 public:
    U16String(const U16String& stringSrc);
    U16String(U16Char ch, unsigned int nRepeat = 1);
    U16String(const U16Char* lpsz = NULL, unsigned int nLength = 0);
    const U16String& operator=(const U16String& stringSrc);
    const U16String& operator=(U16Char ch);
    const U16String& operator=(const U16Char* lpsz);

    operator const U16Char*() const;
    operator U16Char*() const;

    const U16String& operator+=(const U16String& string);
    const U16String& operator+=(U16Char ch);
    const U16String& operator+=(const U16Char* lpsz);
    U16Char operator [](size_t nIndex) const;
    U16Char& operator [](size_t nIndex);

    ~U16String();

//Return: 0 ==; <0 String<lpsz; >0 String>lpsz;
    int Compare(const U16Char *lpsz) const;
    int CompareNoCase(const U16Char *lpsz) const;
    size_t Delete(int iIndex, int nCount = 1);
    void Empty();

    int Find(U16Char ch) const;
    int Find(const U16Char* lpszSub) const;
    int Find(U16Char ch, int nStart) const;
    int Find(const U16Char* lpszSub, int nStart) const;
    int FindOneOf(const U16Char* lpCharsSet) const;
    int FindOneOf(const U16Char* lpCharsSet, int nStart) const;

    void Format(const U16Char* lpszFormat, ...);
    void FormatV(const U16Char* lpszFormat, va_list argList);

    U16Char GetAt(int nIndex) const;
    const U16Char* GetBuffer() const;
    size_t GetLength() const;

    size_t Insert(size_t iIndex, U16Char ch);
    size_t Insert(size_t iIndex, const U16Char* lpsz);

    bool IsEmpty() const;
    bool IsNull() const;
    operator bool() const;
    U16String Left(int nCount) const;
    void MakeLower();
    void MakeReverse();
    void MakeUpper();
    U16String Mid(size_t nFirst) const;
    U16String Mid(size_t nFirst, size_t nCount) const;
    size_t Remove(U16Char ch);
    size_t Replace(U16Char chOld, U16Char chNew);
    size_t Replace(const U16Char* lpszOld, const U16Char* lpszNew);

    int ReverseFind(U16Char ch) const;
    int ReverseFind(const U16Char* lpszSub) const;
    int ReverseFind(U16Char ch, int nStart) const;
    int ReverseFind(const U16Char* lpszSub, int nStart) const;

    U16String Right(int nCount) const;
    void SetAt(unsigned int nIndex, U16Char ch) const;
    U16String SpanExcluding(const U16Char lpszCharSet) const;
    U16String SpanIncluding(const U16Char lpszCharSet) const;

    void TrimLeft();
    void TrimLeft(U16Char chTarget);
    void TrimLeft(const U16Char* lpszTargets);
    void TrimRight();
    void TrimRight(U16Char chTarget);
    void TrimRight(const U16Char* lpszTargets);

    friend bool operator==(const U16String& s1, const U16String& s2);
    friend bool operator==(const U16String& s1, const U16Char* s2);
    friend bool operator==(const U16Char* s1, const U16String& s2);
    friend bool operator !=(const U16String& s1, const U16String& s2);
    friend bool operator !=(const U16String& s1, const U16Char* s2);
    friend bool operator !=(const U16Char* s1, const U16String& s2);
    friend bool operator <(const U16String& s1, const U16String& s2);
    friend bool operator <(const U16String& s1, const U16Char* s2);
    friend bool operator <(const U16Char* s1, const U16String& s2);
    friend bool operator >(const U16String& s1, const U16String& s2);
    friend bool operator >(const U16String& s1, const U16Char* s2);
    friend bool operator >(const U16Char* s1, const U16String& s2);
    friend bool operator <=(const U16String& s1, const U16String& s2);
    friend bool operator <=(const U16String& s1, const U16Char* s2);
    friend bool operator <=(const U16Char* s1, const U16String& s2);
    friend bool operator >=(const U16String& s1, const U16String& s2);
    friend bool operator >=(const U16String& s1, const U16Char* s2);
    friend bool operator >=(const U16Char* s1, const U16String& s2);
    friend U16String operator +(const U16String& string1,
                                const U16String& string2);
    friend U16String operator +(const U16String& string, U16Char ch);
    friend U16String operator +(U16Char ch, const U16String& string);
    friend U16String operator +(const U16String& string, const U16Char* lpsz);
    friend U16String operator +(const U16Char* lpsz, const U16String& string);

    //从utf8转换为utf16
    U16String(const U8String& u8stringSrc);
    U16String(const char* u8stringSrc);

    void fromU8String(const U8String& u8stringSrc);
    void fromU8String(const char* u8stringSrc);

    const U16String& operator=(const U8String& u8stringSrc);
    const U16String& operator=(const char* u8stringSrc);
};

#if defined(UNICODE)
#define UString U16String
#else
#define UString U8String
#endif

//std::string u16string2utf8string(const U16string & in) {
//  std::string out;
//  out = U16StrToU8Str(in.c_str());
//  return out;
//}

/* --------------------------------------------------------------------- */
#if 0
/*
 string 转换为 wstring
 */
std::wstring c2w(const char *pc)
{
    std::wstring val = L"";

    if(NULL == pc) {
        return val;
    }
//size_t size_of_ch = strlen(pc)*sizeof(char);
//size_t size_of_wc = get_wchar_size(pc);
    size_t size_of_wc;
    size_t destlen = mbstowcs(0,pc,0);
    if (destlen ==(size_t)(-1)) {
        return val;
    }
    size_of_wc = destlen+1;
    wchar_t * pw = new wchar_t[size_of_wc];
    mbstowcs(pw,pc,size_of_wc);
    val = pw;
    delete pw;
    return val;
}
/*
 wstring 转换为 string
 */
std::string w2c(const wchar_t * pw)
{
    std::string val = "";
    if(!pw) {
        return val;
    }
    size_t size= wcslen(pw)*sizeof(wchar_t);
    char *pc = NULL;
    if(!(pc = (char*)malloc(size))) {
        return val;
    }
    size_t destlen = wcstombs(pc,pw,size);
    /*转换不为空时，返回值为-1。如果为空，返回值0*/
    if (destlen ==(size_t)(0)) {
        return val;
    }
    val = pc;
    delete pc;
    return val;
}

std::string url_encode( std::string sSrc )
{
    const char SAFE[256] = {
        /*     0 1 2 3  4 5 6 7  8 9 A B  C D E F */
        /* 0 */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 1 */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 2 */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 3 */1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

        /* 4 */0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
        /* 5 */1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
        /* 6 */0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
        /* 7 */1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

        /* 8 */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* 9 */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* A */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* B */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

        /* C */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* D */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* E */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
        /* F */0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
    };
    const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const int SRC_LEN = sSrc.length();
    unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
    unsigned char * pEnd = pStart;
    const unsigned char * const SRC_END = pSrc + SRC_LEN;

    for (; pSrc < SRC_END; ++pSrc) {
        if (SAFE[*pSrc])
        *pEnd++ = *pSrc;
        else {
            // escape this char
            *pEnd++ = '%';
            *pEnd++ = DEC2HEX[*pSrc >> 4];
            *pEnd++ = DEC2HEX[*pSrc & 0x0F];
        }
    }

    std::string sResult((char *)pStart, (char *)pEnd);
    delete [] pStart;
    return sResult;
}

std::string url_decode( std::string sSrc )
{
// Note from RFC1630:  "Sequences which start with a percent sign
// but are not followed by two hexadecimal characters (0-9, A-F) are reserved
// for future extension"

    const char HEX2DEC[256] = {
        /*      0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
        /* 0 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 1 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 2 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 3 */00, 1, 2, 3, 04, 5, 6, 7, 08, 9,-1,-1, -1,-1,-1,-1,

        /* 4 */-1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 5 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 6 */-1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 7 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* 8 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* 9 */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* A */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* B */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

        /* C */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* D */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* E */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
        /* F */-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
    };

    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const int SRC_LEN = sSrc.length();
    const unsigned char * const SRC_END = pSrc + SRC_LEN;
    const unsigned char * const SRC_LAST_DEC = SRC_END - 2;  // last decodable '%'

    char * const pStart = new char[SRC_LEN];
    char * pEnd = pStart;

    while (pSrc < SRC_LAST_DEC) {
        if (*pSrc == '%') {
            char dec1, dec2;
            if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
                    && -1 != (dec2 = HEX2DEC[*(pSrc + 2)])) {
                *pEnd++ = (dec1 << 4) + dec2;
                pSrc += 3;
                continue;
            }
        }

        *pEnd++ = *pSrc++;
    }

// the last 2- chars
    while (pSrc < SRC_END)
    *pEnd++ = *pSrc++;

    std::string sResult(pStart, pEnd);
    delete [] pStart;
    return sResult;
}
#endif

}
/* namespace VA */

#endif /* VASTRING_H_ */


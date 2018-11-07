/*
 * VAString.cpp
 *
 *  Created on: 2013-4-8
 *      Author: pffang
 */

#include "VAString.h"

namespace VA {
static const int halfShift = 10; /* used for shifting by 10 bits */

static const unsigned long halfBase = 0x0010000UL;
static const unsigned long halfMask = 0x3FFUL;

#define UNI_SUR_HIGH_START  (unsigned long)0xD800
#define UNI_SUR_HIGH_END    (unsigned long)0xDBFF
#define UNI_SUR_LOW_START   (unsigned long)0xDC00
#define UNI_SUR_LOW_END     (unsigned long)0xDFFF

/*
 * Index into the table below with the first byte of a UTF-8 sequence to
 * get the number of trailing bytes that are supposed to follow it.
 * Note that *legal* UTF-8 values can't have 4 or 5-bytes. The table is
 * left as-is for anyone who may want to do such conversion, which was
 * allowed in earlier algorithms.
 */
static const unsigned char trailingBytesForUTF8[256] = { 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
        4, 4, 4, 4, 5, 5, 5, 5 };

/*
 * Magic values subtracted from a buffer value during UTF8 conversion.
 * This table contains as many values as there might be trailing bytes
 * in a UTF-8 sequence.
 */
static const unsigned long offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL,
        0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/*
 * Once the bits are split out into bytes of UTF-8, this is a mask OR-ed
 * into the first byte, depending on how many bytes follow.  There are
 * as many entries in this table as there are UTF-8 sequence types.
 * (I.e., one byte sequence, two byte... etc.). Remember that sequencs
 * for *legal* UTF-8 will be 4 or fewer bytes total.
 */
static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0,
        0xF8, 0xFC };

/* --------------------------------------------------------------------- */

/* The interface converts a whole buffer to avoid function-call overhead.
 * Constants have been gathered. Loops & conditionals have been removed as
 * much as possible for efficiency, in favor of drop-through switches.
 * (See "Note A" at the bottom of the file for equivalent code.)
 * If your compiler supports it, the "isLegalUTF8" call can be turned
 * into an inline function.
 */

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF16(const U32Char* sourceStart,
                                     const U32Char* sourceEnd,
                                     U16Char* targetStart, U16Char* targetEnd,
                                     ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const unsigned long* source = (const unsigned long*) sourceStart;
    U16Char* target = targetStart;
    while (source < (const unsigned long*) sourceEnd) {
        unsigned long ch;
        if (target >= targetEnd) {
            result = targetExhausted;
            break;
        }
        assert(target < targetEnd);
        ch = *source++;
        if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
            /* UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = (U16Char) ch; /* normal case */
            }
        } else if (ch > UNI_MAX_LEGAL_UTF32) {
            if (flags == strictConversion) {
                result = sourceIllegal;
            } else {
                *target++ = UNI_REPLACEMENT_CHAR;
            }
        } else {
            /* target is a character in range 0xFFFF - 0x10FFFF. */
            if (target + 1 >= targetEnd) {
                --source; /* Back up source pointer! */
                result = targetExhausted;
                break;
            }
            ch -= halfBase;
            *target++ = (U16Char)((ch >> halfShift) + UNI_SUR_HIGH_START);
            *target++ = (U16Char)((ch & halfMask) + UNI_SUR_LOW_START);
        }
    }
    //*sourceStart = source;
    //*targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF16toUTF32(const U16Char* sourceStart,
                                     const U16Char* sourceEnd,
                                     U32Char* targetStart, U32Char* targetEnd,
                                     ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const U16Char* source = sourceStart;
    U32Char* target = targetStart;
    unsigned long ch, ch2;
    while (source < sourceEnd) {
        const U16Char* oldSource = source; /*  In case we have to back up because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
            /* If the 16 bits following the high surrogate are in the source buffer... */
            if (source < sourceEnd) {
                ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
                    ch = ((ch - UNI_SUR_HIGH_START ) << halfShift)
                            + (ch2 - UNI_SUR_LOW_START ) + halfBase;
                    ++source;
                } else if (flags == strictConversion) { /* it's an unpaired high surrogate */
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            } else { /* We don't have the 16 bits following the high surrogate. */
                --source; /* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        } else if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        if (target >= targetEnd) {
            source = oldSource; /* Back up source pointer! */
            result = targetExhausted;
            break;
        }
        assert(target < targetEnd);
        *target++ = ch;
    }
//     *sourceStart = source;
//     *targetStart = target;
#ifdef CVTUTF_DEBUG
    if (result == sourceIllegal) {
        fprintf(stderr, "ConvertUTF16toUTF32 illegal seq 0x%04x,%04x\n", ch, ch2);
        fflush(stderr);
    }
#endif
    return result;
}
ConversionResult ConvertUTF16toUTF8(const U16Char* sourceStart,
                                    const U16Char* sourceEnd,
                                    U8Char* targetStart, U8Char* targetEnd,
                                    ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const U16Char* source = sourceStart;
    U8Char* target = targetStart;
    while (source < sourceEnd) {
        unsigned long ch;
        unsigned short bytesToWrite = 0;
        const U32Char byteMask = 0xBF;
        const U32Char byteMark = 0x80;
        const U16Char* oldSource = source; /* In case we have to back up because of target overflow. */
        ch = *source++;
        /* If we have a surrogate pair, convert to UTF32 first. */
        if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_HIGH_END) {
            /* If the 16 bits following the high surrogate are in the source buffer... */
            if (source < sourceEnd) {
                unsigned long ch2 = *source;
                /* If it's a low surrogate, convert to UTF32. */
                if (ch2 >= UNI_SUR_LOW_START && ch2 <= UNI_SUR_LOW_END) {
                    ch = ((ch - UNI_SUR_HIGH_START ) << halfShift)
                            + (ch2 - UNI_SUR_LOW_START ) + halfBase;
                    ++source;
                } else if (flags == strictConversion) { /* it's an unpaired high surrogate */
                    --source; /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                }
            } else { /* We don't have the 16 bits following the high surrogate. */
                --source; /* return to the high surrogate */
                result = sourceExhausted;
                break;
            }
        } else if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_LOW_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /* Figure out how many bytes the result will require */
        if (ch < (U32Char) 0x80) {
            bytesToWrite = 1;
        } else if (ch < (U32Char) 0x800) {
            bytesToWrite = 2;
        } else if (ch < (U32Char) 0x10000) {
            bytesToWrite = 3;
        } else if (ch < (U32Char) 0x110000) {
            bytesToWrite = 4;
        } else {
            bytesToWrite = 3;
            ch = UNI_REPLACEMENT_CHAR;
        }

        target += bytesToWrite;
        if (target > targetEnd) {
            source = oldSource; /* Back up source pointer! */
            target -= bytesToWrite;
            result = targetExhausted;
            break;
        }
        assert(target < targetEnd);
        switch (bytesToWrite) { /* note: everything falls through. */
            case 4:
                *--target = (U8Char) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 3:
                *--target = (U8Char) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 2:
                *--target = (U8Char) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 1:
                *--target = (U8Char) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
//     *sourceStart = source;
//     *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF32toUTF8(const U32Char* sourceStart,
                                    const U32Char* sourceEnd,
                                    U8Char* targetStart, U8Char* targetEnd,
                                    ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const U32Char* source = sourceStart;
    U8Char* target = targetStart;
    while (source < sourceEnd) {
        unsigned long ch;
        unsigned short bytesToWrite = 0;
        const U32Char byteMask = 0xBF;
        const U32Char byteMark = 0x80;
        ch = *source++;
        if (flags == strictConversion) {
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                --source; /* return to the illegal value itself */
                result = sourceIllegal;
                break;
            }
        }
        /*
         * Figure out how many bytes the result will require. Turn any
         * illegally large UTF32 things (> Plane 17) into replacement chars.
         */
        if (ch < (U32Char) 0x80) {
            bytesToWrite = 1;
        } else if (ch < (U32Char) 0x800) {
            bytesToWrite = 2;
        } else if (ch < (U32Char) 0x10000) {
            bytesToWrite = 3;
        } else if (ch <= UNI_MAX_LEGAL_UTF32) {
            bytesToWrite = 4;
        } else {
            bytesToWrite = 3;
            ch = UNI_REPLACEMENT_CHAR;
            result = sourceIllegal;
        }

        target += bytesToWrite;
        if (target > targetEnd) {
            --source; /* Back up source pointer! */
            target -= bytesToWrite;
            result = targetExhausted;
            break;
        }
        assert(target < targetEnd);
        switch (bytesToWrite) { /* note: everything falls through. */
            case 4:
                *--target = (U8Char) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 3:
                *--target = (U8Char) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 2:
                *--target = (U8Char) ((ch | byteMark) & byteMask);
                ch >>= 6;
            case 1:
                *--target = (U8Char) (ch | firstByteMark[bytesToWrite]);
        }
        target += bytesToWrite;
    }
//     *sourceStart = source;
//     *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

/*
 * Utility routine to tell whether a sequence of bytes is legal UTF-8.
 * This must be called with the length pre-determined by the first byte.
 * If not calling this from ConvertUTF8to*, then the length can be set by:
 *  length = trailingBytesForUTF8[*source]+1;
 * and the sequence is illegal right away if there aren't that many bytes
 * available.
 * If presented with a length > 4, this returns false.  The Unicode
 * definition of UTF-8 goes up to 4-byte sequences.
 */

static bool isLegalUTF8(const unsigned char* source, int length) {
    unsigned char a;
    const unsigned char *srcptr = source + length;
    switch (length) {
        default:
            return false;
            /* Everything else falls through when "true"... */
        case 4:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;
        case 3:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;
        case 2:
            if ((a = (*--srcptr)) < 0x80 || a > 0xBF)
                return false;

            switch (*source) {
                /* no fall-through in this inner switch */
                case 0xE0:
                    if (a < 0xA0)
                        return false;
                    break;
                case 0xED:
                    if (a > 0x9F)
                        return false;
                    break;
                case 0xF0:
                    if (a < 0x90)
                        return false;
                    break;
                case 0xF4:
                    if (a > 0x8F)
                        return false;
                    break;
                default:
                    if (a < 0x80)
                        return false;
            }

        case 1:
            if (*source >= 0x80 && *source < 0xC2)
                return false;
    }
    if (*source > 0xF4)
        return false;
    return true;
}

/* --------------------------------------------------------------------- */

/*
 * Exported function to return whether a UTF-8 sequence is legal or not.
 * This is not used here; it's just exported.
 */
bool isLegalUTF8Sequence(const unsigned char *source,
                         const unsigned char *sourceEnd) {
    int length = trailingBytesForUTF8[*source] + 1;
    if (length > sourceEnd - source) {
        return false;
    }
    return isLegalUTF8(source, length);
}

/* --------------------------------------------------------------------- */

/*
 * Exported function to return the total number of bytes in a codepoint
 * represented in UTF-8, given the value of the first byte.
 */
unsigned int getNumBytesForUTF8(unsigned char first) {
    return trailingBytesForUTF8[first] + 1;
}

/* --------------------------------------------------------------------- */

/*
 * Exported function to return whether a UTF-8 string is legal or not.
 * This is not used here; it's just exported.
 */
bool isLegalUTF8String(const unsigned char *sourceStart,
                       const unsigned char *sourceEnd) {
    const unsigned char *source = sourceStart;
    while (source != sourceEnd) {
        int length = trailingBytesForUTF8[*source] + 1;
        if (length > sourceEnd - source || !isLegalUTF8(source, length))
            return false;
        source += length;
    }
    return true;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF16(const U8Char* sourceStart,
                                    const U8Char* sourceEnd,
                                    U16Char* targetStart, U16Char* targetEnd,
                                    ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const unsigned char* source = (const unsigned char*) sourceStart;
    U16Char* target = targetStart;
    while (source < (const unsigned char*) sourceEnd) {
        unsigned long ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (extraBytesToRead >= (const unsigned char*) sourceEnd - source) {
            result = sourceExhausted;
            break;
        }
        /* Do this check whether lenient or strict */
        if (!isLegalUTF8(source, extraBytesToRead + 1)) {
            result = sourceIllegal;
            break;
        }
        /*
         * The cases all fall through. See "Note A" below.
         */
        switch (extraBytesToRead) {
            case 5:
                ch += *source++;
                ch <<= 6; /* remember, illegal UTF-8 */
            case 4:
                ch += *source++;
                ch <<= 6; /* remember, illegal UTF-8 */
            case 3:
                ch += *source++;
                ch <<= 6;
            case 2:
                ch += *source++;
                ch <<= 6;
            case 1:
                ch += *source++;
                ch <<= 6;
            case 0:
                ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];

        if (target >= targetEnd) {
            source -= (extraBytesToRead + 1); /* Back up source pointer! */
            result = targetExhausted;
            break;
        }
        if (ch <= UNI_MAX_BMP) { /* Target is a character <= 0xFFFF */
            /* UTF-16 surrogate values are illegal in UTF-32 */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    source -= (extraBytesToRead + 1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = (U16Char) ch; /* normal case */
            }
        } else if (ch > UNI_MAX_UTF16) {
            if (flags == strictConversion) {
                result = sourceIllegal;
                source -= (extraBytesToRead + 1); /* return to the start */
                break; /* Bail out; shouldn't continue */
            } else {
                *target++ = UNI_REPLACEMENT_CHAR;
            }
        } else {
            /* target is a character in range 0xFFFF - 0x10FFFF. */
            if (target + 1 >= targetEnd) {
                source -= (extraBytesToRead + 1); /* Back up source pointer! */
                result = targetExhausted;
                break;
            }
            ch -= halfBase;
            *target++ = (U16Char)((ch >> halfShift) + UNI_SUR_HIGH_START);
            *target++ = (U16Char)((ch & halfMask) + UNI_SUR_LOW_START);
        }
    }
//     *sourceStart = source;
//     *targetStart = target;
    return result;
}

/* --------------------------------------------------------------------- */

ConversionResult ConvertUTF8toUTF32(const U8Char* sourceStart,
                                    const U8Char* sourceEnd,
                                    U32Char* targetStart, U32Char* targetEnd,
                                    ConversionFlags flags) {
    ConversionResult result = conversionOK;
    const unsigned char* source = (const unsigned char*) sourceStart;
    U32Char* target = targetStart;
    while (source < (const unsigned char*) sourceEnd) {
        unsigned long ch = 0;
        unsigned short extraBytesToRead = trailingBytesForUTF8[*source];
        if (extraBytesToRead >= (const unsigned char*) sourceEnd - source) {
            result = sourceExhausted;
            break;
        }
        /* Do this check whether lenient or strict */
        if (!isLegalUTF8(source, extraBytesToRead + 1)) {
            result = sourceIllegal;
            break;
        }
        /*
         * The cases all fall through. See "Note A" below.
         */
        switch (extraBytesToRead) {
            case 5:
                ch += *source++;
                ch <<= 6;
            case 4:
                ch += *source++;
                ch <<= 6;
            case 3:
                ch += *source++;
                ch <<= 6;
            case 2:
                ch += *source++;
                ch <<= 6;
            case 1:
                ch += *source++;
                ch <<= 6;
            case 0:
                ch += *source++;
        }
        ch -= offsetsFromUTF8[extraBytesToRead];

        if (target >= targetEnd) {
            source -= (extraBytesToRead + 1); /* Back up the source pointer! */
            result = targetExhausted;
            break;
        }
        assert(target < targetEnd);
        if (ch <= UNI_MAX_LEGAL_UTF32) {
            /*
             * UTF-16 surrogate values are illegal in UTF-32, and anything
             * over Plane 17 (> 0x10FFFF) is illegal.
             */
            if (ch >= UNI_SUR_HIGH_START && ch <= UNI_SUR_LOW_END) {
                if (flags == strictConversion) {
                    source -= (extraBytesToRead + 1); /* return to the illegal value itself */
                    result = sourceIllegal;
                    break;
                } else {
                    *target++ = UNI_REPLACEMENT_CHAR;
                }
            } else {
                *target++ = ch;
            }
        } else { /* i.e., ch > UNI_MAX_LEGAL_UTF32 */
            result = sourceIllegal;
            *target++ = UNI_REPLACEMENT_CHAR;
        }
    }
//     *sourceStart = source;
//     *targetStart = target;
    return result;
}

/* ---------------------------------------------------------------------

 Note A.
 The fall-through switches in UTF-8 reading code save a
 temp variable, some decrements & conditionals.  The switches
 are equivalent to the following loop:
 {
 int tmpBytesToRead = extraBytesToRead+1;
 do {
 ch += *source++;
 --tmpBytesToRead;
 if (tmpBytesToRead) ch <<= 6;
 } while (tmpBytesToRead > 0);
 }
 In UTF-8 writing code, the switches on "bytesToWrite" are
 similarly unrolled loops.

 --------------------------------------------------------------------- */

std::string TrimLeft(const std::string& str) {
    if (str.empty())
        return str;

    std::string t = str;
    unsigned int i = 0;
    while (i < t.length() && isspace(t[i])) {
        i++;
    }
    t.erase(0, i);
    return t;
}

std::string TrimRight(const std::string& str) {
    if (str.empty())
        return str;

    std::string t = str;
    size_t i = t.length() - 1;

    while (i != (size_t) - 1 && isspace(t[i])) {
        i--;
    }
    t.erase(i + 1, t.length());

    return t;
}

std::string Trim(const std::string& str) {
    if (str.empty())
        return str;
    std::string t = str;

    unsigned int i = 0;
    while (i < t.length() && isspace(t[i])) {
        i++;
    }
    assert(i <= t.length());
    t.erase(0, i);

    if (t.empty())
        return t;

    size_t j = t.length() - 1;
    while (j != (size_t) - 1 && isspace(t[j])) {
        j--;
    }
    assert(j + 1 <= t.length());
    t.erase(j + 1, t.length());

    return t;
}

// 计算UTF8编码的字符串的长度
// From:
// http://stackoverflow.com/questions/5117393/utf-8-strings-length-in-linux-c
size_t strlen_utf8(const U8Char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80)
            j++;
        i++;
    }
    return j;
}

std::vector<std::string> &split(const std::string &s, char delim,
		std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

/*如果你在使用Format时崩溃，请务必保证你试图输入的字符串是C风格的字符串而不是一个类！*/
void stringFormatV(std::string& str, const char *sformat, va_list args_list) {
#if VA_VC_WINCE
    int size = 1023;
    HRESULT ret = S_OK;
    do
    {
        str.resize(size + 1);
        ret = StringCchVPrintfA(&str[0], size + 1, sformat, args_list);
        if (ret == STRSAFE_E_INSUFFICIENT_BUFFER)
        {
            size = size*2;
        }
        assert(ret == S_OK || ret == STRSAFE_E_INSUFFICIENT_BUFFER);
    }while (ret == STRSAFE_E_INSUFFICIENT_BUFFER);
#elif VA_VC
    int size = 0;
    size = _vscprintf(sformat, args_list);  // length after formatting
    str.resize(size + 1);
    HRESULT ret = S_OK;
    ret = StringCchVPrintfA(&str[0], size + 1, sformat, args_list);
    assert(ret == S_OK);
#elif VA_GNUC
    int n = 0;
    {
        va_list aq;
        va_copy(aq,args_list);
        n = vsnprintf(NULL, 0, sformat, aq);
        va_end(aq);
    }
    
    str.resize(n + 1);
    {
        va_list aq2;
        va_copy(aq2,args_list);
        n = vsnprintf(&str[0], n + 1, sformat, aq2);
        va_end(aq2);
    }
#else
#error "This compiler is not supported."
#endif  
}
void stringFormat(std::string& str, const char *sformat, ...) {
    va_list marker;
    va_start(marker, sformat);
    stringFormatV(str, sformat, marker);
    va_end(marker);
}

void stringFormatAppendV(std::string& str, const char *sformat,
                         va_list args_list) {
    std::string stemp;
#if VA_VC_WINCE
    int size = 1023;
    HRESULT ret = S_OK;
    do
    {
        stemp.resize(size + 1);
        ret = StringCchVPrintfA(&stemp[0], size + 1, sformat, args_list);
        if (ret == STRSAFE_E_INSUFFICIENT_BUFFER)
        {
            size = size*2;
        }
        assert(ret == S_OK || ret == STRSAFE_E_INSUFFICIENT_BUFFER);
    }while (ret == STRSAFE_E_INSUFFICIENT_BUFFER);
#elif VA_VC
    HRESULT ret = S_OK;
    int size = 1023;
    size = _vscprintf(sformat, args_list);  // length after formatting
    stemp.resize(size + 1);
    ret = StringCchVPrintfA(&stemp[0], size + 1, sformat, args_list);
    assert(ret == S_OK);
#else
    int n = 0;
    n = vsnprintf(NULL, 0, sformat, args_list);
    stemp.resize(n + 1);
    n = vsnprintf(&stemp[0], n + 1, sformat, args_list);
#endif
    str += stemp;
}

void stringFormatAppend(std::string& str, const char *sformat, ...) {
    va_list marker;
    va_start(marker, sformat);
    stringFormatAppendV(str, sformat, marker);
    va_end(marker);
}
/*如果你在使用Format时崩溃，请务必保证你试图输入的字符串是C风格的字符串而不是一个类！*/
void wstringFormatV(std::wstring& wstr, const wchar_t *wsformat,
                    va_list args_list) {
#if VA_VC_WINCE
    int size = 1023;
    HRESULT ret = S_OK;
    do
    {
        wstr.resize(size + 1);
        ret = StringCchVPrintfW(&wstr[0], size + 1, wsformat, args_list);
        if (ret == STRSAFE_E_INSUFFICIENT_BUFFER)
        {
            size = size*2;
        }
        assert(ret == S_OK || ret == STRSAFE_E_INSUFFICIENT_BUFFER);
    }while (ret == STRSAFE_E_INSUFFICIENT_BUFFER);
#elif VA_VC
    int size = 0;
    size = _vscwprintf(wsformat, args_list);  // length after formatting
    wstr.resize(size + 1);
    HRESULT ret = S_OK;
    ret = StringCchVPrintfW(&wstr[0], size + 1, wsformat, args_list);
    assert(ret == S_OK);
#elif VA_GNUC
    int n = 0;
    n = vswprintf(NULL, 0, wsformat, args_list);
    wstr.resize(n + 1);
    n = vswprintf(&wstr[0], n + 1, wsformat, args_list);
#else
#error "This compiler is not supported."
#endif
}

void wstringFormat(std::wstring& wstr, const wchar_t *wsformat, ...) {
    va_list marker;
    va_start(marker, wsformat);
    wstringFormatV(wstr, wsformat, marker);
    va_end(marker);
}

void wstringFormatAppendV(std::wstring& wstr, const wchar_t *wsformat,
                          va_list args_list) {
    std::wstring stemp;
#if VA_VC_WINCE
    int size = 1023;
    HRESULT ret = S_OK;
    do
    {
        stemp.resize(size + 1);
        ret = StringCchVPrintfW(&stemp[0], size + 1, wsformat, args_list);
        if (ret == STRSAFE_E_INSUFFICIENT_BUFFER)
        {
            size = size*2;
        }
        assert(ret == S_OK || ret == STRSAFE_E_INSUFFICIENT_BUFFER);
    }while (ret == STRSAFE_E_INSUFFICIENT_BUFFER);
#elif VA_VC
    HRESULT ret = S_OK;
    int size = 1023;
    size = _vscwprintf(wsformat, args_list);  // length after formatting
    stemp.resize(size + 1);
    ret = StringCchVPrintfW(&stemp[0], size + 1, wsformat, args_list);
    assert(ret == S_OK);
#else
    int n = 0;
    n = vswprintf(NULL, 0, wsformat, args_list);
    stemp.resize(n + 1);
    n = vswprintf(&stemp[0], n + 1, wsformat, args_list);
#endif
    wstr += stemp;
}

void wstringFormatAppend(std::wstring& wstr, const wchar_t *wsformat, ...) {
    va_list marker;
    va_start(marker, wsformat);
    wstringFormatAppendV(wstr, wsformat, marker);
    va_end(marker);
}

size_t U16CharToU8Char(const U16Char utf16char, U8Char* putf8char) {
    unsigned int ucode = utf16char;
    if (ucode < 0x80) {
        putf8char[0] = (ucode >> 0 & 0x7F) | 0x00;
        return 1;
    } else if (ucode < 0x0800) {
        putf8char[0] = (ucode >> 6 & 0x1F) | 0xC0;
        putf8char[1] = (ucode >> 0 & 0x3F) | 0x80;
        return 2;
    } else if (ucode < 0x010000) {
        putf8char[0] = (ucode >> 12 & 0x0F) | 0xE0;
        putf8char[1] = (ucode >> 6 & 0x3F) | 0x80;
        putf8char[2] = (ucode >> 0 & 0x3F) | 0x80;
        return 3;
    } else if (ucode < 0x110000) {
        putf8char[0] = (ucode >> 18 & 0x07) | 0xF0;
        putf8char[1] = (ucode >> 12 & 0x3F) | 0x80;
        putf8char[2] = (ucode >> 6 & 0x3F) | 0x80;
        putf8char[3] = (ucode >> 0 & 0x3F) | 0x80;
        return 4;
    }
    return 0;
}

size_t U8CharToU16Char(const U8Char *byte, U16Char& utf16char) {
    const unsigned char *pU8Char = (const unsigned char *) byte;
    if (!pU8Char[0])
        return 0;
    if ((pU8Char[0] & 0x80) == 0x0) {  //one byte
        utf16char = pU8Char[0];
        return 1;
    } else if ((pU8Char[0] & 0xE0) == 0xC0) {  //two bytes
        unsigned long TEMP0 = pU8Char[0];
        unsigned long TEMP1 = pU8Char[1];
        utf16char = ((TEMP0 & 0x1F) << 6) | (TEMP1 & 0x3F);
        return 2;
    } else if ((pU8Char[0] & 0xF0) == 0xE0) {  //three bytes
        unsigned long TEMP0 = pU8Char[0];
        unsigned long TEMP1 = pU8Char[1];
        unsigned long TEMP2 = pU8Char[2];
        utf16char = ((TEMP0 & 0x0F) << 12) | ((TEMP1 & 0x3F) << 6)
                | (TEMP2 & 0x3F);
        return 3;
    } else if ((pU8Char[0] & 0xF8) == 0xF0) {  //four bytes
        unsigned long TEMP0 = pU8Char[0];
        unsigned long TEMP1 = pU8Char[1];
        unsigned long TEMP2 = pU8Char[2];
        unsigned long TEMP3 = pU8Char[3];
        utf16char = ((TEMP0 & 0x07) << 18) | ((TEMP1 & 0x3F) << 12)
                | ((TEMP2 & 0x3F) << 6) | (TEMP3 & 0x3F);
        return 4;
    } else if ((pU8Char[0] & 0xFC) == 0xF8) {  //five bytes
        unsigned long TEMP0 = pU8Char[0];
        unsigned long TEMP1 = pU8Char[1];
        unsigned long TEMP2 = pU8Char[2];
        unsigned long TEMP3 = pU8Char[3];
        unsigned long TEMP4 = pU8Char[4];
        utf16char = ((TEMP0 & 0x03) << 24) | ((TEMP1 & 0x3F) << 18)
                | ((TEMP2 & 0x3F) << 12) | ((TEMP3 & 0x3F) << 6)
                | (TEMP4 & 0x3F);
        return 5;
    } else if ((pU8Char[0] & 0xFE) == 0xFC) {  //six bytes
        unsigned long TEMP0 = pU8Char[0];
        unsigned long TEMP1 = pU8Char[1];
        unsigned long TEMP2 = pU8Char[2];
        unsigned long TEMP3 = pU8Char[3];
        unsigned long TEMP4 = pU8Char[4];
        unsigned long TEMP5 = pU8Char[5];
        utf16char = ((TEMP0 & 0x01) << 30) | ((TEMP1 & 0x3F) << 24)
                | ((TEMP2 & 0x3F) << 18) | ((TEMP3 & 0x3F) << 12)
                | ((TEMP4 & 0x3F) << 6) | (TEMP5 & 0x3F);
        return 6;
    }
    return 0;
}
size_t U32CharToU8Char(const U32Char utf32char, U8Char* putf8char) {
    int count;
    U32Char temputf32char = utf32char;
    if (temputf32char < 0x80)
        count = 1;
    else if (temputf32char < 0x800)
        count = 2;
    else if (temputf32char < 0x10000)
        count = 3;
    else if (temputf32char < 0x200000)
        count = 4;
    else if (temputf32char < 0x4000000)
        count = 5;
    else if (temputf32char <= 0x7fffffff)
        count = 6;
    else
        return 0;

    switch (count) {
        case 6:
            putf8char[5] = 0x80 | (temputf32char & 0x3f);
            temputf32char = temputf32char >> 6;
            temputf32char |= 0x4000000;
        case 5:
            putf8char[4] = 0x80 | (temputf32char & 0x3f);
            temputf32char = temputf32char >> 6;
            temputf32char |= 0x200000;
        case 4:
            putf8char[3] = 0x80 | (temputf32char & 0x3f);
            temputf32char = temputf32char >> 6;
            temputf32char |= 0x10000;
        case 3:
            putf8char[2] = 0x80 | (temputf32char & 0x3f);
            temputf32char = temputf32char >> 6;
            temputf32char |= 0x800;
        case 2:
            putf8char[1] = 0x80 | (temputf32char & 0x3f);
            temputf32char = temputf32char >> 6;
            temputf32char |= 0xc0;
        case 1:
            putf8char[0] = (U8Char) temputf32char;
    }
    return count;
}
size_t U8CharToU32Char(const U8Char* putf8char, U32Char& utf32char) {
    unsigned char c = putf8char[0];

    if (c < 0x80) {
        utf32char = c;
        return 1;
    } else if (c < 0xc2) {
        return 0;
    } else if (c < 0xe0) {
        if (!((putf8char[1] ^ 0x80) < 0x40))
            return 0;
        utf32char = (((U32Char) c & 0x1f) << 6)
                | ((U32Char) putf8char[1] ^ 0x80);
        return 2;
    } else if (c < 0xf0) {
        if (!((putf8char[1] ^ 0x80) < 0x40 && (putf8char[2] ^ 0x80) < 0x40
                && (c >= 0xe1 || putf8char[1] >= 0xa0)))
            return 0;
        utf32char = (((U32Char) c & 0x0f) << 12)
                | (((U32Char) putf8char[1] ^ 0x80) << 6)
                | ((U32Char) putf8char[2] ^ 0x80);
        return 3;
    } else if (c < 0xf8) {
        if (!((putf8char[1] ^ 0x80) < 0x40 && (putf8char[2] ^ 0x80) < 0x40
                && (putf8char[3] ^ 0x80) < 0x40
                && (c >= 0xf1 || putf8char[1] >= 0x90)))
            return 0;
        utf32char = (((U32Char) c & 0x07) << 18)
                | (((U32Char) putf8char[1] ^ 0x80) << 12)
                | (((U32Char) putf8char[2] ^ 0x80) << 6)
                | ((U32Char) putf8char[3] ^ 0x80);
        return 4;
    } else if (c < 0xfc) {
        if (!((putf8char[1] ^ 0x80) < 0x40 && (putf8char[2] ^ 0x80) < 0x40
                && (putf8char[3] ^ 0x80) < 0x40 && (putf8char[4] ^ 0x80) < 0x40
                && (c >= 0xf9 || putf8char[1] >= 0x88)))
            return 0;
        utf32char = (((U32Char) c & 0x03) << 24)
                | (((U32Char) putf8char[1] ^ 0x80) << 18)
                | (((U32Char) putf8char[2] ^ 0x80) << 12)
                | (((U32Char) putf8char[3] ^ 0x80) << 6)
                | ((U32Char) putf8char[4] ^ 0x80);
        return 5;
    } else if (c < 0xfe) {
        if (!((putf8char[1] ^ 0x80) < 0x40 && (putf8char[2] ^ 0x80) < 0x40
                && (putf8char[3] ^ 0x80) < 0x40 && (putf8char[4] ^ 0x80) < 0x40
                && (putf8char[5] ^ 0x80) < 0x40
                && (c >= 0xfd || putf8char[1] >= 0x84)))
            return 0;
        utf32char = ((U32Char)(c & 0x01) << 30)
                | (((U32Char) putf8char[1] ^ 0x80) << 24)
                | (((U32Char) putf8char[2] ^ 0x80) << 18)
                | (((U32Char) putf8char[3] ^ 0x80) << 12)
                | (((U32Char) putf8char[4] ^ 0x80) << 6)
                | ((U32Char) putf8char[5] ^ 0x80);
        return 6;
    } else
        return 0;
}
size_t U32CharToU16Char(const U32Char utf32char, U16Char* putf16char) {
    if (utf32char <= 0xFFFF) {
        if (putf16char != NULL) {
            *putf16char = (U16Char) utf32char;
        }

        return 1;
    } else if (utf32char <= 0xEFFFF) {
        if (putf16char != NULL) {
            putf16char[0] = (U16Char)(0xD800 + (utf32char >> 10) - 0x40);  // 高10位
            putf16char[1] = (U16Char)(0xDC00 + (utf32char & 0x03FF));   // 低10位
        }
        return 2;
    } else {
        return 0;
    }
}
size_t U16CharToU32Char(const U16Char* putf16char, U32Char& utf32char) {
    U16Char w1, w2;

    if (putf16char == NULL) {   // 参数错误
        return 0;
    }
    w1 = putf16char[0];
    if (w1 >= 0xD800 && w1 <= 0xDFFF) {   // 编码在替代区域（Surrogate Area）
        if (w1 < 0xDC00) {
            w2 = putf16char[1];
            if (w2 >= 0xDC00 && w2 <= 0xDFFF) {
                utf32char = (w2 & 0x03FF) + (((w1 & 0x03FF) + 0x40) << 10);
                return 2;
            }
        }

        return 0;   // 非法UTF16编码
    } else {
        utf32char = w1;
        return 1;
    }
}

#include "codeTable.h"
int IsGBKChar(unsigned short code) {
    int m, h, l, found;
    m = 0;
    l = 0;
    h = sizeof(GBK2UC) / sizeof(GBK2UC[0]) - 1;
    found = -1;

    while (l <= h && found < 0) {
        m = (l + h) >> 1;
        if (code > GBK2UC[m].GBKCode) {
            l = m + 1;
        } else if (code < GBK2UC[m].GBKCode) {
            h = m - 1;
        } else {
            found = m;
        }
    }
    return found;
}
int IsUTF16Char(U16Char code) {
    int m, h, l, found;

    m = 0;
    l = 0;
    h = sizeof(UC2GBK) / sizeof(UC2GBK[0]) - 1;
    found = -1;

    while (l <= h && found < 0) {
        m = (l + h) >> 1;
        if (code > UC2GBK[m].UniCode) {
            l = m + 1;
        } else if (code < UC2GBK[m].UniCode) {
            h = m - 1;
        } else {
            found = m;
        }
    }
    return found;
}

size_t U16CharToGBKChar(const U16Char utf16char, char* pgbkchar) {
    if ((unsigned int) utf16char < 128) {
        pgbkchar[0] = (char) utf16char;
        return 1;
    }

    unsigned short gbk = 0;
    int found = IsUTF16Char(utf16char);
    if (found < 0) {
        return 0;
    } else {
        gbk = UC2GBK[found].GBKCode;
        pgbkchar[0] = (gbk & 0xFF00) >> 8;
        pgbkchar[1] = gbk & 0x00FF;
        return 2;
    }
}

size_t GBKCharToU16Char(const char* byte, U16Char& utf16char) {
    if ((unsigned int) byte[0] < 128) {
        utf16char = byte[0];
        return 1;
    }

    unsigned short gcode;
    /* for little-endian system */
    gcode = ((unsigned char) byte[0] << 8) | ((unsigned char) byte[1]);
    int found = IsGBKChar(gcode);
    if (found < 0) {
        return 0;
    } else {
        utf16char = GBK2UC[found].UniCode;
        return 2;
    }
}

//U16StrToU8Str
U16StrToU8Str::U16StrToU8Str(const U16Char* s) {
#if VA_VC
    size_t l = wcslen(s);
#else
    size_t l = ustrlen(s);
#endif
    m_s = new U8Char[l * 6 + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF16toUTF8(s, s + l, m_s, m_s + l * 6 + 1, lenientConversion);
    assert(conversionOK == ret);
}
U16StrToU8Str::~U16StrToU8Str() {
    delete[] m_s;
    m_s = NULL;
}
U16StrToU8Str::operator const U8Char*() const {
    return m_s;
}
//U8StrToU16Str
U8StrToU16Str::U8StrToU16Str(const U8Char* s) {
    size_t l = strlen(s);
    m_s = new U16Char[l + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF8toUTF16(s, s + l + 1, m_s, m_s + l + 1, lenientConversion);
    assert(conversionOK == ret);
}
U8StrToU16Str::~U8StrToU16Str() {
    delete[] m_s;
    m_s = NULL;
}
U8StrToU16Str::operator const U16Char*() const {
    return m_s;
}
//U32StrToU8Str
U32StrToU8Str::U32StrToU8Str(const U32Char* s) {
#ifdef __linux
    size_t l = wcslen((const wchar_t*)s);
#else
    size_t l = ustrlen(s);
#endif
    m_s = new U8Char[l * 6 + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF32toUTF8(s, s + l, m_s, m_s + l * 6 + 1, lenientConversion);
    assert(conversionOK == ret);
}
U32StrToU8Str::~U32StrToU8Str() {
    delete[] m_s;
    m_s = NULL;
}
U32StrToU8Str::operator const U8Char*() const {
    return m_s;
}
//U8StrToU32Str
U8StrToU32Str::U8StrToU32Str(const U8Char* s) {
    size_t l = strlen_utf8(s);
    size_t lb = strlen(s);
    m_s = new U32Char[l + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF8toUTF32(s, s + lb + 1, m_s, m_s + l + 1, lenientConversion);
    assert(conversionOK == ret);
}
U8StrToU32Str::~U8StrToU32Str() {
    delete[] m_s;
    m_s = NULL;
}
U8StrToU32Str::operator const U32Char*() const {
    return m_s;
}
//U32StrToU16Str
U32StrToU16Str::U32StrToU16Str(const U32Char* s) {
#ifdef __linux
    size_t l = wcslen((const wchar_t*)s);
#else
    size_t l = ustrlen(s);
#endif
    m_s = new U16Char[l * 2 + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF32toUTF16(s, s + l, m_s, m_s + l * 2 + 1,
                              lenientConversion);
    assert(conversionOK == ret);
}
U32StrToU16Str::~U32StrToU16Str() {
    delete[] m_s;
    m_s = NULL;
}
U32StrToU16Str::operator const U16Char*() const {
    return m_s;
}
//U16StrToU32Str
U16StrToU32Str::U16StrToU32Str(const U16Char* s) {
#ifdef _WIN32
    size_t l = wcslen(s);
#else
    size_t l = ustrlen(s);
#endif
    m_s = new U32Char[l + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF16toUTF32(s, s + l, m_s, m_s + l + 1, lenientConversion);
    assert(conversionOK == ret);
}
U16StrToU32Str::~U16StrToU32Str() {
    delete[] m_s;
    m_s = NULL;
}
U16StrToU32Str::operator const U32Char*() const {
    return m_s;
}
//GBKStrToU8Str
GBKStrToU8Str::GBKStrToU8Str(const char* s) {
    size_t len = strlen(s);
    U16Char* tmp = new U16Char[len + 1]();
    size_t i = 0, j = 0, n = 0;
    ;
    for (; i < len; i += n, j++) {
        assert(i < len);
        n = GBKCharToU16Char(s + i, tmp[j]);
        if (n > 0)
            continue;
        else
            break;
    }
    assert(j < len + 1);
    assert(tmp[j] == 0);
    //tmp[j] = 0;
#if VA_VC
    len = wcslen(tmp);
#else
    len = ustrlen(tmp);
#endif
    m_s = new U8Char[len * 6 + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF16toUTF8(tmp, tmp + len, m_s, m_s + len * 6 + 1,
                             lenientConversion);
    assert(conversionOK == ret);
    delete[] tmp;
}
GBKStrToU8Str::~GBKStrToU8Str() {
    delete[] m_s;
    m_s = NULL;
}
GBKStrToU8Str::operator const U8Char*() const {
    return m_s;
}
//GBKStrToU16Str
GBKStrToU16Str::GBKStrToU16Str(const char* s) {
    size_t len = strlen(s);
    m_s = new U16Char[len + 1]();

    size_t i = 0, j = 0, n = 0;
    for (; i < len; i += n, j++) {
        assert(i < len);
        n = GBKCharToU16Char(s + i, m_s[j]);
        if (n > 0)
            continue;
        else
            break;
    }
    assert(j < len + 1);
    assert(m_s[j] == 0);
    //m_s[j] = 0;
}

GBKStrToU16Str::~GBKStrToU16Str() {
    delete[] m_s;
    m_s = NULL;
}

GBKStrToU16Str::operator const U16Char*() const {
    return m_s;
}
//U8StrToGBKStr
U8StrToGBKStr::U8StrToGBKStr(const char* s) {
    size_t len = strlen(s);
    U16Char* tmp = new U16Char[len + 1]();
    ConversionResult ret = conversionOK;
    ret = ConvertUTF8toUTF16(s, s + len + 1, tmp, tmp + len + 1,
                             lenientConversion);
    assert(conversionOK == ret);

#if VA_VC
    len = wcslen(tmp);
#else
    len = ustrlen(tmp);
#endif
    m_s = new char[len * 2 + 1]();

    size_t i = 0, j = 0;
    for (; i < len; i++) {
        j += U16CharToGBKChar(tmp[i], m_s + j);
    }
    assert(j < len * 2 + 1);
    assert(m_s[j] == 0);
    //m_s[j] = 0;
    delete[] tmp;
}
U8StrToGBKStr::~U8StrToGBKStr() {
    delete[] m_s;
    m_s = NULL;
}
U8StrToGBKStr::operator const char*() const {
    return m_s;
}

//U16StrToGBKStr
U16StrToGBKStr::U16StrToGBKStr(const U16Char* s) {
#if VA_VC
    size_t len = wcslen(s);
#else
    size_t len = ustrlen(s);
#endif
    m_s = new char[len * 2 + 1]();

    size_t i = 0, j = 0;
    for (; i < len; i++) {
        j += U16CharToGBKChar(s[i], m_s + j);
    }
    assert(j < len * 2 + 1);
    assert(m_s[j] == 0);
    //m_s[j] = 0;
}
U16StrToGBKStr::~U16StrToGBKStr() {
    delete[] m_s;
    m_s = NULL;
}
U16StrToGBKStr::operator const char*() const {
    return m_s;
}

U8Char* U8String::__StrRStr(const U8Char* string, const U8Char* lpszSub) const {
    U8Char *r = NULL;

    if (!lpszSub[0])
        return (U8Char*) string + strlen(string);
    for (;;) {
        U8Char *p = (U8Char *) strstr(string, lpszSub);
        if (!p)
            return r;
        r = p;
        string = p + 1;
    }
//    return NULL;
}
U8Char U8String::__Reverse(U8Char ch) {
    if (isupper(ch)) {
        return (U8Char) tolower(ch);
    }
    if (islower(ch)) {
        return (U8Char) toupper(ch);
    }
    return ch;
}

//protected:
void U8String::__ConcatCopy(const U8Char* str1, size_t nLen1,
                            const U8Char* str2, size_t nLen2) {
    if (m_s) {
        delete[] m_s;
        m_s = NULL;
        m_l = 0;
    }

    m_l = nLen1 + nLen2;

    try {
        m_s = new U8Char[m_l + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        m_l = 0;
        throw;
        return;
    }
#if VA_VC
    if (nLen1 > 0)
    StringCchCopyNA(m_s, m_l + 1, str1, nLen1);
    if (nLen2 > 0)
    StringCchCatNA(m_s, m_l + 1, str2, nLen2);
#else
    if (nLen1 > 0)
        strncpy(m_s, str1, nLen1);
    if (nLen2 > 0)
        strncat(m_s, str2, nLen2);
#endif
    m_s[m_l] = 0;
}

U8String::U8String(const U8String& stringSrc)/*拷贝构造*/
        : m_s(NULL),
          m_l(0) {
    if (stringSrc.m_s) {
        size_t l0 = stringSrc.m_l;
        try {
            m_s = new U8Char[l0 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
#if VA_VC
        StringCchCopyNA(m_s, l0 + 1, stringSrc.m_s, l0);
#else
        strncpy(m_s, stringSrc.m_s, l0);
#endif
        m_s[l0] = 0;
        m_l = l0;
    }
}
U8String::U8String(U8Char ch, unsigned int nRepeat)
        : m_s(NULL),
          m_l(0) {
    size_t l0 = nRepeat;
    try {
        m_s = new U8Char[l0 + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    U8Char* p = m_s;
    while (nRepeat && (*p++ = ch) != 0)
        nRepeat--;

    m_s[l0] = 0;
    m_l = l0;
}

U8String::U8String(const U8Char* lpsz, unsigned int nLength)
        : m_s(NULL),
          m_l(0) {
    if (lpsz) {
        size_t l0 = nLength ? nLength : strlen(lpsz);
        try {
            m_s = new U8Char[l0 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
#if VA_VC
        StringCchCopyNA(m_s, l0 + 1, lpsz, l0);
#else
        strncpy(m_s, lpsz, l0);
#endif
        m_s[l0] = 0;
        m_l = l0;
    }
}

U8String::U8String(const std::string& stringSrc)
        : m_s(NULL),
          m_l(0) {
    if (stringSrc.empty()) {
        m_s = NULL;
        m_l = 0;
    } else {
        size_t l0 = stringSrc.length();
        try {
            m_s = new U8Char[l0 + 1];
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
#if VA_VC
        StringCchCopyNA(m_s, l0+1, stringSrc.c_str(), l0);
#else
        strncpy(m_s, stringSrc.c_str(), l0);
#endif
        m_s[l0] = 0;
        m_l = l0;
    }
}

U8String::U8String(const U16String& u16stringSrc)
        : m_s(NULL),
          m_l(0) {
    fromU16String(u16stringSrc);
}
U8String::U8String(const U16Char* lpsz)
        : m_s(NULL),
          m_l(0) {
    fromU16String(lpsz);
}

const U8String& U8String::operator=(const U8String& stringSrc) {
    if (m_s)
        delete[] m_s;
    m_s = NULL;
    m_l = 0;

    if (stringSrc.m_s) {
        try {
            m_s = new U8Char[stringSrc.m_l + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
#if VA_VC
        StringCchCopyNA(m_s, stringSrc.m_l+1, stringSrc.m_s, stringSrc.m_l);
#else
        strncpy(m_s, stringSrc.m_s, stringSrc.m_l);
#endif
        m_s[stringSrc.m_l] = 0;
        m_l = stringSrc.m_l;
    }
    return *this;
}
const U8String& U8String::operator=(U8Char ch) {
    if (m_s)
        delete[] m_s;
    m_s = NULL;
    m_l = 0;

    try {
        m_s = new U8Char[2]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
    }
    m_s[0] = ch;
    m_s[1] = 0;
    m_l = 1;
    return *this;
}
const U8String& U8String::operator=(const U8Char* lpsz) {
    if (m_s)
        delete[] m_s;
    m_s = NULL;
    m_l = 0;

    if (lpsz) {
        size_t l0 = strlen(lpsz);
        try {
            m_s = new U8Char[l0 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
#if VA_VC
        StringCchCopyNA(m_s, l0+1, lpsz, l0);
#else
        strncpy(m_s, lpsz, l0);
#endif
        m_s[l0] = 0;
        m_l = l0;
    }
    return *this;
}
const U8String& U8String::operator=(const U16String& u16stringSrc) {
    fromU16String(u16stringSrc);
    return *this;
}
const U8String& U8String::operator=(const U16Char* lpsz) {
    fromU16String(lpsz);
    return *this;
}
const U8String& U8String::operator=(const std::string& stringSrc) {
    if (stringSrc.empty()) {
        this->m_s = NULL;
        this->m_l = 0;
    } else {
        m_l = stringSrc.length();
        try {
            this->m_s = new U8Char[this->m_l + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
#if VA_VC
        StringCchCopyNA(this->m_s, this->m_l + 1, stringSrc.c_str(), this->m_l);
#else
        strncpy(this->m_s, stringSrc.c_str(), this->m_l);
#endif
        m_s[m_l] = 0;
    }
    return *this;
}

U8String::operator const U8Char*() const {
    return m_s;
}
U8String::operator U8Char*() const {
    return m_s;
}
U8String::operator const unsigned char*() const {
    return (const unsigned char*) m_s;
}
U8String::operator unsigned char*() const {
    return (unsigned char*) m_s;
}

const U8String& U8String::operator+=(const U8String& string) {
    if (m_s) {
        if (string.m_s) {
            U8Char* s0 = m_s;
            size_t l0 = m_l;
            U8Char* s1;
            size_t l1 = m_l + string.m_l;
            try {
                s1 = new U8Char[l1 + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
#if VA_VC
            StringCchCopyNA(s1, l1 + 1, s0, l0);
            StringCchCatNA(s1, l1 + 1, string.m_s, string.m_l);
#else
            strncpy(s1, s0, l0);
            strncat(s1, string.m_s, string.m_l);
#endif
            s1[l1] = 0;
            m_s = s1;
            m_l = l1;
            delete[] s0;
        }
    } else {
        if (string.m_s) {
            U8Char* s1;
            try {
                s1 = new U8Char[string.m_l + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
#if VA_VC
            StringCchCopyNA(s1, string.m_l + 1, string.m_s, string.m_l);
#else
            strncpy(s1, string.m_s, string.m_l);
#endif
            s1[string.m_l] = 0;
            m_s = s1;
            m_l = string.m_l;
        }
    }
    return *this;
}
const U8String& U8String::operator+=(U8Char ch) {
    if (m_s) {
        U8Char* s0 = m_s;
        size_t l0 = m_l;
        U8Char* s1;
        size_t l1 = m_l + 1;
        try {
            s1 = new U8Char[l1 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
#if VA_VC
        StringCchCopyNA(s1, l1 + 1, s0, l0);
#else
        strncpy(s1, s0, l0);
#endif
        s1[l0] = ch;
        s1[l1] = 0;
        m_s = s1;
        m_l = l1;
        delete[] s0;
    } else {
        U8Char* s1;
        size_t l1 = 1;
        try {
            s1 = new U8Char[l1 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
        s1[0] = ch;
        s1[1] = 0;
        m_s = s1;
        m_l = l1;
    }
    return *this;
}
const U8String& U8String::operator+=(const U8Char* lpsz) {
    if (m_s) {
        if (lpsz) {
            U8Char* s0 = m_s;
            size_t l0 = m_l;
            U8Char* s1;
            size_t l1 = strlen(lpsz) + l0;
            try {
                s1 = new U8Char[l1 + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
#if VA_VC
            StringCchCopyNA(s1, l1 + 1, s0, l0);
            StringCchCatNA(s1, l1 + 1, lpsz, l1 - l0);
#else
            strncpy(s1, s0, l0);
            strncat(s1, lpsz, l1 - l0);
#endif
            s1[l1] = 0;
            m_s = s1;
            m_l = l1;
            delete[] s0;
        }
    } else {
        if (lpsz) {
            U8Char* s1;
            size_t l1 = strlen(lpsz);
            try {
                s1 = new U8Char[l1 + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
#if VA_VC
            StringCchCopyNA(s1, l1 + 1, lpsz, l1);
#else
            strncpy(s1, lpsz, l1);
#endif
            s1[l1] = 0;
            m_s = s1;
            m_l = l1;
        }
    }
    return *this;
}

U8Char U8String::operator [](unsigned int nIndex) const {
    assert(nIndex <= m_l);
    return m_s[nIndex];
}
U8Char& U8String::operator [](unsigned int nIndex) {
    assert(nIndex <= m_l);
    return m_s[nIndex];
}

U8String::~U8String() {
    if (m_s) {
        memset(m_s, 0, sizeof(U8Char) * (m_l + 1));
        delete[] m_s;
        m_s = NULL;
    }
    m_l = 0;
}

//Return: 0 ==; <0 String<lpsz; >0 String>lpsz;
int U8String::Compare(const U8Char *lpsz) const {
    return strcmp(m_s, lpsz);
}
int U8String::CompareNoCase(const U8Char *lpsz) const {
#if VA_VC
    return _stricmp(m_s, lpsz);
#else
    return strcasecmp(m_s, lpsz);
#endif
}
size_t U8String::Delete(size_t iIndex, size_t nCount) {
    if (iIndex + nCount <= m_l) {
        size_t nNewLength = m_l - nCount;
        size_t nCharsToCopy = m_l - iIndex - nCount;
        memmove(m_s + iIndex, m_s + iIndex + nCount,
                nCharsToCopy * sizeof(U8Char));
        m_s[nNewLength] = 0;
        m_l = nNewLength;
    }
    return m_l;
}
void U8String::Empty() {
    if (m_s) {
        memset(m_s, 0, sizeof(U8Char) * (m_l + 1));
        delete[] m_s;
        m_s = NULL;
    }
    m_l = 0;
}

int U8String::Find(U8Char ch) const {
    return Find(ch, 0);
}
int U8String::Find(const U8Char* lpszSub) const {
    return Find(lpszSub, 0);
}
int U8String::Find(U8Char ch, int nStart) const {
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    U8Char* lpsz = strchr(m_s + nStart, ch);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
int U8String::Find(const U8Char* lpszSub, int nStart) const {
    assert(lpszSub);
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    U8Char* lpsz = strstr(m_s + nStart, lpszSub);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
int U8String::FindOneOf(const U8Char* lpCharsSet) const {
    return FindOneOf(lpCharsSet, 0);
}
int U8String::FindOneOf(const U8Char* lpCharsSet, int nStart) const {
    assert(lpCharsSet);
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    U8Char* lpsz = strpbrk(m_s + nStart, lpCharsSet);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
/*如果你在使用Format时崩溃，请务必保证你试图输入的字符串是C风格的字符串而不是一个类！*/
/*例如：X.Format("%s", Y.GetBuffer()); X.Format("%s", Y.c_str());*/
void U8String::Format(const U8Char* lpszFormat, ...) {
    //assert(false);  //Not implemented
    va_list appList;
    va_start(appList, lpszFormat);
    FormatV(lpszFormat, appList);
    va_end(appList);
}
void U8String::FormatV(const U8Char* lpszFormat, va_list argList) {
#if VA_VC_WINCE
    int l = 1023;
    HRESULT ret = S_OK;
    do
    {
        if (l > 0) {
            if (m_s)
            delete[] m_s;
            m_s = NULL;
            m_l = 0;

            try {
                m_s = new U8Char[l + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
                return;
            }
            ret = StringCchVPrintfA(m_s, l + 1, lpszFormat, argList);
            if (ret == STRSAFE_E_INSUFFICIENT_BUFFER)
            {
                l = l*2;
            }
            assert(ret == S_OK || ret == STRSAFE_E_INSUFFICIENT_BUFFER);
        }
    }while (ret == STRSAFE_E_INSUFFICIENT_BUFFER);
    m_l = strlen(m_s);
#elif VA_VC
    int l = 0;
    l = _vscprintf(lpszFormat, argList);  // length after formatting
    if (l > 0) {
        if (m_s)
        delete[] m_s;
        m_s = NULL;
        m_l = 0;

        try {
            m_s = new U8Char[l + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
        HRESULT ret = S_OK;
        ret = StringCchVPrintfA(m_s, l + 1, lpszFormat, argList);
        assert(ret == S_OK);
        m_l = l;
    }
#elif VA_GNUC
    int l = 0;
    l = vsnprintf(NULL, 0, lpszFormat, argList);
    if (l > 0) {
        if (m_s)
        delete[] m_s;
        m_s = NULL;
        m_l = 0;

        try {
            m_s = new U8Char[l + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
        l = vsnprintf(m_s, l + 1, lpszFormat, argList);
        m_l = l;
    }
#endif
}

U8Char U8String::GetAt(int nIndex) const {
    return m_s[nIndex];
}
const U8Char* U8String::GetBuffer() const {
    return m_s;
}
size_t U8String::GetLength() const {
    return m_l;
}

size_t U8String::Insert(size_t iIndex, U8Char ch) {
    assert(m_s != NULL && iIndex < m_l);
    assert(ch != 0);

    size_t nNewLength = m_l + 1;

    U8Char* tmp;
    try {
        tmp = new U8Char[nNewLength + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
    }
#if VA_VC
    StringCchCopyNA(tmp, nNewLength + 1, m_s, iIndex);
#else
    strncpy(tmp, m_s, iIndex);
#endif
    tmp[iIndex] = ch;
#if VA_VC
    StringCchCopyNA(tmp + iIndex + 1, nNewLength - iIndex, m_s + iIndex, m_l - iIndex + 1);
#else
    strncpy(tmp + iIndex + 1, m_s + iIndex, m_l - iIndex + 1);
#endif
    tmp[nNewLength] = 0;

    delete[] m_s;
    m_s = tmp;
    m_l = nNewLength;
    return m_l;
}
size_t U8String::Insert(size_t iIndex, const U8Char* lpsz) {
    assert(m_s != NULL && iIndex < m_l);
    assert(lpsz != NULL);

    size_t nCHARsToCopy = strlen(lpsz);
    size_t nNewLength = m_l + nCHARsToCopy;

    U8Char* tmp;
    try {
        tmp = new U8Char[nNewLength + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
    }
#if VA_VC
    StringCchCopyNA(tmp, nNewLength + 1, m_s, iIndex);
    StringCchCopyNA(tmp + iIndex, nNewLength + 1 - iIndex, lpsz, nCHARsToCopy);
    StringCchCopyNA(tmp + iIndex + nCHARsToCopy, nNewLength + 1 - iIndex - nCHARsToCopy, m_s + iIndex, m_l - iIndex + 1);
#else
    strncpy(tmp, m_s, iIndex);
    strncpy(tmp + iIndex, lpsz, nCHARsToCopy);
    strncpy(tmp + iIndex + nCHARsToCopy, m_s + iIndex, m_l - iIndex + 1);
#endif
    tmp[nNewLength] = 0;

    delete[] m_s;
    m_s = tmp;
    m_l = nNewLength;
    return *this;
}

bool U8String::IsEmpty() const {
    return GetLength() == 0;
}
bool U8String::IsNull() const {
    return m_s == NULL;
}
U8String::operator bool() const {
    return m_s != NULL;
}
U8String U8String::Left(int nCount) const {
    return Mid(0, nCount);
}
void U8String::MakeLower() {
    for (unsigned int i = 0; i <= m_l; ++i)
        m_s[i] = (U8Char) tolower(m_s[i]);
}
void U8String::MakeReverse() {
    for (unsigned int i = 0; i <= m_l; ++i)
        m_s[i] = __Reverse(m_s[i]);
}
void U8String::MakeUpper() {
    for (unsigned int i = 0; i <= m_l; ++i)
        m_s[i] = (U8Char) toupper(m_s[i]);
}
U8String U8String::Mid(size_t nFirst) const {
    return Mid(nFirst, m_l - nFirst);
}
U8String U8String::Mid(size_t nFirst, size_t nCount) const {
    assert(nFirst >= 0 && nFirst < m_l);
    assert(nCount > 0 && nCount <= m_l);
    U8Char* s = (U8Char*) alloca((nCount + 1) * sizeof(U8Char));
#if VA_VC
    StringCchCopyNA(s, nCount + 1, m_s + nFirst, nCount);
#else
    strncpy(s, m_s + nFirst, nCount * sizeof(U8Char));
#endif
    s[nCount] = 0;
    return s;
}
size_t U8String::Remove(U8Char ch) {
    for (size_t i = 0; i < m_l; i++)
        if (m_s[i] == ch)
            Delete(i);
    return m_l;
}
size_t U8String::Replace(U8Char chOld, U8Char chNew) {
    assert(chOld != 0);
    assert(chNew != 0);
    for (U8Char* p = m_s; *p; ++p)
        if (*p == chOld)
            *p = chNew;
    return m_l;
}
// 	int U8String::Replace(const U8Char* lpszOld, const U8Char* lpszNew) {
// 		assert(false);
// 		return -1;
// 	}

int U8String::ReverseFind(U8Char ch) const {
    return ReverseFind(ch, 0);
}
int U8String::ReverseFind(const U8Char* lpszSub) const {
    return ReverseFind(lpszSub, 0);
}
int U8String::ReverseFind(U8Char ch, size_t nStart) const {
    if (nStart >= m_l)
        return -1;
    U8Char* lpsz = strrchr(m_s + nStart, ch);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
int U8String::ReverseFind(const U8Char* lpszSub, size_t nStart) const {
    assert(lpszSub);
    if (nStart >= m_l)
        return -1;
    U8Char* lpsz = __StrRStr(m_s + nStart, lpszSub);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}

U8String U8String::Right(int nCount) const {
    return Mid(m_l - nCount, nCount);
}
void U8String::SetAt(unsigned int nIndex, U8Char ch) const {
    assert(nIndex <= m_l);
    assert(ch != 0);
    m_s[nIndex] = ch;
}
// 	U8String U8String::SpanExcluding(const U8Char lpszCharSet) const {
// 		//Not implemented
// 		assert(false);
// 		return -1;
// 	}
// 	U8String U8String::SpanIncluding(const U8Char lpszCharSet) const {
// 		//Not implemented
// 		assert(false);
// 		return -1;
// 	}

void U8String::TrimLeft() {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U8Char* lpsz = m_s;
    while (isspace(*lpsz) && (0 != *lpsz))
        lpsz++;
    if (lpsz != m_s) {
        size_t nDataLength = m_l - (lpsz - m_s);
        memmove(m_s, lpsz, (nDataLength + 1) * sizeof(U8Char));
        m_l = nDataLength;
    }
}
void U8String::TrimLeft(U8Char chTarget) {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U8Char* lpsz = m_s;

    while ((chTarget == *lpsz) && (0 != *lpsz))
        lpsz++;
    if (lpsz != m_s) {
        size_t nDataLength = m_l - (lpsz - m_s);
        memmove(m_s, lpsz, (nDataLength + 1) * sizeof(U8Char));
        m_l = nDataLength;
    }
}
void U8String::TrimLeft(const U8Char* lpszTargets) {
    if ((NULL == m_s) || (0 == m_l) || (NULL == lpszTargets)
            || (0 == *lpszTargets))
        return;

    U8Char* lpsz = m_s;
    while (0 != *lpsz) {
        if (strchr(lpszTargets, *lpsz) == NULL)
            break;
        lpsz++;
    }
    if (lpsz != m_s) {
        size_t nDataLength = m_l - (lpsz - m_s);
        memmove(m_s, lpsz, (nDataLength + 1) * sizeof(U8Char));
        m_l = nDataLength;
    }
}
void U8String::TrimRight() {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U8Char* lpszLast = m_s + m_l;
    U8Char* pNext = lpszLast - 1;

    while (isspace(*pNext) && (0 != *pNext))
        pNext--;
    lpszLast = pNext + 1;
    if (lpszLast != m_s + m_l) {
        *lpszLast = 0;
        m_l = lpszLast - m_s;
    }
}
void U8String::TrimRight(U8Char chTarget) {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U8Char* lpszLast = m_s + m_l;
    U8Char* pNext = lpszLast - 1;

    while ((pNext >= m_s) && (chTarget == *pNext) && (0 != *pNext))
        pNext--;
    lpszLast = pNext + 1;
    if (lpszLast != m_s + m_l) {
        *lpszLast = 0;
        m_l = lpszLast - m_s;
    }
}
void U8String::TrimRight(const U8Char* lpszTargets) {
    if ((NULL == m_s) || (0 == m_l) || (NULL == lpszTargets)
            || (0 == *lpszTargets))
        return;

    U8Char* lpszLast = m_s + m_l;
    U8Char* pNext = lpszLast - 1;
    while ((0 != *pNext) && (pNext >= m_s)) {
        if (strchr(lpszTargets, *pNext) == NULL)
            break;
        pNext--;
    }
    lpszLast = pNext + 1;
    if (lpszLast != m_s + m_l) {
        *lpszLast = 0;
        m_l = lpszLast - m_s;
    }
}

bool operator==(const U8String& s1, const U8String& s2) {
    return s1.Compare(s2) == 0;
}
bool operator==(const U8String& s1, const U8Char* s2) {
    return s1.Compare(s2) == 0;
}
bool operator==(const U8Char* s1, const U8String& s2) {
    return s2.Compare(s1) == 0;
}
bool operator !=(const U8String& s1, const U8String& s2) {
    return s1.Compare(s2) != 0;
}
bool operator !=(const U8String& s1, const U8Char* s2) {
    return s1.Compare(s2) != 0;
}
bool operator !=(const U8Char* s1, const U8String& s2) {
    return s2.Compare(s1) != 0;
}
bool operator <(const U8String& s1, const U8String& s2) {
    return s1.Compare(s2) < 0;
}
bool operator <(const U8String& s1, const U8Char* s2) {
    return s1.Compare(s2) < 0;
}
bool operator <(const U8Char* s1, const U8String& s2) {
    return s2.Compare(s1) > 0;
}
bool operator >(const U8String& s1, const U8String& s2) {
    return s1.Compare(s2) > 0;
}
bool operator >(const U8String& s1, const U8Char* s2) {
    return s1.Compare(s2) > 0;
}
bool operator >(const U8Char* s1, const U8String& s2) {
    return s2.Compare(s1) < 0;
}
bool operator <=(const U8String& s1, const U8String& s2) {
    return s1.Compare(s2) <= 0;
}
bool operator <=(const U8String& s1, const U8Char* s2) {
    return s1.Compare(s2) <= 0;
}
bool operator <=(const U8Char* s1, const U8String& s2) {
    return s2.Compare(s1) >= 0;
}
bool operator >=(const U8String& s1, const U8String& s2) {
    return s1.Compare(s2) >= 0;
}
bool operator >=(const U8String& s1, const U8Char* s2) {
    return s1.Compare(s2) >= 0;
}
bool operator >=(const U8Char* s1, const U8String& s2) {
    return s2.Compare(s1) <= 0;
}

U8String operator +(const U8String& string1, const U8String& string2) {
    U8String s;
    s.__ConcatCopy(string1, string1.GetLength(), string2, string2.GetLength());
    return s;
}
U8String operator +(const U8String& string, U8Char ch) {
    U8Char t[2];
    t[0] = ch;
    t[1] = 0;
    U8String s;
    s.__ConcatCopy(string, string.GetLength(), t, 1);
    return s;
}
U8String operator +(U8Char ch, const U8String& string) {
    U8Char t[2];
    t[0] = ch;
    t[1] = 0;
    U8String s;
    s.__ConcatCopy(t, 1, string, string.GetLength());
    return s;
}
U8String operator +(const U8String& string, const U8Char* lpsz) {
    U8String s;
    s.__ConcatCopy(string, string.GetLength(), lpsz, strlen(lpsz));
    return s;
}
U8String operator +(const U8Char* lpsz, const U8String& string) {
    U8String s;
    s.__ConcatCopy(lpsz, strlen(lpsz), string, string.GetLength());
    return s;
}

void U8String::fromU16String(const U16String& u16stringSrc) {
    if (m_s) {
        delete[] m_s;
        m_s = NULL;
        m_l = 0;
    }

    size_t l = u16stringSrc.GetLength();
    try {
        m_s = new U8Char[l * 6 + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    size_t i = 0, j = 0;
    for (; i < l && j < l * 6; i++) {
        j += U16CharToU8Char(u16stringSrc[i], m_s + j);
    }
    assert(j <= l * 6 + 1);
    m_s[j] = 0;
    m_l = j;
}

void U8String::fromU16String(const U16Char* lpsz) {
    if (m_s) {
        delete[] m_s;
        m_s = NULL;
        m_l = 0;
    }
#ifdef _WIN32
    size_t l = wcslen(lpsz);
#else
    size_t l = ustrlen(lpsz);
#endif
    try {
        m_s = new U8Char[l * 6 + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    size_t i = 0, j = 0;
    for (; i < l && j < l * 6; i++) {
        j += U16CharToU8Char(lpsz[i], m_s + j);
    }
    assert(j < l * 6 + 1);
    m_s[j] = 0;
    m_l = j;
}

/*C-Style can override by C library functions to increase speed*/
size_t U16String::__StrLen(const U16Char* str) const {
#if VA_VC
    return wcslen(str);
#else
    const U16Char* eos = str;
    while (*eos++)
        ;
    return (size_t)(eos - str - 1);
#endif
}

U16Char* U16String::__StrNCpy(U16Char* destStr, const U16Char* srcStr,
                              size_t nCount) const {
#if VA_VC
    StringCchCopyNW(destStr, nCount + 1, srcStr, nCount);
    return destStr;
#else
    U16Char* start = destStr;
    while (nCount && (*destStr++ = *srcStr++))
        nCount--;

    if (nCount)
        while (--nCount)
            ;
    *destStr++ = 0;
    return start;
#endif
}

int U16String::__StrCmp(const U16Char* src, const U16Char* dst) const {
#if VA_VC
    return wcscmp(src, dst);
#else
    int ret = 0;
    if (src == dst)
        return ret;

    U16Char c1, c2;
    do {
        c1 = *src++;
        c2 = *dst++;
        if (c2 == 0)
            return c1 - c2;
    } while (c1 == c2);

    return c1 < c2 ? -1 : 1;
#endif
}

int U16String::__StrNoCaseCmp(const U16Char* src, const U16Char* dst) const {
#if VA_VC
    return _wcsicmp(src, dst);
#else
    if (src == dst)
        return 0;
    U16Char c1, c2;
    do {
        c1 = tolower(*src++);
        c2 = tolower(*dst++);
        if (c2 == 0)
            return c1 - c2;
    } while (c1 == c2);
    return c1 < c2 ? -1 : 1;
#endif
}
const U16Char* U16String::__StrChr(const U16Char* string, U16Char ch) const {
#if VA_VC
    return wcschr(string, ch);
#else
    do {
        if (*string == ch)
            return (U16Char*) string;
    } while (*string++ != 0);

    return NULL;
#endif
}
U16Char* U16String::__StrRChr(U16Char* string, U16Char ch) const {
#if VA_VC
    return wcsrchr(string, ch);
#else
    U16Char *start = string;
    while (*string++)
        /* find end of string */;
    /* search towards front */
    while (--string != start && *string != ch)
        ;
    if (*string == ch) /* char found ? */
        return string;
    return NULL;
#endif
}
const U16Char* U16String::__StrStr(const U16Char* string,
                                   const U16Char* lpszSub) const {
#if VA_VC
    return wcsstr(string, lpszSub);
#else
    const U16Char* cp = string;
    const U16Char* s1;
    const U16Char* s2;
    if (!*lpszSub)
        return string;
    while (*cp) {
        s1 = cp;
        s2 = lpszSub;
        while (*s1 && *s2 && !(*s1 - *s2))
            s1++, s2++;
        if (!*s2)
            return cp;
        cp++;
    }
    return NULL;
#endif
}
const U16Char* U16String::__StrRStr(const U16Char* string,
                                    const U16Char* lpszSub) const {
    const U16Char *r = NULL;

    if (!lpszSub[0])
        return (U16Char*) string + __StrLen(string);
    for (;;) {
        const U16Char *p = __StrStr(string, lpszSub);
        if (!p)
            return r;
        r = p;
        string = p + 1;
    }
//    return NULL;
}
U16Char* U16String::__StrPbrk(U16Char* string, const U16Char* CharsSet) const {
#if VA_VC
    return wcspbrk(string, CharsSet);
#else
    const U16Char* cs;

    while (*string) {
        for (cs = CharsSet; *cs; cs++)
            if (*cs == *string)
                return string;
        string++;
    }
    return NULL;
#endif
}
U16Char U16String::__ToLower(U16Char ch) {
    return (U16Char) tolower(ch);
}
U16Char U16String::__ToUpper(U16Char ch) {
    return (U16Char) toupper(ch);
}
U16Char U16String::__Reverse(U16Char ch) {
    if (isupper(ch)) {
        return (U16Char) tolower(ch);
    }
    if (islower(ch)) {
        return (U16Char) toupper(ch);
    }
    return ch;
}
void U16String::__ConcatCopy(const U16Char* str1, size_t nLen1,
                             const U16Char* str2, size_t nLen2) {
    if (m_s) {
        delete[] m_s;
        m_s = NULL;
        m_l = 0;
    }

    m_l = nLen1 + nLen2;

    try {
        m_s = new U16Char[m_l + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    if (nLen1 > 0)
        __StrNCpy(m_s, str1, nLen1);
    if (nLen2 > 0)
        __StrNCpy(m_s + nLen1, str2, nLen2);
    m_s[m_l] = 0;
}

U16String::U16String(const U16String& stringSrc)/*拷贝构造*/
        : m_s(NULL),
          m_l(0) {
    if (stringSrc.m_s) {
        size_t l0 = stringSrc.m_l;
        try {
            m_s = new U16Char[l0 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
        __StrNCpy(m_s, stringSrc.m_s, l0);
        m_s[l0] = 0;
        m_l = l0;
    }
}
U16String::U16String(U16Char ch, unsigned int nRepeat)
        : m_s(NULL),
          m_l(0) {
    size_t l0 = nRepeat;
    try {
        m_s = new U16Char[l0 + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    U16Char* p = m_s;
    while (nRepeat && (*p++ = ch) != 0)
        nRepeat--;

    m_s[l0] = 0;
    m_l = l0;
}
U16String::U16String(const U16Char* lpsz, unsigned int nLength)

        : m_s(NULL),
          m_l(0) {
    if (lpsz) {
        size_t l0 = nLength ? nLength : __StrLen(lpsz);
        try {
            m_s = new U16Char[l0 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
        __StrNCpy(m_s, lpsz, l0);
        m_s[l0] = 0;
        m_l = l0;
    }
}

const U16String& U16String::operator=(const U16String& stringSrc) {
    if (m_s)
        delete[] m_s;
    m_s = NULL;
    m_l = 0;

    if (stringSrc.m_s) {
        try {
            m_s = new U16Char[stringSrc.m_l + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
        __StrNCpy(m_s, stringSrc.m_s, stringSrc.m_l);
        m_s[stringSrc.m_l] = 0;
        m_l = stringSrc.m_l;
    }
    return *this;
}
const U16String& U16String::operator=(U16Char ch) {
    if (m_s)
        delete[] m_s;
    m_s = NULL;
    m_l = 0;

    try {
        m_s = new U16Char[2]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
    }
    m_s[0] = ch;
    m_s[1] = 0;
    m_l = 1;
    return *this;
}
const U16String& U16String::operator=(const U16Char* lpsz) {
    if (m_s)
        delete[] m_s;
    m_s = NULL;
    m_l = 0;

    if (lpsz) {
        size_t l0 = __StrLen(lpsz);
        try {
            m_s = new U16Char[l0 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
        __StrNCpy(m_s, lpsz, l0);
        m_s[l0] = 0;
        m_l = l0;
    }
    return *this;
}

U16String::operator const U16Char*() const {
    return m_s;
}
U16String::operator U16Char*() const {
    return m_s;
}

const U16String& U16String::operator+=(const U16String& string) {
    if (m_s) {
        if (string.m_s) {
            U16Char* s0 = m_s;
            size_t l0 = m_l;
            U16Char* s1;
            size_t l1 = m_l + string.m_l;
            try {
                s1 = new U16Char[l1 + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
            __StrNCpy(s1, s0, l0);
            __StrNCpy(s1 + l0, string.m_s, string.m_l);
            s1[l1] = 0;
            m_s = s1;
            m_l = l1;
            delete[] s0;
        }
    } else {
        if (string.m_s) {
            U16Char* s1;
            try {
                s1 = new U16Char[string.m_l + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
            __StrNCpy(s1, string.m_s, string.m_l);
            s1[string.m_l] = 0;
            m_s = s1;
            m_l = string.m_l;
        }
    }
    return *this;
}
const U16String& U16String::operator+=(U16Char ch) {
    if (m_s) {
        U16Char* s0 = m_s;
        size_t l0 = m_l;
        U16Char* s1;
        size_t l1 = m_l + 1;
        try {
            s1 = new U16Char[l1 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
        __StrNCpy(s1, s0, l0);
        s1[l0] = ch;
        s1[l1] = 0;
        m_s = s1;
        m_l = l1;
        delete[] s0;
    } else {
        U16Char* s1;
        size_t l1 = 1;
        try {
            s1 = new U16Char[l1 + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
        }
        s1[0] = ch;
        s1[1] = 0;
        m_s = s1;
        m_l = l1;
    }
    return *this;
}
const U16String& U16String::operator+=(const U16Char* lpsz) {
    if (m_s) {
        if (lpsz) {
            U16Char* s0 = m_s;
            size_t l0 = m_l;
            U16Char* s1;
            size_t l1 = __StrLen(lpsz) + l0;
            try {
                s1 = new U16Char[l1 + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
            __StrNCpy(s1, s0, l0);
            __StrNCpy(s1 + l0, lpsz, l1 - l0);
            s1[l1] = 0;
            m_s = s1;
            m_l = l1;
            delete[] s0;
        }
    } else {
        if (lpsz) {
            U16Char* s1;
            size_t l1 = __StrLen(lpsz);
            try {
                s1 = new U16Char[l1 + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
            }
            __StrNCpy(s1, lpsz, l1);
            s1[l1] = 0;
            m_s = s1;
            m_l = l1;
        }
    }
    return *this;
}

U16Char U16String::operator [](size_t nIndex) const {
    assert(nIndex <= m_l);
    return m_s[nIndex];
}
U16Char& U16String::operator [](size_t nIndex) {
    assert(nIndex <= m_l);
    return m_s[nIndex];
}

U16String::~U16String() {
    if (m_s) {
        memset(m_s, 0, sizeof(U16Char) * m_l);
        delete[] m_s;
        m_s = NULL;
    }
    m_l = 0;
}

//Return: 0 ==; <0 String<lpsz; >0 String>lpsz;
int U16String::Compare(const U16Char *lpsz) const {
    return __StrCmp(m_s, lpsz);
}
int U16String::CompareNoCase(const U16Char *lpsz) const {
    return __StrNoCaseCmp(m_s, lpsz);
}
size_t U16String::Delete(int iIndex, int nCount) {
    if (iIndex + nCount <= (int) m_l) {
        size_t nNewLength = m_l - nCount;
        size_t nCharsToCopy = m_l - iIndex - nCount;
        memmove(m_s + iIndex, m_s + iIndex + nCount,
                nCharsToCopy * sizeof(U16Char));
        m_s[nNewLength] = 0;
        m_l = nNewLength;
    }
    return m_l;
}
void U16String::Empty() {
    if (m_s) {
        memset(m_s, 0, sizeof(U16Char) * m_l);
        delete[] m_s;
        m_s = NULL;
    }
    m_l = 0;
}

int U16String::Find(U16Char ch) const {
    return Find(ch, 0);
}
int U16String::Find(const U16Char* lpszSub) const {
    return Find(lpszSub, 0);
}
int U16String::Find(U16Char ch, int nStart) const {
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    const U16Char* lpsz = __StrChr(m_s + nStart, ch);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
int U16String::Find(const U16Char* lpszSub, int nStart) const {
    assert(lpszSub);
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    const U16Char* lpsz = __StrStr(m_s + nStart, lpszSub);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
int U16String::FindOneOf(const U16Char* lpCharsSet) const {
    return FindOneOf(lpCharsSet, 0);
}
int U16String::FindOneOf(const U16Char* lpCharsSet, int nStart) const {
    assert(lpCharsSet);
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    U16Char* lpsz = __StrPbrk(m_s + nStart, lpCharsSet);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
/*如果你在使用Format时崩溃，请务必保证你试图输入的字符串是C风格的字符串而不是一个类！*/
/*例如：X.Format(L"%s", Y.GetBuffer()); X.Format(L"%s", Y.c_str());*/
void U16String::Format(const U16Char* lpszFormat, ...) {
    va_list appList;
    va_start(appList, lpszFormat);
    FormatV(lpszFormat, appList);
    va_end(appList);
}
void U16String::FormatV(const U16Char* lpszFormat, va_list argList) {
#if VA_VC_WINCE
    int l = 1023;
    HRESULT ret = S_OK;
    do
    {
        if (l > 0) {
            if (m_s)
            delete[] m_s;
            m_s = NULL;
            m_l = 0;

            try {
                m_s = new U16Char[l + 1]();
            } catch (std::exception &e) {
                std::cout << "String() Exception:" << e.what() << std::endl;
                throw;
                return;
            }
            ret = StringCchVPrintfW(m_s, l + 1, lpszFormat, argList);
            if (ret == STRSAFE_E_INSUFFICIENT_BUFFER)
            {
                l = l*2;
            }
            assert(ret == S_OK || ret == STRSAFE_E_INSUFFICIENT_BUFFER);
        }
    }while (ret == STRSAFE_E_INSUFFICIENT_BUFFER);
    m_l = wcslen(m_s);
#elif VA_VC
    int l = 0;
    l = _vscwprintf(lpszFormat, argList);  // length after formatting

    if (l > 0) {
        if (m_s)
        delete[] m_s;
        m_s = NULL;
        m_l = 0;

        try {
            m_s = new U16Char[l + 1]();
        } catch (std::exception &e) {
            std::cout << "String() Exception:" << e.what() << std::endl;
            throw;
            return;
        }
        HRESULT ret = S_OK;
        ret = StringCchVPrintfW(m_s, l + 1, lpszFormat, argList);
        assert(ret == S_OK);
        m_l = l;
    }
#elif VA_GNUC
#pragma message "FormatV Not Implemented In U16String On Linux"
    std::cerr << "FormatV Not Implemented In U16String On Linux" << std::endl;
#endif
}

U16Char U16String::GetAt(int nIndex) const {
    return m_s[nIndex];
}
const U16Char* U16String::GetBuffer() const {
    return m_s;
}
size_t U16String::GetLength() const {
    return m_l;
}

size_t U16String::Insert(size_t iIndex, U16Char ch) {
    assert(m_s != NULL && iIndex < m_l);
    assert(ch != 0);

    size_t nNewLength = m_l + 1;

    U16Char* tmp;
    try {
        tmp = new U16Char[nNewLength + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
    }
    __StrNCpy(tmp, m_s, iIndex);
    tmp[iIndex] = ch;
    __StrNCpy(tmp + iIndex + 1, m_s + iIndex, m_l - iIndex + 1);
    tmp[nNewLength] = 0;

    delete[] m_s;
    m_s = tmp;
    m_l = nNewLength;
    return m_l;
}
size_t U16String::Insert(size_t iIndex, const U16Char* lpsz) {
    assert(m_s != NULL && iIndex < m_l);
    assert(lpsz != NULL);

    size_t nCHARsToCopy = __StrLen(lpsz);
    size_t nNewLength = m_l + nCHARsToCopy;

    U16Char* tmp;
    try {
        tmp = new U16Char[nNewLength + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
    }

    __StrNCpy(tmp, m_s, iIndex);
    __StrNCpy(tmp + iIndex, lpsz, nCHARsToCopy);
    __StrNCpy(tmp + iIndex + nCHARsToCopy, m_s + iIndex, m_l - iIndex + 1);
    tmp[nNewLength] = 0;

    delete[] m_s;
    m_s = tmp;
    m_l = nNewLength;
    return *this;
}

bool U16String::IsEmpty() const {
    return GetLength() == 0;
}
bool U16String::IsNull() const {
    return m_s == NULL;
}
U16String::operator bool() const {
    return m_s != NULL;
}
U16String U16String::Left(int nCount) const {
    return Mid(0, nCount);
}
void U16String::MakeLower() {
    for (unsigned int i = 0; i <= m_l; ++i)
        m_s[i] = __ToLower(m_s[i]);
}
void U16String::MakeReverse() {
    for (unsigned int i = 0; i <= m_l; ++i)
        m_s[i] = __Reverse(m_s[i]);
}
void U16String::MakeUpper() {
    for (unsigned int i = 0; i <= m_l; ++i)
        m_s[i] = __ToUpper(m_s[i]);
}
U16String U16String::Mid(size_t nFirst) const {
    return Mid(nFirst, m_l - nFirst);
}
U16String U16String::Mid(size_t nFirst, size_t nCount) const {
    assert(nFirst >= 0 && nFirst < m_l);
    assert(nCount > 0 && nCount <= m_l);
    U16Char* s = (U16Char*) alloca((nCount + 1) * sizeof(U16Char));
    __StrNCpy(s, m_s + nFirst, nCount);
    s[nCount] = 0;
    return s;
}
size_t U16String::Remove(U16Char ch) {
    for (unsigned int i = 0; i < m_l; i++)
        if (m_s[i] == ch)
            Delete(i);
    return m_l;
}
size_t U16String::Replace(U16Char chOld, U16Char chNew) {
    assert(chOld != 0);
    assert(chNew != 0);
    for (U16Char* p = m_s; *p; ++p)
        if (*p == chOld)
            *p = chNew;
    return m_l;
}
// 	int U16String::Replace(const U16Char* lpszOld, const U16Char* lpszNew) {
// 		assert(false);
// 		return -1;
// 	}

int U16String::ReverseFind(U16Char ch) const {
    return ReverseFind(ch, 0);
}
int U16String::ReverseFind(const U16Char* lpszSub) const {
    return ReverseFind(lpszSub, 0);
}
int U16String::ReverseFind(U16Char ch, int nStart) const {
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    U16Char* lpsz = __StrRChr(m_s + nStart, ch);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}
int U16String::ReverseFind(const U16Char* lpszSub, int nStart) const {
    assert(lpszSub);
    if (nStart < 0)
        return -2;
    if (nStart >= (int) m_l)
        return -1;
    const U16Char* lpsz = __StrRStr(m_s + nStart, lpszSub);
    return (lpsz == NULL) ? -1 : (int) (lpsz - m_s);
}

U16String U16String::Right(int nCount) const {
    return Mid(m_l - nCount, nCount);
}
void U16String::SetAt(unsigned int nIndex, U16Char ch) const {
    assert(nIndex <= m_l);
    assert(ch != 0);

    m_s[nIndex] = ch;
}
// 	U16String U16String::SpanExcluding(const U16Char lpszCharSet) const {
// 		assert(false);  //Not implemented
// 		return -1;
// 	}
// 	U16String U16String::SpanIncluding(const U16Char lpszCharSet) const {
// 		assert(false);  //Not implemented
// 		return -1;
// 	}

void U16String::TrimLeft() {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U16Char* lpsz = m_s;
    while (isspace(*lpsz) && (0 != *lpsz))
        lpsz++;
    if (lpsz != m_s) {
        size_t nDataLength = m_l - (lpsz - m_s);
        memmove(m_s, lpsz, (nDataLength + 1) * sizeof(U16Char));
        m_l = nDataLength;
    }
}
void U16String::TrimLeft(U16Char chTarget) {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U16Char* lpsz = m_s;

    while ((chTarget == *lpsz) && (0 != *lpsz))
        lpsz++;
    if (lpsz != m_s) {
        size_t nDataLength = m_l - (lpsz - m_s);
        memmove(m_s, lpsz, (nDataLength + 1) * sizeof(U16Char));
        m_l = nDataLength;
    }
}
void U16String::TrimLeft(const U16Char* lpszTargets) {
    if ((NULL == m_s) || (0 == m_l) || (NULL == lpszTargets)
            || (0 == *lpszTargets))
        return;

    U16Char* lpsz = m_s;
    while (0 != *lpsz) {
        if (__StrChr(lpszTargets, *lpsz) == NULL)
            break;
        lpsz++;
    }
    if (lpsz != m_s) {
        size_t nDataLength = m_l - (lpsz - m_s);
        memmove(m_s, lpsz, (nDataLength + 1) * sizeof(U16Char));
        m_l = nDataLength;
    }
}
void U16String::TrimRight() {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U16Char* lpszLast = m_s + m_l;
    U16Char* pNext = lpszLast - 1;

    while (isspace(*pNext) && (0 != *pNext))
        pNext--;
    lpszLast = pNext + 1;
    if (lpszLast != m_s + m_l) {
        *lpszLast = 0;
        m_l = lpszLast - m_s;
    }
}
void U16String::TrimRight(U16Char chTarget) {
    if ((NULL == m_s) || (0 == m_l))
        return;

    U16Char* lpszLast = m_s + m_l;
    U16Char* pNext = lpszLast - 1;

    while ((pNext >= m_s) && (chTarget == *pNext) && (0 != *pNext))
        pNext--;
    lpszLast = pNext + 1;
    if (lpszLast != m_s + m_l) {
        *lpszLast = 0;
        m_l = lpszLast - m_s;
    }
}
void U16String::TrimRight(const U16Char* lpszTargets) {
    if ((NULL == m_s) || (0 == m_l) || (NULL == lpszTargets)
            || (0 == *lpszTargets))
        return;

    U16Char* lpszLast = m_s + m_l;
    U16Char* pNext = lpszLast - 1;
    while ((0 != *pNext) && (pNext >= m_s)) {
        if (__StrChr(lpszTargets, *pNext) == NULL)
            break;
        pNext--;
    }
    lpszLast = pNext + 1;
    if (lpszLast != m_s + m_l) {
        *lpszLast = 0;
        m_l = lpszLast - m_s;
    }
}

bool operator==(const U16String& s1, const U16String& s2) {
    return s1.Compare(s2) == 0;
}
bool operator==(const U16String& s1, const U16Char* s2) {
    return s1.Compare(s2) == 0;
}
bool operator==(const U16Char* s1, const U16String& s2) {
    return s2.Compare(s1) == 0;
}
bool operator !=(const U16String& s1, const U16String& s2) {
    return s1.Compare(s2) != 0;
}
bool operator !=(const U16String& s1, const U16Char* s2) {
    return s1.Compare(s2) != 0;
}
bool operator !=(const U16Char* s1, const U16String& s2) {
    return s2.Compare(s1) != 0;
}
bool operator <(const U16String& s1, const U16String& s2) {
    return s1.Compare(s2) < 0;
}
bool operator <(const U16String& s1, const U16Char* s2) {
    return s1.Compare(s2) < 0;
}
bool operator <(const U16Char* s1, const U16String& s2) {
    return s2.Compare(s1) > 0;
}
bool operator >(const U16String& s1, const U16String& s2) {
    return s1.Compare(s2) > 0;
}
bool operator >(const U16String& s1, const U16Char* s2) {
    return s1.Compare(s2) > 0;
}
bool operator >(const U16Char* s1, const U16String& s2) {
    return s2.Compare(s1) < 0;
}
bool operator <=(const U16String& s1, const U16String& s2) {
    return s1.Compare(s2) <= 0;
}
bool operator <=(const U16String& s1, const U16Char* s2) {
    return s1.Compare(s2) <= 0;
}
bool operator <=(const U16Char* s1, const U16String& s2) {
    return s2.Compare(s1) >= 0;
}
bool operator >=(const U16String& s1, const U16String& s2) {
    return s1.Compare(s2) >= 0;
}
bool operator >=(const U16String& s1, const U16Char* s2) {
    return s1.Compare(s2) >= 0;
}
bool operator >=(const U16Char* s1, const U16String& s2) {
    return s2.Compare(s1) <= 0;
}

U16String operator +(const U16String& string1, const U16String& string2) {
    U16String s;
    s.__ConcatCopy(string1, string1.GetLength(), string2, string2.GetLength());
    return s;
}
U16String operator +(const U16String& string, U16Char ch) {
    U16Char t[2];
    t[0] = ch;
    t[1] = 0;
    U16String s;
    s.__ConcatCopy(string, string.GetLength(), t, 1);
    return s;
}
U16String operator +(U16Char ch, const U16String& string) {
    U16Char t[2];
    t[0] = ch;
    t[1] = 0;
    U16String s;
    s.__ConcatCopy(t, 1, string, string.GetLength());
    return s;
}
U16String operator +(const U16String& string, const U16Char* lpsz) {
    U16String s;
#if VA_VC
    s.__ConcatCopy(string, string.GetLength(), lpsz, wcslen(lpsz));
#else
    s.__ConcatCopy(string, string.GetLength(), lpsz, ustrlen(lpsz));
#endif
    return s;
}
U16String operator +(const U16Char* lpsz, const U16String& string) {
    U16String s;
#if VA_VC
    s.__ConcatCopy(lpsz, wcslen(lpsz), string, string.GetLength());
#else
    s.__ConcatCopy(lpsz, ustrlen(lpsz), string, string.GetLength());
#endif
    return s;
}

//从utf8转换为utf16
U16String::U16String(const U8String& u8stringSrc)
        : m_s(NULL),
          m_l(0) {
    fromU8String(u8stringSrc);
}
U16String::U16String(const char* u8stringSrc)
        : m_s(NULL),
          m_l(0) {
    fromU8String(u8stringSrc);
}

void U16String::fromU8String(const U8String& u8stringSrc) {
    if (m_s) {
        delete[] m_s;
        m_s = NULL;
        m_l = 0;
    }

    size_t l = u8stringSrc.GetLength();
    try {
        m_s = new U16Char[l + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    size_t i = 0, j = 0;
    const U8Char* u8str = u8stringSrc;
    for (; i < l && j < l; j++) {
        i += U8CharToU16Char(u8str + i, m_s[j]);
    }
    m_s[j] = 0;
    m_l = j;
}
void U16String::fromU8String(const char* u8stringSrc) {
    if (m_s) {
        delete[] m_s;
        m_s = NULL;
        m_l = 0;
    }

    size_t l = strlen(u8stringSrc);
    try {
        m_s = new U16Char[l + 1]();
    } catch (std::exception &e) {
        std::cout << "String() Exception:" << e.what() << std::endl;
        throw;
        return;
    }
    size_t i = 0, j = 0;
    const U8Char* u8str = u8stringSrc;
    for (; i < l && j < l; j++) {
        i += U8CharToU16Char(u8str + i, m_s[j]);
    }
    m_s[j] = 0;
    m_l = j;
}

const U16String& U16String::operator=(const U8String& u8stringSrc) {
    fromU8String(u8stringSrc);
    return *this;
}
const U16String& U16String::operator=(const char* u8stringSrc) {
    fromU8String(u8stringSrc);
    return *this;
}

} /* namespace VA */

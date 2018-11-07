#ifndef __BASE_64_H__
#define  __BASE_64_H__

namespace VA {

char * base64_encode( const unsigned char * bindata, char * base64, int binlength );

int base64_decode( const char * base64, unsigned char * bindata );

int base64_encode_length(int nLen);
int base64_decode_length(int nLen);
} //namespace VA

#endif //__BASE_64_H__
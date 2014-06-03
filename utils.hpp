#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <openssl/bio.h>
#include <openssl/evp.h>


namespace utils
{
//Encodes a string to base64
char* Base64Encode(const char*);
//Decodes a string to base64
char* Base64Decode(const char*);
}

#endif

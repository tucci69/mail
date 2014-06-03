#include <openssl/bio.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

using namespace std;

namespace utils
{

//Encodes a string to base64
char* Base64Encode(const char* message)
{   
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bmem = BIO_new(BIO_s_mem());
    BIO_set_close(bmem,BIO_NOCLOSE);
//    BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bmem);
    BIO_write(b64, message, strlen(message));
    BIO_flush(b64);
    char* buffer = 0;
    BIO_get_mem_data(bmem, &buffer);
    BIO_free_all(b64);
    char* pc = buffer + strlen(buffer) - 1;
    if(*pc == '\n')
        *pc = 0;

    return buffer;
}

char* Base64Decode(const char* message)
{   
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO* bmem = BIO_new(BIO_s_mem());
    BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);
    BIO_puts(bmem, message);
    BIO_push(b64, bmem);
    size_t sz = strlen(message);
    char* buf = (char*) malloc(sz);
    BIO_read(b64, buf, sz);
    BIO_flush(b64);
    BIO_free_all(b64);
    return buf;
}

}


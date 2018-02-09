#include "cbytebuf.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>


#include <openssl/evp.h>
#include <openssl/rand.h>

#ifdef __linux__
#include<unistd.h>
#endif


#define IP_SIZE 1024
#define OP_SIZE 1024


#ifdef DEBUG
#include <stdio.h>
#endif


CByteBuf::CByteBuf(char * data, int len){
    buffer = data;
    cursor = 0;
    size = len;
    allocated = len;
    doFree = false;
}


CByteBuf::CByteBuf(){
    buffer = (char*)malloc(256);
    cursor = 0;
    size = 0;
    allocated = 256;
    doFree = true;
#ifdef DEBUG
    printf("Created ByteBuf. cursor = %d size = %d allocated = %d\n",cursor,size, allocated);
#endif
}
CByteBuf::CByteBuf(CByteBuf &other){
    buffer = other.getDataCopy();
    cursor = other.cursor;
    allocated = other.getLen();
    size = other.getLen();
    doFree = true;
#ifdef DEBUG
    printf("Copied ByteBuf. cursor = %d size = %d allocated = %d\n",cursor,size, allocated);
#endif
}
CByteBuf::~CByteBuf(){
#ifdef DEBUG
    printf("Freeing ByteBuf. cursor = %d size = %d allocated = %d\n",cursor,size, allocated);
#endif
    if(doFree)free(buffer);
}

bool CByteBuf::decrypt(CByteBuf &other, int bytes, unsigned char *key)
{
    int olen, n;

    unsigned char outbuf[IP_SIZE+64];
    char inbuff[OP_SIZE+64];

    EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
    //EVP_CIPHER_CTX_init (ctx);

    EVP_DecryptInit (ctx,  EVP_aes_256_cbc(), key, key);
    int sum = 0;
    for (;;)
    {
        int remain = std::min(OP_SIZE, bytes-sum);
        if(remain <= 0)break;
        if ((n = read(inbuff, remain)) == -1)
        {
            perror ("read error");
            break;
        }
        else if (n == 0)
            break;
        olen  =  0;
        sum+=n;
        if (EVP_DecryptUpdate (ctx, outbuf, &olen, (unsigned char *)inbuff, n) != 1)
        {
            perror("error in decrypt update");
            return false;
        }
        other.write((char *)outbuf,olen);
    }


    if (EVP_DecryptFinal (ctx, outbuf, &olen) != 1)
    {
        perror("error in decrypt final?");

        return false;
    }


    other.write((char *)outbuf,olen);

    EVP_CIPHER_CTX_free(ctx);



    return true;
}

bool CByteBuf::encrypt(CByteBuf &other, int bytes, unsigned char *key)
{
    unsigned char outbuf[OP_SIZE+64];
    int olen, n;
    char inbuff[IP_SIZE+64];
    EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();

    //EVP_CIPHER_CTX_init ( ctx);
    EVP_EncryptInit ( ctx, EVP_aes_256_cbc(), key, key);
    int sum = 0;
    for (;;)
    {
        int remain = std::min(IP_SIZE, bytes-sum);
        if(remain <= 0)break;

        if ((n = read(inbuff, remain)) == -1)
        {
            perror ("read error");
            break;
        }
        else if (n == 0)
            break;

        if (EVP_EncryptUpdate ( ctx, outbuf, & olen, (unsigned char *)inbuff, n) != 1)
        {
            perror("error in encrypt update");

            return false;
        }


        other.write(outbuf,olen);

    }
    int flen = 0;
    if (EVP_EncryptFinal ( ctx, outbuf, &flen) != 1)
    {
        perror("error in encrypt final");
        return false;
    }
    other.write(outbuf,flen);


    EVP_CIPHER_CTX_free ( ctx);

    return true;
}
void CByteBuf::write(const void * ptr, int nbytes){
    int reqBytes = cursor+nbytes;
    if(reqBytes > allocated){
        int newSize = allocated*2;
        if(newSize <= reqBytes)newSize = reqBytes+256;
        buffer = (char*)realloc(buffer, newSize);
        allocated = newSize;
    }
    memcpy(buffer+cursor, ptr, nbytes);
    size += nbytes;
    cursor += nbytes;
#ifdef DEBUG
    printf("Written ByteBuf. cursor = %d size = %d allocated = %d\n",cursor,size, allocated);
#endif
}
int CByteBuf::read(void * ptr, int nbytes){
    int nwrite = cursor+nbytes > size ? size-cursor : nbytes;
    if(nwrite <= 0)return 0;
    memcpy(ptr,buffer+cursor,nwrite);
    cursor += nwrite;
#ifdef DEBUG
    printf("READ ByteBuf. cursor = %d size = %d allocated = %d\n",cursor,size, allocated);
#endif

    return nwrite;
}

char * CByteBuf::getData(){
    return buffer;
}
char * CByteBuf::getDataCopy(){
    char * copy = (char*)malloc(size);
    memcpy(copy,buffer,size);
    return copy;
}

int CByteBuf::getLen(){
    return size;
}

void CByteBuf::strip(){
    buffer = (char*)realloc(buffer, size);
    allocated = size;
}

void CByteBuf::reset(){
    size = 0;
    cursor = 0;
}

void CByteBuf::reserve(int nall, bool setsize){
    if(setsize)size = nall;
    if(allocated > nall)return;
    buffer = (char*)realloc(buffer, nall);
    allocated = nall;

}

void CByteBuf::seek(int pos){

    if(pos > size){
        reserve(pos+256,false);
        size = pos+1;
    }
    cursor = pos;

#ifdef DEBUG
    printf("Seeked ByteBuf. cursor = %d size = %d allocated = %d\n",cursor,size, allocated);
#endif
}

void CByteBuf::writeString(std::string str){
    short nbytes = str.size();
    writeBigEndianData<short>(nbytes);
    write(str.data(), str.size());
}

#include <logger/log.h>
#include <sstream>

#ifndef O_BINARY
#define O_BINARY 0
#endif

std::string CByteBuf::readString(){
    short len = this->readBigEndianData<short>();
    char * str = (char*)malloc(len);
    read(str, len);
/*
    std::stringstream hexdump;
    char buf[16];
    uint8_t * p = (uint8_t*)str;
    for(int i = 0; i < len; i++){
        sprintf(buf,"%c= %x ",str[i],p[i]);
        hexdump << buf;
    }
*/
    std::string r(str, len);

  //  LOG << r << " HEXDUMP: " << hexdump.str();

    free(str);
    return r;
}

void CByteBuf::save(const char * filename){

    int fd = ::open(filename, O_CREAT | O_WRONLY | O_TRUNC | O_BINARY, 0644);

    if(fd < 0)throw FileIOError("Failed to open file for writing!");
    int towrite = size;
    int written = 0;

    while(written < towrite){

        int r =  ::write(fd,buffer+written,towrite-written);
        if(r < 0){
            throw FileIOError("Writing file failed");
            ::close(fd);
            return;
        }
        written += r;
    }
    ::close(fd);

}

#include <zlib.h>

#define CHUNK 16384

void CByteBuf::loadGZ(const char * filename){

    FILE * source = fopen(filename,"rb");

    if(source == NULL){
        throw FileIOError("Failed to open file for reading!");
    }

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit2(&strm , MAX_WBITS+16);
    if (ret != Z_OK)
        throw FileIOError("Failed to start zlib!");


    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            throw FileIOError("Failed to read file!");
        }

        if (strm.avail_in == 0)
            break;
        strm.next_in = in;


        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            if(ret == Z_BUF_ERROR){
                WARN << "Ignore ZBUF error...";
            }if(ret < 0){
                (void)inflateEnd(&strm);
                throw FileIOError("GZIP Error: "+std::to_string(ret));
            }
            have = CHUNK - strm.avail_out;
            write(out,have);

        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
}

void CByteBuf::load(const char * filename){
     int fd = ::open(filename, O_RDONLY | O_BINARY);
     if(fd < 0)throw FileIOError("Failed to open file for reading!");

     char rdbuf[4096];
     int nbytes;
     while ((nbytes = ::read(fd, rdbuf, 4096)) > 0) {
        write(rdbuf,nbytes);
     }
     ::close(fd);
     if(nbytes < 0)throw FileIOError("Failed to Read file");
}

void CByteBuf::writeCString(char * str, int len){
    short nbytes = (len > 0) ? len :strlen(str);
    writeBigEndianData<short>(nbytes);
    write(str, nbytes);
}

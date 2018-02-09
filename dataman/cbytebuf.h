#ifndef CBYTEBUF_H
#define CBYTEBUF_H

#include <string>



class BInvalidArgumentException : public std::exception{
public:
    BInvalidArgumentException(std::string what){
        msg = what;
    }

    BInvalidArgumentException(const char * what) : msg(what){

    }


    const virtual char* what() const throw()
    {
        return msg.c_str();
    }
private:
    std::string msg;
};
class  FileIOError : public std::exception
{
public:
    FileIOError(std::string what){
        msg = what;
    }

    FileIOError(const char * what) : msg(what){

    }


    const char* what() const throw()
    {
        return msg.c_str();
    }
private:
    std::string msg;
};

class  CByteBuf
{
public:
    CByteBuf();
    CByteBuf(char * data, int len);
    CByteBuf(CByteBuf &other);
    ~CByteBuf();

    bool decrypt(CByteBuf & other, int bytes, unsigned char * key);
    bool encrypt(CByteBuf & other, int bytes, unsigned char * key);

    void write(const void * ptr, int nbytes);
    int read(void * ptr, int nbytes);
    void seek(int pos);

    uint16_t swap_uint16( uint16_t val )
    {
        return (val << 8) | (val >> 8 );
    }


    int16_t swap_int16( int16_t val )
    {
        return (val << 8) | ((val >> 8) & 0xFF);
    }

    uint32_t swap_uint32( uint32_t val )
    {
        val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
        return (val << 16) | (val >> 16);
    }

    int32_t swap_int32( int32_t val )
    {
        val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
        return (val << 16) | ((val >> 16) & 0xFFFF);
    }

    int64_t swap_int64( int64_t val )
    {
        val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
        val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
        return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
    }

    uint64_t swap_uint64( uint64_t val )
    {
        val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
        val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
        return (val << 32) | (val >> 32);
    }

    template<typename T> T unsignedEndianSwap(T in){
        switch(sizeof(T)){
        case 1: return in;
        case 2: return swap_uint16((uint16_t)in);
        case 4: return swap_uint32((uint32_t)in);
        case 8: return swap_uint64((uint64_t)in);
        default: throw BInvalidArgumentException("Invalid bit size for endian swap!");
        }
    }

    template<typename T> T signedEndianSwap(T in){
        switch(sizeof(T)){
        case 1: return in;
        case 2: return swap_int16((int16_t)in);
        case 4: return swap_int32((int32_t)in);
        case 8: return swap_int64((int64_t)in);
        default: throw BInvalidArgumentException("Invalid bit size for endian swap!");
        }
    }

    template<typename T> T readBigEndianData(){
        T ret;
        read(&ret, sizeof(T));
        return signedEndianSwap<T>(ret);
    }

     template<typename T> void writeBigEndianData(T data){
        T swappd = signedEndianSwap<T>(data);
        write(&swappd, sizeof(T));
    }


    template<typename T> void writeData(T data){write(&data, sizeof(T));}

    template<typename T> T readData(){
        T ret;
        read(&ret, sizeof(T));
        return ret;
    }


    template<typename T> inline CByteBuf &operator<<(const T & t){
        this->writeData<T>(t);
        return *this;
    }

    template<typename T> inline CByteBuf &operator>>(const T & t){
        this->read(&t,sizeof(T));
        return *this;
    }


    void writeString(std::string str);
    void writeCString(char * str, int len = -1);
    std::string readString();


    void reset();

    void reserve(int nall, bool setsize = false);



    void save(const char * filename);
    void load(const char * filename);

    char * getData();
    char * getDataCopy();

    int getLen();

    void strip();

    int pos(){return cursor;}

    void loadGZ(const char *filename);
private:
    char * buffer;

    int cursor;


    int allocated;
    int size;

    bool doFree;
};

#endif // CBYTEBUF_H

#ifndef LOG_H
#define LOG_H

#include <sstream>

#ifndef DLLXP
#define DLLXP
#endif


#define LVAL(value) #value" = "<<value<<" "


#ifdef LOGGER_NO_EXTRAINFO
#define LOG Logger(0,-1,"INFO")
#define WARN Logger(0 ,-1,"WARNING")
#define ERR Logger(0 ,-1,"ERROR")
#else


#ifndef LOGGER_MOD
#ifndef __FILE__
#define __FILE__ "unknown"
#warning "your compiler does not expand __FILE__"
#endif

#ifndef __BASE_FILE__
#define __BASE_FILE__ __FILE__
#warning "your compiler does not expand __BASE_FILE__"
#endif

#ifndef __LINE__
#warning "your compiler does not expand __LINE__"
#endif
#warning Set logger module with LOGGER_MOD. Defaulting to __BASE_FILE__
#define LOGGER_MOD __BASE_FILE__
#endif

#ifdef LOGGER_NO_LINENUMBER
#define LOG Logger(LOGGER_MOD,-1, 0)
#define WARN Logger(LOGGER_MOD ,-1, 1)
#define ERR Logger(LOGGER_MOD ,-1, 2)
#else
#define LOG Logger(LOGGER_MOD,__LINE__, 0)
#define WARN Logger(LOGGER_MOD ,__LINE__, 1)
#define ERR Logger(LOGGER_MOD ,__LINE__, 2)
#endif

#endif


#define COLOR_RED     "\x1b[31;1m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34;1m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_LIGHTCYAN    "\x1b[36;1m"
#define COLOR_GRAY    "\x1b[37m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_PINK   "\x1b[35;1m"
#define COLOR_RGB(r,g,b) "\x1b[38;2;"#r";"#g";"#b"m"


template<typename T> struct LoggerAdapter;

class LoggerBackend;

class  Logger
{

public:
    static void init(const char * logpath);

    Logger(const char * file = 0, int line = -1, int what = -1);
    ~Logger();

    template<typename T> void write(const T & t){
        LoggerAdapter<T>::write(*this, t);
    }

    template<typename T> inline Logger &operator<<(const T & t){
        write(t);
        return *this;
    }



    static LoggerBackend * getBackend();

    std::stringstream stream;

    const char * file;
    std::string timestamp;
    int line;
private:

    void genTimestamp();
    int level;

    static LoggerBackend * backend;

};

template<typename T>
struct LoggerAdapter{
    static void write(Logger & logger, const T & value){
        logger.stream << value;
    }
};

#ifdef QT_CORE_LIB
#include <QString>
#include <QByteArray>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QPoint>

template<>
struct LoggerAdapter<QString> {
    static void write(Logger & logger, const QString & value){
        logger << qPrintable(value);
    }
};


template<>
struct LoggerAdapter<QPoint> {
    static void write(Logger & logger, const QPoint & value){
        logger << "("<<value.x()<<", "<<value.y()<<")";
    }
};

template<>
struct LoggerAdapter<QPointF> {
    static void write(Logger & logger, const QPointF & value){
        logger << "("<<value.x()<<", "<<value.y()<<")";
    }
};

template<>
struct LoggerAdapter<QByteArray> {
    static void write(Logger & logger, const QByteArray & t){
        char * hexString = (char*)calloc(t.length()+1,5);
        for(int i = 0; i < t.length(); i++){
            snprintf(hexString,5,"0x%x ",t.at(i));
        }
        logger.write(hexString);
        free(hexString);
    }
};

template<>
struct LoggerAdapter<QJsonObject> {
    static void write(Logger & logger, const QJsonObject & t){
        QJsonDocument doc;
        doc.setObject(t);
        logger.write(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }
};

template<>
struct LoggerAdapter<QJsonArray> {
    static void write(Logger & logger, const QJsonArray & t){
        QJsonDocument doc;
        doc.setArray(t);
        logger.write(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }
};

template <typename SequentialContainer>
inline void psc__(Logger & debug, const char *which, const SequentialContainer &c)
{
    debug << which << '[';
    typename SequentialContainer::const_iterator it = c.begin(), end = c.end();
    if (it != end) {
        debug << *it;
        ++it;
    }
    while (it != end) {
        debug << ", " << *it;
        ++it;
    }
    debug << ']';
}


template<>
struct LoggerAdapter<QStringList> {
    static void write(Logger & logger, const QStringList & value){
        psc__(logger, "" /*for historical reasons*/, value);
    }
};

#endif



#endif // LOG_H

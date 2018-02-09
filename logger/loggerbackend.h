#ifndef LOGGERBACKEND_H
#define LOGGERBACKEND_H

#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <mutex>
#ifndef __linux__
#include "mingw-bug/mingwbug.h"
#endif
typedef struct LogEntry{
    LogEntry(std::string timestamp, std::string string, int level, const char *file, int line);

    std::string timestamp;
    std::string string;
    int level;
    const char * file;
    int line;
} LogEntry;

class LogReciever{
public:
    virtual void onNewLog(LogEntry & entry) = 0;
};

class LoggerBackend{
public:
    LoggerBackend(const char * logpath);
    ~LoggerBackend();
    void append(std::string timestamp, std::string string, int level, const char *file, int line);
    void setBufferSize(int sz);

    const char * getLevelString(int lvl, bool noCol = false);

    std::list<LogEntry> getCurrentEntries();
    void registerReciever(LogReciever * r);
    void removeReciever(LogReciever * r);
    void setColorSupport(bool enable);
private:
    std::vector<LogReciever *> recievers;
    char * logPath;
    std::list<LogEntry> entries;
    int bufferSize;
    std::ofstream logfile;
    std::mutex mutex;
    bool colorsEnabled;
    std::string getColor();
};


#endif // LOGGERBACKEND_H

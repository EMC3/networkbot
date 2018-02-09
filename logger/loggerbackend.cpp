#include "loggerbackend.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <algorithm>


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

LoggerBackend::LoggerBackend(const char *logpath)
{
    logPath = (char *)malloc(strlen(logpath)+1);
    strcpy(logPath,logpath);
    struct stat statbuf;
    if(stat(logPath,&statbuf) >= 0){
        char * newpath = (char *)malloc(strlen(logpath)+3);
        sprintf(newpath,"%s.0",logPath);
        unlink(newpath);
        rename(logPath,newpath);
        unlink(logPath);
        free(newpath);
    }
    logfile.open(logPath,std::ios_base::out | std::ios_base::app);
    bufferSize = 0;
    this->colorsEnabled = false;
}

LoggerBackend::~LoggerBackend()
{
    logfile.close();
}

void LoggerBackend::append(std::string timestamp, std::string string, int level, const char * file, int line)
{
    mutex.lock();
    if(colorsEnabled){
        if(file <= 0){
            std::cout << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "]"<<getLevelString(level)<<": "<<string;
            logfile << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "]"<<getLevelString(level)<<": "<<string;

        }else{
            if(line >= 0){
                std::cout << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "] ["<<COLOR_PINK<<file<<":"<<line<<COLOR_RESET<<"]"<<getLevelString(level)<<": "<<string;
                logfile << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "] ["<<COLOR_PINK<<file<<":"<<line<<COLOR_RESET<<"]"<<getLevelString(level)<<": "<<string;

            }else{
                std::cout << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "] ["<<COLOR_PINK<<file<<COLOR_RESET<<"]"<<getLevelString(level)<<": "<<string;
                logfile << "[" <<COLOR_GRAY<< timestamp <<COLOR_RESET<< "] ["<<COLOR_PINK<<file<<COLOR_RESET<<"]"<<getLevelString(level)<<": "<<string;

            }
        }
        if(string.back() != '\n'){
            logfile << std::endl;
            std::cout <<COLOR_RESET<<std::endl;

        }
    }else{
        if(file <= 0){
            std::cout << "[" << timestamp << "]"<<getLevelString(level)<<": "<<string;
            logfile << "[" << timestamp << "]"<<getLevelString(level,true)<<": "<<string;
        }else{
            if(line >= 0){
                std::cout << "[" << timestamp << "] ["<<file<<":"<<line<<"]"<<getLevelString(level)<<": "<<string;
                logfile << "[" << timestamp << "] ["<<file<<":"<<line<<"]"<<getLevelString(level,true)<<": "<<string;
            }else{
                std::cout << "[" << timestamp << "] ["<<file<<"]"<<getLevelString(level)<<": "<<string;
                logfile << "[" << timestamp << "] ["<<file<<"]"<<getLevelString(level,true)<<": "<<string;
            }
        }
        if(string.back() != '\n'){
            logfile << std::endl;
            std::cout <<std::endl;
        }
    }
    logfile.flush();


    if(recievers.size() > 0 || bufferSize > 0){

        LogEntry entry(timestamp,  string, level,  file,  line);

        for(LogReciever * recv : recievers){
            recv->onNewLog(entry);
        }
        if(bufferSize > 0){
            entries.push_back(entry);
            if(entries.size() > bufferSize)entries.pop_front();
        }
    }
    mutex.unlock();
}

void LoggerBackend::setBufferSize(int sz)
{
    bufferSize = sz;
}

const char *LoggerBackend::getLevelString(int lvl, bool noCol){
    if(!colorsEnabled || noCol){
        if(lvl == 0)       return(" [INFO]");
        else if(lvl == 1)  return(" [WARN]");
        else if(lvl == 2)  return(" [CRIT]");
        else if(lvl < 0)       return("");
        else               return(" [INFO]");
    }

    if(lvl == 0)       return(COLOR_CYAN" [INFO]"COLOR_RESET);
    else if(lvl == 1)  return(COLOR_YELLOW" [WARN]"COLOR_RESET);
    else if(lvl == 2)  return(COLOR_RED" [CRIT]"COLOR_RESET);
    else if(lvl < 0)       return("");
    else               return(COLOR_CYAN" [INFO]"COLOR_RESET);
}

std::list<LogEntry> LoggerBackend::getCurrentEntries()
{
    mutex.lock();
    std::list<LogEntry> copy;
    copy = entries;
    mutex.unlock();
    return copy;
}

void LoggerBackend::registerReciever(LogReciever *r)
{
    mutex.lock();
    recievers.push_back(r);
    mutex.unlock();
}

void LoggerBackend::removeReciever(LogReciever *r)
{
    mutex.lock();
    recievers.erase(std::remove(recievers.begin(), recievers.end(), r), recievers.end());
    mutex.unlock();
}

void LoggerBackend::setColorSupport(bool enable)
{
    this->colorsEnabled = enable;
}



LogEntry::LogEntry(std::string timestamp, std::string string, int level, const char * file, int line)
{
    this->timestamp  = timestamp ;
    this->string     = string    ;
    this->level      = level     ;
    this->file       = file      ;
    this->line       = line      ;
}

std::string LoggerBackend::getColor(){
    int r=(int)(rand()%256);
    int g=(int)(rand()%256);
    int b=(int)(rand()%256);
    return "\x1b[38;2;"+std::to_string(r)+";"+std::to_string(g)+";"+std::to_string(b)+"m";
}

#define LOGGER_NO_EXTRAINFO

#include "log.h"
#include <logger/log.h>
#include <time.h>
#ifdef __linux__
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <ios>
#include <iostream>

#include "loggerbackend.h"

LoggerBackend * Logger::backend = NULL;

void Logger::init(const char * logpath){
    LoggerBackend * backend = new LoggerBackend(logpath);
    Logger::backend = backend;
}

Logger::Logger(const char *file, int line, int what){
    genTimestamp();
    /*if(file != 0){
        this->write(" [");
        this->write(file);
        if(line >= 0){
            this->write(":");
            this->write(line);
            this->write("]");
        }
    }

    if(what == 0)       this->write(" [INFO]: ");
    else if(what == 1)  this->write(" [WARN]: ");
    else if(what == 2)  this->write(" [CRIT]: ");
    else                this->write(" [INFO]: ");*/
    this->line = line;
    this->level = what;
    this->file = file;
}

Logger::~Logger(){
/*    std::string logline = stream.str();
    std::ofstream logfile;
    std::cout << logline;
    logfile.open(backend->logPath,std::ios_base::out | std::ios_base::app);
    logfile << logline;
    if(logline.back() != '\n'){
        logfile << std::endl;
        std::cout << std::endl;
    }
    logfile.close();
*/
    backend->append(timestamp, stream.str(), level, file, line);
}


void Logger::genTimestamp(){

    time_t rawtime;
    time ( &rawtime );
    struct tm *timeinfo = localtime ( &rawtime );



    char datebuf[512];
    sprintf(datebuf,"%02d:%02d:%02d",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

    this->timestamp = datebuf;
}

LoggerBackend *Logger::getBackend()
{
    return backend;
}

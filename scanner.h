#ifndef SCANNER_H
#define SCANNER_H

#include "scanresult.h"
#include <functional>
#include <string>
#include <list>

typedef std::function<void (const ScanResult)> ResultListener;



struct ScanJob{
    enum Type{
        PortScan, PingScan
    };

    std::string addressRange;
    std::string portRange;
    ResultListener event;
    Type type;
    int jobId;

    static ScanJob createPortscanJob(std::string addressRange, std::string portRange, ResultListener event);
    static ScanJob createPingscanJob(std::string addressRange, ResultListener event);

private:
    static int jobIdInc;
    ScanJob();
};


class Scanner
{
public:
    Scanner();
    int getQueueSize();
    int enqueueJob(ScanJob & j);
    void cancelJob(int j);
private:
    std::list<ScanJob> queue;
};

#endif // SCANNER_H

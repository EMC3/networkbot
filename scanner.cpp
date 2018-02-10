#include "scanner.h"

int ScanJob::jobIdInc = 0;

Scanner::Scanner()
{

}

int Scanner::getQueueSize()
{
    return queue.size();
}

int Scanner::enqueueJob(ScanJob &j)
{

}

void Scanner::cancelJob(int j)
{

}

ScanJob ScanJob::createPortscanJob(std::string addressRange, std::string portRange, ResultListener event){
    ScanJob j;
    j.addressRange = addressRange;
    j.portRange = portRange;
    j.event = event;
    j.type = PortScan;
    j.jobId = jobIdInc++;
}

ScanJob ScanJob::createPingscanJob(std::string addressRange, ResultListener event){
    ScanJob j;
    j.addressRange = addressRange;
    j.event = event;
    j.type = PingScan;
    j.jobId = jobIdInc++;
}

ScanJob::ScanJob()
{

}

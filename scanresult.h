#ifndef SCANRESULT_H
#define SCANRESULT_H

#include <string>
#include <vector>

struct Host{
    Host(std::string _ip, std::string _mac = std::string(), std::string _vendor = std::string());
    std::string ip;
    std::string mac;
    std::string vendor;
    std::vector<int> openPorts;
};



class ScanResult
{
public:
    ScanResult();
    int jobId;
    std::vector<Host> nodesUp;
};

#endif // SCANRESULT_H

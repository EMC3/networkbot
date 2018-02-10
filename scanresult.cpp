#include "scanresult.h"

ScanResult::ScanResult()
{

}

Host::Host(std::string _ip, std::string _mac, std::string _vendor){
    ip = _ip;
    mac = _mac;
    vendor = _vendor;
}

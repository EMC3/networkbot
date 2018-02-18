#ifndef DB_H
#define DB_H

#include <QSqlDatabase>

class Db
{
public:
    Db();
    void addScan(int64_t time);

    int getHostIdByIp(std::string ip);
    int getHostIdByMac(std::string mac);
    int getHostIdByName(std::string name);

    int addHost(std::string ip, std::string mac, std::string name);
    void addHostToCurrentScan(int hostId);

    void changeHostIp(int host, std::string name);



    int64_t getLastSeen(int hostId);

    void setHostName(int host, std::string name);
    std::string getHostName(int host);

    std::string getIp(int host);
    std::string getMac(int host);

private:
    int currentScanId;
    QSqlDatabase m_db;
};

#endif // DB_H

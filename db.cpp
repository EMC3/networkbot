#include "db.h"
#include "logger/log.h"
#include <QSqlQuery>
Db::Db()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("history");
    if (!m_db.open()){
        ERR << "Failed to open the Database";
        return;
    }

    QSqlQuery init1("create table if not exists HostsHistory (scanId INTEGER, hostId INTEGER)");
    init1.exec();
    QSqlQuery init2("create table if not exists Timestamps (scanId INTEGER PRIMARY KEY AUTOINCREMENT, timestamp LONG)");
    init2.exec();
    QSqlQuery init3("create table if not exists Hosts (id INTEGER PRIMARY KEY AUTOINCREMENT, macAddress VARCAR(20), ipAddress VARCHAR(20))");
    init3.exec();

    QSqlQuery init4("create table if not exists HostNames (id INTEGER PRIMARY KEY, name VARCHAR(255))");
    init4.exec();
}

void Db::addScan(int64_t time)
{
    QSqlQuery addQuery("INSERT INTO Timestamps (timestamp) VALUES(:ts)");
    addQuery.bindValue(":ts",time);
    addQuery.exec();

    QSqlQuery readBack("SELECT scanId FROM Timestamps WHERE timestamp = :ts");
    readBack.bindValue(":ts",time);
    readBack.exec();

    if (readBack.next()){
        currentScanId = readBack.value(0);
    }else{
        ERR << "Read back failed";
    }

}

int Db::getHostIdByIp(std::string ip)
{
    QSqlQuery readBack("SELECT id FROM Hosts WHERE ipAddress = :ip");
    readBack.bindValue(":ip",ip);
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0);
    }else{
        return -1;
    }
}

int Db::getHostIdByMac(std::string mac)
{
    QSqlQuery readBack("SELECT id FROM Hosts WHERE macAddress = :ip");
    readBack.bindValue(":ip",mac);
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0);
    }else{
        return -1;
    }
}

int Db::getHostIdByName(std::string name)
{
    QSqlQuery readBack("SELECT id FROM HostNames WHERE name = :nm");
    readBack.bindValue(":nm",name);
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0);
    }else{
        return -1;
    }
}

int Db::addHost(std::string ip, std::string mac, std::string name)
{
    QSqlQuery addQuery("INSERT INTO Hosts (macAddress, ipAddress) VALUES(:mac, :ip)");
    addQuery.bindValue(":mac",mac);
    addQuery.bindValue(":ip",ip);
    addQuery.exec();

    QSqlQuery readBack("SELECT id FROM Hosts WHERE macAddress = :mac AND ipAddress = :ip");
    readBack.bindValue(":mac",mac);
    readBack.bindValue(":ip",ip);
    readBack.exec();

    int id;
    if (readBack.next()){
        id = readBack.value(0);
    }else{
        ERR << "Read back failed";
        return -1;
    }

    if(name != ""){
        QSqlQuery addQuery2("INSERT INTO HostNames (id, name) VALUES(:id, :nm)");
        addQuery2.bindValue(":id",id);
        addQuery2.bindValue(":nm",name);
        addQuery2.exec();
    }

    return id;
}

void Db::addHostToCurrentScan(int hostId)
{

}

void Db::changeHostIp(int host, std::string name)
{

}

int64_t Db::getLastSeen(int hostId)
{

}

void Db::setHostName(int host, std::string name)
{
    QSqlQuery addQuery2("INSERT INTO HostNames (id, name) VALUES(:id, :nm) ON DUPLICATE KEY UPDATE name = :nm2");
    addQuery2.bindValue(":id",host);
    addQuery2.bindValue(":nm",name);
    addQuery2.bindValue(":nm2",name);
    addQuery2.exec();
}

std::string Db::getHostName(int host)
{

}

#include "db.h"
#include "logger/log.h"
#include <QSqlQuery>
#include <QVariant>
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
    addQuery.bindValue(":ts",(qlonglong)time);
    addQuery.exec();

    QSqlQuery readBack("SELECT scanId FROM Timestamps WHERE timestamp = :ts");
    readBack.bindValue(":ts",(qlonglong)time);
    readBack.exec();

    if (readBack.next()){
        currentScanId = readBack.value(0).toInt();
    }else{
        ERR << "Read back failed";
    }

}

int Db::getHostIdByIp(std::string ip)
{
    QSqlQuery readBack("SELECT id FROM Hosts WHERE ipAddress = :ip");
    readBack.bindValue(":ip",QString::fromStdString(ip));
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0).toInt();
    }else{
        return -1;
    }
}

int Db::getHostIdByMac(std::string mac)
{
    QSqlQuery readBack("SELECT id FROM Hosts WHERE macAddress = :ip");
    readBack.bindValue(":ip",QString::fromStdString(mac));
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0).toInt();
    }else{
        return -1;
    }
}

int Db::getHostIdByName(std::string name)
{
    QSqlQuery readBack("SELECT id FROM HostNames WHERE name = :nm");
    readBack.bindValue(":nm",QString::fromStdString(name));
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0).toInt();
    }else{
        return -1;
    }
}

int Db::addHost(std::string ip, std::string mac, std::string name)
{
    QSqlQuery addQuery("INSERT INTO Hosts (macAddress, ipAddress) VALUES(:mac, :ip)");
    addQuery.bindValue(":mac",QString::fromStdString(mac));
    addQuery.bindValue(":ip",QString::fromStdString(ip));
    addQuery.exec();

    QSqlQuery readBack("SELECT id FROM Hosts WHERE macAddress = :mac AND ipAddress = :ip");
    readBack.bindValue(":mac",QString::fromStdString(mac));
    readBack.bindValue(":ip",QString::fromStdString(ip));
    readBack.exec();

    int id;
    if (readBack.next()){
        id = readBack.value(0).toInt();
    }else{
        ERR << "Read back failed";
        return -1;
    }

    if(name != ""){
        QSqlQuery addQuery2("INSERT INTO HostNames (id, name) VALUES(:id, :nm)");
        addQuery2.bindValue(":id",id);
        addQuery2.bindValue(":nm",QString::fromStdString(name));
        addQuery2.exec();
    }

    return id;
}

void Db::addHostToCurrentScan(int hostId)
{
    QSqlQuery addQuery("INSERT INTO HostsHistory (scanId, hostId) VALUES(:sc, :hid)");
    addQuery.bindValue(":sc",currentScanId);
    addQuery.bindValue(":hid",hostId);
    addQuery.exec();
}

void Db::changeHostIp(int host, std::string name)
{
    QSqlQuery addQuery("UPDATE hosts SET ip = :nip WHERE id = :hid");
    addQuery.bindValue(":nip",QString::fromStdString(name));
    addQuery.bindValue(":hid",host);
    addQuery.exec();
}

int64_t Db::getLastSeen(int hostId)
{

}

void Db::setHostName(int host, std::string name)
{
    QSqlQuery addQuery2("INSERT INTO HostNames (id, name) VALUES(:id, :nm) ON DUPLICATE KEY UPDATE name = :nm2");
    addQuery2.bindValue(":id",host);
    addQuery2.bindValue(":nm",QString::fromStdString(name));
    addQuery2.bindValue(":nm2",QString::fromStdString(name));
    addQuery2.exec();
}

std::string Db::getHostName(int host)
{
    QSqlQuery readBack("SELECT name FROM HostNames WHERE id = :hid");
    readBack.bindValue(":hid",host);
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0).toString().toStdString();
    }else{
        return "";
    }
}

std::string Db::getIp(int host)
{
    QSqlQuery readBack("SELECT ipAddress FROM Hosts WHERE id = :hid");
    readBack.bindValue(":hid",host);
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0).toString().toStdString();
    }else{
        return "";
    }
}

std::string Db::getMac(int host)
{
    QSqlQuery readBack("SELECT macAddress FROM Hosts WHERE id = :hid");
    readBack.bindValue(":hid",host);
    readBack.exec();

    if (readBack.next()){
        return readBack.value(0).toString().toStdString();
    }else{
        return "";
    }
}

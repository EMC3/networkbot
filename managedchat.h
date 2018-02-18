#ifndef MANAGEDCHAT_H
#define MANAGEDCHAT_H

#include "inttypes.h"
#include "tgbot/tgbot.h"
#include "scanresult.h"
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include "db.h"

class ManagedChat
{
public:
    enum Status{
        Idle, AddDownNotify, AddUpNotify, AddScan, PortscanHost, PortscanPorts, PingscanRange, ChangeNameGetHost, ChangeNameGetName
    };

    ManagedChat(int64_t chatId, TgBot::Bot * bot, Db *db);
    void onList(TgBot::Message::Ptr message);
    void onNotifyUp(TgBot::Message::Ptr message);
    void onNotifyDown(TgBot::Message::Ptr message);
    void onNotifyNew(TgBot::Message::Ptr message);
    void onPortscan(TgBot::Message::Ptr message);
    void onPingscan(TgBot::Message::Ptr message);
    void onAdd(TgBot::Message::Ptr message);
    void onDel(TgBot::Message::Ptr message);
    void onSetname(TgBot::Message::Ptr message);

    void onMessage(TgBot::Message::Ptr message);

    void onScanFinished(ScanResult & res);

    void newDevice(Host & dev);
    void deviceUp(int deviceId);
    void deviceDown(int deviceId);

    void sendMessage(std::stringstream & ss);


    Status status = Idle;

    int64_t chatId;
    TgBot::Bot * bot;

    Db * db;

    std::set<int> notifyUps;
    std::set<int> notifyDowns;
    bool notifyNewDevice = false;
    std::vector<std::string> watchedAddresses;
};

#endif // MANAGEDCHAT_H

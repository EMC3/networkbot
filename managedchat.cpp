#include "managedchat.h"
#include <sstream>
#include "logger/log.h"
using namespace std;

ManagedChat::ManagedChat(int64_t chatId, TgBot::Bot * bot, Db * db)
{
    this->chatId = chatId;
    this->bot = bot;
    this->db = db;
    this->status = Idle;
}

void ManagedChat::onList(TgBot::Message::Ptr message)
{

}

void ManagedChat::onNotifyUp(TgBot::Message::Ptr message)
{
    stringstream s;
    s << "Send me the devices name, ip, mac or id to enable notifications";
    sendMessage(s);
    status = AddUpNotify;
}

void ManagedChat::onNotifyDown(TgBot::Message::Ptr message)
{
    stringstream s;
    s << "Send me the devices name, ip, mac or id to enable notifications";
    sendMessage(s);
    status = AddDownNotify;
}

void ManagedChat::onNotifyNew(TgBot::Message::Ptr message)
{
    this->notifyNewDevice = !notifyNewDevice;
    stringstream s;
    s << "New device notification: ";
    if(notifyNewDevice){
        s << "<b style=\"color:green\">ON</b>";
    }else{
        s << "<b style=\"color:green\">OFF</b>";
    }

    sendMessage(s);
}

void ManagedChat::onPortscan(TgBot::Message::Ptr message)
{
    stringstream s;
    s << "Send me the devices name or ip to begin a portscan";
    sendMessage(s);
    status = PortscanHost;
}

void ManagedChat::onPingscan(TgBot::Message::Ptr message)
{
    stringstream s;
    s << "Send me an address range to discover";
    sendMessage(s);
    status = PingscanRange;
}

void ManagedChat::onAdd(TgBot::Message::Ptr message)
{
    stringstream s;
    s << "Send me an address or address range<br>";
    s << "Example: 1.2.3.4 or 1.2.3.4/24";
    sendMessage(s);
    status = AddScan;
}

void ManagedChat::onDel(TgBot::Message::Ptr message)
{

}

void ManagedChat::onSetname(TgBot::Message::Ptr message)
{
    stringstream s;
    s << "Send me the devices name, ip, mac or id to set or change its name";
    sendMessage(s);
    status = ChangeNameGetHost;
}

void ManagedChat::onMessage(TgBot::Message::Ptr message)
{
    if(status == AddScan) {
        LOG << "Add scan ip: "<<message->text;
        this->watchedAddresses.push_back(message->text);
        stringstream s;
        s << "IP (Range) Added";
        sendMessage(s);
    }
    status = Idle;
}

void ManagedChat::onScanFinished(ScanResult &res)
{

}

void ManagedChat::newDevice(Host &dev)
{
    stringstream s;
    s << "<b>New device detected</b><br>";
    s << "IP: "<<dev.ip<<"<br>";
    if(dev.mac != ""){
        s << "Mac: "<<dev.mac;
        if(dev.vendor != "")s <<" ("<<dev.vendor<<")";
    }

    sendMessage(s);
}

void ManagedChat::deviceUp(int deviceId)
{
    stringstream s;
    s << "<b>Device is Online:</b><br>";
    s << "Name: "<<db->getHostName(deviceId)<<"<br>";
    s << "Ip: "<<db->getIp(deviceId);

    sendMessage(s);
}

void ManagedChat::deviceDown(int deviceId)
{
    stringstream s;
    s << "<b>Device is Offline:</b><br>";
    s << "Name: "<<db->getHostName(deviceId)<<"<br>";
    s << "last Ip: "<<db->getIp(deviceId);

    sendMessage(s);

}

void ManagedChat::sendMessage(stringstream &ss)
{
    bot->getApi().sendMessage(chatId,ss.str(), false, 0, std::make_shared<TgBot::GenericReply>(), "HTML");
}

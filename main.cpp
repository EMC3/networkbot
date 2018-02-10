#include <QApplication>
#include <tgbot/tgbot.h>
#include "config.h"
#include <string>
#include "logger/log.h"
#include "managedchat.h"
#include <map>
#include <boost/algorithm/string.hpp>
#include "command.h"
#include <QDateTime>
#include "scanner.h"

using namespace std;

map<int64_t,ManagedChat> managedChats;

int lastJob;

struct Unauthorized
{
    Unauthorized() {}
};

ManagedChat & getChat(int64_t chatId){
    auto i = managedChats.find(chatId);
    if(i == managedChats.end())throw Unauthorized();
    return i->second;
}

void regularPingscanResult(ScanResult r){
    for(Host h : r.nodesUp){
        string identifier = (h.mac=="") ? h.ip : h.mac;

    }

    if(r.jobId == lastJob){

    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Logger::init("latest.log");


    LOG << "Loading Config...";
    Config conf;


    string botToken = Config::get<string>("token","insert your token here");
    string password = Config::get<string>("password","someSecurePassword");
    Config::set("token",botToken);
    Config::set("password",password);

    Scanner networkScanner;


    LOG << "Setting up Bot...";

    TgBot::Bot bot(botToken);
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
        LOG << "Start command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
        bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
    });

    bot.getEvents().onCommand("list", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onList(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("notifyup", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onNotifyUp(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("notifydown", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onNotifyDown(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("notifynew", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onNotifyNew(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("portscan", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onPortscan(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });


    bot.getEvents().onCommand("pingscan", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onPingscan(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("add", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onAdd(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("del", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onDel(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("setname", [&bot](TgBot::Message::Ptr message) {
        try{
            getChat(message->chat->id).onSetname(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message) {
        try{
            ManagedChat & mc = getChat(message->chat->id);
            if(mc.status != 0)mc.onMessage(message);
        }catch(Unauthorized &error){
            bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
            ERR << "Unauthorized command from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Hello. I'm the network monitoring bot. Please use the command /enable <PASSWORD> to start. The password can be set inside the config. THIS IS A PRIVATE BOT!");
        }
    });

    bot.getEvents().onCommand("enable", [&bot, password](TgBot::Message::Ptr message) {
        Command cmd(message);
        if(cmd.argsCount() != 2){
            bot.getApi().sendMessage(message->chat->id, "Please use /enable <password>");
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your password was Wrong and we have logged everything muhahahahaha");
        bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;

        string pw = cmd.arg<string>(1);
        if(pw == password){
            LOG << "Accepted login from: "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title);
            bot.getApi().sendMessage(message->chat->id, "Your chat was authorized succesfully!");

            vector<int64_t> managedChatIds = Config::get("managedChats",vector<int64_t>());
            managedChatIds.push_back(message->chat->id);
            Config::set("managedChats",managedChatIds);
            Config::save();

            managedChats.emplace(message->chat->id,ManagedChat(message->chat->id, &bot));

            return;
        }

        WARN << "Login attempt from "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title)<<" Used Password: "<< pw;
    });

    vector<int64_t> managedChatIds = Config::get("managedChats",vector<int64_t>());

    for(int64_t chatId : managedChatIds)managedChats.emplace(chatId,ManagedChat(chatId, &bot));

    LOG << "Starting!";
    try {
        LOG << "Bot username: " << bot.getApi().getMe()->username;
        TgBot::TgLongPoll longPoll(bot);
        longPoll.setMaxTime(30);
        int64_t lastScan = 0;
        while (true) {
            longPoll.start();
            if((QDateTime::currentMSecsSinceEpoch() - lastScan)/1000 > Config::get<int64_t>("scanInterval",60)){
                lastScan = QDateTime::currentMSecsSinceEpoch();
                if(networkScanner.getQueueSize() > 0){
                    WARN << "Scan interval elapsed but Scanner still working. Skipping";
                    continue;
                }

                std::set<std::string> adresses;

                for(auto i = managedChats.begin(); i != managedChats.end(); i++){
                    for(std::string str : i->second.watchedAddresses){
                        adresses.insert(str);
                    }
                }

                LOG << "Starting periodic scan with "<<adresses.size()<<" addresses or address ranges";

                for(std::string addr : adresses){
                    ScanJob sj = ScanJob::createPingscanJob(addr,regularPingscanResult);
                    lastJob = networkScanner.enqueueJob(sj);
                }
            }
        }
    } catch (TgBot::TgException& e) {
        LOG << "error: "<< e.what();
    }

    conf.save();

    return 0;
}

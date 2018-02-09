#include <QApplication>
#include <tgbot/tgbot.h>
#include "config.h"
#include <string>
#include "logger/log.h"
#include "managedchat.h"
#include <map>
#include <boost/algorithm/string.hpp>
#include "command.h"
using namespace std;

map<int64_t,ManagedChat> managedChats;

struct Unauthorized
{
    Unauthorized() {}
};

ManagedChat getChat(int64_t chatId){
    auto i = managedChats.find(chatId);
    if(i == managedChats.end())throw Unauthorized();
    return i->second;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Config conf;

    string botToken = Config::get<string>("token","insert your token here");
    string password = Config::get<string>("password","someSecurePassword");
    Config::set("token",botToken);
    Config::set("password",password);




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

    bot.getEvents().onCommand("enable", [&bot, password](TgBot::Message::Ptr message) {
        Command cmd(message);
        if(cmd.argsCount() != 2){
            bot.getApi().sendMessage(message->chat->id, "Please use /enable <password>");
            return;
        }
        string pw = cmd.arg<string>(1);
        if(pw == password){
            bot.getApi().sendMessage(message->chat->id, "Your chat was authorized succesfully!");

            vector<int64_t> managedChatIds = Config::get("managedChats",vector<int64_t>());
            managedChatIds.push_back(message->chat->id);
            Config::set("managedChats",managedChatIds);
            Config::save();
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your password was Wrong and we have logged everything muhahahahaha");
        bool hasUsername = message->chat->type == TgBot::Chat::Type::Private || message->chat->type == TgBot::Chat::Type::Channel;
        WARN << "Login attempt from "<<message->chat->id<< " "<< (hasUsername ? message->chat->username : message->chat->title)<<" Used Password: "<< pw;
    });

    vector<int64_t> managedChatIds = Config::get("managedChats",vector<int64_t>());

    for(int64_t chatId : managedChatIds)managedChats.emplace(chatId,ManagedChat(chatId, bot));

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }

    conf.save();

    return 0;
}

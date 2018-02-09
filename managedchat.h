#ifndef MANAGEDCHAT_H
#define MANAGEDCHAT_H

#include "inttypes.h"
#include "tgbot/tgbot.h"

class ManagedChat
{
public:
    ManagedChat(int64_t chatId, TgBot::Bot &bot);
    void onList(TgBot::Message::Ptr message);
    void onNotifyUp(TgBot::Message::Ptr message);
    void onNotifyDown(TgBot::Message::Ptr message);
    void onNotifyNew(TgBot::Message::Ptr message);
    void onPortscan(TgBot::Message::Ptr message);
    void onPingscan(TgBot::Message::Ptr message);
    void onAdd(TgBot::Message::Ptr message);
    void onDel(TgBot::Message::Ptr message);
    void onSetname(TgBot::Message::Ptr message);

private:
    int64_t chatId;
    TgBot::Bot bot;
};

#endif // MANAGEDCHAT_H

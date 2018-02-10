#include "managedchat.h"

ManagedChat::ManagedChat(int64_t chatId, TgBot::Bot * bot)
{
    this->chatId = chatId;
    this->bot = bot;

}

void ManagedChat::onList(TgBot::Message::Ptr message)
{

}

void ManagedChat::onNotifyUp(TgBot::Message::Ptr message)
{

}

void ManagedChat::onNotifyDown(TgBot::Message::Ptr message)
{

}

void ManagedChat::onNotifyNew(TgBot::Message::Ptr message)
{

}

void ManagedChat::onPortscan(TgBot::Message::Ptr message)
{

}

void ManagedChat::onPingscan(TgBot::Message::Ptr message)
{

}

void ManagedChat::onAdd(TgBot::Message::Ptr message)
{

}

void ManagedChat::onDel(TgBot::Message::Ptr message)
{

}

void ManagedChat::onSetname(TgBot::Message::Ptr message)
{

}

void ManagedChat::onMessage(TgBot::Message::Ptr message)
{

}

void ManagedChat::onScanFinished(ScanResult &res)
{

}

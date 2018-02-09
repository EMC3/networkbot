#ifndef COMMAND_H
#define COMMAND_H
#include "tgbot/types.h"
#include <vector>
class Command
{
public:
    Command(TgBot::Message::Ptr message);

    int argsCount();

    template<typename T>
    T arg( int x){
        T ret;
        std::istringstream strm(args[x]);
        strm >> ret;
        return ret;
    }


    std::vector<std::string> args;
};


#endif // COMMAND_H

#include "command.h"
#include "boost/algorithm/string.hpp"
using namespace std;

Command::Command(TgBot::Message::Ptr message)
{
    string cmd = message->text;
    boost::split(args,cmd,boost::is_any_of(" "));
}

int Command::argsCount()
{
    return args.size();
}

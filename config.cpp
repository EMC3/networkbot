#include "config.h"
#include <logger/log.h>
#include <fstream>
#include <iostream>
#include "string.h"
Config * Config::instance = NULL;

Config::Config()
{
    try{
        root = YAML::LoadFile("config.yml");
    }catch(YAML::Exception &e){
        std::cerr << "Failed to load config: "<<e.what()<<std::endl;
        std::cerr << "Default values will be used!"<<std::endl;
        root = YAML::Node();
    }
    instance = this;
}

YAML::Node Config::getNode(const char *path, YAML::Node parent)
{
    const char * next = strstr(path,"/");
    if(next == 0)return parent[path];
    std::string path2 = std::string(path,next-path);
    const char * path1 = next+1;
    return getNode(path1,parent[path2]);
}

void Config::setNode(const char *path, YAML::Node parent, YAML::Node value)
{
    const  char * next = strstr(path,"/");
    if(next == 0){
        parent[path] = value;
        return;
    }
    if(!parent[std::string(path,next-path)]){
        parent[std::string(path,next-path)] = YAML::Node();
    }
    setNode(next+1,parent[std::string(path,next-path)],value);
}

void Config::save(){
    instance->_save();
}

void Config::_save()
{
    std::ofstream strm;
    strm.open("config.yml");
    strm << "#In case of errors this file will be resetted to default without prior warning!"<<std::endl;
    strm << root;
    strm.close();

}

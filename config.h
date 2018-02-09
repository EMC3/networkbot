#ifndef CONFIG_H
#define CONFIG_H

#include <yaml-cpp/yaml.h>
#include <dataman/yaml2qstring.h>

class Config
{
public:
    Config();

    YAML::Node getNode(const char * path){
        try{
            return getNode(path,root);
        }catch(YAML::Exception &e){
            return YAML::Node();
        }
    }
    void setNode(const char * path, YAML::Node value){
        try{
            setNode(path,root,value);
        }catch(YAML::Exception &e){

        }
    }


    template<typename T> T _get(const char * path, T def){
        try{
            return getNode(path,root).as<T>();
        }catch(YAML::Exception &e){
            return def;
        }
    }
    template<typename T> void _set(const char * path, T value){
        try{
            YAML::Node vn(value);
            setNode(path,root,vn);
        }catch(YAML::Exception &e){

        }
    }

    template<typename T> static void set(const char * path, T value){
        instance->_set(path,value);
    }

    template<typename T> static void set(std::string path, T value){
        instance->_set(path.c_str(),value);
    }

    template<typename T> static void set(QString path, T value){
        instance->_set(qPrintable(path),value);
    }

    template<typename T> static T get(const char * path, T def){
        return instance->_get(path,def);
    }

    template<typename T> static T get(std::string path, T def){
        return instance->_get(path.c_str(),def);
    }

    template<typename T> static T get(QString path, T def){
        return instance->_get(path.toUtf8().constData(),def);
    }

    static void save();

    void _save();
private:
    YAML::Node getNode(const char * path, YAML::Node parent);
    void setNode(const char * path, YAML::Node parent, YAML::Node value);
    YAML::Node root;
    static Config * instance;
};

#endif // CONFIG_H

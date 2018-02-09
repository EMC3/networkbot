#ifndef YAML2QSTRING_H
#define YAML2QSTRING_H

#include <yaml-cpp/yaml.h>
#include <QString>

#include <QList>


namespace YAML {
template<>
struct convert<QString> {
  static Node encode(const QString& rhs) {
    Node node(rhs.toStdString());
    return node;
  }

  static bool decode(const Node& node, QString& rhs) {
      rhs.clear();
      rhs.append(QString::fromStdString(node.Scalar()));

     return true;
  }
};


}


namespace YAML {
template<typename T>
struct convert<QList<T>> {
  static Node encode(const QList<T>& rhs) {
    Node node;
    foreach (T entry, rhs) {
        node.push_back(entry);
    }
    return node;
  }

  static bool decode(const Node& node, QList<T>& rhs) {
      rhs.clear();
      for(auto i = node.begin(); i != node.end(); i++){
          rhs.push_back(i->as<T>());
      }
     return true;
  }
};

}


#endif // YAML2QSTRING_H

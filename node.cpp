#include "node.h"
QSet<unsigned int> Node::idSet = QSet<unsigned int>();  //初始化id集合
Node::Node(QString displayName, NodeType type, unsigned int x, unsigned int y)
{
    this->displayName = displayName;
    this->type        = type;
    this->x           = x;
    this->y           = y;
    //为当前对象申请唯一id
    unsigned int count = idSet.size();
    while (idSet.contains(count)) {
        count++;
    }
    idSet.insert(count);
}

Node::Node(QString displayName, NodeType type, unsigned int x, unsigned int y, unsigned int id)
{
    this->displayName = displayName;
    this->type        = type;
    this->x           = x;
    this->y           = y;
    if (idSet.contains(id)) {
        throw "对象id冲突！";
    }
    this->id = id;
}

unsigned int Node::getId() const
{
    return id;
}

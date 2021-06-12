#include "edge.h"
QSet<unsigned int> Edge::idSet = QSet<unsigned int>();  //初始化id集合
Edge::Edge(QString displayName, EdgeType type, unsigned int length, float cost)
{
    this->displayName = displayName;
    this->type        = type;
    this->length      = length;
    this->cost        = cost;
    //为当前对象申请唯一id
    unsigned int count = idSet.size();
    while (idSet.contains(count)) {
        count++;
    }
    idSet.insert(count);
}

Edge::Edge(QString displayName, EdgeType type, unsigned int length, float cost, unsigned int id)
{
    this->displayName = displayName;
    this->type        = type;
    this->length      = length;
    this->cost        = cost;
    if (idSet.contains(id)) {
        throw "对象id冲突！";
    }
    this->id = id;
}

unsigned int Edge::getId() const
{
    return id;
}

#ifndef EDGE_H
#define EDGE_H
#include <QSet>
#include <QString>
enum EdgeType
{                 //道路类型
    Highways,     //高速路
    Generalroads  //普通路
};
//道路
class Edge
{
public:
    Edge(QString displayName, EdgeType type, unsigned int length, float cost);                   //普通对象构造
    Edge(QString displayName, EdgeType type, unsigned int length, float cost, unsigned int id);  //用于反序列化对象

    unsigned int getId() const;  //获取当前对象唯一id

public:
    QString      displayName;  //显示名称
    EdgeType     type;         //道路类型
    unsigned int length;       //长度，非负，单位米,范围0 〜 4 294 967 295
    float        cost;         //收费，非负,单位：元
    float        Congestion;   //拥堵系数,范围(0,1]

private:
    static QSet<unsigned int> idSet;  //存储所有已注册节点id
    unsigned int              id;     //储存当前节点唯一id(自增)
};

#endif  // EDGE_H

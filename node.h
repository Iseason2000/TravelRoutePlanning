#ifndef NODE_H
#define NODE_H
#include <QMap>
#include <QSet>
#include <QString>
#include "edge.h"
enum NodeType
{             //节点类型,用于图标显示
    Mid,      //中间目标
    End,      //道路端点
    Cross,    //交叉路口
    CrossEnd  //端点+交叉路口
};

class Node  //节点类
{
public:                                                                                         //函数
    Node(QString displayName, NodeType type, unsigned int x, unsigned int y);                   //构造函数
    Node(QString displayName, NodeType type, unsigned int x, unsigned int y, unsigned int id);  //用于反序列化对象
    unsigned int getId() const;                                                                 //获取唯一id

public:                                    //变量
    QString      displayName;              //显示名称
    NodeType     type;                     //节点类型
    unsigned int x, y;                     //在map中的坐标
    bool         hasTrafficLight = false;  //是否有红绿灯,仅限交叉路口可以为true,默认为false;
    float        score;                    //用户评分,范围[0.0,10.0]

private:
    static QSet<unsigned int> idSet;  //存储所有已注册节点id
    unsigned int              id;     //储存当前节点唯一id(自增)
};

#endif  // NODE_H

#ifndef NODE_H
#define NODE_H
#include <QGraphicsItem>
#include <QMap>
#include <QPainter>
#include <QString>
#include "edge.h"

class Node : public QGraphicsItem  //节点类
{
public:  //变量
    enum NodeType
    {              //节点类型,用于图标显示
        Mid,       //中间目标
        End,       //道路端点
        Cross,     //交叉路口
        CrossEnd,  //端点+交叉路口

    };
    QString      displayName;              //显示名称
    NodeType     type;                     //节点类型
    unsigned int x, y;                     //在map中的坐标
    bool         hasTrafficLight = false;  //是否有红绿灯,仅限交叉路口可以为true,默认为false;
    float        score           = 5.0;    //用户评分,范围[0.0,10.0]
    bool         isEmpty         = true;   //是否为空节点
    bool         isSelected      = false;  //是否被选中

public:  //函数
    ~Node();
    Node(unsigned int x, unsigned int y);                                                       //用于构造空节点
    Node(QString displayName, NodeType type, unsigned int x, unsigned int y);                   //构造函数
    Node(QString displayName, NodeType type, unsigned int x, unsigned int y, unsigned int id);  //用于反序列化对象
    unsigned int getId() const;                                                                 //获取唯一id
    QImage       getIcon();                                                                     //获取图标
    void         linkWith(Node& node, Edge& edge);                                              //与点以边链接
    void         unlinkWith(Node& node);                                                        //取消与点的链接
    static Node* getByID(unsigned int id);                                                      //以ID获取节点,不存在返回null
    QRectF       boundingRect() const override;
    void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    unsigned int                     id;                                               //储存当前节点唯一id(自增)
    QMap<unsigned int, unsigned int> neighbours = QMap<unsigned int, unsigned int>();  //使用邻接表储存映射关系.前者为节点id，后者为边id
    static QMap<unsigned int, Node*> idMap;                                            //存储所有已注册节点id
    static QPen                      boundingPen;                                      //设置边框画笔
    static QPen                      selectedPen;                                      //设置边框画笔
    static QFont                     font;                                             //显示字体

private:           //函数
    void setID();  //设置唯一id
};

#endif  // NODE_H

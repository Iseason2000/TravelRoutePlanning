#ifndef EDGE_H
#define EDGE_H
#include <QGraphicsLineItem>
#include <QMap>
#include <QPen>
#include <QString>
//道路
class Edge : public QGraphicsLineItem
{
public:
    enum EdgeType  //道路类型
    {
        Highways,     //高速路
        Generalroads  //普通路
    };
    QString      displayName;  //显示名称
    EdgeType     type;         //道路类型
    unsigned int length;       //长度，非负，单位米,范围0 〜 4 294 967 295
    float        cost;         //收费，非负,单位：元
    float        Congestion;   //拥堵系数,范围(0,1]
    static QPen  linePen;      //用于划线

public:
    ~Edge();
    Edge(QString displayName, EdgeType type, unsigned int length, float cost);                   //普通对象构造
    Edge(QString displayName, EdgeType type, unsigned int length, float cost, unsigned int id);  //用于反序列化对象
    static Edge* getByID(unsigned int id);                                                       //以ID获取节点,不存在返回null
    unsigned int getId() const;                                                                  //获取当前对象唯一id
    QRectF       boundingRect() const override;
    void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    static QMap<unsigned int, Edge*> idMap;  //存储所有已注册节点id
    unsigned int                     id;     //储存当前节点唯一id(自增)
private:
    void setID();  //设置唯一id
};

#endif  // EDGE_H

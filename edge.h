#ifndef EDGE_H
#define EDGE_H
#include <QGraphicsLineItem>
#include <QJsonObject>
#include <QMap>
#include <QPen>
#include <QString>
#include <QtDebug>
//道路
class Edge : public QGraphicsLineItem
{
public:
    enum EdgeType  //道路类型
    {
        Highways,     //高速路 120km/h
        Generalroads  //普通路 40km/h
    };
    QString               displayName;         //显示名称
    EdgeType              type;                //道路类型
    unsigned int          length     = 0.0;    //长度，非负，单位米,范围0 〜 4 294 967 295
    float                 cost       = 0.0;    //收费，非负,单位：元
    float                 congestion = 1.0;    //拥堵系数,范围(0,1] 1 表示不拥堵
    bool                  isLighting = false;  //是否高亮
    QLineF                line;                //绘制的线路径
    static QVector<Edge*> getEdges();          //获取所有已注册节点
    static QPen           normalLinePen;       //用于普通路划线
    static QPen           highLinePen;         //用于高速路划线
    static QPen           lightLinePen;        //用于高亮路划线

public:
    ~Edge();
    Edge(QString displayName, EdgeType type, unsigned int length, float cost);                   //普通对象构造
    Edge(QString displayName, EdgeType type, unsigned int length, float cost, unsigned int id);  //用于反序列化对象
    void         drawLine(QLineF line);                                                          //绘制线
    float        getWeight(int searchType, bool isWalking);                                      //根据类型获取权值
    QJsonObject  toJsonObject();                                                                 //转化为Json对象,序列化
    static Edge* fromJsonObject(QJsonObject object);                                             //由json对象创建对象，反序列化
    static Edge* getByID(unsigned int id);                                                       //以ID获取节点,不存在返回null
    unsigned int getId() const;                                                                  //获取当前对象唯一id
    static void  reSet();                                                                        //重置id
    QRectF       boundingRect() const override;
    void         paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    static QMap<unsigned int, Edge*> getIdMap();

private:
    static QMap<unsigned int, Edge*> idMap;  //存储所有已注册节点id
    unsigned int                     id;     //储存当前节点唯一id(自增)
private:
    void setID();  //设置唯一id
};

#endif  // EDGE_H

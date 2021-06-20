#include "edge.h"
#include <QPainter>
#include <QtDebug>
QMap<unsigned int, Edge*> Edge::idMap         = QMap<unsigned int, Edge*>();  //初始化id表
QPen                      Edge::normalLinePen = QPen(QColor(50, 100, 250), 4);
QPen                      Edge::highLinePen   = QPen(QColor(50, 200, 100), 4);
QPen                      Edge::lightLinePen  = QPen(QColor(250, 50, 100), 5);
Edge::~Edge()
{
    idMap.remove(id);
}

Edge::Edge(QString displayName, EdgeType type, unsigned int length, float cost)
{
    this->displayName = displayName;
    this->type        = type;
    this->length      = length;
    this->cost        = cost;
    setID();  //为当前对象申请唯一id
}

Edge::Edge(QString displayName, EdgeType type, unsigned int length, float cost, unsigned int id)
{
    this->displayName = displayName;
    this->type        = type;
    this->length      = length;
    this->cost        = cost;
    if (idMap.contains(id)) {
        throw "对象id冲突!";
    }
    this->id = id;
    idMap.insert(id, this);
}

Edge* Edge::getByID(unsigned int id)
{
    if (idMap.contains(id))
        return idMap[id];
    return nullptr;
}

unsigned int Edge::getId() const
{
    return id;
}

QRectF Edge::boundingRect() const
{
    return QGraphicsLineItem::boundingRect();
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (isLighting)
        setPen(Edge::lightLinePen);
    else {
        if (type == Highways) {
            setPen(Edge::highLinePen);
        } else {
            setPen(Edge::normalLinePen);
        }
    }
    QGraphicsLineItem::paint(painter, option, widget);
}

void Edge::setID()
{
    auto count = idMap.size();
    while (idMap.contains(count)) {
        count++;
    }
    idMap.insert(count, this);
    this->id = count;
}

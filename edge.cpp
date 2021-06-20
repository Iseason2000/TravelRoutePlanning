#include "edge.h"

QMap<unsigned int, Edge*> Edge::idMap         = QMap<unsigned int, Edge*>();  //初始化id表
QPen                      Edge::normalLinePen = QPen(QColor(50, 200, 100), 4);
QPen                      Edge::highLinePen   = QPen(QColor(50, 100, 250), 4);
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
QVector<Edge*> Edge::getEdges()
{
    return Edge::idMap.values().toVector();
}

void Edge::drawLine(QLineF line)
{
    this->line = line;
}

QJsonObject Edge::toJsonObject()
{
    QJsonObject obj;
    obj["name"]       = displayName;
    obj["type"]       = type;
    obj["length"]     = (int)length;
    obj["cost"]       = cost;
    obj["congestion"] = congestion;
    obj["line"]       = QString::number(line.x1())
                      .append(",")
                      .append(QString::number(line.y1()))
                      .append(",")
                      .append(QString::number(line.x2()))
                      .append(",")
                      .append(QString::number(line.y2()));
    obj["id"] = (int)id;
    return obj;
}

Edge* Edge::fromJsonObject(QJsonObject object)
{
    auto  displayName = object["name"].toString();
    auto  type        = EdgeType(object["type"].toInt());
    auto  length      = object["length"].toInt();
    auto  cost        = object["cost"].toDouble();
    auto  congestion  = object["congestion"].toDouble();
    auto  id          = object["id"].toInt();
    auto  lineP       = object["line"].toString().split(",");
    Edge* edge        = new Edge(displayName, type, length, cost, id);

    edge->drawLine(QLineF(lineP[0].toInt(), lineP[1].toInt(), lineP[2].toInt(), lineP[3].toInt()));
    edge->congestion = congestion;
    return edge;
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

void Edge::reSet()
{
    Edge::idMap.clear();
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
    setLine(line);
    QGraphicsLineItem::paint(painter, option, widget);
}

QMap<unsigned int, Edge*> Edge::getIdMap()
{
    return idMap;
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

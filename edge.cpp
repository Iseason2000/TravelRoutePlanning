#include "edge.h"

QMap<unsigned int, Edge*> Edge::idMap         = QMap<unsigned int, Edge*>();  //初始化id表
QPen                      Edge::normalLinePen = QPen(QColor(50, 200, 100), 4);
QPen                      Edge::highLinePen   = QPen(QColor(50, 100, 250), 4);
QPen                      Edge::lightLinePen  = QPen(QColor(250, 50, 100), 6);

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

float Edge::getWeight(int searchType, bool isWalking)
{
    switch (searchType) {
        case 0: {
            return length;
        }
        case 1: {
            if (isWalking) {
                return length;
            } else {
                float speed;
                if (congestion <= 0)
                    congestion = 0.05F;
                if (type == Highways) {
                    speed = 120 * congestion;
                } else {
                    speed = 40 * congestion;
                }
                return length / speed;
            }
        }
        case 2: {
            return 1;
        }
        case 3: {
            return 0;  //红绿灯不能判断
        }
        case 4: {
            return cost;
        }
        case 5: {
            return (int)(type != Highways);  // 为高速时返回0 否则1
        }
        default: {
            return (int)(type != Generalroads);  // 为普通时时返回0 否则1
        }
    }
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
        QPen pen;
        if (type == Highways) {
            pen = Edge::highLinePen;
        } else {
            pen = Edge::normalLinePen;
        }
        auto   color = pen.color();
        QColor c     = QColor(color.red() + 150 * (1 - congestion), color.green(), color.blue());
        setPen(QPen(c, 5));
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

#include "node.h"
#include <QJsonDocument>
#include <QMessageBox>
QMap<unsigned int, Node*> Node::idMap       = QMap<unsigned int, Node*>();  //初始化id表
QPen                      Node::boundingPen = QPen(QColor(180, 180, 180));
QPen                      Node::selectedPen = QPen(QColor(200, 40, 40), 4);
QFont                     Node::font        = QFont("SimSun", 5);
Node::~Node()
{
    idMap.remove(id);
}

Node::Node(unsigned int x, unsigned int y)
{
    this->x = x;
    this->y = y;
}

Node::Node(QString displayName, NodeType type, unsigned int x, unsigned int y)
{
    this->displayName = displayName;
    this->type        = type;
    this->x           = x;
    this->y           = y;
    this->isEmpty     = false;
    //为当前对象申请唯一id
    auto count = idMap.size();
    while (idMap.contains(count)) {
        count++;
    }
    idMap.insert(count, this);
    this->id = count;
}

Node::Node(QString displayName, NodeType type, unsigned int x, unsigned int y, unsigned int id)
{
    this->displayName = displayName;
    this->type        = type;
    this->x           = x;
    this->y           = y;
    this->isEmpty     = false;
    if (idMap.contains(id)) {
        QMessageBox::warning(NULL, "错误", "对象id冲突!请检查id唯一性", QMessageBox::Yes, QMessageBox::Yes);
        //        throw "对象id冲突!";
    }
    this->id = id;
    idMap.insert(id, this);
}

QJsonObject Node::toJsonObject()
{
    QJsonObject obj;
    obj["name"]            = displayName;
    obj["type"]            = type;
    obj["pos"]             = QString::number(x) + "," + QString::number(y);
    obj["hasTrafficLight"] = hasTrafficLight;
    obj["score"]           = score;
    obj["id"]              = (int)id;
    QJsonObject neighbour;
    auto        iter = neighbours.begin();
    while (iter != neighbours.end()) {
        neighbour[QString::number(iter.key())] = (int)iter.value();
        iter++;
    }
    obj["neighbours"] = neighbour;
    return obj;
}

Node* Node::fromJsonObject(QJsonObject object)
{
    auto name            = object["name"].toString();
    auto type            = Node::NodeType(object["type"].toInt());
    auto pos             = object["pos"].toString().split(",");
    auto hasTrafficLight = object["hasTrafficLight"].toBool();
    auto score           = (float)object["score"].toDouble();
    auto x               = pos[0].toInt();
    auto y               = pos[1].toInt();
    auto id              = object["id"].toInt();
    auto variantMap      = object["neighbours"].toObject().toVariantMap();

    QMap<unsigned int, unsigned int> neighbours;
    auto                             iter = variantMap.begin();
    while (iter != variantMap.end()) {
        neighbours[iter.key().toUInt()] = iter.value().toUInt();
        iter++;
    }
    Node* node            = new Node(name, type, x, y, id);
    node->hasTrafficLight = hasTrafficLight;
    node->score           = score;
    node->neighbours      = neighbours;
    return node;
}

unsigned int Node::getId() const
{
    return id;
}

QImage Node::getIcon()
{
    if (type == Mid)
        return QImage("://icons/mid.png");
    if (type == End)
        return QImage("://icons/end.png");
    if (type == Cross)
        return QImage("://icons/cross.png");
    return QImage("://icons/crossmid.png");
}

bool Node::linkWith(Node* node, Edge* edge)
{
    if (neighbours.contains(node->getId()))
        return false;
    neighbours.insert(node->getId(), edge->getId());
    return true;
}

void Node::unlinkWith(Node* node)
{
    neighbours.remove(node->getId());
}

bool Node::hasNeighbour()
{
    return !neighbours.isEmpty();
}

Node* Node::getByID(unsigned int id)
{
    if (idMap.contains(id))
        return idMap[id];
    return nullptr;
}

Node* Node::getByName(QString name)
{
    foreach (auto id, idMap) {
        if (id->displayName == name)
            return id;
    }
    return nullptr;
}

void Node::reSet()
{
    Node::idMap.clear();
}

QRectF Node::boundingRect() const
{
    return QRectF(0, 0, 50, 50);
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //设置绘制内容
    if (isSelected)
        painter->setPen(selectedPen);
    else
        painter->setPen(boundingPen);
    painter->drawRect(1, 1, 49, 49);
    if (isEmpty)
        return;
    painter->setFont(font);
    painter->drawImage(boundingRect(), getIcon());
    painter->drawText(5, 40, displayName);
}

QMap<unsigned int, Node*> Node::getIdMap()
{
    return idMap;
}

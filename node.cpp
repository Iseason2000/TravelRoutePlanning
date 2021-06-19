#include "node.h"
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
    setID();  //为当前对象申请唯一id
}

Node::Node(QString displayName, NodeType type, unsigned int x, unsigned int y, unsigned int id)
{
    this->displayName = displayName;
    this->type        = type;
    this->x           = x;
    this->y           = y;
    this->isEmpty     = false;
    if (idMap.contains(id)) {
        throw "对象id冲突!";
    }
    this->id = id;
    idMap.insert(id, this);
}

void Node::setID()
{
    auto count = idMap.size();
    while (idMap.contains(count)) {
        count++;
    }
    idMap.insert(count, this);
    this->id = count;
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

void Node::linkWith(Node& node, Edge& edge)
{
    neighbours.insert(node.getId(), edge.getId());
}

void Node::unlinkWith(Node& node)
{
    neighbours.remove(node.getId());
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

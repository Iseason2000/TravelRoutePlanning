#include "map.h"
#include <QGraphicsRectItem>
#include <QGridLayout>
#include <QPushButton>
Map::Map(unsigned int x, unsigned int y, QGraphicsView* graphicsView)
{
    this->x            = x;
    this->y            = y;
    this->graphicsView = graphicsView;
}

void Map::initMap()
{
    QGraphicsScene* scene = new QGraphicsScene();
    for (unsigned int i = 0; i < x; i++) {
        for (unsigned int n = 0; n < y; n++) {
            auto item = new Node(i, n);  // 定义一个矩形图元
            item->setPos(i * 50, n * 50);
            scene->addItem(item);
        }
    }
    graphicsView->setScene(scene);
    graphicsView->update();
}

QVector<Node*> Map::PathSearch(Node* start, Node* end, Map::PathSearchType type, bool isWalking)
{
    return QVector<Node*>();
}

QVector<Node*> Map::roundSearch(Node* center, unsigned int radius, RoundSearchType type, bool isNearest, bool isBestScore)
{
    // center :中心点
    // radius :搜索半径
    // type :搜索的节点类型,在节点名称中查找，比如“酒店”类型 搜索所有名字中带有“酒店”的节点
    // isNearest :是否最近，如果为true，则显示距离最近的节点
    // isBestScore ：是否评分最好，如果为true，则显示评分最好的节点
    // isNearest与isBestScore不能同时为true，但可以同时为false

    return QVector<Node*>();
}

unsigned int Map::getX() const
{
    return x;
}

unsigned int Map::getY() const
{
    return y;
}

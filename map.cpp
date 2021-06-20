#include "map.h"
#include <QGraphicsRectItem>
#include <QGridLayout>
#include <QPushButton>
Map::Map(unsigned int x, unsigned int y, QGraphicsView* graphicsView)
{
    nodes              = QVector<Node>();
    edges              = QVector<Edge>();
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

// void Map::link(Node& fistNode, Node& secondNode, Edge& edge)
//{
//    fistNode.linkWith(secondNode, edge);
//    secondNode.linkWith(fistNode, edge);
//}

// void Map::unlink(Node& fistNode, Node& secondNode)
//{
//    fistNode.unlinkWith(secondNode);
//    secondNode.unlinkWith(fistNode);
//}

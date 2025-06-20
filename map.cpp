#include "map.h"
#include <QGraphicsRectItem>
#include <QGridLayout>
#include <QPushButton>
#include <QtMath>
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

QVector<Node*>* Map::PathSearch(Node* start, Node* end, Map::PathSearchType type, bool isWalking)
{
    if (path) {
        delete path;
        path = nullptr;
    }
    if (paths) {
        paths->clear();
        paths->shrink_to_fit();
        delete paths;
        paths = nullptr;
    }
    this->path  = new QVector<Node*>();
    this->paths = new QVector<QVector<Node*>*>();
    getPaths(start, nullptr, start, end);
    float minWeight = -1.0;
    float minCount  = 0;
    int   count     = 0;

    foreach (auto p, *paths) {
        float weight = 0.0;
        auto  size   = p->size();
        for (int n = 0; n < size - 1; n++) {
            if (type != TarfficLights) {
                auto edge = p->at(n)->getLinkedEdge(p->at(n + 1));
                weight += edge->getWeight(type, isWalking);
            } else {
                weight += (int)p->at(n)->hasTrafficLight;
            }
        }
        if (minWeight == -1.0)
            minWeight = weight;
        if (weight < minWeight) {
            minWeight = weight;
            minCount  = count;
        }
        count++;
    }
    if (paths->isEmpty()) {
        return nullptr;
    } else
        return paths->at(minCount);
}
bool Map::getPaths(Node* cNode, Node* pNode, Node* sNode, Node* eNode)
{
    if (cNode != nullptr && pNode != nullptr && cNode == pNode) {
        return false;
    }
    if (cNode != nullptr) {
        path->push_back(cNode);
        if (cNode == eNode) {
            auto newPath = new QVector<Node*>(*path);
            paths->push_back(newPath);
            return true;
        } else {
            auto related = cNode->getRelationNodes();
            foreach (auto node, related) {
                if (pNode != nullptr && (node == sNode || node == pNode || path->contains(node))) {
                    continue;
                }
                if (getPaths(node, cNode, sNode, eNode)) {
                    path->pop_back();
                }
            }
            path->pop_back();
            return false;
        }
    } else
        return false;
}
// center :中心点
// radius :搜索半径
// type :搜索的节点类型,在节点名称中查找，比如“酒店”类型 搜索所有名字中带有“酒店”的节点
// isNearest :是否最近，如果为true，则显示距离最近的节点
// isBestScore ：是否评分最好，如果为true，则显示评分最好的节点
// isNearest与isBestScore不能同时为true，但可以同时为false
QVector<Node*>* Map::roundSearch(Node* center, unsigned int radius, RoundSearchType type, bool isNearest, bool isBestScore)
{
    auto    cPos        = center->pos();
    auto    cX          = cPos.x();
    auto    cY          = cPos.y();
    auto    nodes       = Node::getNodes();
    auto    resultNodes = new QVector<Node*>();
    QString keyWord;
    switch (type) {
        case RoundSearchType::Hotel: {
            keyWord = QString::fromUtf8("酒店");
            break;
        }
        case RoundSearchType::Supermarket: {
            keyWord = QString::fromUtf8("超市");
            break;
        }
        case RoundSearchType::Bank: {
            keyWord = QString::fromUtf8("银行");
            break;
        }
        case RoundSearchType::Attraction: {
            keyWord = QString::fromUtf8("景点");
            break;
        }
        case RoundSearchType::Food: {
            keyWord = QString::fromUtf8("美食");
            break;
        }
        case RoundSearchType::Other: {
            keyWord = QString::fromUtf8("其他");
            break;
        }
    }
    foreach (auto node, nodes) {
        if (node->displayName.contains(keyWord)) {
            resultNodes->push_back(node);
        }
    }
    int    bestCount = 0;
    double distance  = -1.0;
    double bestScore = -1.0;
    auto   nearNodes = new QVector<Node*>();
    for (int n = 0; n < resultNodes->size(); n++) {
        auto node = resultNodes->at(n);
        auto x    = node->pos().x();
        auto y    = node->pos().y();
        auto dx   = cX - x;
        auto dy   = cY - y;
        auto dist = qSqrt(dx * dx + dy * dy);
        if (dist < radius / 2) {
            nearNodes->push_back(node);
        } else
            continue;
        if (isNearest) {
            if (distance < 0) {
                distance = dist;
            } else if (dist < distance) {
                distance  = dist;
                bestCount = nearNodes->size() - 1;
            }
        } else if (isBestScore) {
            auto score = node->score;
            if (bestScore < 0) {
                bestScore = score;
            } else if (score > bestScore) {
                bestScore = score;
                bestCount = nearNodes->size() - 1;
            }
        }
    }
    delete resultNodes;
    if (isNearest || isBestScore) {
        auto res = new QVector<Node*>();
        res->push_back(nearNodes->at(bestCount));
        delete nearNodes;
        return res;
    }
    return nearNodes;
}

unsigned int Map::getX() const
{
    return x;
}

unsigned int Map::getY() const
{
    return y;
}

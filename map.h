#ifndef MAP_H
#define MAP_H
#include <QVector>
#include "edge.h"
#include "graph.h"
#include "node.h"
class Map  //储存地图信息
{
public:
    Map(unsigned int x, unsigned int y);
    void resize();

private:
    unsigned int         x;      //地图x尺寸
    unsigned int         y;      //地图y尺寸
    static QVector<Node> nodes;  //储存所有节点
    static QVector<Edge> edges;  //储存所有边
    Graph                graph;  //储存节点与边的关系（无向图，领接表）
};

#endif  // MAP_H

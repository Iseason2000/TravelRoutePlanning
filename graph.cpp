#include "graph.h"

Graph::Graph(QVector<Node>* nodes, QVector<Edge>* edges)
{
    this->nodes                 = nodes;
    this->edges                 = edges;
    unsigned int          count = nodes->size();
    QVector<QVector<int>> temp  = QVector<QVector<int>>(count);
    for (unsigned int n = 0; n < count; n++) {
        temp[n] = QVector<int>(count, -1);
    }
}

void Graph::link(Node& fistNode, Node& secondNode, Edge& edge)
{
    int fistNum   = nodes->indexOf(fistNode);
    int secondNum = nodes->indexOf(secondNode);
    if (fistNum < 0 || secondNum < 0)
        throw "没有匹配的节点！";
    int edgeNum = edges->indexOf(edge);
    if (edgeNum < 0)
        throw "没有匹配的边！";
    graph[fistNum][secondNum] = edgeNum;
    graph[secondNum][fistNum] = edgeNum;
}

void Graph::unlink(Node& fistNode, Node& secondNode)
{
    int fistNum   = nodes->indexOf(fistNode);
    int secondNum = nodes->indexOf(secondNode);
    if (fistNum < 0 || secondNum < 0)
        throw "没有匹配的节点！";
    graph[fistNum][secondNum] = -1;
    graph[secondNum][fistNum] = -1;
}

#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"
#include "node.h"

using namespace std;
enum PathSearchType  //路径搜索的类型
{
    Distance,       //距离最短
    Time,           //时间最短
    Road,           //道路最少（转弯少）
    TarfficLights,  //红绿灯最少
    Cost,           //花费最少
    HighSpeed,      //高速公路优先
    NormalSpeed     //普通公路优先
};
enum RoundSearchType  //范围搜索的类型
{
    Hotel,        //酒店
    Supermarket,  //超市
    Bank,         //银行
    Attraction,   //景点
    Food,         //食物

};
class Graph  //图,储存顶点与边的连接关系
{
public:
    Graph(QVector<Node>* nodes, QVector<Edge>* edges);
    void link(Node& fistNode, Node& secondNode, Edge& edge);  //将2个点以边链接
    void unlink(Node& fistNode, Node& secondNode);            //取消2个点之间的链接

    QVector<Node> PathSearch(Node& start, Node& end, PathSearchType type);               //求两点最短路径，返回节点列表
    QVector<Node> roundSearch(Node& center, unsigned int radius, RoundSearchType type);  //范围搜索,默认按评价高低排序，返回节点列表

private:
    QVector<Node>*        nodes;  //储存所有节点
    QVector<Edge>*        edges;  //储存所有边
    QVector<QVector<int>> graph;  //邻接矩阵，<0表示不相连。权值为边数组索引
};

#endif  // GRAPH_H

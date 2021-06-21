#ifndef MAP_H
#define MAP_H
#include <QGraphicsView>
#include <QVector>
#include "edge.h"
#include "node.h"
class Map : public QObject  //储存地图信息
{
public:
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
        Other         //其他

    };
    Map(unsigned int x, unsigned int y, QGraphicsView* graphicsView);
    void           initMap();                                                                //初始化地图
    QVector<Node*> PathSearch(Node* start, Node* end, PathSearchType type, bool isWalking);  //求两点最短路径，返回节点列表
    QVector<Node*> roundSearch(Node* center, unsigned int radius, RoundSearchType type, bool isNearest,
                               bool isBestScore);  //范围搜索,默认按评价高低排序，返回节点列表

    unsigned int getX() const;
    unsigned int getY() const;

private:
    unsigned int   x;                       //地图x尺寸
    unsigned int   y;                       //地图y尺寸
    QGraphicsView* graphicsView = nullptr;  //储存地图显示对象
};

#endif  // MAP_H

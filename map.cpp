#include "map.h"
QVector<Node> Map::nodes = QVector<Node>();
QVector<Edge> Map::edges = QVector<Edge>();
Map::Map(unsigned int x, unsigned int y)
{
    this->x = x;
    this->y = y;
}

void Map::resize() {}

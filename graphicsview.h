#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QMouseEvent>
#include <QWheelEvent>
#include "node.h"
#include "ui_trpmainwindow.h"
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = nullptr);
    void  loadUI(Ui::TRPMainWindow* ui);
    void  showAttribute();                                     //根据当前对象读取节点属性
    void  newNode();                                           //新建节点
    void  deleteCurrentNode();                                 //删除当前节点
    bool  hasCurrentItem();                                    //是否有选中的节点
    void  link(Node* fistNode, Node* secondNode, Edge* edge);  //将2个点以边链接
    void  link(Node* fistNode, Node* secondNode);              //将2个点以默认边链接
    void  unlink(Node* fistNode, Node* secondNode);            //取消2个点之间的链接
    void  hightLightPath(bool isHightLight);                   //高亮当前搜索结果对应的路
    void  hightLightNode(bool isHightLight);                   //高亮当前搜索结果对应的节点
    Node* getCurrentNode() const;
    Node* mark1 = nullptr;
    Node* mark2 = nullptr;

public:
    bool            isLinking      = false;    //是否正在链接点
    bool            isMark1        = false;    //标记点1
    bool            isMark2        = false;    //标记点2
    QVector<Node*>* currentResult1 = nullptr;  //储存当前结果1
    QVector<Node*>* currentResult2 = nullptr;  //储存当前结果2

protected:
    void wheelEvent(QWheelEvent* event);
    //用于右键拖动视图
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private slots:

private:
    Node*              currentNode = nullptr;  //储存当前选中物品
    Ui::TRPMainWindow* ui;                     //储存主窗口，用于设定窗口组件
    bool               isMoving;               //视图移动状态
};

#endif  // GRAPHICSVIEW_H

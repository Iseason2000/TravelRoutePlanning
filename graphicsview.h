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
    void  unlink(Node* fistNode, Node* secondNode);            //取消2个点之间的链接
    Node* getCurrentNode() const;

public:
    bool isLinking = false;  //是否正在链接点
protected:
    void wheelEvent(QWheelEvent* event);
    //用于右键拖动视图
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Node*              currentNode = nullptr;  //储存当前选中物品
    Ui::TRPMainWindow* ui;                     //储存主窗口，用于设定窗口组件
    bool               isMoving;               //视图移动状态
};

#endif  // GRAPHICSVIEW_H

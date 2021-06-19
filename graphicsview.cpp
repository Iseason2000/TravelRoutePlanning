#include "graphicsview.h"
#include <QDebug>
GraphicsView::GraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    resize(800, 800);
}

void GraphicsView::loadUI(Ui::TRPMainWindow* ui)
{
    this->ui = ui;
}

void GraphicsView::showAttribute()
{
    ui->linkedcomboBox->clear();
    if (!currentNode || currentNode->isEmpty) {
        ui->nodeAblecheckBox->setChecked(false);
        ui->nameEdit->setText("");
        ui->typeBox->setCurrentIndex(0);
        ui->posLabel->setText("x,x");
        ui->scoreSpinBox->setValue(5.0);
        ui->trafficLigthcheck->setChecked(false);
        ui->linkedNode->setDisabled(true);
        return;
    }
    ui->linkedNode->setDisabled(false);
    ui->nodeAblecheckBox->setChecked(true);
    auto name    = currentNode->displayName;
    auto type    = currentNode->type;
    auto x       = currentNode->x;
    auto y       = currentNode->y;
    auto score   = currentNode->score;
    auto traffic = currentNode->hasTrafficLight;
    ui->nameEdit->setText(name);
    ui->typeBox->setCurrentIndex(type);
    ui->posLabel->setText(QString::number(x) + "," + QString::number(y));
    ui->scoreSpinBox->setValue(score);
    ui->trafficLigthcheck->setChecked(traffic);
    //节点显示模块
    if (currentNode->hasNeighbour()) {
        ui->EdgeInfo->setDisabled(false);
        auto neighbours = currentNode->neighbours;
        ui->linkedcomboBox->clear();
        for (auto id : neighbours) {
            ui->linkedcomboBox->addItem(Node::getByID(id)->displayName);
        }
    } else {
        ui->EdgeInfo->setDisabled(true);
        ui->nameEditEdge->setText("");
        ui->edgeLengthspinBox->setValue(0);
        ui->costSpinBox->setValue(0);
        ui->yonduSpinBox->setValue(0);
    }
}

void GraphicsView::newNode()
{
    currentNode->isEmpty = false;
    currentNode->setID();
    currentNode->displayName = QString::fromLocal8Bit("新节点") + QString::number(currentNode->getId());
    currentNode->type        = Node::NodeType::Mid;
}

void GraphicsView::deleteCurrentNode()
{
    currentNode->isEmpty     = true;
    currentNode->displayName = "";
}

bool GraphicsView::hasCurrentItem()
{
    return currentNode != nullptr;
}

void GraphicsView::link(Node& fistNode, Node& secondNode, Edge& edge)
{
    fistNode.linkWith(secondNode, edge);
    secondNode.linkWith(fistNode, edge);
    edge.line = new QGraphicsLineItem();
    edge.line->setPen(Edge::linePen);
    edge.line->setLine(QLineF(fistNode.pos() + QPointF(25, 25), secondNode.pos() + QPointF(25, 25)));
    scene()->addItem(edge.line);
    edge.line->stackBefore(&secondNode);
    edge.line->stackBefore(&fistNode);
    showAttribute();
}

void GraphicsView::unlink(Node* fistNode, Node* secondNode)
{
    auto edgeId = fistNode->neighbours[secondNode->getId()];
    auto edge   = Edge::getByID(edgeId);
    scene()->removeItem(edge->line);
    delete edge;
    fistNode->unlinkWith(*secondNode);
    secondNode->unlinkWith(*fistNode);
    showAttribute();
    viewport()->update();
}

void GraphicsView::wheelEvent(QWheelEvent* event)
{
    qreal s = event->angleDelta().y() < 0 ? 0.95 : 1.053;
    scale(s, s);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (isMoving) {
        centerOn(event->pos());
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        isMoving = true;
    }
    if (event->button() == Qt::LeftButton) {
        auto temp = currentNode;
        auto item = itemAt(event->pos());
        if (currentNode)
            currentNode->isSelected = false;
        if (!item)
            currentNode = nullptr;
        else
            currentNode = dynamic_cast<Node*>(item);  //失败时为null
        if (currentNode) {
            if (isLinking && temp && !currentNode->isEmpty) {
                Edge* edge = new Edge(QString::fromLocal8Bit("新道路"), Edge::EdgeType::Generalroads, 100, 10);
                link(*temp, *currentNode, *edge);
                currentNode = temp;
                isLinking   = false;
            }
            currentNode->isSelected = true;
        }
        showAttribute();
        viewport()->update();
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
        isMoving = false;
}

Node* GraphicsView::getCurrentNode() const
{
    return currentNode;
}

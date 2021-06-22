#include "graphicsview.h"
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
        foreach (auto id, neighbours.keys()) {
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
    auto newNode = new Node(QString::fromLocal8Bit("新节点"), Node::NodeType::Mid, currentNode->x, currentNode->y);
    newNode->displayName += QString::number(newNode->getId());
    auto pos = currentNode->pos();
    scene()->removeItem(currentNode);
    delete currentNode;
    currentNode = newNode;
    currentNode->setPos(pos);
    currentNode->isSelected = true;
    scene()->addItem(currentNode);
}
void GraphicsView::deleteCurrentNode()
{
    auto temp = new Node(currentNode->x, currentNode->y);
    auto pos  = currentNode->pos();
    scene()->removeItem(currentNode);
    delete currentNode;
    currentNode = temp;
    currentNode->setPos(pos);
    currentNode->isSelected = true;
    scene()->addItem(currentNode);
}

bool GraphicsView::hasCurrentItem()
{
    return currentNode != nullptr;
}

void GraphicsView::link(Node* fistNode, Node* secondNode, Edge* edge)
{
    if (!fistNode->linkWith(secondNode, edge))
        return;
    if (!secondNode->linkWith(fistNode, edge))
        return;
    edge->drawLine(QLineF(fistNode->pos() + QPointF(25, 25), secondNode->pos() + QPointF(25, 25)));
    scene()->addItem(edge);
    edge->stackBefore(secondNode);
    edge->stackBefore(fistNode);
    showAttribute();
}

void GraphicsView::link(Node* fistNode, Node* secondNode)
{
    Edge* edge = new Edge(QString::fromLocal8Bit("新道路"), Edge::EdgeType::Generalroads, 100, 10);
    link(fistNode, secondNode, edge);
}

void GraphicsView::unlink(Node* fistNode, Node* secondNode)
{
    if (!fistNode || !secondNode)
        return;
    if (!fistNode->neighbours.contains(secondNode->getId()))
        return;
    auto edgeId = fistNode->neighbours[secondNode->getId()];
    auto edge   = Edge::getByID(edgeId);
    scene()->removeItem(edge);
    delete edge;
    fistNode->unlinkWith(secondNode);
    secondNode->unlinkWith(fistNode);
    showAttribute();
    viewport()->update();
}

void GraphicsView::hightLightPath(bool isHightLight)
{
    if (!currentResult1 || currentResult1->isEmpty())
        return;
    auto size = currentResult1->size();
    if (size < 2)
        return;
    for (int n = 0; n < size - 1; n++) {
        auto node1       = currentResult1->at(n);
        auto node2       = currentResult1->at(n + 1);
        auto edge        = node1->getLinkedEdge(node2);
        edge->isLighting = isHightLight;
    }
    if (!isHightLight) {
        delete currentResult1;
    }
}

void GraphicsView::hightLightNode(bool isHightLight)
{
    if (!currentResult2 || currentResult2->isEmpty())
        return;
    foreach (auto node, *currentResult2) {
        node->isMarked = isHightLight;
    }
    if (!isHightLight) {
        delete currentResult2;
    }
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
            if (isMark1) {
                currentNode->isMarked = true;
                mark1                 = currentNode;
                isMark1               = false;
                ui->startNode->setText(mark1->displayName);
            }
            if (isMark2) {
                currentNode->isMarked = true;
                mark2                 = currentNode;
                isMark2               = false;
                ui->endNode->setText(mark2->displayName);
            }
            if (isLinking && temp && !currentNode->isEmpty) {
                link(temp, currentNode);
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

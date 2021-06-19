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
    if (!currentNode || currentNode->isEmpty) {
        ui->nodeAblecheckBox->setChecked(false);
        ui->nameEdit->setText("");
        ui->typeBox->setCurrentIndex(0);
        ui->posLabel->setText("x,x");
        ui->scoreSpinBox->setValue(5.0);
        ui->trafficLigthcheck->setChecked(false);
        ui->linkedcomboBox->clear();
        return;
    }
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
}

void GraphicsView::newNode()
{
    currentNode->isEmpty     = false;
    currentNode->displayName = QString::fromLocal8Bit("新节点");
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
        auto item = itemAt(event->pos());
        if (currentNode)
            currentNode->isSelected = false;
        if (!item)
            currentNode = nullptr;
        else
            currentNode = dynamic_cast<Node*>(item);  //失败时为null
        if (currentNode)
            currentNode->isSelected = true;
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

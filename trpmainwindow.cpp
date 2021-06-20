#include "trpmainwindow.h"
#include <QMessageBox>
#include <QtDebug>
#include "ui_trpmainwindow.h"
TRPMainWindow::TRPMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::TRPMainWindow)
{
    ui->setupUi(this);
    connect(ui->new_2, &QAction::triggered, this, &TRPMainWindow::newMap);
    view = new GraphicsView(this);
    view->loadUI(ui);
    view->move(20, 40);
}

TRPMainWindow::~TRPMainWindow()
{
    delete ui;
}

void TRPMainWindow::newMap()
{
    if (this->map) {
        delete this->map;
        this->map = nullptr;
    }
    this->map = new Map(20, 20, view);
    map->initMap();
}

void TRPMainWindow::on_nodeAblecheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked) {
        ui->nodeAttribute->setDisabled(true);
        ui->linkedNode->setDisabled(true);
        if (!view->hasCurrentItem())
            return;
        view->deleteCurrentNode();
        view->showAttribute();
        view->viewport()->update();

    } else if (!view->hasCurrentItem()) {
        QMessageBox::warning(NULL, "错误", "请先选择一个空节点!", QMessageBox::Yes, QMessageBox::Yes);
        ui->nodeAblecheckBox->setChecked(false);
    } else {
        ui->nodeAttribute->setDisabled(false);
        ui->linkedNode->setDisabled(false);
        if (view->getCurrentNode()->isEmpty)
            view->newNode();
        view->showAttribute();
        view->viewport()->update();
    }
}

void TRPMainWindow::on_nodeSaveButton_clicked()
{
    if (!view->hasCurrentItem())
        return;
    view->getCurrentNode()->displayName     = ui->nameEdit->text();
    view->getCurrentNode()->type            = Node::NodeType(ui->typeBox->currentIndex());
    view->getCurrentNode()->score           = ui->scoreSpinBox->value();
    view->getCurrentNode()->hasTrafficLight = ui->trafficLigthcheck->isChecked();
    view->showAttribute();
    view->viewport()->update();
}

void TRPMainWindow::on_addEdgeButton_clicked()
{
    view->isLinking = true;
}

void TRPMainWindow::on_removeEdgeButton_2_clicked()
{
    if (ui->linkedcomboBox->count() <= 0)
        return;
    auto secondNode = Node::getByName(ui->linkedcomboBox->currentText());
    if (!secondNode)
        return;
    view->unlink(view->getCurrentNode(), secondNode);
}

void TRPMainWindow::on_linkedcomboBox_currentIndexChanged(const QString& arg1)
{
    if (arg1.isEmpty())
        return;
    auto node   = Node::getByName(arg1);
    auto edgeId = view->getCurrentNode()->neighbours[node->getId()];
    auto edge   = Edge::getByID(edgeId);
    ui->nameEditEdge->setText(edge->displayName);
    ui->typeBoxEdge->setCurrentIndex(edge->type);
    ui->edgeLengthspinBox->setValue(edge->length);
    ui->costSpinBox->setValue(edge->cost);
    ui->yonduSpinBox->setValue(edge->Congestion);
}

void TRPMainWindow::on_edgeSaveButton_clicked()
{
    auto name = ui->linkedcomboBox->currentText();
    if (name.isEmpty())
        return;
    auto node         = Node::getByName(name);
    auto edgeId       = view->getCurrentNode()->neighbours[node->getId()];
    auto edge         = Edge::getByID(edgeId);
    edge->displayName = ui->nameEditEdge->text();
    edge->type        = Edge::EdgeType(ui->typeBoxEdge->currentIndex());
    edge->length      = ui->edgeLengthspinBox->value();
    edge->cost        = ui->costSpinBox->value();
    edge->Congestion  = ui->yonduSpinBox->value();
}

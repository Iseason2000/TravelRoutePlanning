#include "trpmainwindow.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QtDebug>
#include "ui_trpmainwindow.h"
TRPMainWindow::TRPMainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::TRPMainWindow)
{
    ui->setupUi(this);
    connect(ui->new_2, &QAction::triggered, this, &TRPMainWindow::newMap);
    connect(ui->save, &QAction::triggered, this, &TRPMainWindow::saveMap);
    connect(ui->open, &QAction::triggered, this, &TRPMainWindow::openMap);
    connect(ui->actionlogin, &QAction::triggered, this, &TRPMainWindow::login);
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
    bool    ok;
    QString size = QInputDialog::getText(this, QString::fromLocal8Bit("新建地图"), QString::fromLocal8Bit("请大图大小：x,y"), QLineEdit::Normal, 0, &ok);
    if (!ok || size.isEmpty()) {
        return;
    }
    auto sizes = size.split(",");
    if (this->view) {
        delete this->view;
        this->view = nullptr;
    }
    this->view = new GraphicsView(this);
    this->view->loadUI(ui);
    this->view->move(20, 40);
    this->view->setVisible(true);
    if (this->map) {
        delete this->map;
        this->map = nullptr;
    }
    this->map = new Map(sizes[0].toInt(), sizes[1].toInt(), view);
    map->initMap();
    Node::reSet();
    Edge::reSet();
    qDebug() << Node::getIdMap();
}

void TRPMainWindow::saveMap()
{
    QJsonObject mapJson;
    mapJson["mapSize"] = QString::number(map->getX()).append(",").append(QString::number(map->getY()));
    auto       nodes   = Node::getIdMap().values();
    QJsonArray nodesJson;
    foreach (auto node, nodes) {
        nodesJson.push_back(node->toJsonObject());
    }
    mapJson["nodes"] = nodesJson;
    auto       edges = Edge::getIdMap().values();
    QJsonArray edgesJson;
    foreach (auto edge, edges) {
        edgesJson.push_back(edge->toJsonObject());
    }
    mapJson["edges"] = edgesJson;
    QJsonDocument document;
    document.setObject(mapJson);
    auto    json    = document.toJson();
    QString strPath = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("选择保存路径"), "default", tr("Json File(*.json)"));
    if (strPath == "") {
        return;
    }
    QFile filename(strPath);
    if (!filename.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("保存文件失败"), QMessageBox::Ok);
        return;
    } else {
        filename.write(json);
    }
    QMessageBox::information(this, QString::fromLocal8Bit("保存文件"), QString::fromLocal8Bit("保存文件成功!"), QMessageBox::Ok);
    filename.close();
}

void TRPMainWindow::openMap()
{
    QString strPath = QFileDialog::getOpenFileName(NULL, QString::fromLocal8Bit("选择地图文件"), "", QObject::tr("Json File(*.json)"));
    if (strPath == "") {
        QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("选择文件失败，无路径"), "OK");
        return;  //用户点击的取消按钮
    }
    QFile      file(strPath);
    QByteArray json;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("打开文件失败!"), QMessageBox::Ok);
        return;
    } else {
        json = file.readAll();
    }
    if (json.isEmpty())
        return;

    auto jsondoc = QJsonDocument::fromJson(json);
    if (jsondoc.isNull() || jsondoc.isEmpty())
        return;
    auto jsonObject = jsondoc.object();
    file.close();
    auto size = jsonObject["mapSize"].toString().split(",");
    int  x    = size[0].toInt();
    int  y    = size[1].toInt();
    if (this->view) {
        delete this->view;
        this->view = nullptr;
    }
    this->view = new GraphicsView(this);
    this->view->loadUI(ui);
    this->view->move(20, 40);
    this->view->setVisible(true);
    if (this->map) {
        delete this->map;
        this->map = nullptr;
    }
    this->map             = new Map(x, y, this->view);
    QGraphicsScene* scene = new QGraphicsScene();
    view->setScene(scene);
    map->initMap();
    Node::reSet();
    Edge::reSet();
    auto edges = jsonObject["edges"].toArray();
    foreach (auto edge, edges) {
        auto n = Edge::fromJsonObject(edge.toObject());
        view->scene()->addItem(n);
    }
    auto nodes = jsonObject["nodes"].toArray();
    foreach (auto node, nodes) {
        auto   n = Node::fromJsonObject(node.toObject());
        QPoint pos(n->x * 50, n->y * 50);
        auto   item = view->itemAt(pos);
        if (!item) {
            view->scene()->removeItem(item);
            delete item;
        }
        n->setPos(pos);
        view->scene()->addItem(n);
    }
    view->scene()->update();
}

void TRPMainWindow::login()
{
    bool    ok;
    QString size = QInputDialog::getText(this, QString::fromLocal8Bit("登录到后台"), QString::fromLocal8Bit("请输入密码"), QLineEdit::Normal, 0, &ok);
    if (!ok || size.isEmpty()) {
        return;
    }
    if (size != "3119000108") {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("密码错误！"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    } else {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("成功"), QString::fromLocal8Bit("登录成功！"), QMessageBox::Yes, QMessageBox::Yes);
        isLogin = true;
        ui->nodeAblecheckBox->setDisabled(false);
    }
}

void TRPMainWindow::on_nodeAblecheckBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked) {
        ui->nodeAttribute->setDisabled(true);
        ui->linkedNode->setDisabled(true);
        if (!view->hasCurrentItem())
            return;
        if (view->getCurrentNode()->hasNeighbour()) {
            QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), ("请先删除该点链接的边!"), QMessageBox::Yes, QMessageBox::Yes);
            ui->nodeAblecheckBox->setChecked(true);
            return;
        }
        view->deleteCurrentNode();
        view->showAttribute();
        view->viewport()->update();

    } else if (!view->hasCurrentItem()) {
        ui->nodeAblecheckBox->setChecked(false);
        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先选择一个空节点!"), QMessageBox::Yes, QMessageBox::Yes);
    } else {
        ui->nodeAttribute->setDisabled(false);
        ui->linkedNode->setDisabled(false);
        if (view->getCurrentNode()->isEmpty) {
            view->newNode();
        }
        view->showAttribute();
        view->viewport()->update();
    }
}

void TRPMainWindow::on_nodeSaveButton_clicked()
{
    //    if (!isLogin) {
    //        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先登录到后台！"), QMessageBox::Yes, QMessageBox::Yes);
    //        return;
    //    }
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
    //    if (!isLogin) {
    //        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先登录到后台！"), QMessageBox::Yes, QMessageBox::Yes);
    //        return;
    //    }
    view->isLinking = true;
}

void TRPMainWindow::on_removeEdgeButton_2_clicked()
{
    //    if (!isLogin) {
    //        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先登录到后台！"), QMessageBox::Yes, QMessageBox::Yes);
    //        return;
    //    }
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
    ui->yonduSpinBox->setValue(edge->congestion);
}

void TRPMainWindow::on_edgeSaveButton_clicked()
{
    //    if (!isLogin) {
    //        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先登录到后台！"), QMessageBox::Yes, QMessageBox::Yes);
    //        return;
    //    }
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
    edge->congestion  = ui->yonduSpinBox->value();
    view->showAttribute();
    view->viewport()->update();
}

void TRPMainWindow::on_setStartButton_clicked()
{
    view->isMark1 = true;
    if (view->mark1) {
        view->mark1->isMarked = false;
    }
    view->mark1 = nullptr;
}

void TRPMainWindow::on_setEndButton_clicked()
{
    view->isMark2 = true;
    if (view->mark2) {
        view->mark2->isMarked = false;
    }
    view->mark2 = nullptr;
}

void TRPMainWindow::on_pushButton_clicked()
{
    auto pathSearchType = Map::PathSearchType(ui->searchTypecomboBox->currentIndex());
    auto moveType       = ui->walkTypecomboBox->currentIndex();
    if (!view->mark1 || !view->mark2)
        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先设置出点和入点！"), QMessageBox::Yes, QMessageBox::Yes);
    if (moveType == 0 && pathSearchType > 3) {
        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("步行方式不支持该查询范围！"), QMessageBox::Yes, QMessageBox::Yes);
    }
    auto result = map->PathSearch(view->mark1, view->mark2, pathSearchType, !moveType);
    view->hightLightPath(false);
    view->currentResult = &result;
    view->hightLightPath(true);
}

void TRPMainWindow::on_setStartButton_area_clicked()
{
    view->isMark1 = true;
    if (view->mark1) {
        view->mark1->isMarked = false;
    }
    view->mark1 = nullptr;
}

void TRPMainWindow::on_pushButton_2_clicked()
{
    if (!view->mark1)
        QMessageBox::warning(NULL, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请先设置搜索中点！"), QMessageBox::Yes, QMessageBox::Yes);
    auto radius = ui->areaBox->value();
    if (radius < 1)
        return;
    auto nodeType = Map::RoundSearchType(ui->typecomboBox_area->currentIndex());

    auto isBestScore = ui->radioButton_2->isChecked();
    auto isNearest   = ui->radioButton->isChecked();
    if (!ui->checkBox->isChecked()) {
        isBestScore = false;
        isNearest   = false;
    }
    auto result = map->roundSearch(view->mark1, radius, nodeType, isNearest, isBestScore);
    view->hightLightNode(false);
    view->currentResult = &result;
    view->hightLightNode(true);
}

void TRPMainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked) {
        ui->radioButton->setDisabled(true);
        ui->radioButton_2->setDisabled(true);
    } else {
        ui->radioButton->setDisabled(false);
        ui->radioButton_2->setDisabled(false);
    }
}

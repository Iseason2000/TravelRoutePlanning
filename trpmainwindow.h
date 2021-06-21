#ifndef TRPMAINWINDOW_H
#define TRPMAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"
#include "map.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class TRPMainWindow;
}
QT_END_NAMESPACE

class TRPMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    TRPMainWindow(QWidget* parent = nullptr);
    ~TRPMainWindow();

private:
    Ui::TRPMainWindow* ui;
    Map*               map     = nullptr;
    GraphicsView*      view    = nullptr;
    bool               isLogin = false;
private slots:
    void newMap();   //新地图
    void saveMap();  //保存地图
    void openMap();  //打开地图
    void login();    //登陆
    void on_nodeAblecheckBox_stateChanged(int arg1);
    void on_nodeSaveButton_clicked();
    void on_addEdgeButton_clicked();
    void on_removeEdgeButton_2_clicked();
    void on_linkedcomboBox_currentIndexChanged(const QString& arg1);
    void on_edgeSaveButton_clicked();
    void on_setStartButton_clicked();
    void on_setEndButton_clicked();
    void on_pushButton_clicked();
    void on_setStartButton_area_clicked();
    void on_pushButton_2_clicked();
    void on_checkBox_stateChanged(int arg1);
};
#endif  // TRPMAINWINDOW_H

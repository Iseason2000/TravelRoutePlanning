﻿#ifndef TRPMAINWINDOW_H
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
    Map*               map = nullptr;
    GraphicsView*      view;
private slots:
    void newMap();
    void on_nodeAblecheckBox_stateChanged(int arg1);
    void on_nodeSaveButton_clicked();
};
#endif  // TRPMAINWINDOW_H

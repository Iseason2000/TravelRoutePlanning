#include "trpmainwindow.h"
#include "ui_trpmainwindow.h"

TRPMainWindow::TRPMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TRPMainWindow)
{
    ui->setupUi(this);
}

TRPMainWindow::~TRPMainWindow()
{
    delete ui;
}


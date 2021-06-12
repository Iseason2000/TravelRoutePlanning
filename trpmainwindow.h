#ifndef TRPMAINWINDOW_H
#define TRPMAINWINDOW_H

#include <QMainWindow>

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
};
#endif  // TRPMAINWINDOW_H

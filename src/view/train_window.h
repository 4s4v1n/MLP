#ifndef TRAINWINDOW_H
#define TRAINWINDOW_H

#include <QDialog>
#include <QString>
#include <QPen>
#include <QCloseEvent>
#include "qcustomplot.h"

namespace Ui {
class TrainWindow;
}

class TrainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrainWindow(QWidget *parent = nullptr);
    ~TrainWindow();

public:
    auto AddData(const QString mode,const std::vector<double> values) -> void;

private:
    Ui::TrainWindow *ui;
};

#endif // TRAINWINDOW_H

#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>

namespace Ui {
class TestWindow;
}

class TestWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = nullptr);
    ~TestWindow();

public:
    auto SetAccuracy(const double accuracy) -> void;
    auto SetPrecison(const double precision) -> void;
    auto SetRecall(const double recall) -> void;
    auto SetMeasure(const double measure) -> void;
    auto SetTime(const double time) -> void;

private:
    Ui::TestWindow *ui;
};

#endif // TESTWINDOW_H

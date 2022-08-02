#include "test_window.h"

#include <QString>

#include "ui_test_window.h"

TestWindow::TestWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::TestWindow) {
  ui->setupUi(this);
}

TestWindow::~TestWindow() { delete ui; }

auto TestWindow::SetAccuracy(const double accuracy) -> void {
  ui->accuracyLabel->setText(QString::number(accuracy * 100, 'g', 2) + " %");
}

auto TestWindow::SetPrecison(const double precision) -> void {
  ui->precisionLabel->setText(QString::number(precision * 100, 'g', 2) + " %");
}

auto TestWindow::SetRecall(const double recall) -> void {
  ui->recallLabel->setText(QString::number(recall * 100, 'g', 2) + " %");
}

auto TestWindow::SetMeasure(const double measure) -> void {
  ui->measureLabel->setText(QString::number(measure * 100, 'g', 2) + " %");
}

auto TestWindow::SetTime(const double time) -> void {
  ui->timeLabel->setText(QString::number(time, 'g', 2) + " s");
}

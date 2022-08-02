#include "train_window.h"

#include <QColor>
#include <QVector>

#include "ui_train_window.h"

TrainWindow::TrainWindow(QWidget* parent)
    : QDialog(parent), ui(new Ui::TrainWindow) {
  ui->setupUi(this);
  ui->plot->setBackground(QColor(32, 32, 32));
  ui->plot->xAxis->setSubTicks(false);
  ui->plot->yAxis->setSubTicks(false);
  ui->plot->xAxis->setBasePen(QPen(Qt::white));
  ui->plot->xAxis->setTickPen(QPen(Qt::white));
  ui->plot->yAxis->setBasePen(QPen(Qt::white));
  ui->plot->yAxis->setTickPen(QPen(Qt::white));
  ui->plot->xAxis->setLabelColor(Qt::white);
  ui->plot->yAxis->setLabelColor(Qt::white);
  ui->plot->xAxis->setTickLabelColor(Qt::white);
  ui->plot->yAxis->setTickLabelColor(Qt::white);
  ui->plot->yAxis->setLabel("ERRORS%");
  ui->plot->yAxis->setRange(0, 100);
}

TrainWindow::~TrainWindow() { delete ui; }

auto TrainWindow::AddData(const QString mode, const std::vector<double> values)
    -> void {
  QCPBars* bars = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
  bars->setPen(QPen(Qt::white));
  bars->setBrush(QColor((48 + 65) / 2, (213 + 105) / 2, (200 + 225) / 2));
  QVector<double> barValues;
  QVector<double> barKeys;
  for (auto& value : values) {
    barValues.emplaceBack(value);
  }
  for (size_t key = 1; key <= values.size(); ++key) {
    barKeys.emplaceBack(static_cast<double>(key));
  }
  ui->plot->xAxis->setRange(0, static_cast<double>(values.size()) + 0.5);
  ui->plot->xAxis->setLabel(mode);
  bars->addData(barKeys, barValues);
  ui->plot->addGraph();
  ui->plot->replot();
}

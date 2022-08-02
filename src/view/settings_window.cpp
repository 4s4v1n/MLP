/**
 * @file settingswindow.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include "settings_window.h"

#include <QString>

#include "ui_settings_window.h"

SettingsWindow::SettingsWindow(QWidget* parent)
    : QDialog(parent), ui(new Ui::SettingsWindow) {
  ui->setupUi(this);
  for (int i = 2; i <= 5; ++i) {
    ui->layersBox->addItem(QString::number(i));
  }
  for (int i = 1; i <= 5; ++i) {
    ui->epochBox->addItem(QString::number(i));
  }
  ui->kGroupsSpinBox->setDisabled(true);
}

SettingsWindow::~SettingsWindow() { delete ui; }

auto SettingsWindow::GetLayersNumber() -> int {
  return ui->layersBox->currentText().toInt();
}

auto SettingsWindow::closeEvent(QCloseEvent* event) -> void {
  s21::NetworkType netType;
  if (ui->matrixTypeButton->isChecked()) {
    netType = s21::NetworkType::MATRIX;
  } else {
    netType = s21::NetworkType::GRAPH;
  }
  m_controller->SetNet(netType, ui->layersBox->currentText().toInt());
  event->accept();
}

auto SettingsWindow::SetController(s21::Controller* controller) -> void {
  m_controller = controller;
}

auto SettingsWindow::GetSelectionPart() -> double {
  return ui->selectonPartSpinBox->value();
}

void SettingsWindow::on_crossValidationCheckBox_stateChanged(int arg1) {
  if (arg1) {
    ui->kGroupsSpinBox->setEnabled(true);
  } else {
    ui->kGroupsSpinBox->setValue(1);
    ui->kGroupsSpinBox->setDisabled(true);
  }
}

auto SettingsWindow::GetEpochNumber() -> size_t {
  return ui->epochBox->currentText().toUInt();
}

auto SettingsWindow::IsCrossValidation() -> bool {
  return ui->crossValidationCheckBox->isChecked();
}

auto SettingsWindow::GetKGroups() -> size_t {
  return static_cast<size_t>(ui->kGroupsSpinBox->value());
}

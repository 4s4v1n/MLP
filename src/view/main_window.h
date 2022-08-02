/**
 * @file mainwindow.h
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <chrono>
#include <thread>

#include "../model/emnist_reader.h"
#include "settings_window.h"
#include "test_window.h"
#include "train_window.h"

using namespace std::chrono_literals;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  /**
   * @page MainWindow
   * @brief Defines main apllication window
   */

  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public:
  auto Predict() -> void;
  auto ChangeGUIAccept(bool accept) -> void;

 private slots:
  void on_trainButton_clicked();
  void on_settingsButton_clicked();
  void on_saveWeightsButton_clicked();
  void on_loadWeigthsButton_clicked();
  void on_saveImageButton_clicked();
  void on_loadImageButton_clicked();
  void on_testButton_clicked();
  void showTestWindow();
  void showTrainWindow(const std::vector<double>& values);

 signals:
    void testDone();
    void trainDone(const std::vector<double>& values);

 private:
  Ui::MainWindow* ui{};
  QImage image{};
  QString filePath{};
  QString fileName{};
  s21::Controller* m_controller{};
  s21::Metrics m_metrics{};
  SettingsWindow* settingsWindow{};
  TestWindow* testWindow{};
  TrainWindow* trainWindow{};
  std::thread m_thread{};

 private:
  static constexpr int asciiBias = 65;
  static constexpr int defaultLayers = 2;
  const QString weightsPath = "../src/weights/";
  const QString bmpPath = "../src/bmp/";
  const QString emnistPath = "../src/emnist/";
};
#endif  // MAINWINDOW_H

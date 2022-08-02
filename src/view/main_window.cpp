/**
 * @file mainwindow.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include "main_window.h"

#include <QString>

#include "ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      settingsWindow(new SettingsWindow),
      testWindow(new TestWindow),
      trainWindow(new TrainWindow),
      m_controller(new s21::Controller) {
  ui->setupUi(this);
  ui->renderingScene->SetMainWindow(this);
  m_controller->SetNet(s21::NetworkType::MATRIX, defaultLayers);
  settingsWindow->SetController(m_controller);
  connect(this, &MainWindow::testDone, this, &MainWindow::showTestWindow);
  connect(this, &MainWindow::trainDone, this, &MainWindow::showTrainWindow);
}

MainWindow::~MainWindow() {
  delete settingsWindow;
  delete testWindow;
  delete ui;
}

void MainWindow::on_trainButton_clicked() {
  filePath =
      QFileDialog::getOpenFileName(this, QFileDialog::tr("Open file"),
                                   emnistPath, QFileDialog::tr("(*.csv)"));
  if (!filePath.isEmpty()) {
    ChangeGUIAccept(false);
    m_thread = std::thread([&]() {
      if (settingsWindow->IsCrossValidation()) {
        if (settingsWindow->GetKGroups() != 1) {
          emit trainDone(m_controller->CrossValidation(
              filePath.toStdString(), settingsWindow->GetKGroups()));
        }
      } else {
        emit trainDone(m_controller->Train(filePath.toStdString(),
                                           settingsWindow->GetEpochNumber()));
      }
      ChangeGUIAccept(true);
    });
    m_thread.detach();
  }
}

void MainWindow::on_settingsButton_clicked() { settingsWindow->show(); }

void MainWindow::on_saveWeightsButton_clicked() {
  filePath = QFileDialog::getExistingDirectory(
                 this, "Open directory to save file", weightsPath,
                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) +
             "/";
  if (!filePath.isEmpty()) {
    fileName = filePath + "weights_" +
               QString::number((settingsWindow->GetLayersNumber()));
    m_controller->SaveWeights(fileName.toStdString() + ".txt");
  }
}

void MainWindow::on_loadWeigthsButton_clicked() {
  filePath =
      QFileDialog::getOpenFileName(this, QFileDialog::tr("Open file"),
                                   weightsPath, QFileDialog::tr("(*.txt)"));
  if (!filePath.isEmpty()) {
    m_controller->LoadWeights(filePath.toStdString());
  }
}

void MainWindow::on_saveImageButton_clicked() {
  if (!ui->renderingScene->IsClear()) {
    QString currentTime =
        QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
    filePath =
        QFileDialog::getExistingDirectory(
            this, "Open directory to save file", bmpPath,
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) +
        "/";
    if (!filePath.isEmpty()) {
      fileName = filePath + ui->answerLabel->text() + "_" + currentTime;
      ui->renderingScene->GetImage().save(fileName + ".bmp", "BMP");
    }
  }
}

void MainWindow::on_loadImageButton_clicked() {
  filePath = QFileDialog::getOpenFileName(this, QFileDialog::tr("Open file"),
                                          bmpPath, QFileDialog::tr("(*.bmp)"));
  if (!filePath.isEmpty()) {
    ui->renderingScene->SetImage(filePath);
    Predict();
  }
}

auto MainWindow::Predict() -> void {
  if (!ui->renderingScene->IsClear()) {
    m_controller->FeedInitValues(ui->renderingScene->VectorFromImage());
    m_controller->FeedForward();
    ui->answerLabel->setText(QString(
        QChar(static_cast<int>(m_controller->GetResult()) + asciiBias)));
  } else {
    ui->answerLabel->setText("");
  }
}

void MainWindow::ChangeGUIAccept(bool accept) {
  if (accept) {
    this->setEnabled(true);
  } else {
    this->setDisabled(true);
  }
}

void MainWindow::on_testButton_clicked() {
  if (settingsWindow->GetSelectionPart() != 0.) {
    filePath =
        QFileDialog::getOpenFileName(this, QFileDialog::tr("Open file"),
                                     emnistPath, QFileDialog::tr("(*.csv)"));
    ChangeGUIAccept(false);
    if (!filePath.isEmpty()) {
      m_thread = std::thread([&]() {
        m_metrics = m_controller->GetMetrics(
            filePath.toStdString(), settingsWindow->GetSelectionPart());
        testWindow->SetAccuracy(m_metrics.accuracy);
        testWindow->SetPrecison(m_metrics.precision);
        testWindow->SetRecall(m_metrics.recall);
        testWindow->SetMeasure(m_metrics.f_measure);
        testWindow->SetTime(m_metrics.ed_time);
        ChangeGUIAccept(true);
        emit testDone();
      });
      m_thread.detach();
    }
  }
}

void MainWindow::showTestWindow() { testWindow->show(); }

void MainWindow::showTrainWindow(const std::vector<double>& values) {
  QString labelName =
      settingsWindow->IsCrossValidation() ? "K-GROUPS" : "EPOCH";
  trainWindow->AddData(labelName, values);
  trainWindow->show();
}

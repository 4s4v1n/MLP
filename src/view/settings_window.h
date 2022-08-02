/**
 * @file settingswindow.h
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QCloseEvent>
#include "../controller/controller.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    /**
     * @page SettingsWindow
     * @brief Defines settings page for application
     */

    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

public:
    auto GetLayersNumber() -> int;
    auto SetController(s21::Controller* controller) -> void;
    auto GetSelectionPart() -> double;
    auto GetEpochNumber() -> size_t;
    auto IsCrossValidation() -> bool;
    auto GetKGroups() -> size_t;


private slots:
    void on_crossValidationCheckBox_stateChanged(int arg1);

private:
    auto closeEvent(QCloseEvent* event) -> void override;

private:
    Ui::SettingsWindow * ui           {};
    s21::Controller*     m_controller {};
};

#endif // SETTINGSWINDOW_H

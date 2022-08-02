/**
 * @file render_scene.h
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */


#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QColor>
#include <QImage>
#include <vector>
#include "main_window.h"

class RenderScene: public QWidget
{

  /**
   * @class RenderScene
   * @brief Defines rendering scene by mouse
   */

  Q_OBJECT

public:
    explicit RenderScene(QWidget* parent = nullptr);
    ~RenderScene() override = default;

public:
    auto VectorFromImage() -> const std::vector<double>&;
    auto GetImage() const -> const QImage&;
    auto SetImage(const QString& filePath) -> void;
    auto IsClear() const -> bool;
    auto SetMainWindow(MainWindow* window) -> void;

protected:
    auto mousePressEvent(QMouseEvent *event) -> void override;
    auto mouseMoveEvent(QMouseEvent *event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;
    auto paintEvent(QPaintEvent* event) -> void override;

private:
    QPoint m_lastPoint                {};
    QImage m_image                    {};
    QImage m_resized_image            {};
    bool   m_drag                     {};
    std::vector<double> m_pixelVector {};
    MainWindow*          m_mainWindow {};

private:
    static constexpr double lineWidth = 40;
    static constexpr int    pixelSize = 28;

private:
    auto clearImage() -> void;
    auto lineTo(const QPoint& newPoint) -> void;

};

#endif // RENDER_SCENE_H

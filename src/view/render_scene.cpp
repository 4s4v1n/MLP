/**
 * @file render_scene.cpp
 * @authors Anton S. <telvina(at)student.21-school.ru>
 * @authors Andrey D. <sreana(at)student.21-school.ru>
 */

#include "render_scene.h"

#include <iostream>

RenderScene::RenderScene(QWidget* parent)
    : QWidget(parent),
      m_image(QSize(400, 400), QImage::Format_RGB16),
      m_drag(false) {
  clearImage();
}

auto RenderScene::mousePressEvent(QMouseEvent* event) -> void {
  if (event->button() == Qt::LeftButton) {
    m_lastPoint = event->pos();
    m_drag = true;
  } else {
    clearImage();
  }
}

auto RenderScene::mouseMoveEvent(QMouseEvent* event) -> void {
  if ((event->buttons() & Qt::LeftButton) && m_drag) {
    lineTo(event->pos());
  }
}

auto RenderScene::mouseReleaseEvent(QMouseEvent* event) -> void {
  if (event->button() == Qt::LeftButton && m_drag) {
    lineTo(event->pos());
    m_drag = false;
  }
  m_mainWindow->Predict();
}

auto RenderScene::paintEvent(QPaintEvent* event) -> void {
  QPainter painter(this);
  QRect rect(event->rect());
  painter.drawImage(rect, m_image, rect);
}

auto RenderScene::lineTo(const QPoint& newPoint) -> void {
  QPainter painter(&m_image);
  painter.setPen(
      QPen(Qt::black, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.drawLine(m_lastPoint, newPoint);

  int rad = (lineWidth / 2) + 2;
  update(QRect(m_lastPoint, newPoint)
             .normalized()
             .adjusted(-rad, -rad, +rad, +rad));
  m_lastPoint = newPoint;
}

auto RenderScene::clearImage() -> void {
  m_image.fill(qRgb(0xFF, 0xFF, 0xFF));
  update();
}

auto RenderScene::VectorFromImage() -> const std::vector<double>& {
  if (!m_pixelVector.empty()) {
    m_pixelVector.clear();
  }
  m_resized_image = m_image.scaled(pixelSize, pixelSize);
  for (int i = 0; i < pixelSize; ++i) {
    for (int j = 0; j < pixelSize; ++j) {
      m_pixelVector.emplace_back(
          static_cast<double>(m_resized_image.pixelColor(i, j).blackF()));
    }
  }
  return m_pixelVector;
}

auto RenderScene::GetImage() const -> const QImage& { return m_image; }

auto RenderScene::SetImage(const QString& filePath) -> void {
  clearImage();
  m_image.load(filePath);
  update();
}

auto RenderScene::IsClear() const -> bool {
  for (int i = 0; i < m_image.width(); ++i) {
    for (int j = 0; j < m_image.height(); ++j) {
      if (m_image.pixelColor(i, j).blackF() != 0.f) {
        return false;
      }
    }
  }
  return true;
}

auto RenderScene::SetMainWindow(MainWindow* window) -> void {
  m_mainWindow = window;
}

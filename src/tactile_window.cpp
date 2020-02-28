#include "tactile_window.h"

#include <QOpenGLFunctions>
#include <QPainter>
#include <iostream>

#include "ui_window.h"

namespace tactile {

TactileWindow::TactileWindow(QWidget* parent)
    : QMainWindow{parent}, m_ui{new Ui::MainWindow}
{
  m_ui->setupUi(this);
  m_renderSurface = m_ui->renderSurface;

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  m_renderSurface->setFormat(format);
}

TactileWindow::~TactileWindow()
{
  delete m_ui;
}

void TactileWindow::render() noexcept
{
  QOpenGLFunctions* f = m_renderSurface->context()->functions();
  f->glClearColor(0xFF, 0, 0, 0xFF);
  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QPainter painter{m_renderSurface};
  painter.fillRect(0, 0, width(), height(), Qt::black);

  painter.drawText(get_render_surface_bounds(),
                   Qt::AlignCenter,
                   "Hello Qt!");
}

QRect TactileWindow::get_render_surface_bounds() const noexcept
{
  return {m_renderSurface->x(),
          m_renderSurface->y(),
          m_renderSurface->width(),
          m_renderSurface->height()};
}

void TactileWindow::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  render();
}

}  // namespace tactile

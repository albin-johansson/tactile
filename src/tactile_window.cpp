#include "tactile_window.h"

#include <QtGui>

namespace tactile {

TactileWindow::TactileWindow() = default;

void TactileWindow::initializeGL()
{
  setWidth(800);
  setHeight(600);

  const auto screenSize = screen()->size();
  setPosition((screenSize.width() - width()) / 2,
              (screenSize.height() - height()) / 2);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  setFormat(format);
}

void TactileWindow::resizeGL(int w, int h)
{
  // TODO...
}

void TactileWindow::paintGL()
{
  QOpenGLFunctions* f = context()->functions();
  f->glClearColor(0xFF, 0, 0, 0xFF);
  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QPainter painter{this};
  painter.fillRect(0, 0, width(), height(), Qt::white);
  painter.drawText(get_bounds(), Qt::AlignCenter, "Hello Qt!");
}

QRect TactileWindow::get_bounds() const noexcept
{
  return {0, 0, width(), height()};
}

}  // namespace tactile

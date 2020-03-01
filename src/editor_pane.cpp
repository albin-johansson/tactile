#include "editor_pane.h"

#include <QOpenGLFunctions>
#include <QPainter>
#include <QSurfaceFormat>
#include <iostream>

namespace tactile {

EditorPane::EditorPane(QWidget* parent) noexcept : QOpenGLWidget{parent}
{}

void EditorPane::initializeGL()
{
  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  setFormat(format);
}

void EditorPane::resizeGL(int w, int h)
{
  // ...
}

void EditorPane::paintGL()
{
  QOpenGLFunctions* f = context()->functions();
  f->glClearColor(0xFF, 0, 0, 0xFF);
  f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QPainter painter{this};
  painter.drawText(get_bounds(), Qt::AlignCenter, "Hello Qt!");
}

QRect EditorPane::get_bounds() const noexcept
{
  return {x(), y(), width(), height()};
}

}  // namespace tactile

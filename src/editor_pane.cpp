#include "editor_pane.h"

#include <QOpenGLFunctions>
#include <QPainter>
#include <QSurfaceFormat>

namespace tactile {

EditorPane::EditorPane(QWidget* parent) noexcept : QWidget{parent}
{
  QSizePolicy policy;
  policy.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
  policy.setVerticalPolicy(QSizePolicy::Policy::Expanding);
  setSizePolicy(policy);
}

void EditorPane::paintEvent(QPaintEvent* event)
{
  QWidget::paintEvent(event);
  paint(event);
}

//void EditorPane::resizeEvent(QResizeEvent* event)
//{
//  QWidget::resizeEvent(event);
//}

void EditorPane::paint(QPaintEvent* event) noexcept
{
  if (!event) {
    return;
  }

  QPainter painter{this};
  const auto bounds = QRect{0, 0, width(), height()};
  painter.fillRect(bounds, Qt::black);

}

}  // namespace tactile

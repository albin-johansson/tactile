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
  emit received_paint_event();
}

}  // namespace tactile

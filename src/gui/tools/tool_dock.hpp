#pragma once

#include <QDockWidget>

#include "tool_widget.hpp"

namespace tactile::gui {

class tool_dock final : public QDockWidget
{
  Q_OBJECT

 public:
  explicit tool_dock(QWidget* parent = nullptr);

  [[nodiscard]] auto get_tool_widget() noexcept -> tool_widget*
  {
    return m_widget;
  }

 signals:
  void stamp_enabled();

  void bucket_enabled();

  void eraser_enabled();

 private:
  tool_widget* m_widget{};
};

}  // namespace tactile::gui

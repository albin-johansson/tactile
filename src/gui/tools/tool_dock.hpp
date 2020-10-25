#pragma once

#include "dock_widget.hpp"
#include "tool_widget.hpp"

namespace tactile::gui {

class tool_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit tool_dock(QWidget* parent = nullptr);

  void enable_tools()
  {
    m_widget->enable_tools();
  }

  void disable_tools()
  {
    m_widget->disable_tools();
  }

  void stamp_enabled()
  {
    m_widget->handle_enable_stamp();
  }

  void eraser_enabled()
  {
    m_widget->handle_enable_eraser();
  }

  void bucket_enabled()
  {
    m_widget->handle_enable_bucket();
  }

 signals:
  void enable_stamp();
  void enable_bucket();
  void enable_eraser();

 private:
  tool_widget* m_widget{};
};

}  // namespace tactile::gui

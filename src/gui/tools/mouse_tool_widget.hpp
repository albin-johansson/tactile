#pragma once

#include <QDockWidget>
#include <memory>

#include "tactile_types.hpp"

class QButtonGroup;

namespace Ui {

class ToolButtonBarUI;

}

namespace tactile::ui {

class mouse_tool_widget final : public QWidget {
  Q_OBJECT

 public:
  explicit mouse_tool_widget(QWidget* parent = nullptr);

  ~mouse_tool_widget() noexcept override;

  void enable_tools() noexcept;

  void disable_tools() noexcept;

 signals:
  void request_enable_stamp();

  void request_enable_bucket();

  void request_enable_eraser();

  void request_enable_rectangle();

  void request_enable_find_same();

 public slots:
  void handle_enable_stamp() noexcept;

  void handle_enable_bucket() noexcept;

  void handle_enable_eraser() noexcept;

  void handle_enable_rectangle() noexcept;

  void handle_enable_find_same() noexcept;

 private:
  owner<Ui::ToolButtonBarUI*> m_ui{};
  QButtonGroup* m_group{};

  void set_tools_disabled(bool disabled) noexcept;
};

}  // namespace tactile::gui

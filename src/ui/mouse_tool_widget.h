#pragma once
#include <QDockWidget>

#include "smart_pointers.h"

class QButtonGroup;

namespace Ui {

class ToolButtonBarUI;

}

namespace tactile {

class MouseToolWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit MouseToolWidget(QWidget* parent = nullptr);

  ~MouseToolWidget() noexcept override;

  void enable_tools() noexcept;

  void disable_tools() noexcept;

 signals:
  void s_stamp_enabled();

  void s_bucket_enabled();

  void s_eraser_enabled();

  void s_rectangle_enabled();

  void s_find_same_enabled();

 public slots:
  void enable_stamp() noexcept;

  void enable_bucket() noexcept;

  void enable_eraser() noexcept;

  void enable_rectangle() noexcept;

  void enable_find_same() noexcept;

 private:
  Ui::ToolButtonBarUI* m_ui;
  Unique<QButtonGroup> m_group;

  void set_tools_disabled(bool disabled) noexcept;
};

}  // namespace tactile

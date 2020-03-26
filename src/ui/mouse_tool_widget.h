#pragma once
#include <QDockWidget>

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
  void stamp_enabled();

  void bucket_enabled();

  void eraser_enabled();

  void rectangle_enabled();

  void find_same_enabled();

 public slots:
  void enable_stamp() noexcept;

  void enable_bucket() noexcept;

  void enable_eraser() noexcept;

  void enable_rectangle() noexcept;

  void enable_find_same() noexcept;

 private:
  Ui::ToolButtonBarUI* m_ui;

  void set_tools_disabled(bool disabled) noexcept;
};

}  // namespace tactile

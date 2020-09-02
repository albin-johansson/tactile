#pragma once

#include <qdockwidget.h>

#include <memory>

#include "tactile_types.hpp"

class QButtonGroup;

namespace Ui {

class tool_widget;

}

namespace tactile::gui {

class tool_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit tool_widget(QWidget* parent = nullptr);

  ~tool_widget() noexcept override;

  void enable_tools();

  void disable_tools();

 signals:
  void stamp_enabled();

  void bucket_enabled();

  void eraser_enabled();

  void rectangle_enabled();

  void find_same_enabled();

 public slots:
  void handle_enable_stamp();

  void handle_enable_bucket();

  void handle_enable_eraser();

  void handle_enable_rectangle();

  void handle_enable_find_same();

 private:
  owner<Ui::tool_widget*> m_ui{};
  QButtonGroup* m_group{};

  void set_tools_disabled(bool disabled);
};

}  // namespace tactile::gui

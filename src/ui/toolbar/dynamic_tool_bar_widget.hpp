#pragma once

#include <QWidget>
#include <memory>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

class QLayout;

namespace tactile::ui {

class dynamic_tool_bar_widget final : public QWidget {
  Q_OBJECT

 public:
  explicit dynamic_tool_bar_widget(QWidget* parent = nullptr);

 signals:
  void s_stamp_tool();

  void s_bucket_tool();

  void s_eraser_tool();

 private:
  std::unique_ptr<QLayout> m_layout;
  stamp_toolbar* m_stampToolbar;
};

}  // namespace tactile::ui

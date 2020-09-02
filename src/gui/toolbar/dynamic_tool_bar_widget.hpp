#pragma once

#include <qwidget.h>

#include "tactile_fwd.hpp"
#include "tactile_types.hpp"

class QLayout;

namespace tactile::gui {

class dynamic_tool_bar_widget final : public QWidget
{
  Q_OBJECT

 public:
  explicit dynamic_tool_bar_widget(QWidget* parent = nullptr);

 signals:
  void request_stamp_tool();

  void request_bucket_tool();

  void request_eraser_tool();

 private:
  QLayout* m_layout{};
  stamp_toolbar* m_stampToolbar{};
};

}  // namespace tactile::gui

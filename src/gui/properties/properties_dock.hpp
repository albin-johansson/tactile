#pragma once

#include "dock_widget.hpp"
#include "map_document.hpp"

namespace tactile::gui {

class properties_widget;

class properties_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit properties_dock(QWidget* parent = nullptr);

 public slots:
  void selected_map(const core::map_document& document);

 private:
  properties_widget* m_widget{};
};

}  // namespace tactile::gui

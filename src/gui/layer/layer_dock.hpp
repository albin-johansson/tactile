#pragma once

#include "dock_widget.hpp"
#include "forward_declare.hpp"
#include "map_document.hpp"
#include "not_null.hpp"

TACTILE_FORWARD_DECLARE(tactile::gui, layer_widget)

namespace tactile::gui {

class layer_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit layer_dock(QWidget* parent = nullptr);

 public slots:
  void switched_map(not_null<core::map_document*> document);

 private:
  layer_widget* m_widget{};
};

}  // namespace tactile::gui

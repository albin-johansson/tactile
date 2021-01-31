#pragma once

#include "dock_widget.hpp"
#include "layer.hpp"
#include "layer_model.hpp"
#include "map_document.hpp"
#include "not_null.hpp"
#include "smart_pointers.hpp"

namespace tactile::gui {

class layer_widget;

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

#pragma once

#include "dock_widget.hpp"
#include "layer.hpp"
#include "layer_model.hpp"
#include "map_document.hpp"
#include "smart_pointers.hpp"

namespace tactile::gui {

class layer_widget;

class layer_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit layer_dock(QWidget* parent = nullptr);

 public slots:
  void selected_map(const core::map_document& document,
                    const shared<vm::layer_model>& model);

 private:
  layer_widget* m_widget{};
};

}  // namespace tactile::gui

#pragma once

#include "dock_widget.hpp"
#include "map_document.hpp"
#include "property.hpp"
#include "smart_pointers.hpp"

namespace tactile::gui {

class properties_widget;

class properties_dock final : public dock_widget
{
  Q_OBJECT

 public:
  explicit properties_dock(QWidget* parent = nullptr);

 public slots:
  void selected_map(const core::map_document& document,
                    const shared<vm::property_model>& propertyModel);

 private:
  properties_widget* m_widget{};
};

}  // namespace tactile::gui

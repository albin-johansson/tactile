#include "layer_widget_context_menu.hpp"

#include "icons.hpp"

namespace tactile::gui {

layer_widget_context_menu::layer_widget_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_add{addMenu(icons::add(), tr("Add new layer..."))}
{
  m_tileLayer = m_add->addAction(icons::tile_layer(), tr("Tile layer"));
  m_objectLayer = m_add->addAction(icons::object_layer(), tr("Object layer"));

  // clang-format off
  connect(m_tileLayer, &QAction::triggered,
          this, &layer_widget_context_menu::add_tile_layer);

  connect(m_objectLayer, &QAction::triggered,
          this, &layer_widget_context_menu::add_object_layer);
  // clang-format on
}

}  // namespace tactile::gui

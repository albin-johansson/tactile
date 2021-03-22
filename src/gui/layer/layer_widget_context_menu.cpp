#include "layer_widget_context_menu.hpp"

#include "icons.hpp"

namespace tactile::gui {

add_layer_context_menu::add_layer_context_menu(QWidget* parent)
    : QMenu{parent}
    , m_tileLayer{addAction(icons::tile_layer(), tr("Tile layer"))}
    , m_objectLayer{addAction(icons::object_layer(), tr("Object layer"))}
{
  // clang-format off
  connect(m_tileLayer, &QAction::triggered,
          this, &add_layer_context_menu::add_tile_layer);
  connect(m_objectLayer, &QAction::triggered,
          this, &add_layer_context_menu::add_object_layer);
  // clang-format on
}

layer_widget_context_menu::layer_widget_context_menu(
    add_layer_context_menu* addLayerMenu,
    QWidget* parent)
    : QMenu{parent}
    , m_add{addMenu(icons::add(), tr("Add new layer..."))}
{
  for (auto* action : addLayerMenu->actions())
  {
    m_add->addAction(action);
  }
}

}  // namespace tactile::gui

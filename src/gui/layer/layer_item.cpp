#include "layer_item.hpp"

#include "icons.hpp"

namespace tactile::gui {

layer_item::layer_item(const QString& name,
                       const layer_id id,
                       QListWidget* parent)
    : QListWidgetItem{name, parent}
    , m_layer{id}
{
  setIcon(icons::tile_layer());
  setFlags(flags() | Qt::ItemIsEditable);
}

}  // namespace tactile::gui

#include "layer_item.hpp"

namespace tactile::gui {

layer_item::layer_item(const QString& name, layer_id id, QListWidget* parent)
    : QListWidgetItem{name, parent},
      m_layer{id}
{
  setIcon(QIcon{QStringLiteral(u":resources/icons/icons8/color/64/layer.png")});
}



}  // namespace tactile::gui

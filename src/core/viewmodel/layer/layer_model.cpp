#include "layer_model.hpp"

#include "layer_item.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {

layer_model::layer_model(core::map_document* document)
    : QStandardItemModel{}
    , m_document{document}
{
  if (!m_document) {
    throw tactile_error{"layer_model requires non-null map document!"};
  }

  document->each_layer(
      [this](const layer_id id, const core::shared_layer& layer) {
        Q_ASSERT(layer);
        this->appendRow(layer_item::make(id, *layer.get()));
      });

  // clang-format off
  connect(m_document, &core::map_document::added_layer,
          this, &layer_model::add_item);

  connect(m_document, &core::map_document::removed_layer,
          this, &layer_model::remove_item);
  // clang-format on
}

void layer_model::add_tile_layer()
{
  m_document->add_tile_layer();
}

void layer_model::add_object_layer()
{
  m_document->add_object_layer();
}

void layer_model::add_item(const layer_id id, const core::layer& layer)
{
  appendRow(layer_item::make(id, layer));
}

void layer_model::remove_item(const layer_id id)
{
  const auto* root = invisibleRootItem();
  const auto nRows = rowCount();
  for (auto row = 0; row < nRows; ++row) {
    if (const auto* item = dynamic_cast<const layer_item*>(root->child(row))) {
      if (item->get_id() == id) {
        removeRow(item->row());
      }
    }
  }
}

}  // namespace tactile::vm

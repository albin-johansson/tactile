#include "layer_model.hpp"

#include "icons.hpp"
#include "layer_item_role.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {
namespace {

class layer_item final : public QStandardItem
{
 public:
  explicit layer_item(const QString& name) : QStandardItem{name}
  {}

  void set_id(const layer_id id)
  {
    setData(id.get(), static_cast<int>(layer_item_role::id));
  }

  void set_type(const core::layer_type type)
  {
    setData(static_cast<int>(type), static_cast<int>(layer_item_role::type));
  }

  [[nodiscard]] auto get_id() const -> layer_id
  {
    const auto role = static_cast<int>(layer_item_role::id);
    const auto id = data(role).value<int>();
    return layer_id{id};
  }

  [[nodiscard]] auto get_layer_type() const -> core::layer_type
  {
    const auto role = static_cast<int>(layer_item_role::type);
    const auto type = data(role).value<int>();
    return core::layer_type{type};
  }
};

[[nodiscard]] auto icon_for_layer(const core::layer_type type) -> const QIcon&
{
  switch (type) {
    case core::layer_type::tile_layer:
      return icons::tile_layer();

    case core::layer_type::object_layer:
      return icons::object_layer();

    default:
      throw tactile_error{"Did not recognize layer type!"};
  }
}

[[nodiscard]] auto make_item(const layer_id id, const core::layer* layer)
    -> QStandardItem*
{
  auto* item = new layer_item{layer->name()};

  item->set_id(id);
  item->set_type(layer->type());
  item->setIcon(icon_for_layer(layer->type()));

  return item;
}

}  // namespace

layer_model::layer_model(core::map_document* document)
    : QStandardItemModel{}
    , m_document{document}
{
  if (!m_document) {
    throw tactile_error{"layer_model requires non-null map document!"};
  }

  document->each_layer(
      [this](const layer_id id, const core::shared_layer& layer) {
        this->appendRow(make_item(id, layer.get()));
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
  appendRow(make_item(id, &layer));
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

#include "layer_item.hpp"

#include "icons.hpp"
#include "layer_item_role.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {
namespace {

[[nodiscard]] auto icon_for_layer(const core::LayerType type) -> const QIcon&
{
  switch (type)
  {
    case core::LayerType::tile_layer:
      return IconTileLayer();

    case core::LayerType::object_layer:
      return IconObjectLayer();

    default:
      throw tactile_error{"Did not recognize layer type!"};
  }
}

}  // namespace

layer_item::layer_item(const QString& name) : QStandardItem{name}
{}

auto layer_item::make(const layer_id id, const core::ILayer& layer)
    -> layer_item*
{
  auto* item = new layer_item{layer.Name()};

  item->set_id(id);
  item->set_type(layer.Type());
  item->setIcon(icon_for_layer(layer.Type()));

  return item;
}

void layer_item::set_id(layer_id id)
{
  setData(id.get(), static_cast<int>(layer_item_role::id));
}

void layer_item::set_type(core::LayerType type)
{
  setData(static_cast<int>(type), static_cast<int>(layer_item_role::type));
}

auto layer_item::get_id() const -> layer_id
{
  const auto role = static_cast<int>(layer_item_role::id);
  const auto id = data(role).value<int>();
  return layer_id{id};
}

auto layer_item::get_layer_type() const -> core::LayerType
{
  const auto role = static_cast<int>(layer_item_role::type);
  const auto type = data(role).value<int>();
  return core::LayerType{type};
}

}  // namespace tactile::vm

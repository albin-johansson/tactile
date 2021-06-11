#include "layer_item.hpp"

#include "icons.hpp"
#include "layer_item_role.hpp"
#include "tactile_error.hpp"

namespace tactile::vm {
namespace {

[[nodiscard]] auto GetIcon(const core::LayerType type) -> const QIcon&
{
  switch (type)
  {
    case core::LayerType::tile_layer:
      return IconTileLayer();

    case core::LayerType::object_layer:
      return IconObjectLayer();

    default:
      throw TactileError{"Did not recognize layer type!"};
  }
}

}  // namespace

LayerItem::LayerItem(const layer_id id, const core::ILayer& layer)
    : QStandardItem{layer.Name()}
{
  SetId(id);
  SetType(layer.Type());
  setIcon(GetIcon(layer.Type()));
}

void LayerItem::SetId(const layer_id id)
{
  setData(id.get(), static_cast<int>(layer_item_role::id));
}

void LayerItem::SetType(const core::LayerType type)
{
  setData(static_cast<int>(type), static_cast<int>(layer_item_role::type));
}

auto LayerItem::GetId() const -> layer_id
{
  const auto role = static_cast<int>(layer_item_role::id);
  const auto id = data(role).value<int>();
  return layer_id{id};
}

auto LayerItem::GetLayerType() const -> core::LayerType
{
  const auto role = static_cast<int>(layer_item_role::type);
  const auto type = data(role).value<int>();
  return core::LayerType{type};
}

}  // namespace tactile::vm

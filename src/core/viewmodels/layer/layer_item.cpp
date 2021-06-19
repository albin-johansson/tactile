#include "layer_item.hpp"

#include "icons.hpp"
#include "layer_item_role.hpp"
#include "tactile_error.hpp"
#include "to_underlying.hpp"

namespace tactile::vm {
namespace {

[[nodiscard]] auto GetIcon(const core::LayerType type) -> const QIcon&
{
  switch (type)
  {
    case core::LayerType::TileLayer:
      return IconTileLayer();

    case core::LayerType::ObjectLayer:
      return IconObjectLayer();

    default:
      throw TactileError{"Did not recognize layer type!"};
  }
}

}  // namespace

LayerItem::LayerItem(const layer_id id, const core::ILayer& layer)
    : QStandardItem{layer.GetName()}
{
  SetId(id);
  SetType(layer.GetType());
  setIcon(GetIcon(layer.GetType()));
}

void LayerItem::SetId(const layer_id id)
{
  setData(id.get(), LayerItemRole::Id);
}

void LayerItem::SetType(const core::LayerType type)
{
  setData(ToUnderlying(type), LayerItemRole::Type);
}

auto LayerItem::GetId() const -> layer_id
{
  const auto id = data(LayerItemRole::Id).value<int>();
  return layer_id{id};
}

auto LayerItem::GetLayerType() const -> core::LayerType
{
  const auto type = data(LayerItemRole::Type).value<int>();
  return core::LayerType{type};
}

}  // namespace tactile::vm

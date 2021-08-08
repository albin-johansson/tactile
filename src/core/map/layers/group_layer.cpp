#include "group_layer.hpp"

#include <utility>  // move

#include "aliases/layer_stack_resource.hpp"
#include "core/tactile_error.hpp"
#include "layer_utils.hpp"

namespace Tactile {

GroupLayer::GroupLayer() : ALayer{LayerType::GroupLayer}
{
  SetName("Group layer");
}

auto GroupLayer::Clone() const -> SharedLayer
{
  return std::make_shared<GroupLayer>(*this);
}

void GroupLayer::AddLayer(const layer_id id, SharedLayer layer)
{
  mLayers.emplace(id, std::move(layer));
}

auto GroupLayer::GetLayer(const layer_id id) const -> SharedLayer
{
  return Tactile::GetLayer(mLayers, id);
}

auto GroupLayer::FindLayer(const layer_id id) -> ILayer*
{
  return Tactile::FindLayer(mLayers, id).get();
}

auto GroupLayer::FindLayer(const layer_id id) const -> const ILayer*
{
  return Tactile::FindLayer(mLayers, id).get();
}

auto GroupLayer::ContainsLayer(const layer_id id) const -> bool
{
  return FindLayer(id) != nullptr;
}

auto GroupLayer::GetLayers() -> storage_type&
{
  return mLayers;
}

auto GroupLayer::GetLayers() const -> const storage_type&
{
  return mLayers;
}

}  // namespace Tactile

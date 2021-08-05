#include "group_layer.hpp"

#include <utility>  // move

#include "aliases/layer_stack_resource.hpp"
#include "core/tactile_error.hpp"
#include "utils/buffer_utils.hpp"

namespace Tactile {

GroupLayer::GroupLayer() : mDelegate{LayerType::GroupLayer}
{
  SetName("Group layer");
}

void GroupLayer::SetVisible(const bool visible) noexcept
{
  mDelegate.SetVisible(visible);
}

void GroupLayer::SetOpacity(const float opacity)
{
  mDelegate.SetOpacity(opacity);
}

void GroupLayer::SetName(std::string name)
{
  mDelegate.SetName(std::move(name));
}

auto GroupLayer::GetType() const -> LayerType
{
  return mDelegate.GetType();
}

auto GroupLayer::IsVisible() const -> bool
{
  return mDelegate.IsVisible();
}

auto GroupLayer::GetOpacity() const noexcept -> float
{
  return mDelegate.GetOpacity();
}

auto GroupLayer::Clone() const -> SharedLayer
{
  return std::make_shared<GroupLayer>(*this);
}

void GroupLayer::AddProperty(const std::string& name, const PropertyType type)
{
  mDelegate.AddProperty(name, type);
}

void GroupLayer::AddProperty(const std::string& name, const Property& property)
{
  mDelegate.AddProperty(name, property);
}

void GroupLayer::RemoveProperty(const std::string_view name)
{
  mDelegate.RemoveProperty(name);
}

void GroupLayer::RenameProperty(const std::string_view oldName,
                                const std::string& newName)
{
  mDelegate.RenameProperty(oldName, newName);
}

void GroupLayer::SetProperty(const std::string_view name, const Property& property)
{
  mDelegate.SetProperty(name, property);
}

void GroupLayer::ChangePropertyType(const std::string_view name,
                                    const PropertyType type)
{
  mDelegate.ChangePropertyType(name, type);
}

auto GroupLayer::HasProperty(const std::string_view name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto GroupLayer::GetProperty(const std::string_view name) const -> const Property&
{
  return mDelegate.GetProperty(name);
}

auto GroupLayer::GetProperties() const -> const PropertyMap&
{
  return mDelegate.GetProperties();
}

auto GroupLayer::GetPropertyCount() const -> usize
{
  return mDelegate.GetPropertyCount();
}

auto GroupLayer::GetName() const -> std::string_view
{
  return mDelegate.GetName();
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

auto GroupLayer::GetLayers() -> layer_map&
{
  return mLayers;
}

auto GroupLayer::GetLayers() const -> const layer_map&
{
  return mLayers;
}

}  // namespace Tactile

#include "layer_delegate.hpp"

#include <algorithm>  // clamp
#include <utility>    // move

namespace tactile::core {

LayerDelegate::LayerDelegate(const layer_type type) noexcept : mType{type}
{}

void LayerDelegate::set_visible(const bool visible) noexcept
{
  mVisible = visible;
}

void LayerDelegate::set_opacity(const double opacity)
{
  mOpacity = std::clamp(opacity, 0.0, 1.0);
}

void LayerDelegate::set_name(QString name)
{
  mName = std::move(name);
}

auto LayerDelegate::type() const noexcept -> layer_type
{
  return mType;
}

auto LayerDelegate::visible() const noexcept -> bool
{
  return mVisible;
}

auto LayerDelegate::opacity() const noexcept -> double
{
  return mOpacity;
}

auto LayerDelegate::name() const -> const QString&
{
  return mName;
}

void LayerDelegate::add_property(const QString& name, const property_type type)
{
  mProperties.add_property(name, type);
}

void LayerDelegate::add_property(const QString& name, const property& property)
{
  mProperties.add_property(name, property);
}

void LayerDelegate::remove_property(const QString& name)
{
  mProperties.remove_property(name);
}

void LayerDelegate::rename_property(const QString& oldName,
                                    const QString& newName)
{
  mProperties.rename_property(oldName, newName);
}

void LayerDelegate::set_property(const QString& name, const property& property)
{
  mProperties.set_property(name, property);
}

void LayerDelegate::change_property_type(const QString& name,
                                         const property_type type)
{
  mProperties.change_property_type(name, type);
}

auto LayerDelegate::get_property(const QString& name) const -> const property&
{
  return mProperties.get_property(name);
}

auto LayerDelegate::get_property(const QString& name) -> property&
{
  return mProperties.get_property(name);
}

auto LayerDelegate::has_property(const QString& name) const -> bool
{
  return mProperties.has_property(name);
}

auto LayerDelegate::property_count() const noexcept -> int
{
  return mProperties.property_count();
}

auto LayerDelegate::properties() const -> const property_manager::property_map&
{
  return mProperties.properties();
}

}  // namespace tactile::core

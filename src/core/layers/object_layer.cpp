#include "object_layer.hpp"

#include <utility>  // move

#include "tactile_qstring.hpp"

namespace tactile::core {

ObjectLayer::ObjectLayer() : mDelegate{LayerType::object_layer}
{
  mDelegate.SetName(TACTILE_QSTRING(u"Object layer"));
}

void ObjectLayer::add_property(const QString& name, const property_type type)
{
  mDelegate.AddProperty(name, type);
}

void ObjectLayer::add_property(const QString& name, const property& property)
{
  mDelegate.AddProperty(name, property);
}

void ObjectLayer::remove_property(const QString& name)
{
  mDelegate.RemoveProperty(name);
}

void ObjectLayer::rename_property(const QString& oldName,
                                  const QString& newName)
{
  mDelegate.RenameProperty(oldName, newName);
}

void ObjectLayer::set_property(const QString& name, const property& property)
{
  mDelegate.SetProperty(name, property);
}

void ObjectLayer::change_property_type(const QString& name,
                                       const core::property_type type)
{
  mDelegate.ChangePropertyType(name, type);
}

auto ObjectLayer::get_property(const QString& name) const -> const property&
{
  return mDelegate.GetProperty(name);
}

auto ObjectLayer::get_property(const QString& name) -> property&
{
  return mDelegate.GetProperty(name);
}

auto ObjectLayer::has_property(const QString& name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto ObjectLayer::property_count() const noexcept -> int
{
  return mDelegate.PropertyCount();
}

auto ObjectLayer::properties() const -> const property_map&
{
  return mDelegate.Properties();
}

void ObjectLayer::SetVisible(const bool visible) noexcept
{
  mDelegate.SetVisible(visible);
}

void ObjectLayer::SetOpacity(const double opacity)
{
  mDelegate.SetOpacity(opacity);
}

void ObjectLayer::SetName(QString name)
{
  mDelegate.SetName(std::move(name));
}

auto ObjectLayer::Type() const -> LayerType
{
  return mDelegate.Type();
}

auto ObjectLayer::IsVisible() const noexcept -> bool
{
  return mDelegate.Visible();
}

auto ObjectLayer::Opacity() const noexcept -> double
{
  return mDelegate.Opacity();
}

auto ObjectLayer::Name() const -> const QString&
{
  return mDelegate.Name();
}

auto ObjectLayer::Clone() const -> shared<ILayer>
{
  return std::make_shared<ObjectLayer>(*this);
}

void ObjectLayer::AddObject(const object_id id, object obj)
{
  Q_ASSERT(!HasObject(id));
  mObjects.emplace(id, std::move(obj));
}

void ObjectLayer::AddPoint(const object_id id, const double x, const double y)
{
  Q_ASSERT(!HasObject(id));
  auto& [key, point] = mObjects.emplace(id, object_type::point);
  point.set_x(x);
  point.set_y(y);
}

void ObjectLayer::AddRectangle(const object_id id,
                               const double x,
                               const double y,
                               const double width,
                               const double height)
{
  Q_ASSERT(!HasObject(id));
  auto& [key, rect] = mObjects.emplace(id, object_type::rectangle);
  rect.set_x(x);
  rect.set_y(y);
  rect.set_width(width);
  rect.set_height(height);
}

void ObjectLayer::RemoveObject(const object_id id)
{
  Q_ASSERT(HasObject(id));
  mObjects.erase(id);
}

auto ObjectLayer::HasObject(const object_id id) const -> bool
{
  return mObjects.contains(id);
}

auto ObjectLayer::GetObject(const object_id id) -> object&
{
  return mObjects.at(id);
}

auto ObjectLayer::GetObject(const object_id id) const -> const object&
{
  return mObjects.at(id);
}

auto ObjectLayer::ObjectCount() const noexcept -> int
{
  return static_cast<int>(mObjects.size());
}

}  // namespace tactile::core

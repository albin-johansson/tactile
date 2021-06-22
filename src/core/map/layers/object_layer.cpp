#include "object_layer.hpp"

#include <cassert>  // assert
#include <utility>  // move

namespace tactile {

ObjectLayer::ObjectLayer() : mDelegate{LayerType::ObjectLayer}
{}

void ObjectLayer::SetVisible(const bool visible) noexcept
{
  mDelegate.SetVisible(visible);
}

void ObjectLayer::SetOpacity(const float opacity)
{
  mDelegate.SetOpacity(opacity);
}

void ObjectLayer::SetName(std::string name)
{
  mDelegate.SetName(std::move(name));
}

auto ObjectLayer::GetType() const -> LayerType
{
  return mDelegate.GetType();
}

auto ObjectLayer::IsVisible() const -> bool
{
  return mDelegate.IsVisible();
}

auto ObjectLayer::GetOpacity() const noexcept -> float
{
  return mDelegate.GetOpacity();
}

auto ObjectLayer::Clone() const -> Shared<ILayer>
{
  return std::make_shared<ObjectLayer>(*this);
}

void ObjectLayer::AddObject(const object_id id, Object object)
{
  assert(!HasObject(id));
  mObjects.emplace(id, std::move(object));
}

void ObjectLayer::AddPoint(const object_id id, const float x, const float y)
{
  assert(!HasObject(id));
  auto& [key, point] = mObjects.emplace(id, ObjectType::Point);
  point.SetX(x);
  point.SetY(y);
}

void ObjectLayer::AddRectangle(const object_id id,
                               const float x,
                               const float y,
                               const float width,
                               const float height)
{
  assert(!HasObject(id));
  auto& [key, rect] = mObjects.emplace(id, ObjectType::Rectangle);
  rect.SetX(x);
  rect.SetY(y);
  rect.SetWidth(width);
  rect.SetHeight(height);
}

void ObjectLayer::RemoveObject(const object_id id)
{
  assert(HasObject(id));
  mObjects.erase(id);
}

auto ObjectLayer::HasObject(const object_id id) const -> bool
{
  return mObjects.contains(id);
}

auto ObjectLayer::GetObject(const object_id id) -> Object&
{
  return mObjects.at(id);
}

auto ObjectLayer::GetObject(const object_id id) const -> const Object&
{
  return mObjects.at(id);
}

auto ObjectLayer::GetObjectCount() const noexcept -> usize
{
  return mObjects.size();
}

void ObjectLayer::AddProperty(const std::string& name, const PropertyType type)
{
  mDelegate.AddProperty(name, type);
}

void ObjectLayer::AddProperty(const std::string& name, const Property& property)
{
  mDelegate.AddProperty(name, property);
}

void ObjectLayer::RemoveProperty(const std::string_view name)
{
  mDelegate.RemoveProperty(name);
}

void ObjectLayer::RenameProperty(const std::string_view oldName,
                                 const std::string& newName)
{
  mDelegate.RenameProperty(oldName, newName);
}

void ObjectLayer::SetProperty(const std::string_view name,
                              const Property& property)
{
  mDelegate.SetProperty(name, property);
}

void ObjectLayer::ChangePropertyType(const std::string_view name,
                                     const PropertyType type)
{
  mDelegate.ChangePropertyType(name, type);
}

auto ObjectLayer::HasProperty(const std::string_view name) const -> bool
{
  return mDelegate.HasProperty(name);
}

auto ObjectLayer::GetProperty(const std::string_view name) const
    -> const Property&
{
  return mDelegate.GetProperty(name);
}

auto ObjectLayer::GetProperties() const -> const PropertyMap&
{
  return mDelegate.GetProperties();
}

auto ObjectLayer::GetPropertyCount() const -> usize
{
  return mDelegate.GetPropertyCount();
}

auto ObjectLayer::GetName() const -> std::string_view
{
  return mDelegate.GetName();
}

}  // namespace tactile

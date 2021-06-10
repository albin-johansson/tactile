#include "object.hpp"

#include <utility>  // move

namespace tactile::core {

Object::Object(const object_type type) : mType{type}
{}

void Object::SetX(const double x) noexcept
{
  mX = x;
}

void Object::SetY(const double y) noexcept
{
  mY = y;
}

void Object::SetWidth(const double width) noexcept
{
  mWidth = width;
}

void Object::SetHeight(const double height) noexcept
{
  mHeight = height;
}

void Object::SetVisible(const bool visible) noexcept
{
  mVisible = visible;
}

void Object::SetName(const QString& name)
{
  mName = name;
}

void Object::SetCustomType(maybe<QString> type)
{
  mCustomType = std::move(type);
}

auto Object::X() const noexcept -> double
{
  return mX;
}

auto Object::Y() const noexcept -> double
{
  return mY;
}

auto Object::Width() const noexcept -> double
{
  return mWidth;
}

auto Object::Height() const noexcept -> double
{
  return mHeight;
}

auto Object::IsVisible() const noexcept -> bool
{
  return mVisible;
}

auto Object::Name() const -> QString
{
  return mName;
}

auto Object::Type() const noexcept -> object_type
{
  return mType;
}

auto Object::CustomType() const -> maybe<QString>
{
  return mCustomType;
}

auto Object::IsPoint() const noexcept -> bool
{
  return Type() == object_type::point;
}

auto Object::IsRectangle() const noexcept -> bool
{
  return Type() == object_type::rectangle;
}

void Object::AddProperty(const QString& name, const PropertyType type)
{
  mProperties.AddProperty(name, type);
}

void Object::AddProperty(const QString& name, const Property& property)
{
  mProperties.AddProperty(name, property);
}

void Object::RemoveProperty(const QString& name)
{
  mProperties.RemoveProperty(name);
}

void Object::RenameProperty(const QString& oldName, const QString& newName)
{
  mProperties.RenameProperty(oldName, newName);
}

void Object::SetProperty(const QString& name, const Property& property)
{
  mProperties.SetProperty(name, property);
}

void Object::ChangePropertyType(const QString& name, const PropertyType type)
{
  mProperties.ChangePropertyType(name, type);
}

auto Object::GetProperty(const QString& name) const -> const Property&
{
  return mProperties.GetProperty(name);
}

auto Object::GetProperty(const QString& name) -> Property&
{
  return mProperties.GetProperty(name);
}

auto Object::HasProperty(const QString& name) const -> bool
{
  return mProperties.HasProperty(name);
}

auto Object::PropertyCount() const noexcept -> int
{
  return mProperties.PropertyCount();
}

auto Object::GetProperties() const -> const property_map&
{
  return mProperties.GetProperties();
}

}  // namespace tactile::core

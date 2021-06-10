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

void Object::add_property(const QString& name, const property_type type)
{
  mProperties.add_property(name, type);
}

void Object::add_property(const QString& name, const property& property)
{
  mProperties.add_property(name, property);
}

void Object::remove_property(const QString& name)
{
  mProperties.remove_property(name);
}

void Object::rename_property(const QString& oldName, const QString& newName)
{
  mProperties.rename_property(oldName, newName);
}

void Object::set_property(const QString& name, const property& property)
{
  mProperties.set_property(name, property);
}

void Object::change_property_type(const QString& name,
                                  const core::property_type type)
{
  mProperties.change_property_type(name, type);
}

auto Object::get_property(const QString& name) const -> const property&
{
  return mProperties.get_property(name);
}

auto Object::get_property(const QString& name) -> property&
{
  return mProperties.get_property(name);
}

auto Object::has_property(const QString& name) const -> bool
{
  return mProperties.has_property(name);
}

auto Object::property_count() const noexcept -> int
{
  return mProperties.property_count();
}

auto Object::properties() const -> const property_map&
{
  return mProperties.properties();
}

}  // namespace tactile::core

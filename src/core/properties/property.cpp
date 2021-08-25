#include "property.hpp"

namespace Tactile {

void Property::Reset()
{
  mValue.emplace<std::monostate>();
}

void Property::ResetToDefault(const PropertyType type)
{
  if (type == PropertyType::Integer)
  {
    SetValue<integer_type>(0);
  }
  else if (type == PropertyType::Floating)
  {
    SetValue<float_type>(0);
  }
  else if (type == PropertyType::Boolean)
  {
    SetValue<bool>(false);
  }
  else if (type == PropertyType::String)
  {
    SetValue<string_type>(string_type{});
  }
  else if (type == PropertyType::Color)
  {
    SetValue<color_type>(cen::colors::black);
  }
  else if (type == PropertyType::Object)
  {
    SetValue<object_ref>(object_ref{});
  }
  else if (type == PropertyType::File)
  {
    SetValue<file_type>(file_type{});
  }
}

auto Property::HasValue() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(mValue);
}

auto Property::AsString() const -> const string_type&
{
  return As<string_type>();
}

auto Property::AsInt() const -> integer_type
{
  return As<integer_type>();
}

auto Property::AsFloat() const -> float_type
{
  return As<float_type>();
}

auto Property::AsBool() const -> bool
{
  return As<bool>();
}

auto Property::AsFile() const -> const file_type&
{
  return As<file_type>();
}

auto Property::AsObject() const -> object_ref
{
  return As<object_ref>();
}

auto Property::AsColor() const -> const color_type&
{
  return As<color_type>();
}

auto Property::IsString() const noexcept -> bool
{
  return GetType() == PropertyType::String;
}

auto Property::IsInt() const noexcept -> bool
{
  return GetType() == PropertyType::Integer;
}

auto Property::IsFloat() const noexcept -> bool
{
  return GetType() == PropertyType::Floating;
}

auto Property::IsBool() const noexcept -> bool
{
  return GetType() == PropertyType::Boolean;
}

auto Property::IsFile() const noexcept -> bool
{
  return GetType() == PropertyType::File;
}

auto Property::IsObject() const noexcept -> bool
{
  return GetType() == PropertyType::Object;
}

auto Property::IsColor() const noexcept -> bool
{
  return GetType() == PropertyType::Color;
}

auto Property::GetType() const -> Maybe<PropertyType>
{
  if (Is<integer_type>())
  {
    return PropertyType::Integer;
  }
  else if (Is<float_type>())
  {
    return PropertyType::Floating;
  }
  else if (Is<string_type>())
  {
    return PropertyType::String;
  }
  else if (Is<bool>())
  {
    return PropertyType::Boolean;
  }
  else if (Is<file_type>())
  {
    return PropertyType::File;
  }
  else if (Is<object_ref>())
  {
    return PropertyType::Object;
  }
  else if (Is<color_type>())
  {
    return PropertyType::Color;
  }
  else
  {
    return nothing;
  }
}

}  // namespace Tactile

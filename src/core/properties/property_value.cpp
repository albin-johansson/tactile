#include "property_value.hpp"

namespace Tactile {

void PropertyValue::Reset()
{
  mValue.emplace<std::monostate>();
}

void PropertyValue::ResetToDefault(const PropertyType type)
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
    SetValue<ObjectRef>(ObjectRef{});
  }
  else if (type == PropertyType::File)
  {
    SetValue<file_type>(file_type{});
  }
}

auto PropertyValue::HasValue() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(mValue);
}

auto PropertyValue::AsString() const -> const string_type&
{
  return As<string_type>();
}

auto PropertyValue::AsInt() const -> integer_type
{
  return As<integer_type>();
}

auto PropertyValue::AsFloat() const -> float_type
{
  return As<float_type>();
}

auto PropertyValue::AsBool() const -> bool
{
  return As<bool>();
}

auto PropertyValue::AsFile() const -> const file_type&
{
  return As<file_type>();
}

auto PropertyValue::AsObject() const -> ObjectRef
{
  return As<ObjectRef>();
}

auto PropertyValue::AsColor() const -> const color_type&
{
  return As<color_type>();
}

auto PropertyValue::IsString() const noexcept -> bool
{
  return GetType() == PropertyType::String;
}

auto PropertyValue::IsInt() const noexcept -> bool
{
  return GetType() == PropertyType::Integer;
}

auto PropertyValue::IsFloat() const noexcept -> bool
{
  return GetType() == PropertyType::Floating;
}

auto PropertyValue::IsBool() const noexcept -> bool
{
  return GetType() == PropertyType::Boolean;
}

auto PropertyValue::IsFile() const noexcept -> bool
{
  return GetType() == PropertyType::File;
}

auto PropertyValue::IsObject() const noexcept -> bool
{
  return GetType() == PropertyType::Object;
}

auto PropertyValue::IsColor() const noexcept -> bool
{
  return GetType() == PropertyType::Color;
}

auto PropertyValue::GetType() const -> Maybe<PropertyType>
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
  else if (Is<ObjectRef>())
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

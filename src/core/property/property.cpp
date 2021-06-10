#include "property.hpp"

#include "tactile_error.hpp"
#include "tactile_qstring.hpp"

namespace tactile::core {

void Property::Reset()
{
  mValue.emplace<std::monostate>();
}

void Property::SetDefault(const PropertyType type)
{
  if (type == PropertyType::integer)
  {
    SetValue<int>();
  }
  else if (type == PropertyType::floating)
  {
    SetValue<double>();
  }
  else if (type == PropertyType::boolean)
  {
    SetValue<bool>();
  }
  else if (type == PropertyType::string)
  {
    SetValue<QString>();
  }
  else if (type == PropertyType::color)
  {
    SetValue<QColor>(Qt::black);
  }
  else if (type == PropertyType::object)
  {
    SetValue<object_ref>();
  }
  else if (type == PropertyType::file)
  {
    SetValue<QFileInfo>();
  }
}

auto Property::HasValue() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(mValue);
}

auto Property::Type() const -> maybe<PropertyType>
{
  if (Is<int>())
  {
    return PropertyType::integer;
  }
  else if (Is<double>())
  {
    return PropertyType::floating;
  }
  else if (Is<QString>())
  {
    return PropertyType::string;
  }
  else if (Is<bool>())
  {
    return PropertyType::boolean;
  }
  else if (Is<QFileInfo>())
  {
    return PropertyType::file;
  }
  else if (Is<object_ref>())
  {
    return PropertyType::object;
  }
  else if (Is<QColor>())
  {
    return PropertyType::color;
  }
  else
  {
    return std::nullopt;
  }
}

auto Property::AsString() const -> const QString&
{
  return As<QString>();
}

auto Property::AsInteger() const -> int
{
  return As<int>();
}

auto Property::AsFloating() const -> double
{
  return As<double>();
}

auto Property::AsBoolean() const -> bool
{
  return As<bool>();
}

auto Property::AsFile() const -> const QFileInfo&
{
  return As<QFileInfo>();
}

auto Property::AsObject() const -> object_ref
{
  return As<object_ref>();
}

auto Property::AsColor() const -> const QColor&
{
  return As<QColor>();
}

auto Property::IsString() const noexcept -> bool
{
  return Type() == PropertyType::string;
}

auto Property::IsInteger() const noexcept -> bool
{
  return Type() == PropertyType::integer;
}

auto Property::IsFloating() const noexcept -> bool
{
  return Type() == PropertyType::floating;
}

auto Property::IsBoolean() const noexcept -> bool
{
  return Type() == PropertyType::boolean;
}

auto Property::IsFile() const noexcept -> bool
{
  return Type() == PropertyType::file;
}

auto Property::IsObject() const noexcept -> bool
{
  return Type() == PropertyType::object;
}

auto Property::IsColor() const noexcept -> bool
{
  return Type() == PropertyType::color;
}

auto Stringify(PropertyType type) -> QString
{
  switch (type)
  {
    case PropertyType::string:
      return TACTILE_QSTRING(u"string");

    case PropertyType::integer:
      return TACTILE_QSTRING(u"int");

    case PropertyType::floating:
      return TACTILE_QSTRING(u"float");

    case PropertyType::boolean:
      return TACTILE_QSTRING(u"bool");

    case PropertyType::file:
      return TACTILE_QSTRING(u"file");

    case PropertyType::color:
      return TACTILE_QSTRING(u"color");

    case PropertyType::object:
      return TACTILE_QSTRING(u"object");

    default:
      throw TactileError{"Did not recognize property type!"};
  }
}

}  // namespace tactile::core

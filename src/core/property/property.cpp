#include "property.hpp"

#include "tactile_error.hpp"

namespace tactile::core {

void Property::Reset()
{
  mValue.emplace<std::monostate>();
}

void Property::SetDefault(const PropertyType type)
{
  if (type == PropertyType::Integer)
  {
    SetValue<int>();
  }
  else if (type == PropertyType::Floating)
  {
    SetValue<double>();
  }
  else if (type == PropertyType::Boolean)
  {
    SetValue<bool>();
  }
  else if (type == PropertyType::String)
  {
    SetValue<QString>();
  }
  else if (type == PropertyType::Color)
  {
    SetValue<QColor>(Qt::black);
  }
  else if (type == PropertyType::Object)
  {
    SetValue<object_ref>();
  }
  else if (type == PropertyType::File)
  {
    SetValue<QFileInfo>();
  }
}

auto Property::HasValue() const noexcept -> bool
{
  return !std::holds_alternative<std::monostate>(mValue);
}

auto Property::Type() const -> Maybe<PropertyType>
{
  if (Is<int>())
  {
    return PropertyType::Integer;
  }
  else if (Is<double>())
  {
    return PropertyType::Floating;
  }
  else if (Is<QString>())
  {
    return PropertyType::String;
  }
  else if (Is<bool>())
  {
    return PropertyType::Boolean;
  }
  else if (Is<QFileInfo>())
  {
    return PropertyType::File;
  }
  else if (Is<object_ref>())
  {
    return PropertyType::Object;
  }
  else if (Is<QColor>())
  {
    return PropertyType::Color;
  }
  else
  {
    return nothing;
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
  return Type() == PropertyType::String;
}

auto Property::IsInteger() const noexcept -> bool
{
  return Type() == PropertyType::Integer;
}

auto Property::IsFloating() const noexcept -> bool
{
  return Type() == PropertyType::Floating;
}

auto Property::IsBoolean() const noexcept -> bool
{
  return Type() == PropertyType::Boolean;
}

auto Property::IsFile() const noexcept -> bool
{
  return Type() == PropertyType::File;
}

auto Property::IsObject() const noexcept -> bool
{
  return Type() == PropertyType::Object;
}

auto Property::IsColor() const noexcept -> bool
{
  return Type() == PropertyType::Color;
}

auto Stringify(PropertyType type) -> QString
{
  switch (type)
  {
    case PropertyType::String:
      return QStringLiteral(u"string");

    case PropertyType::Integer:
      return QStringLiteral(u"int");

    case PropertyType::Floating:
      return QStringLiteral(u"float");

    case PropertyType::Boolean:
      return QStringLiteral(u"bool");

    case PropertyType::File:
      return QStringLiteral(u"file");

    case PropertyType::Color:
      return QStringLiteral(u"color");

    case PropertyType::Object:
      return QStringLiteral(u"object");

    default:
      throw TactileError{"Did not recognize property type!"};
  }
}

}  // namespace tactile::core

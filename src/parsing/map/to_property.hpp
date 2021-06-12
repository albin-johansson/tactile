#pragma once

#include <QColor>     // QColor
#include <QFileInfo>  // QFileInfo
#include <QString>    // QString
#include <optional>   // optional

#include "element_id.hpp"
#include "map_parse_concepts.hpp"
#include "parse_error.hpp"
#include "property.hpp"
#include "tactile_qstring.hpp"

namespace tactile {

template <IsParserObject T>
[[nodiscard]] auto ToProperty(const T& object,
                              const QString& propType,
                              ParseError& error)
    -> std::optional<core::Property>
{
  core::Property property;

  if (propType == TACTILE_QSTRING(u"string"))
  {
    property.SetValue(object.String(ElementId::Value).value());
  }
  else if (propType == TACTILE_QSTRING(u"int"))
  {
    property.SetValue(object.Integer(ElementId::Value).value());
  }
  else if (propType == TACTILE_QSTRING(u"float"))
  {
    property.SetValue(object.Floating(ElementId::Value).value());
  }
  else if (propType == TACTILE_QSTRING(u"bool"))
  {
    property.SetValue(object.Boolean(ElementId::Value).value());
  }
  else if (propType == TACTILE_QSTRING(u"file"))
  {
    const auto file = object.String(ElementId::Value).value();
    property.SetValue(QFileInfo{file});
  }
  else if (propType == TACTILE_QSTRING(u"object"))
  {
    const auto obj = object.Integer(ElementId::Value).value();
    property.SetValue(core::object_ref{obj});
  }
  else if (propType == TACTILE_QSTRING(u"color"))
  {
    const auto color = object.String(ElementId::Value).value();
    property.SetValue(QColor{color});
  }
  else
  {
    error = ParseError::UnknownPropertyType;
    return std::nullopt;
  }

  return property;
}

}  // namespace tactile

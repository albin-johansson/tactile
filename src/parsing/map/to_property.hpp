#pragma once

#include <QColor>     // QColor
#include <QFileInfo>  // QFileInfo
#include <QString>    // QString

#include "element_id.hpp"
#include "map_parse_concepts.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "property.hpp"

namespace tactile {

template <IsParserObject T>
[[nodiscard]] auto ToProperty(const T& object,
                              const QString& propType,
                              ParseError& error) -> Maybe<core::Property>
{
  core::Property property;

  if (propType == QStringLiteral(u"string"))
  {
    property.SetValue(object.String(ElementId::Value).value());
  }
  else if (propType == QStringLiteral(u"int"))
  {
    property.SetValue(object.Integer(ElementId::Value).value());
  }
  else if (propType == QStringLiteral(u"float"))
  {
    property.SetValue(object.Floating(ElementId::Value).value());
  }
  else if (propType == QStringLiteral(u"bool"))
  {
    property.SetValue(object.Boolean(ElementId::Value).value());
  }
  else if (propType == QStringLiteral(u"file"))
  {
    const auto file = object.String(ElementId::Value).value();
    property.SetValue(QFileInfo{file});
  }
  else if (propType == QStringLiteral(u"object"))
  {
    const auto obj = object.Integer(ElementId::Value).value();
    property.SetValue(core::object_ref{obj});
  }
  else if (propType == QStringLiteral(u"color"))
  {
    const auto color = object.String(ElementId::Value).value();
    property.SetValue(QColor{color});
  }
  else
  {
    error = ParseError::UnknownPropertyType;
    return nothing;
  }

  return property;
}

}  // namespace tactile

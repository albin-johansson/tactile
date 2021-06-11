#pragma once

#include <QColor>     // QColor
#include <QFileInfo>  // QFileInfo
#include <QString>    // QString

#include "element_id.hpp"
#include "map_parse_concepts.hpp"
#include "maybe.hpp"
#include "parse_error.hpp"
#include "property.hpp"
#include "tactile_qstring.hpp"

namespace tactile::parse {

template <is_object T>
[[nodiscard]] auto to_property(const T& object,
                               const QString& propType,
                               parse_error& error) -> maybe<core::Property>
{
  core::Property property;

  if (propType == TACTILE_QSTRING(u"string"))
  {
    property.SetValue(object.string(ElementId::Value).Value());
  }
  else if (propType == TACTILE_QSTRING(u"int"))
  {
    property.SetValue(object.integer(ElementId::Value).Value());
  }
  else if (propType == TACTILE_QSTRING(u"float"))
  {
    property.SetValue(object.floating(ElementId::Value).Value());
  }
  else if (propType == TACTILE_QSTRING(u"bool"))
  {
    property.SetValue(object.boolean(ElementId::Value).Value());
  }
  else if (propType == TACTILE_QSTRING(u"file"))
  {
    const auto file = object.string(ElementId::Value).Value();
    property.SetValue(QFileInfo{file});
  }
  else if (propType == TACTILE_QSTRING(u"object"))
  {
    const auto obj = object.integer(ElementId::Value).Value();
    property.SetValue(core::object_ref{obj});
  }
  else if (propType == TACTILE_QSTRING(u"color"))
  {
    const auto color = object.string(ElementId::Value).Value();
    property.SetValue(QColor{color});
  }
  else
  {
    error = parse_error::unknown_property_type;
    return std::nullopt;
  }

  return property;
}

}  // namespace tactile::parse

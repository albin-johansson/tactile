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
                               parse_error& error) -> maybe<core::property>
{
  core::property prop;

  if (propType == TACTILE_QSTRING(u"string")) {
    prop.set_value(object.string(element_id::value).value());

  } else if (propType == TACTILE_QSTRING(u"int")) {
    prop.set_value(object.integer(element_id::value).value());

  } else if (propType == TACTILE_QSTRING(u"float")) {
    prop.set_value(object.floating(element_id::value).value());

  } else if (propType == TACTILE_QSTRING(u"bool")) {
    prop.set_value(object.boolean(element_id::value).value());

  } else if (propType == TACTILE_QSTRING(u"file")) {
    const auto file = object.string(element_id::value).value();
    prop.set_value(QFileInfo{file});

  } else if (propType == TACTILE_QSTRING(u"object")) {
    const auto obj = object.integer(element_id::value).value();
    prop.set_value(core::object_ref{obj});

  } else if (propType == TACTILE_QSTRING(u"color")) {
    const auto color = object.string(element_id::value).value();
    prop.set_value(QColor{color});

  } else {
    error = parse_error::unknown_property_type;
    return std::nullopt;
  }

  return prop;
}

}  // namespace tactile::parse

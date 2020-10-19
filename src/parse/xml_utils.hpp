#pragma once

#include <QFileInfo>
#include <QtXml>
#include <concepts>  // constructible_from, invocable

#include "tactile_error.hpp"

namespace tactile::xml {

template <std::constructible_from<int> T = int>
[[nodiscard]] auto int_attr(const QDomElement& element, const QString& key) -> T
{
  bool ok{};
  if (const auto result = element.attribute(key).toInt(&ok); ok) {
    return T{result};
  } else {
    throw tactile_error{"Failed to obtain attribute from XML attribute!"};
  }
}

template <std::constructible_from<int> T = int>
[[nodiscard]] auto int_attr(const QDomElement& element,
                            const QString& key,
                            T fallback) -> T
{
  bool ok{};
  if (const auto result = element.attribute(key).toInt(&ok); ok) {
    return T{result};
  } else {
    return fallback;
  }
}

template <std::constructible_from<double> T = double>
[[nodiscard]] auto double_attr(const QDomElement& element,
                               const QString& key,
                               T fallback) -> T
{
  bool ok{};
  if (const auto result = element.attribute(key).toDouble(&ok); ok) {
    return T{result};
  } else {
    return fallback;
  }
}

template <std::invocable<const QDomNode&> T>
void each_elem(const QDomElement& element, const QString& tag, T&& callable)
{
  const auto elements = element.elementsByTagName(tag);
  const auto count = elements.count();
  for (auto i = 0; i < count; ++i) {
    callable(elements.at(i));
  }
}

[[nodiscard]] auto to_elem(const QDomNode& node) -> QDomElement;

[[nodiscard]] auto from_file(const QFileInfo& path) -> QDomDocument;

void write_file(const QFileInfo& path, const QDomDocument& document);

}  // namespace tactile::xml

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
    qDebug() << QStringLiteral(u"No integer attribute:") << key
             << QStringLiteral(u"in element with tag:") << element.tagName();
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

template <std::invocable<QDomNode> T>
void each_elem(const QDomElement& element, const QString& tag, T&& callable)
{
  const auto elements = element.elementsByTagName(tag);
  const auto count = elements.count();
  for (int i = 0; i < count; ++i) {
    callable(elements.at(i));
  }
}

[[nodiscard]] auto to_elem(const QDomNode& node) -> QDomElement;

[[nodiscard]] auto from_file(const QFileInfo& path) -> QDomDocument;

}  // namespace tactile::xml

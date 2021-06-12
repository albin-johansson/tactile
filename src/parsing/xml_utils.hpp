#pragma once

#include <QDomElement>  // QDomElement, QDomDocument
#include <QFileInfo>    // QFileInfo
#include <concepts>     // constructible_from, invocable

#include "maybe.hpp"

namespace tactile {

[[nodiscard]] auto ReadXml(const QFileInfo& path) -> Maybe<QDomDocument>;

void WriteXml(const QFileInfo& path, const QDomDocument& document);

template <std::constructible_from<int> T = int>
[[nodiscard]] auto GetIntAttr(const QDomElement& element, const QString& key)
    -> Maybe<T>
{
  bool ok{};
  if (const auto result = element.attribute(key).toInt(&ok); ok)
  {
    return T{result};
  }
  else
  {
    return nothing;
  }
}

template <std::constructible_from<int> T = int>
[[nodiscard]] auto GetIntAttr(const QDomElement& element,
                              const QString& key,
                              T fallback) -> T
{
  bool ok{};
  if (const auto result = element.attribute(key).toInt(&ok); ok)
  {
    return T{result};
  }
  else
  {
    return fallback;
  }
}

template <std::constructible_from<double> T = double>
[[nodiscard]] auto GetDoubleAttr(const QDomElement& element,
                                 const QString& key,
                                 T fallback) -> T
{
  bool ok{};
  if (const auto result = element.attribute(key).toDouble(&ok); ok)
  {
    return T{result};
  }
  else
  {
    return fallback;
  }
}

template <std::invocable<const QDomNode&> T>
void VisitElements(const QDomElement& element, const QString& tag, T&& callable)
{
  const auto elements = element.elementsByTagName(tag);
  const auto count = elements.count();
  for (auto i = 0; i < count; ++i)
  {
    callable(elements.at(i));
  }
}

}  // namespace tactile

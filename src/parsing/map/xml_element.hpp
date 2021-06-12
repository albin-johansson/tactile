#pragma once

#include <QDomElement>  // QDomElement

#include "element_id.hpp"
#include "maybe.hpp"

namespace tactile {

class XmlElement final
{
 public:
  explicit XmlElement(const QDomElement& element);

  [[nodiscard]] auto Contains(ElementId id) const -> bool;

  [[nodiscard]] auto Integer(ElementId id) const -> Maybe<int>;

  [[nodiscard]] auto Integer(ElementId id, int def) const -> Maybe<int>;

  [[nodiscard]] auto Floating(ElementId id) const -> Maybe<double>;

  [[nodiscard]] auto Floating(ElementId id, double def) const -> double;

  [[nodiscard]] auto String(ElementId id) const -> Maybe<QString>;

  [[nodiscard]] auto String(ElementId id, const QString& def) const -> QString;

  [[nodiscard]] auto Boolean(ElementId id) const -> Maybe<bool>;

  auto operator->() noexcept -> QDomElement*
  {
    return &mElement;
  }

  auto operator->() const noexcept -> const QDomElement*
  {
    return &mElement;
  }

 private:
  QDomElement mElement;

  [[nodiscard]] static auto StringifyElementId(ElementId type) -> QString;
};

}  // namespace tactile

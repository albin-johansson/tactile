#pragma once

#include <QDomElement>  // QDomElement
#include <optional>     // optional

#include "element_id.hpp"

namespace tactile {

class XmlElement final
{
 public:
  explicit XmlElement(const QDomElement& element);

  [[nodiscard]] auto Contains(ElementId id) const -> bool;

  [[nodiscard]] auto Integer(ElementId id) const -> std::optional<int>;

  [[nodiscard]] auto Integer(ElementId id, int def) const -> std::optional<int>;

  [[nodiscard]] auto Floating(ElementId id) const -> std::optional<double>;

  [[nodiscard]] auto Floating(ElementId id, double def) const -> double;

  [[nodiscard]] auto String(ElementId id) const -> std::optional<QString>;

  [[nodiscard]] auto String(ElementId id, const QString& def) const -> QString;

  [[nodiscard]] auto Boolean(ElementId id) const -> std::optional<bool>;

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

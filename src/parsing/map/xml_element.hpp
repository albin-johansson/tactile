#pragma once

#include <QDomElement>  // QDomElement

#include "element_id.hpp"
#include "maybe.hpp"

namespace tactile::parse {

class xml_element final
{
 public:
  explicit xml_element(const QDomElement& element);

  [[nodiscard]] auto contains(ElementId id) const -> bool;

  [[nodiscard]] auto integer(ElementId id) const -> maybe<int>;

  [[nodiscard]] auto integer(ElementId id, int def) const -> maybe<int>;

  [[nodiscard]] auto floating(ElementId id) const -> maybe<double>;

  [[nodiscard]] auto floating(ElementId id, double def) const -> double;

  [[nodiscard]] auto string(ElementId id) const -> maybe<QString>;

  [[nodiscard]] auto string(ElementId id, const QString& def) const -> QString;

  [[nodiscard]] auto boolean(ElementId id) const -> maybe<bool>;

  auto operator->() noexcept -> QDomElement*
  {
    return &m_element;
  }

  auto operator->() const noexcept -> const QDomElement*
  {
    return &m_element;
  }

 private:
  QDomElement m_element;

  [[nodiscard]] static auto stringify_element_id(ElementId type) -> QString;
};

}  // namespace tactile::parse

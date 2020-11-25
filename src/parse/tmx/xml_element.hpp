#pragma once

#include <QtXml>

#include "element_id.hpp"
#include "maybe.hpp"

namespace tactile::tmx {

class xml_element final
{
 public:
  explicit xml_element(const QDomElement& element);

  [[nodiscard]] auto contains(element_id id) const -> bool;

  [[nodiscard]] auto integer(element_id id) const -> maybe<int>;

  [[nodiscard]] auto integer(element_id id, int def) const -> maybe<int>;

  [[nodiscard]] auto floating(element_id id, double def) const -> double;

  [[nodiscard]] auto string(element_id id) const -> maybe<QString>;

  [[nodiscard]] auto string(element_id id, const QString& def) const -> QString;

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

  [[nodiscard]] static auto stringify_element_id(element_id type) -> QString;
};

}  // namespace tactile::tmx

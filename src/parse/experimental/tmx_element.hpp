#pragma once

#include <QtXml>

#include "element_type.hpp"
#include "maybe.hpp"

namespace tactile::tmx {

class tmx_element final
{
 public:
  explicit tmx_element(const QDomElement& element);

  [[nodiscard]] auto contains(const QString& str) const -> bool;

  [[nodiscard]] auto integer(const QString& str) const -> maybe<int>;
  [[nodiscard]] auto integer(element_type id) const -> maybe<int>;
  [[nodiscard]] auto integer(const QString& str, const int def) const -> int;

  [[nodiscard]] auto floating(const QString& str, const double def) const
      -> double;

  [[nodiscard]] auto string(const QString& str) const -> maybe<QString>;
  [[nodiscard]] auto string(const QString& str, const QString& def) const
      -> QString;

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

  [[nodiscard]] static auto stringify_element_id(element_type type) -> QString;
};

}  // namespace tactile::tmx

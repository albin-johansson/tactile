#pragma once

#include <QJsonObject>  // QJsonObject

#include "element_id.hpp"
#include "maybe.hpp"

namespace tactile::parse {

class json_element final
{
 public:
  explicit json_element(QJsonObject object);

  [[nodiscard]] auto contains(ElementId id) const -> bool;

  [[nodiscard]] auto integer(const QString& str) const -> maybe<int>;

  [[nodiscard]] auto integer(ElementId id) const -> maybe<int>;

  [[nodiscard]] auto integer(ElementId id, int def) const -> maybe<int>;

  [[nodiscard]] auto integer(const QString& str, int def) const -> int;

  [[nodiscard]] auto floating(ElementId id) const -> maybe<double>;

  [[nodiscard]] auto floating(const QString& str, double def) const -> double;

  [[nodiscard]] auto floating(ElementId id, double def) const -> double;

  [[nodiscard]] auto string(const QString& str) const -> maybe<QString>;

  [[nodiscard]] auto string(const QString& str, const QString& def) const
      -> QString;

  [[nodiscard]] auto string(ElementId id) const -> maybe<QString>;

  [[nodiscard]] auto string(ElementId id, const QString& def) const -> QString;

  [[nodiscard]] auto boolean(ElementId id) const -> maybe<bool>;

  auto operator->() noexcept -> QJsonObject*
  {
    return &m_object;
  }

  auto operator->() const noexcept -> const QJsonObject*
  {
    return &m_object;
  }

 private:
  QJsonObject m_object;

  [[nodiscard]] static auto stringify_element_id(ElementId type)
      -> QStringView;
};

}  // namespace tactile::parse

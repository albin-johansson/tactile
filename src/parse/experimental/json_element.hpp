#pragma once

#include <QJsonObject>

#include "element_type.hpp"
#include "maybe.hpp"

namespace tactile::tmx {

class json_element final
{
 public:
  explicit json_element(QJsonObject object);

  [[nodiscard]] auto contains(const QString& str) const -> bool;

  [[nodiscard]] auto integer(const QString& str) const -> maybe<int>;

  [[nodiscard]] auto integer(element_type id) const -> maybe<int>;

  [[nodiscard]] auto integer(const QString& str, int def) const -> int;

  [[nodiscard]] auto floating(const QString& str, double def) const -> double;

  [[nodiscard]] auto string(const QString& str) const -> maybe<QString>;

  [[nodiscard]] auto string(const QString& str, const QString& def) const
      -> QString;

 private:
  QJsonObject m_object;

  [[nodiscard]] static auto stringify_element_id(element_type type)
      -> QStringView;
};

}  // namespace tactile::tmx

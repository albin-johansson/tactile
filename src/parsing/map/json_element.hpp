#pragma once

#include <QJsonObject>  // QJsonObject

#include "element_id.hpp"
#include "maybe.hpp"

namespace tactile::parse {

class JsonElement final
{
 public:
  explicit JsonElement(QJsonObject object);

  [[nodiscard]] auto Contains(ElementId id) const -> bool;

  [[nodiscard]] auto Integer(const QString& str) const -> maybe<int>;

  [[nodiscard]] auto Integer(ElementId id) const -> maybe<int>;

  [[nodiscard]] auto Integer(ElementId id, int def) const -> maybe<int>;

  [[nodiscard]] auto Integer(const QString& str, int def) const -> int;

  [[nodiscard]] auto Floating(ElementId id) const -> maybe<double>;

  [[nodiscard]] auto Floating(const QString& str, double def) const -> double;

  [[nodiscard]] auto Floating(ElementId id, double def) const -> double;

  [[nodiscard]] auto String(const QString& str) const -> maybe<QString>;

  [[nodiscard]] auto String(const QString& str, const QString& def) const
      -> QString;

  [[nodiscard]] auto String(ElementId id) const -> maybe<QString>;

  [[nodiscard]] auto String(ElementId id, const QString& def) const -> QString;

  [[nodiscard]] auto Boolean(ElementId id) const -> maybe<bool>;

  auto operator->() noexcept -> QJsonObject*
  {
    return &mObject;
  }

  auto operator->() const noexcept -> const QJsonObject*
  {
    return &mObject;
  }

 private:
  QJsonObject mObject;

  [[nodiscard]] static auto StringifyElementId(ElementId type) -> QStringView;
};

}  // namespace tactile::parse

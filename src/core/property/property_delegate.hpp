#pragma once

#include <QString>

#include "property.hpp"
#include "property_manager.hpp"
#include "vector_map.hpp"

namespace tactile::core {

class property_delegate final : public property_manager
{
 public:
  ~property_delegate() noexcept override = default;

  void add_property(const QString& name, property::type type) override;

  void add_property(const QString& name, const property& property) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name, const property& property) override;

  // clang-format off

  [[nodiscard]]
  auto get_property(const QString& name) const -> const property& override;

  [[nodiscard]]
  auto get_property(const QString& name) -> property& override;

  [[nodiscard]]
  auto property_count() const noexcept -> int override;

  [[nodiscard]]
  auto properties() const -> const property_map& override;

  // clang-format on

 private:
  vector_map<QString, property> m_properties;
};

}  // namespace tactile::core

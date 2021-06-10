#pragma once

#include <QString>  // QString

#include "property.hpp"
#include "property_manager.hpp"
#include "vector_map.hpp"

namespace tactile::core {

class property_delegate final : public IPropertyManager
{
 public:
  ~property_delegate() noexcept override = default;

  void AddProperty(const QString& name, property_type type) override;

  void AddProperty(const QString& name, const property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name, const property& property) override;

  void ChangePropertyType(const QString& name,
                            core::property_type type) override;

  // clang-format off

  [[nodiscard]]
  auto GetProperty(const QString& name) const -> const property& override;

  [[nodiscard]]
  auto GetProperty(const QString& name) -> property& override;

  [[nodiscard]]
  auto HasProperty(const QString & name) const -> bool override;

  [[nodiscard]]
  auto PropertyCount() const noexcept -> int override;

  [[nodiscard]]
  auto GetProperties() const -> const property_map& override;

  // clang-format on

 private:
  vector_map<QString, property> m_properties;
};

}  // namespace tactile::core

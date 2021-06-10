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

  void AddProperty(const QString& name, PropertyType type) override;

  void AddProperty(const QString& name, const Property& property) override;

  void RemoveProperty(const QString& name) override;

  void RenameProperty(const QString& oldName, const QString& newName) override;

  void SetProperty(const QString& name, const Property& property) override;

  void ChangePropertyType(const QString& name,
                          core::PropertyType type) override;

  [[nodiscard]] auto GetProperty(const QString& name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperty(const QString& name) -> Property& override;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool override;

  [[nodiscard]] auto PropertyCount() const noexcept -> int override;

  [[nodiscard]] auto GetProperties() const -> const property_map& override;

 private:
  vector_map<QString, Property> mProperties;
};

}  // namespace tactile::core

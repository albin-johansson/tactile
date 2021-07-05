#pragma once

#include <string>  // string

#include "property_context.hpp"

namespace Tactile {

class PropertyDelegate final : public IPropertyContext
{
 public:
  PropertyDelegate() = default;

  explicit PropertyDelegate(std::string name);

  void SetName(std::string name);

  void AddProperty(const std::string& name, PropertyType type) override;

  void AddProperty(const std::string& name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, const std::string& newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string_view name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> std::string_view override;

 private:
  PropertyMap mProperties;
  std::string mName;
};

}  // namespace Tactile

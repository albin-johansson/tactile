#pragma once

#include <string>  // string

#include "property_context.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

class PropertyDelegate final : public IPropertyContext
{
 public:
  PropertyDelegate() = default;

  explicit PropertyDelegate(std::string name);

  void SetName(std::string name);

  void AddProperty(std::string name, PropertyType type) override;

  void AddProperty(std::string name, const Property& property) override;

  void RemoveProperty(std::string_view name) override;

  void RenameProperty(std::string_view oldName, std::string newName) override;

  void SetProperty(std::string_view name, const Property& property) override;

  void ChangePropertyType(std::string name, PropertyType type) override;

  [[nodiscard]] auto HasProperty(std::string_view name) const -> bool override;

  [[nodiscard]] auto GetProperty(std::string_view name) const
      -> const Property& override;

  [[nodiscard]] auto GetProperties() const -> const PropertyMap& override;

  [[nodiscard]] auto GetPropertyCount() const -> usize override;

  [[nodiscard]] auto GetName() const -> const std::string& override;

 private:
  PropertyMap mProperties;
  std::string mName;
};

/// \} End of group core

}  // namespace Tactile

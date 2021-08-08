#pragma once

#include <string>  // string

#include "core/properties/property_context.hpp"
#include "core/properties/property_delegate.hpp"
#include "layer_type.hpp"

namespace Tactile {

/// \addtogroup core
/// \{

/**
 * \class LayerDelegate
 *
 * \brief A delegate meant to be used to implement the basic layer API.
 *
 * \note This class doesn't inherit from `ILayer` because it can't provide a
 * meaningful `ILayer::Clone()` implementation.
 *
 * \headerfile layer_delegate.hpp
 */
class LayerDelegate final : public IPropertyContext
{
 public:
  explicit LayerDelegate(LayerType type);

  /// \name Layer API
  /// \{

  void SetVisible(bool visible) noexcept;

  void SetOpacity(float opacity);

  void SetName(std::string name);

  [[nodiscard]] auto GetType() const noexcept -> LayerType
  {
    return mType;
  }

  [[nodiscard]] auto GetOpacity() const noexcept -> float
  {
    return mOpacity;
  }

  [[nodiscard]] auto IsVisible() const noexcept -> bool
  {
    return mVisible;
  }

  /// \} End of layer API

  /// \name Property API
  /// \{

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

  /// \} End of property API

 private:
  LayerType mType;
  PropertyDelegate mProperties;
  float mOpacity{1};
  bool mVisible{true};
};

/// \} End of group core

}  // namespace Tactile

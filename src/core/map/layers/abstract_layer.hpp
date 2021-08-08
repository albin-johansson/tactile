#pragma once

#include "layer.hpp"
#include "layer_delegate.hpp"

namespace Tactile {

/**
 * \brief An ABC for layer implementations.
 *
 * \ingroup core
 *
 * \details This abstract class implements all of the `IPropertyContext` and `ILayer`
 * interfaces, except for `ILayer::Clone()`.
 *
 * \see `ILayer`
 * \see `TileLayer`
 * \see `ObjectLayer`
 * \see `GroupLayer`
 */
class ALayer : public ILayer
{
 public:
  explicit ALayer(LayerType type);

  /// \name Layer API
  /// \{

  void SetVisible(bool visible) override;

  void SetOpacity(float opacity) override;

  void SetName(std::string name) override;

  [[nodiscard]] auto IsVisible() const -> bool override;

  [[nodiscard]] auto GetOpacity() const noexcept -> float override;

  [[nodiscard]] auto GetType() const -> LayerType override;

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
  LayerDelegate mDelegate;
};

}  // namespace Tactile

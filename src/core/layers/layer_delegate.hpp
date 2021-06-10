#pragma once

#include <QString>  // QString

#include "layer_type.hpp"
#include "property_delegate.hpp"

namespace tactile::core {

/**
 * \class LayerDelegate
 *
 * \brief A delegate meant to be used to implement the basic layer API.
 *
 * \note This class doesn't inherit from `ILayer` because it can't provide a
 * meaningful `ILayer::Clone()` implementation.
 *
 * \since 0.2.0
 *
 * \headerfile layer_delegate.hpp
 */
class LayerDelegate final
{
 public:
  explicit LayerDelegate(LayerType type) noexcept;

  /// \name Layer API
  /// \{

  void SetVisible(bool visible) noexcept;

  void SetOpacity(double opacity);

  void SetName(QString name);

  [[nodiscard]] auto Type() const noexcept -> LayerType;

  [[nodiscard]] auto Visible() const noexcept -> bool;

  [[nodiscard]] auto Opacity() const noexcept -> double;

  [[nodiscard]] auto Name() const -> const QString&;

  /// \} End of layer API

  /// \name Property API
  /// \{

  void AddProperty(const QString& name, property_type type);

  void AddProperty(const QString& name, const property& property);

  void RemoveProperty(const QString& name);

  void RenameProperty(const QString& oldName, const QString& newName);

  void SetProperty(const QString& name, const property& property);

  void ChangePropertyType(const QString& name, property_type type);

  [[nodiscard]] auto GetProperty(const QString& name) const -> const property&;

  [[nodiscard]] auto GetProperty(const QString& name) -> property&;

  [[nodiscard]] auto HasProperty(const QString& name) const -> bool;

  [[nodiscard]] auto PropertyCount() const noexcept -> int;

  [[nodiscard]] auto Properties() const
      -> const property_manager::property_map&;

  /// \} End of property API

 private:
  LayerType mType;
  QString mName;
  property_delegate mProperties;
  double mOpacity{1};
  bool mVisible{true};
};

}  // namespace tactile::core

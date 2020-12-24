#pragma once

#include "layer.hpp"
#include "layer_delegate.hpp"
#include "property_delegate.hpp"
#include <memory>    // shared_ptr

namespace tactile::core {

/**
 * \class object_layer
 *
 * \brief Represents a layer that only contains map objects.
 *
 * \since 0.2.0
 *
 * \headerfile object_layer.hpp
 */
class object_layer final : public layer
{
 public:
  ~object_layer() noexcept override = default;

  /// \name Layer API

  void set_visible(bool visible) noexcept override;

  void set_opacity(double opacity) override;

  void set_name(QString name) override;

  [[nodiscard]] auto visible() const noexcept -> bool override;

  [[nodiscard]] auto opacity() const noexcept -> double override;

  [[nodiscard]] auto name() const -> const QString& override;

  [[nodiscard]] auto clone() const -> shared_layer override;

  /// \}

  /// \name Properties
  /// \{

  void add_property(const QString& name, property::type type) override;

  void remove_property(const QString& name) override;

  void rename_property(const QString& oldName, const QString& newName) override;

  void set_property(const QString& name, const property& property) override;

  [[nodiscard]] auto get_property(const QString& name) const
      -> const property& override;

  [[nodiscard]] auto get_property(const QString& name) -> property& override;

  [[nodiscard]] auto property_count() const noexcept -> int override;

  /// \}

 private:
  // TODO m_objects
  layer_delegate m_layerDelegate;
  property_delegate m_propertyDelegate;
};

using shared_object_layer = std::shared_ptr<object_layer>;

}  // namespace tactile::core

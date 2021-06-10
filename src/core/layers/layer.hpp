#pragma once

#include <QString>  // QString

#include "layer_type.hpp"
#include "property_manager.hpp"
#include "smart_pointers.hpp"

namespace tactile::core {

/**
 * \interface ILayer
 *
 * \brief Represents a layer in a map.
 *
 * \see map
 * \see tile_layer
 * \see object_layer
 *
 * \since 0.2.0
 *
 * \headerfile layer.hpp
 */
class ILayer : public IPropertyManager
{
 public:
  /**
   * \brief Sets whether or not the layer is visible.
   *
   * \param visible `true` if the layer should be visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  virtual void SetVisible(bool visible) noexcept = 0;

  /**
   * \brief Sets the opacity of the layer.
   *
   * \param opacity the new opacity of the layer, will be clamped to [0, 1].
   *
   * \since 0.2.0
   */
  virtual void SetOpacity(double opacity) = 0;

  /**
   * \brief Sets the name of the layer.
   *
   * \param name the new name of the layer.
   *
   * \since 0.2.0
   */
  virtual void SetName(QString name) = 0;

  /**
   * \brief Indicates whether or not the layer is visible.
   *
   * \note Layers are visible by default.
   *
   * \return `true` if the layer is visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto IsVisible() const noexcept -> bool = 0;

  /**
   * \brief Returns the opacity of the layer.
   *
   * \return the opacity of the layer, in the range [0, 1].
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto Opacity() const noexcept -> double = 0;

  /**
   * \brief Returns the name associated with the layer.
   *
   * \return the name of the layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto Name() const -> const QString& = 0;

  /**
   * \brief Returns a copy of the layer.
   *
   * \return a copy of the layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto Clone() const -> shared<ILayer> = 0;

  /**
   * \brief Returns the type of the layer.
   *
   * \return the type of with the layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto Type() const -> LayerType = 0;
};

}  // namespace tactile::core

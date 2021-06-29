#pragma once

#include <string>  // string

#include "aliases/shared.hpp"
#include "aliases/unique.hpp"
#include "core/properties/property_context.hpp"
#include "layer_type.hpp"

namespace Tactile {

class ILayer;

using UniqueLayer = Unique<ILayer>;
using SharedLayer = Shared<ILayer>;

/**
 * \interface ILayer
 *
 * \brief Represents different kinds of tilemap layers.
 *
 * \see `Map`
 * \see `TileLayer`
 * \see `ObjectLayer`
 *
 * \headerfile layer.hpp
 */
class ILayer : public IPropertyContext
{
 public:
  /**
   * \brief Sets whether or not the layer is visible.
   *
   * \param visible `true` if the layer should be visible; `false` otherwise.
   */
  virtual void SetVisible(bool visible) = 0;

  /**
   * \brief Sets the opacity of the layer.
   *
   * \param opacity the new opacity of the layer, will be clamped to [0, 1].
   */
  virtual void SetOpacity(float opacity) = 0;

  /**
   * \brief Sets the name of the layer.
   *
   * \param name the new name of the layer.
   */
  virtual void SetName(std::string name) = 0;

  /**
   * \brief Indicates whether or not the layer is visible.
   *
   * \details Layers are visible by default.
   *
   * \return `true` if the layer is visible; `false` otherwise.
   */
  [[nodiscard]] virtual auto IsVisible() const -> bool = 0;

  /**
   * \brief Returns the opacity of the layer.
   *
   * \return the opacity of the layer, in the range [0, 1].
   */
  [[nodiscard]] virtual auto GetOpacity() const noexcept -> float = 0;

  /**
   * \brief Returns the type of the layer.
   *
   * \return the type of with the layer.
   */
  [[nodiscard]] virtual auto GetType() const -> LayerType = 0;

  /**
   * \brief Returns a copy of the layer.
   *
   * \return a copy of the layer.
   */
  [[nodiscard]] virtual auto Clone() const -> SharedLayer = 0;
};

}  // namespace Tactile

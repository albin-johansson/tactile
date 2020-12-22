#pragma once

#include <QString>

#include "property_manager.hpp"

namespace tactile::core {

/**
 * \interface layer
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
class layer : public property_manager
{
 public:
  ~layer() noexcept override = default;

  /**
   * \brief Sets whether or not the layer is visible.
   *
   * \param visible `true` if the layer should be visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  virtual void set_visible(bool visible) noexcept = 0;

  /**
   * \brief Sets the opacity of the layer.
   *
   * \param opacity the new opacity of the layer, will be clamped to [0, 1].
   *
   * \since 0.2.0
   */
  virtual void set_opacity(double opacity) = 0;

  /**
   * \brief Sets the name of the layer.
   *
   * \param name the new name of the layer.
   *
   * \since 0.2.0
   */
  virtual void set_name(QString name) = 0;

  /**
   * \brief Indicates whether or not the layer is visible.
   *
   * \return `true` if the layer is visible; `false` otherwise.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto visible() const noexcept -> bool = 0;

  /**
   * \brief Returns the opacity of the layer.
   *
   * \return the opacity of the layer, in the range [0, 1].
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto opacity() const noexcept -> double = 0;

  /**
   * \brief Returns the name associated with the layer.
   *
   * \note By default, layers have no name assigned to them.
   *
   * \return the name of the layer.
   *
   * \since 0.2.0
   */
  [[nodiscard]] virtual auto name() const -> const QString& = 0;
};

}  // namespace tactile::core

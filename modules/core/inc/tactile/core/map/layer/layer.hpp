// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/context/meta_context.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class ILayerVisitor;
class IConstLayerVisitor;

/**
 * \interface ILayer
 * \brief The common interface for all layer variants.
 */
class TACTILE_CORE_API ILayer : public IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(ILayer);

  using IMetaContext::accept;

  /**
   * \brief Inspects the layer using the given visitor.
   *
   * \param visitor the visitor to use.
   */
  virtual void accept(ILayerVisitor& visitor) = 0;

  /**
   * \copydoc accept(ILayerVisitor&)
   */
  virtual void accept(IConstLayerVisitor& visitor) const = 0;

  /**
   * \brief Sets the associated identifier used in save files.
   *
   * \param id the new persistent identifier, if any.
   */
  virtual void set_persistent_id(Maybe<int32> id) = 0;

  /**
   * \brief Sets the opacity of the layer content.
   *
   * \param opacity the opacity value, will be clamped to [0, 1].
   */
  virtual void set_opacity(float opacity) = 0;

  /**
   * \brief Sets the visibility of the layer content.
   *
   * \param visible true if the layer content should be rendered; false otherwise.
   */
  virtual void set_visible(bool visible) = 0;

  /**
   * \brief Returns the associated persistent identifier, if any.
   *
   * \return a layer identifier.
   */
  [[nodiscard]]
  virtual auto get_persistent_id() const -> Maybe<int32> = 0;

  /**
   * \brief Returns the layer content opacity.
   *
   * \return an opacity value in the interval [0, 1].
   */
  [[nodiscard]]
  virtual auto get_opacity() const -> float = 0;

  /**
   * \brief Indicates whether the layer content is rendered.
   *
   * \return true if the content is visible; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_visible() const -> bool = 0;

  /**
   * \brief Creates a clone of the layer.
   *
   * \note The layer clone may not be identical to the source layer, the only guarantee
   *       is that the layer clone is logically equivalent. For example, associated
   *       identifiers are not cloned.
   *
   * \return the new layer.
   */
  [[nodiscard]]
  virtual auto clone() const -> Shared<ILayer> = 0;
};

}  // namespace tactile

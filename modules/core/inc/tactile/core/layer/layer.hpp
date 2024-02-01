// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/meta_context.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class ILayerVisitor;
class IConstLayerVisitor;

/**
 * The common interface for all layer variants.
 */
class ILayer : public IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(ILayer);

  using IMetaContext::accept;

  /**
   * Inspects the layer using the given visitor.
   *
   * \param visitor The visitor to use.
   */
  virtual void accept(ILayerVisitor& visitor) = 0;

  /** \copydoc accept(ILayerVisitor&) */
  virtual void accept(IConstLayerVisitor& visitor) const = 0;

  /**
   * Sets the associated identifier used in save files.
   *
   * \param id The new persistent identifier, if any.
   */
  virtual void set_persistent_id(Maybe<int32> id) = 0;

  /**
   * Sets the opacity of the layer content.
   *
   * \param opacity The opacity value, will be clamped to [0, 1].
   */
  virtual void set_opacity(float opacity) = 0;

  /**
   * Sets the visibility of the layer content.
   *
   * \param visible True if the layer content should be rendered; false otherwise.
   */
  virtual void set_visible(bool visible) = 0;

  /**
   * Returns the associated persistent identifier, if any.
   *
   * \return
   *    A layer identifier.
   */
  [[nodiscard]]
  virtual auto get_persistent_id() const -> Maybe<int32> = 0;

  /**
   * Returns the layer content opacity.
   *
   * \return
   *    An opacity value in the interval [0, 1].
   */
  [[nodiscard]]
  virtual auto get_opacity() const -> float = 0;

  /**
   * Indicates whether the layer content is rendered.
   *
   * \return
   *    True if the content is visible; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_visible() const -> bool = 0;

  /**
   * Creates a clone of the layer.
   *
   * \note
   *    The layer clone may not be identical to the source layer, the only guarantee is that the layer clone is
   *    logically equivalent. For example, associated identifiers are not cloned.
   *
   * \return
   *    The new layer.
   */
  [[nodiscard]]
  virtual auto clone() const -> Shared<ILayer> = 0;
};

}  // namespace tactile::core

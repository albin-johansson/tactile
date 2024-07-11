// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

class IDocumentVisitor;
class ILayerView;
class IMetaView;

/**
 * A read-only view of an object in an object layer.
 */
class IObjectView
{
 public:
  TACTILE_INTERFACE_CLASS(IObjectView);

  /**
   * Inspects the object.
   *
   * \param visitor The visitor to use.
   */
  virtual void accept(IDocumentVisitor& visitor) const = 0;

  /**
   * Returns a view of the parent object layer.
   *
   * \return
   * An object layer view.
   */
  [[nodiscard]]
  virtual auto get_parent_layer() const -> const ILayerView& = 0;

  /**
   * Returns the type of the object.
   *
   * \return
   * An object type.
   */
  [[nodiscard]]
  virtual auto get_type() const -> ObjectType = 0;

  /**
   * Returns the position of the object.
   *
   * \return
   * An object position.
   */
  [[nodiscard]]
  virtual auto get_position() const -> Float2 = 0;

  /**
   * Returns the size of the object.
   *
   * \return
   * An object size.
   */
  [[nodiscard]]
  virtual auto get_size() const -> Float2 = 0;

  /**
   * Returns the type tag.
   *
   * \return
   * A type tag.
   */
  [[nodiscard]]
  virtual auto get_tag() const -> StringView = 0;

  /**
   * Indicates whether the object is visible.
   *
   * \return
   * True if the object is rendered; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_visible() const -> bool = 0;

  /**
   * Returns a view of the associated metadata.
   *
   * \return
   * A metadata view.
   */
  [[nodiscard]]
  virtual auto get_meta() const -> const IMetaView& = 0;
};

}  // namespace tactile

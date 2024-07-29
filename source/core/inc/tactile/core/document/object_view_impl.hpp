// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/document/object_view.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/document/meta_view_impl.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class IDocument;

/**
 * An object view implementation.
 */
class ObjectViewImpl final : public IObjectView
{
 public:
  /**
   * Creates a view of an object in an object layer.
   *
   * \param document     The associated document.
   * \param parent_layer The host layer view.
   * \param object_id    The object entity identifier.
   */
  ObjectViewImpl(const IDocument* document,
                 const ILayerView* parent_layer,
                 EntityID object_id);

  /**
   * Creates a view of an object in a tile definition.
   *
   * \param document    The associated document.
   * \param parent_tile The host tile view.
   * \param object_id   The object entity identifier.
   */
  ObjectViewImpl(const IDocument* document,
                 const ITileView* parent_tile,
                 EntityID object_id);

  [[nodiscard]]
  auto accept(IDocumentVisitor& visitor) const -> Result<void> override;

  [[nodiscard]]
  auto get_parent_layer() const -> const ILayerView* override;

  [[nodiscard]]
  auto get_parent_tile() const -> const ITileView* override;

  [[nodiscard]]
  auto get_type() const -> ObjectType override;

  [[nodiscard]]
  auto get_id() const -> ObjectID override;

  [[nodiscard]]
  auto get_position() const -> Float2 override;

  [[nodiscard]]
  auto get_size() const -> Float2 override;

  [[nodiscard]]
  auto get_tag() const -> StringView override;

  [[nodiscard]]
  auto is_visible() const -> bool override;

  [[nodiscard]]
  auto get_meta() const -> const IMetaView& override;

 private:
  const IDocument* mDocument;
  const ILayerView* mParentLayer;
  const ITileView* mParentTile;
  EntityID mObjectId;
  MetaViewImpl mMeta;
};

}  // namespace tactile

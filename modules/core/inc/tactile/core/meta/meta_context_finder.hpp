// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/meta_context_visitor.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

class IMetaContext;

/**
 * Helper class for finding meta contexts.
 */
class TACTILE_CORE_API MetaContextFinder final : public IMetaContextVisitor {
 public:
  /**
   * Creates a finder.
   *
   * \param target_uuid The UUID of the meta context to look for.
   */
  explicit MetaContextFinder(const UUID& target_uuid);

  void visit(OrthogonalMap& map) override;

  void visit(GroupLayer& layer) override;

  void visit(TileLayer& layer) override;

  void visit(ObjectLayer& layer) override;

  void visit(Object& object) override;

  void visit(Tileset& tileset) override;

  void visit(Tile& tile) override;

  /**
   * Returns the found context.
   *
   * \return
   *    The found context, or a null pointer.
   */
  [[nodiscard]]
  auto found_context() -> IMetaContext*;

 private:
  UUID mTargetUuid;
  IMetaContext* mFoundContext {};

  void _check(IMetaContext& context);
};

}  // namespace tactile::core

// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/meta/meta_context_visitor.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class Metadata;

/**
 * Interface for all objects that feature attached properties and components.
 */
class IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(IMetaContext);

  /**
   * Inspects the meta context using the provided visitor.
   *
   * \param visitor the visitor to use.
   */
  virtual void accept(IMetaContextVisitor& visitor) = 0;

  /**
   * Returns the associated metadata.
   *
   * \return
   *    The metadata.
   */
  [[nodiscard]]
  virtual auto meta() -> Metadata& = 0;

  /** \copydoc meta() */
  [[nodiscard]]
  virtual auto meta() const -> const Metadata& = 0;
};

}  // namespace tactile

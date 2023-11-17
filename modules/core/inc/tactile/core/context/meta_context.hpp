// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/context/meta_context_visitor.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

class Metadata;

/**
 * \interface IMetaContext
 * \brief Base interface for all objects that feature attached properties and components.
 */
class TACTILE_CORE_API IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(IMetaContext);

  /**
   * \brief Inspects the meta context using the provided visitor.
   *
   * \param visitor the visitor to use.
   */
  virtual void accept(IMetaContextVisitor& visitor) = 0;

  /**
   * \brief Returns the associated metadata.
   *
   * \return the metadata.
   */
  [[nodiscard]]
  virtual auto get_meta() -> Metadata& = 0;

  /**
   * \copydoc get_meta()
   */
  [[nodiscard]]
  virtual auto get_meta() const -> const Metadata& = 0;
};

}  // namespace tactile

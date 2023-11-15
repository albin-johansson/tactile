// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/context/meta_context.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

TACTILE_FWD(class GroupLayer)
TACTILE_FWD(class TileFormat)

/**
 * \interface IMap
 * \brief Basic interface implemented by all tilemap variations.
 */
class TACTILE_CORE_API IMap : public IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(IMap);

  /**
   * \brief Returns the invisible root layer.
   *
   * \return the root group layer.
   */
  [[nodiscard]]
  virtual auto get_root_layer() -> GroupLayer& = 0;

  /**
   * \copydoc get_root_layer()
   */
  [[nodiscard]]
  virtual auto get_root_layer() const -> const GroupLayer& = 0;

  /**
   * \brief Returns the associated tile format configuration.
   *
   * \return a tile format configuration.
   */
  [[nodiscard]]
  virtual auto get_tile_format() -> TileFormat& = 0;

  /**
   * \copydoc get_tile_format()
   */
  [[nodiscard]]
  virtual auto get_tile_format() const -> const TileFormat& = 0;
};

}  // namespace tactile
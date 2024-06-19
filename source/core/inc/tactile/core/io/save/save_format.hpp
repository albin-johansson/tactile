// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/io/save/ir.hpp"

namespace tactile {

struct SaveFormatReadOptions;
struct SaveFormatWriteOptions;
class MapDocument;

/**
 * Interface for save file format reader/parser implementations.
 *
 * \details
 * Save format implementations aren't required to support both save and load
 * operations, and may simply report an error instead. However, a useful
 * implementation would of course provide at least one of these operations.
 */
class ISaveFormat
{
 public:
  TACTILE_INTERFACE_CLASS(ISaveFormat);

  /**
   * Attempts to load a map file.
   *
   * \param map_path The path to the map file.
   * \param options  The read options.
   *
   * \return
   * An intermediate map if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto load_map(const Path& map_path,
                        const SaveFormatReadOptions& options) const
      -> Result<ir::Map> = 0;

  /**
   * Attempts to save a map.
   *
   * \param map_document The map document to save.
   * \param options      The write options.
   *
   * \return
   * Nothing if successful; an error code otherwise.
   */
  [[nodiscard]]
  virtual auto save_map(const MapDocument& map_document,
                        const SaveFormatWriteOptions& options) const
      -> Result<void> = 0;
};

}  // namespace tactile

// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/**
 * \brief Provides save format parse options.
 */
struct SaveFormatReadOptions final {
  FilePath base_dir;     ///< The parent directory of the map or tileset file.
  bool strict_mode : 1;  ///< Whether strict parsing is to be enforced.
};

/**
 * \brief Provides save format save options.
 */
struct SaveFormatWriteOptions final {
  FilePath base_dir;               ///< The parent directory of the map or tileset file.
  bool use_external_tilesets : 1;  ///< Whether tilesets are saved in separate files.
  bool use_indentation : 1;        ///< Whether the output is indented.
};

}  // namespace tactile

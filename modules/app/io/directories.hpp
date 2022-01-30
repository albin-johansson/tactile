#pragma once

#include <filesystem>  // path

namespace tactile {

/**
 * \brief Returns the path to the persistent file directory.
 *
 * \ingroup io
 *
 * \return the persistent file directory path.
 */
[[nodiscard]] auto persistent_file_dir() -> const std::filesystem::path&;

}  // namespace tactile

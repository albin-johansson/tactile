#pragma once

#include <filesystem>  // path

namespace tactile {

/**
 * \brief Returns the path to the persistent file directory.
 *
 * \return the persistent file directory path.
 *
 * \ingroup io
 */
[[nodiscard]] auto persistent_file_dir() -> const std::filesystem::path&;

}  // namespace tactile

#pragma once

#include <filesystem>  // path

namespace Tactile {

/**
 * \brief Returns the path to the persistent file directory.
 *
 * \ingroup io
 *
 * \return the persistent file directory path.
 */
[[nodiscard]] auto GetPersistentFileDir() -> const std::filesystem::path&;

}  // namespace Tactile

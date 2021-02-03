#pragma once

#include <QString>  // QString

#include "map_document.hpp"
#include "parse_error.hpp"

namespace tactile {

/**
 * \brief Creates and returns a map document based on a save file.
 *
 * \warning You must claim ownership of the returned map document. The map
 * document has no parent, so you should set the parent as quickly as possible
 * to avoid a memory leak.
 *
 * \param path the path of the save file.
 * \param[out] error pointer to an error code will be written to.
 *
 * \return a pointer to the created map document; null if something went wrong.
 *
 * \throw tactile_error if the file extension isn't recognized.
 *
 * \since 0.1.0
 */
[[nodiscard]] auto open_map_document(const QString& path,
                                     parse::parse_error& error)
    -> core::map_document*;

}  // namespace tactile

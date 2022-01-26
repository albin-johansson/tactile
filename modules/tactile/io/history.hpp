#pragma once

#include <deque>       // deque
#include <filesystem>  // path
#include <string>      // string

namespace tactile {

/// \addtogroup io
/// \{

/// \name Recent file history API
/// \{

/**
 * \brief Loads the recent file history from the persistent file directory.
 */
void load_file_history();

/**
 * \brief Saves the in-memory file history to the persistent file directory.
 */
void save_file_history();

/**
 * \brief Clears the current in-memory recent file history.
 */
void clear_file_history();

/**
 * \brief Adds a file path to the recent file history.
 *
 * \details This function has no effect if the file path is already stored in the recent
 * file history.
 *
 * \param path the file path that will be stored.
 */
void add_file_to_history(const std::filesystem::path& path);

/**
 * \brief Sets the most recently closed file, and stores it in the file history.
 *
 * \param path the file path that will be stored.
 */
void set_last_closed_file(const std::filesystem::path& path);

/**
 * \brief Returns the current in-memory recent file history.
 *
 * \return the recent file history.
 */
[[nodiscard]] auto file_history() -> const std::deque<std::string>&;

/**
 * \brief Indicates whether there is a valid most recently closed file entry.
 *
 * \details The last closed file is considered valid if it's set and refers to an
 * existing file.
 *
 * \return `true` if the last closed file is valid; `false` otherwise.
 */
[[nodiscard]] auto is_last_closed_file_valid() -> bool;

/**
 * \brief Returns the file path to the most recently closed file.
 *
 * \return the path of the most recently closed file.
 *
 * \throws TactileError if there is no valid last closed file.
 *
 * \see `is_last_closed_file_valid()`
 */
[[nodiscard]] auto last_closed_file() -> const std::string&;

/// \} End of file history API

/// \} End of group io

}  // namespace tactile

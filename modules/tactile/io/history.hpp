#pragma once

#include <deque>       // deque
#include <filesystem>  // path
#include <string>      // string

namespace tactile {

/// \name File history API
/// \{

/// Loads the file history from the persistent file directory
void LoadFileHistory();

/// Saves the in-memory file history to the persistent file directory
void SaveFileHistory();

/// Clears the current in-memory file history
void ClearFileHistory();

/// Stores a path to the file history (if it isn't already there)
void AddFileToHistory(const std::filesystem::path& path);

/// Updates the last closed file and registers it in the file history
void SetLastClosedFile(const std::filesystem::path& path);

/// Returns the current file history
[[nodiscard]] auto GetFileHistory() -> const std::deque<std::string>&;

/// Indicates whether or not there is a valid last closed file entry
[[nodiscard]] auto HasValidLastClosedFile() -> bool;

/// Returns the last closed file path (remember to check HasValidLastClosedFile() before)
[[nodiscard]] auto GetLastClosedFile() -> const std::string&;

/// \} End of file history API

}  // namespace tactile

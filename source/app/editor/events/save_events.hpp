#pragma once

#include <filesystem>  // path

namespace tactile {

/// \addtogroup events
/// \{

struct save_event final
{};

struct save_as_event final
{
  std::filesystem::path path;
};

struct open_save_as_dialog_event final
{};

/// \} End of group events

}  // namespace tactile

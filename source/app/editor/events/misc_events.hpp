#pragma once

#include <filesystem>  // path

namespace tactile {

/// \addtogroup events
/// \{

struct quit_event final
{};

struct toggle_ui_event final
{};

struct show_settings_event final
{};

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
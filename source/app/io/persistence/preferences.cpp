#include "preferences.hpp"

#include <filesystem>  // path, exists
#include <utility>     // move

#include "io/directories.hpp"
#include "misc/logging.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto get_file_path() -> const std::filesystem::path&
{
  static const auto path = persistent_file_dir() / "settings.bin";
  return path;
}

inline preference_state _settings;

}  // namespace

void load_preferences()
{
  _settings = preference_state{};

  const auto& path = get_file_path();
  if (std::filesystem::exists(path)) {
    _settings.parse(path);
  }
  else {
    log_info("Did not find a preferences file, assuming defaults");
    save_preferences();
  }

  _settings.print();
}

void save_preferences()
{
  const auto& path = get_file_path();
  _settings.save(path);
}

void set_preferences(preference_state prefs)
{
  _settings = std::move(prefs);
}

auto get_preferences() -> preference_state&
{
  return _settings;
}

}  // namespace tactile

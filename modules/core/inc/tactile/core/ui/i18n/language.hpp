// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // to_underlying

#include "tactile/core/api.hpp"
#include "tactile/core/ui/i18n/language_id.hpp"
#include "tactile/core/ui/i18n/string_id.hpp"
#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * Represents a set of translated user-facing strings.
 */
class TACTILE_CORE_API Language final {
 public:
  TACTILE_DELETE_COPY(Language);
  TACTILE_DEFAULT_MOVE(Language);

  ~Language() noexcept = default;

  /**
   * Attempts to parse a language file.
   *
   * \param path     The path to the language file.
   * \param fallback The language to use when translated strings are missing.
   *
   * \return
   *    The parsed language, or an error code.
   */
  [[nodiscard]]
  static auto parse(const FilePath& path, const Language* fallback = nullptr)
      -> Result<Language>;

  /**
   * Prepends suitable icons to the strings.
   *
   * \note
   *    This function shouldn't be called directly, the icons
   *    will be loaded automatically if the language was
   *    loaded via `load_languages`.
   */
  void add_icons();

  /**
   * Returns the translated string associated with a given identifier.
   *
   * \pre The string identifier must refer to a valid string in the language.
   *
   * \note
   *    This function is called very often, so it's inlined and doesn't perform
   *    any bounds checks (in release builds) to be as fast as possible.
   *
   * \param id A string identifier.
   *
   * \return
   *    A translated string.
   */
  [[nodiscard]]
  auto get(const StringID id) const noexcept -> const char*
  {
    TACTILE_ASSERT(std::to_underlying(id) < mStrings.size());
    return mStrings[std::to_underlying(id)].c_str();
  }

 private:
  Vector<String> mStrings {};

  explicit Language(Vector<String> strings);
};

using LanguageMap = HashMap<LanguageID, Language>;

/**
 * Loads translation files from a directory on disk.
 *
 * \param dir The file path to the directory that stores the translation files.
 *
 * \return
 *    The loaded languages.
 */
[[nodiscard]]
TACTILE_CORE_API auto load_languages(const FilePath& dir) -> Result<LanguageMap>;

}  // namespace tactile::core

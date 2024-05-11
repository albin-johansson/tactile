// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/i18n/language.hpp"
#include "tactile/core/ui/i18n/language_id.hpp"
#include "tactile/core/ui/i18n/string_id.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * Utility for parsing language INI files.
 */
class LanguageParser final
{
 public:
  TACTILE_DELETE_COPY(LanguageParser);
  TACTILE_DEFAULT_MOVE(LanguageParser);

  LanguageParser();

  ~LanguageParser() noexcept = default;

  /**
   * Attempts to parse a language file.
   *
   * \param id       The language identifier.
   * \param path     The path to the language INI file.
   * \param fallback The fallback language used whenever a string is missing.
   *
   * \return
   * A language if successful; an error code otherwise.
   */
  [[nodiscard]]
  auto parse(LanguageID id,
             const Path& path,
             const Language* fallback = nullptr) const -> Result<Language>;

 private:
  HashMap<StringView, StringID> mMiscNames {};
  HashMap<StringView, StringID> mVerbNames {};
  HashMap<StringView, StringID> mNounNames {};
  HashMap<StringView, StringID> mAdjectiveNames {};
  HashMap<StringView, StringID> mActionNames {};
  HashMap<StringView, StringID> mMenuNames {};
  HashMap<StringView, StringID> mWidgetNames {};
};

/**
 * Attempts to parse a language file from disk.
 *
 * \param id The identifier associated with the desired language.
 *
 * \return
 * A parsed language if successful; an error code otherwise.
 */
[[nodiscard]]
auto parse_language_from_disk(LanguageID id) -> Result<Language>;

/// \}

}  // namespace tactile::ui

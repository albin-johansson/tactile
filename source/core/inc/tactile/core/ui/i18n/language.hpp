// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // to_underlying

#include "tactile/base/container/string.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/ui/i18n/language_id.hpp"
#include "tactile/core/ui/i18n/string_id.hpp"

namespace tactile::ui {

/// \addtogroup UI
/// \{

/**
 * Represents a set of translated user-facing strings.
 */
class Language final
{
 public:
  friend class LanguageParser;

  TACTILE_DEFAULT_MOVE(Language);
  TACTILE_DELETE_COPY(Language);

  ~Language() noexcept = default;

  /**
   * Prepends suitable icons to the strings in the language.
   */
  void add_icons();

  /**
   * Returns the identifier associated with the language.
   *
   * \return
   * A language identifier.
   */
  [[nodiscard]]
  auto get_id() const -> LanguageID;

  /**
   * Returns the translated string associated with a given identifier.
   *
   * \note
   * This function is called very often, so it's inlined and performs no bounds
   * checks whatsoever to be as fast as possible.
   *
   * \pre The string identifier must refer to a valid string in the language.
   *
   * \param id A string identifier.
   *
   * \return
   * A translated string.
   */
  [[nodiscard]]
  auto get(const StringID id) const noexcept -> const char*
  {
    TACTILE_ASSERT(std::to_underlying(id) < mStrings.size());
    return mStrings[std::to_underlying(id)].c_str();
  }

 private:
  LanguageID mID;
  Vector<String> mStrings {};

  Language(LanguageID id, Vector<String> strings);
};

/// \}

}  // namespace tactile::ui

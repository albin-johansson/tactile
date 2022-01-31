#pragma once

#include "persistence/preference_state.hpp"

namespace tactile {

/// \addtogroup io
/// \{

/// \name Preference API
/// \{

/**
 * \brief Loads existing persistent preferences or loads the default preferences.
 */
void load_preferences();

/**
 * \brief Saves the current preferences in the persistent file directory.
 */
void save_preferences();

/**
 * \brief Overrides the current preferences.
 *
 * \param prefs the new preferences.
 */
void set_preferences(preference_state prefs);

/**
 * \brief Returns the current preferences.
 *
 * \return the preferences.
 */
[[nodiscard]] auto get_preferences() -> preference_state&;

/// \} End of preference API

/// \} End of group io

}  // namespace tactile

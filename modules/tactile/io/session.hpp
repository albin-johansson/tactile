#pragma once

namespace Tactile {

class Model;

/// \addtogroup io
/// \{

/**
 * \brief Restores the maps from the previous session.
 *
 * \param model the model that the previous session will be restored to.
 *
 * \see `SaveSession()`
 */
void RestoreLastSession(Model& model);

/**
 * \brief Saves the current session.
 *
 * \param model the model that represent the current session that will be saved.
 *
 * \see `RestoreLastSession()`
 */
void SaveSession(const Model& model);

/// \} End of group io

}  // namespace Tactile

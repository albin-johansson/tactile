#pragma once

namespace tactile {

class Model;
class texture_manager;

/// \addtogroup io
/// \{

/**
 * \brief Restores the maps from the previous session.
 *
 * \param model the model that the previous session will be restored to.
 * \param textures the texture manager that will be used.
 *
 * \see `save_session()`
 */
void restore_last_session(Model& model, texture_manager& textures);

/**
 * \brief Saves the current session.
 *
 * \param model the model that represent the current session that will be saved.
 *
 * \see `restore_last_session()`
 */
void save_session(const Model& model);

/// \} End of group io

}  // namespace tactile

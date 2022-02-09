#pragma once

namespace tactile {

class document_model;
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
void restore_last_session(document_model& model, texture_manager& textures);

/**
 * \brief Saves the current session.
 *
 * \param model the model that represent the current session that will be saved.
 *
 * \see `restore_last_session()`
 */
void save_session(const document_model& model);

/// \} End of group io

}  // namespace tactile

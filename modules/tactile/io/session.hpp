#pragma once

namespace Tactile {

class Model;
class TextureManager;

/// \addtogroup io
/// \{

/**
 * \brief Restores the maps from the previous session.
 *
 * \param model the model that the previous session will be restored to.
 * \param textures the texture manager that will be used.
 *
 * \see `SaveSession()`
 */
void RestoreLastSession(Model& model, TextureManager& textures);

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

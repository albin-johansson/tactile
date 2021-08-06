#pragma once

namespace Tactile {

class Model;

void RestoreLastSession(Model& model);

void SaveSession(const Model& model);

}  // namespace Tactile

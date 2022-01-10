#pragma once

#include <tactile_def.hpp>
#include <tactile_io.hpp>

#include "dialog.hpp"

namespace Tactile {

class MapImportErrorDialog final : public ADialog {
 public:
  MapImportErrorDialog();

  void Open(IO::ParseError error);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

 private:
  Maybe<IO::ParseError> mError;
};

}  // namespace Tactile

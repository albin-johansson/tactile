#pragma once

#include "dialog.hpp"
#include "io/parser/parse_error.hpp"
#include "tactile_def.hpp"

namespace tactile {

class MapImportErrorDialog final : public ADialog {
 public:
  MapImportErrorDialog();

  void Open(parsing::parse_error error);

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;

 private:
  Maybe<parsing::parse_error> mError;
};

}  // namespace tactile

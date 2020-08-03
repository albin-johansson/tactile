#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

class remove_col final : public QUndoCommand {
 public:
  explicit remove_col(model::core* core);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
};

}  // namespace tactile::cmd

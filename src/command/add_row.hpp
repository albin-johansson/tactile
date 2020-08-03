#pragma once

#include <QUndoCommand>

#include "core.hpp"

namespace tactile::cmd {

class add_row final : public QUndoCommand {
 public:
  explicit add_row(model::core* core);

  void undo() override;

  void redo() override;

 private:
  model::core* m_core;
};

}  // namespace tactile::cmd

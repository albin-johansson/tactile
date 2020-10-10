#pragma once

#include <QUndoCommand>
#include <memory>  // shared_ptr

#include "map_document.hpp"

namespace tactile::cmd {

class remove_tileset final : public QUndoCommand
{
 public:
  remove_tileset(core::map_document* document,
                 std::shared_ptr<core::tileset> tileset,
                 tileset_id id);

  void undo() override;

  void redo() override;

 private:
  core::map_document* m_document;
  std::shared_ptr<core::tileset> m_tileset;
  tileset_id m_id;
  bool m_first{true};  ///< Used to determine when to notify UI
};

}  // namespace tactile::cmd

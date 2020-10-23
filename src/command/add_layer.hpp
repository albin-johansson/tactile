#pragma once

#include <QUndoCommand>
#include <memory>  // shared_ptr

#include "layer.hpp"
#include "layer_id.hpp"

namespace tactile {
namespace core {
class map_document;
}

namespace cmd {

class add_layer final : public QUndoCommand
{
 public:
  add_layer(core::map_document* document,
            std::shared_ptr<core::layer> layer,
            layer_id id);

  void undo() override;

  void redo() override;

 private:
  core::map_document* m_document{};
  std::shared_ptr<core::layer> m_layer;
  layer_id m_id;
};

}  // namespace cmd
}  // namespace tactile

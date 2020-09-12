#pragma once

#include <vector>

#include "abstract_command.hpp"
#include "position.hpp"
#include "small_map.hpp"
#include "types.hpp"

namespace tactile::cmd {

class erase_sequence final : public abstract_command
{
 public:
  erase_sequence(not_null<core::map*> map,
                 small_map<core::position, tile_id>&& oldState,
                 std::vector<core::position>&& positions);

  void undo() override;

  void redo() override;

 private:
  small_map<core::position, tile_id> m_oldState;
  std::vector<core::position> m_positions;
  bool m_first{true};
};

}  // namespace tactile::cmd

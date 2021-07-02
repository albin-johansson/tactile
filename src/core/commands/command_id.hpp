#pragma once

namespace Tactile {

struct CommandId final
{
  enum Value
  {
    AddRow,
    AddColumn,
    RemoveRow,
    RemoveColumn
  };
};

}  // namespace Tactile

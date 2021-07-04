#pragma once

namespace Tactile {

struct CommandId final
{
  enum Value
  {
    AddRow,
    AddColumn,
    RemoveRow,
    RemoveColumn,
    AddLayer,
    RemoveLayer,
    SelectLayer,
    DuplicateLayer,
    MoveLayerUp,
    MoveLayerDown,
    SetLayerOpacity,
    AddProperty,
    RemoveProperty,
    RenameProperty,
    SetProperty,
    ChangePropertyType,
    SetPropertyContext
  };
};

}  // namespace Tactile

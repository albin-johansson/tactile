#pragma once

namespace Tactile {

/// \addtogroup commands
/// \{

struct CommandId final {
  enum Value {
    AddRow,
    AddColumn,
    RemoveRow,
    RemoveColumn,
    ResizeMap,

    AddLayer,
    RemoveLayer,
    DuplicateLayer,
    MoveLayerUp,
    MoveLayerDown,
    SetLayerOpacity,
    SetLayerVisible,
    RenameLayer,

    AddProperty,
    RemoveProperty,
    RenameProperty,
    UpdateProperty,
    ChangePropertyType,

    StampSequence,
    EraserSequence,
    Bucket,

    AddTileset,
    RemoveTileset,
    SetTilesetName,

    SetObjectName,
    MoveObject,
    SetObjectTag,
    SetObjectVisibility,

    CreateComponentDef,
    RemoveComponentDef,
    RenameComponent,
    CreateComponentDefAttribute,
    SetComponentAttributeType,
    RenameComponentAttribute,
    UpdateComponentAttribute
  };
};

/// \} End of group commands

}  // namespace Tactile

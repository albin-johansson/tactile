#pragma once

namespace tactile::cmd {

enum class CommandId
{
  AddColumn = 1,
  AddLayer,
  AddProperty,
  AddRow,
  AddTileset,
  BucketFill,
  ChangePropertyType,
  Eraser,
  RemoveColumn,
  RemoveLayer,
  RemoveProperty,
  RemoveRow,
  RemoveTileset,
  RenameProperty,
  ResizeMap,
  Stamp,
  UpdateProperty,
  DuplicateLayer,
  ChangeOpacity,
  MoveLayerForward,
  MoveLayerBack,
  SelectLayer,
  SetLayerName,
  SetLayerVisibility,
  SetTilesetName
};

}  // namespace tactile::cmd

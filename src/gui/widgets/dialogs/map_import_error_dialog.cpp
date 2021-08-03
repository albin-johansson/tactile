#include "map_import_error_dialog.hpp"

#include <imgui.h>

#include "aliases/czstring.hpp"
#include "aliases/maybe.hpp"
#include "gui/icons.hpp"

namespace Tactile {
namespace {

inline Maybe<IO::ParseError> current_error;

[[nodiscard]] auto GetCause(const IO::ParseError error) -> czstring
{
  switch (error)
  {
    default:
      [[fallthrough]];
    case IO::ParseError::None:
      return "N/A";  // This shouldn't ever be used

    case IO::ParseError::MapUnsupportedExtension:
      return "The map file featured an unsupported file extension.";

    case IO::ParseError::MapDoesNotExist:
      return "The specified map file did not exist.";

    case IO::ParseError::MapMissingNextLayerId:
      return "The map was missing the next layer ID attribute!";

    case IO::ParseError::MapMissingNextObjectId:
      return "The map was missing the next object ID attribute!";

    case IO::ParseError::MapMissingTilesets:
      return "The map was missing the tilesets attribute!";

    case IO::ParseError::MapMissingLayers:
      return "The map was missing the layers attribute!";

    case IO::ParseError::MapMissingTileWidth:
      return "The map was missing a tile width attribute!";

    case IO::ParseError::MapMissingTileHeight:
      return "The map was missing a tile height attribute!";

    case IO::ParseError::LayerMissingWidth:
      return "The map featured a layer without a width!";

    case IO::ParseError::LayerMissingHeight:
      return "The map featured a layer without a height!";

    case IO::ParseError::LayerMissingType:
      return "The map featured a layer without a type!";

    case IO::ParseError::LayerMissingId:
      return "The map featured a layer without an ID!";

    case IO::ParseError::LayerUnknownType:
      return "The map featured a layer with an unknown type!";

    case IO::ParseError::CouldNotParseTiles:
      return "The map featured a tile layer with corrupt tile data!";

    case IO::ParseError::UnsupportedTileEncoding:
      return "The map featured a tile layer that used an unsupported tile encoding!";

    case IO::ParseError::CouldNotReadExternalTileset:
      return "Could not read an external tileset!";

    case IO::ParseError::TilesetMissingFirstGid:
      return "The map featured a tileset without a first global tile ID attribute!";

    case IO::ParseError::TilesetMissingTileWidth:
      return "The map featured a tileset without a tile width attribute!";

    case IO::ParseError::TilesetMissingTileHeight:
      return "The map featured a tileset without a tile height attribute!";

    case IO::ParseError::TilesetMissingImagePath:
      return "The map featured a tileset without an image path attribute!";

    case IO::ParseError::TilesetMissingName:
      return "The map featured a tileset without a name!";

    case IO::ParseError::TilesetImageDoesNotExist:
      return "The map featured a tileset based on a non-existent image!";

    case IO::ParseError::CouldNotParseProperty:
      return "The map featured a corrupt property!";

    case IO::ParseError::PropertyMissingName:
      return "The map featured a property without a name!";

    case IO::ParseError::PropertyUnknownType:
      return "The map featured a property with an unknown type!";

    case IO::ParseError::ObjectMissingId:
      return "The map featured an object layer containing an object without an ID!";
  }
}

}  // namespace

void UpdateMapImportErrorDialog()
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (ImGui::BeginPopupModal(ICON_FA_EXCLAMATION_TRIANGLE " Map import error",
                             nullptr,
                             flags))
  {
    ImGui::TextUnformatted("Oops, failed to open the specified map!");
    ImGui::Text("Cause: %s", GetCause(current_error.value()));

    ImGui::Spacing();
    ImGui::Separator();

    if (ImGui::Button("OK"))
    {
      current_error.reset();
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}

void OpenMapImportErrorDialog(const IO::ParseError error)
{
  current_error = error;
  ImGui::OpenPopup(ICON_FA_EXCLAMATION_TRIANGLE " Map import error");
}

}  // namespace Tactile

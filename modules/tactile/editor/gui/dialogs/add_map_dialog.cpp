#include "add_map_dialog.hpp"

#include <cassert>  // assert

#include <imgui.h>

#include "editor/events/map_events.hpp"
#include "io/preferences.hpp"

namespace Tactile {

AddMapDialog::AddMapDialog() : ADialog{"Create New Map"}
{
  SetAcceptButtonLabel("Create");
}

void AddMapDialog::Open()
{
  ADialog::Show();
  mTileWidth = Prefs::GetPreferredTileWidth();
  mTileHeight = Prefs::GetPreferredTileHeight();
}

void AddMapDialog::UpdateContents(const entt::registry&, entt::dispatcher&)
{
  ImGui::DragInt("Tile width", &mTileWidth, 1.0f, 1, 10'000);
  ImGui::DragInt("Tile height", &mTileHeight, 1.0f, 1, 10'000);

  // TODO include map size
}

void AddMapDialog::OnAccept(entt::dispatcher& dispatcher)
{
  assert(mTileWidth > 0);
  assert(mTileHeight > 0);
  dispatcher.enqueue<AddMapEvent>(mTileWidth, mTileHeight);
}

}  // namespace Tactile

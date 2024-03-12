// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_layer_opacity.hpp"

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetLayerOpacity::SetLayerOpacity(Shared<Layer> layer, const float opacity)
    : mLayer {std::move(layer)},
      mNewOpacity {opacity}
{
  if (!mLayer) {
    throw Exception {"Invalid null layer"};
  }
}

void SetLayerOpacity::undo()
{
  mLayer->set_opacity(mOldOpacity.value());
  mOldOpacity.reset();
}

void SetLayerOpacity::redo()
{
  mOldOpacity = mLayer->get_opacity();
  mLayer->set_opacity(mNewOpacity);
}

auto SetLayerOpacity::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetLayerOpacity*>(cmd)) {
    if (mLayer->get_uuid() == other->mLayer->get_uuid()) {
      mNewOpacity = other->mNewOpacity;
      return true;
    }
  }

  return false;
}

auto SetLayerOpacity::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.set_layer_opacity;
}

}  // namespace tactile::cmd

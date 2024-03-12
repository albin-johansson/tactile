// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "move_object.hpp"

#include <utility>  // move

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

MoveObject::MoveObject(Shared<Object> object,
                       const Float2& previous,
                       const Float2& updated)
    : mObject {std::move(object)},
      mPreviousPos {previous},
      mUpdatedPos {updated}
{
  if (!mObject) {
    throw Exception {"Invalid null object!"};
  }
}

void MoveObject::undo()
{
  mObject->set_pos(mPreviousPos);
}

void MoveObject::redo()
{
  mObject->set_pos(mUpdatedPos);
}

auto MoveObject::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.move_object;
}

}  // namespace tactile::cmd

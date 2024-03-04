/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "tactile/base/prelude.hpp"

#define TACTILE_FWD_DECLARE_CLASS(Name) class Name;
#define TACTILE_FWD_DECLARE_STRUCT(Name) struct Name;

#define TACTILE_FWD_DECLARE_CLASS_NS(Ns, Name) \
  namespace Ns {                               \
  class Name;                                  \
  }

#define TACTILE_FWD_DECLARE_STRUCT_NS(Ns, Name) \
  namespace Ns {                                \
  struct Name;                                  \
  }

#define TACTILE_DECLARE_SPECIAL_MEMBERS(Class) \
  TACTILE_DECLARE_COPY(Class);                 \
  TACTILE_DECLARE_MOVE(Class);                 \
  ~Class()

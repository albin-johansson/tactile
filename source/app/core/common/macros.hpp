/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#define TACTILE_DEFAULT_COPY(Class) \
  Class(const Class&) = default;    \
  auto operator=(const Class&)->Class& = default

#define TACTILE_DEFAULT_MOVE(Class)  \
  Class(Class&&) noexcept = default; \
  auto operator=(Class&&) noexcept -> Class& = default

#define TACTILE_DELETE_COPY(Class) \
  Class(const Class&) = delete;    \
  auto operator=(const Class&)->Class& = delete

#define TACTILE_DELETE_MOVE(Class)  \
  Class(Class&&) noexcept = delete; \
  auto operator=(Class&&) noexcept -> Class& = delete

#define TACTILE_DECLARE_COPY(Class) \
  Class(const Class&);              \
  Class& operator=(const Class&)

#define TACTILE_DECLARE_MOVE(Class) \
  Class(Class&&) noexcept;          \
  Class& operator=(Class&&) noexcept

#define TACTILE_DEFINE_COPY(Class)      \
  Class::Class(const Class&) = default; \
  Class& Class::operator=(const Class&) = default

#define TACTILE_DEFINE_MOVE(Class)          \
  Class::Class(Class&&) noexcept = default; \
  Class& Class::operator=(Class&&) noexcept = default

#define TACTILE_DECLARE_SPECIAL_MEMBERS(Class) \
  TACTILE_DECLARE_COPY(Class);                 \
  TACTILE_DECLARE_MOVE(Class);                 \
  ~Class()

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

namespace tactile {

struct MouseInfo;
struct Region;

class Attribute;
class TilePos;

class DocumentModel;
class ADocument;
class MapDocument;
class TilesetDocument;

class ICommand;
class CommandStack;

class TextureManager;
class IRenderer;
class ToolManager;

class IElement;

class IContext;
class IContextVisitor;
class ContextDelegate;

class PropertyBundle;

class ComponentIndex;
class ComponentBundle;
class ComponentDefinition;
class Component;

class ILayerVisitor;
class IConstLayerVisitor;
class ILayer;
class TileLayer;
class ObjectLayer;
class GroupLayer;
class LayerDelegate;

class Map;
class Object;
class Viewport;

struct TilesetInfo;
class TilesetBundle;
class Tileset;
class TilesetRef;
class Tile;

}  // namespace tactile
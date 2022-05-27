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
class TextureManager;
class IRenderer;
class ToolManager;

namespace comp {

struct ActiveAttributeContext;
struct ActiveLayer;
struct ActiveObject;
struct ActiveTileset;
struct ActiveTool;

struct CurrentRectangleStroke;
struct CurrentEllipseStroke;

struct AnimationFrame;
struct Animation;
struct Viewport;
struct Parent;

struct AttributeContext;
struct Property;
struct ComponentDef;
struct Component;

struct MapInfo;
struct Layer;
struct TileLayer;
struct ObjectLayer;
struct GroupLayer;
struct LayerTreeNode;
struct Object;
struct ObjectDragInfo;

struct Texture;
struct TilesetContext;
struct Tileset;
struct TilesetCache;
struct UvTileSize;
struct MetaTile;

}  // namespace comp
}  // namespace tactile
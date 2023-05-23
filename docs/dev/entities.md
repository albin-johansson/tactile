# Entities

This document provides an overview of the components featured by different kinds of entities.

## Context components

These components are globally accessible in the model.

* `DocumentContext`
* `Settings`
* `Languages`
* `FileHistory`
* `Icons`
* `TextureCache`
* `TextureCallbacks`
* `MenuItems`
* `ComponentDockState` (TODO)
* `MouseInfo` (TODO)

## Documents

* `Document`
* `CommandStack`
* `Viewport`
* `MapDocument` (if map document)
* `TilesetDocument` (if tileset document)

## Tilesets

* `Context`
* `Tileset`

## Attached tilesets

* `AttachedTileset`
* `Viewport`

## Maps

* `Context`
* `Map`
* `MapIdentifiers`
* `MapLayerSuffixes`
* `TileFormat`

## Layers

* `Context`
* `Layer`
* `TileLayer` (if tile layer)
* `ObjectLayer` (if object layer)
* `GroupLayer` (if group layer)

## Tiles

* `Context`
* `Tile`
* `TileAnimation` (if animated)

## Textures

* `Texture`
* `OpenGLTexture` (if OpenGL backend is used)
* `VulkanTexture` (if Vulkan backend is used)

## Widgets

* `Widget`

## Menu items

* `MenuItem`
* `MenuItemCallbacks`
# Entities

This document provides an overview of the components featured by different kinds of entities.

## Context components

These components are globally accessible in the model.

* `DocumentContext`
* `TextureCache`
* `Icons`
* `LanguageContext` (TODO)
* `ComponentDockState` (TODO)

## Document entities

* `Document`
* `CommandStack`
* `Viewport`
* `MapDocument` (if map document)
* `TilesetDocument` (if tileset document)

## Tileset entities

* `Context`
* `Tileset`

## Attached tileset entities

* `AttachedTileset`
* `Viewport`

## Map entities

* `Context`
* `Map`
* `TileFormat`

## Layer entities

* `Context`
* `Layer`
* `TileLayer` (if tile layer)
* `ObjectLayer` (if object layer)
* `GroupLayer` (if group layer)

## Tile entities

* `Context`
* `Tile`
* `TileAnimation` (if animated)

## Texture entities

* `Texture`
* `OpenGLTexture` (if OpenGL backend is used)
* `VulkanTexture` (if Vulkan backend is used)
